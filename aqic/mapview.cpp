#include "mapview.hpp"
#include "towntooltip.hpp"
#include "../town/town.hpp"
#include "../qtools/tinyhtml.hpp"
#include "../tools/geoconstants.hpp"
#include "../tools/geoconverter.hpp"
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlContext>
#include <QQmlProperty>
#include <QGeoLocation>
#include <QGeoAddress>
#include <QGeoPolygon>
#include <QGeoRectangle>
#include <QElapsedTimer>
#include <QToolTip>
#include <QVBoxLayout>
#include <cmath>

using namespace spart;

void CMapView::init (QGeoCoordinate const & center, double zoomLevel, QUrl url)
{
  if (url.isEmpty ())
  {
    url.setUrl (QStringLiteral ("qrc:/map.qml"));
  }

  qmlRegisterType<CTownTooltip>("aqic.town.tooltip", 1, 0, "TownTooltip");

  QQmlEngine*  engine  = this->engine ();
  QQmlContext* context = engine->rootContext ();
  m_townTooltip        = new CTownTooltip (engine);
  m_townTooltip->setMapView (this);
  context->setContextProperty ("townTooltip", m_townTooltip);

  setSource (url);
  QQuickItem* rootItem = this->rootObject ();
  Q_ASSERT (rootItem != nullptr);

  QList<QQuickItem*> items = rootItem->childItems ();
  Q_ASSERT (!items.isEmpty ());

  m_mapQuickItem = items.first ();
  setResizeMode (QQuickView::SizeRootObjectToView);
  QQmlProperty::write (m_mapQuickItem, QStringLiteral ("center"), QVariant::fromValue<QGeoCoordinate>(center));
  QQmlProperty::write (m_mapQuickItem, QStringLiteral ("zoomLevel"), zoomLevel);
}

void CMapView::mapProperties (double& latitude, double& longitude, double& zoomLevel)
{
  zoomLevel             = QQmlProperty::read (m_mapQuickItem, QStringLiteral ("zoomLevel")).toDouble ();
  QGeoCoordinate center = QQmlProperty::read (m_mapQuickItem, QStringLiteral ("center")).value<QGeoCoordinate>();
  latitude              = center.latitude ();
  longitude             = center.longitude ();
}

void CMapView::addTown (CTown const & town)
{
  if (!contains (InsertOnlyValidIndex) || town.airQuality ().index (CAirQuality::Global) != 0)
  {
    QVariantList          path;
    CTown::TPaths const & townPaths = town.paths ();
    for (CTown::TPath const & townPath : townPaths)
    {
      path.reserve (townPath.size ());
      for (TVertex const & vertex : townPath)
      {
        qreal lat = static_cast<qreal>(vertex.x ());
        qreal lon = static_cast<qreal>(vertex.y ());
        path.append (QVariant::fromValue (QGeoCoordinate (lat, lon)));
        ++m_vertexCount;
      }

      QString const & code  = town.code ();
      QColor          color = QColor::fromRgb (town.airQuality ().color ());
      QMetaObject::invokeMethod (m_mapQuickItem, "addPolygon", Q_ARG (QVariant, path),
                                 Q_ARG (QVariant, code), Q_ARG (QVariant, color));
      path.clear ();
    }
  }
}

void CMapView::setTooltipActivated (bool activated)
{
  CTownTooltip::resetTooltip ();
  QQmlProperty::write (m_mapQuickItem, QStringLiteral ("toolTipActivated"), activated);
}

void CMapView::fitInView ()
{
  if (m_boundingBox.isValid ())
  {
    QGeoCoordinate p0 (m_boundingBox.latMax (), m_boundingBox.lonMin ());
    QGeoCoordinate p1 (m_boundingBox.latMin (), m_boundingBox.lonMax ());
    QList<QGeoCoordinate> coords;
    coords << p0 << p1;
    QGeoRectangle rc (coords);
    setVisibleRegion (rc);
  }
}

void CMapView::addTowns (QMap<QString, CTown> const & towns, bool fitInView)
{
  QElapsedTimer ti;
  ti.start ();

  setTooltipActivated (false);
  remove (CMapView::AllTownsInserted);
  clearTowns ();
  m_towns       = towns;
  m_vertexCount = 0;
  m_boundingBox.init ();
  for (CTown const & town : towns)
  {
    CBoundingBox const & bnd = town.boundingBox ();
    m_boundingBox.add (bnd);
    addTown (town);
  }

  if (fitInView)
  {
    this->fitInView ();
  }

  add (CMapView::AllTownsInserted);
  setTooltipActivated (true);

  qDebug () << "Town count: " << towns.size () << "Vertex count: " << m_vertexCount;
  qDebug () << "Add towns to map: " << ti.elapsed () / 1000.0 << 's';
}

CBoundingBox CMapView::boundingBox (int value, CAirQuality::EIndexType index) const
{
  QVector<CBoundingBox> bnds;
  bnds.reserve (10);
  for (CTown const & town : m_towns)
  {
    if (town.airQuality ().index (index) == value)
    {
      CBoundingBox const &            townBnd    = town.boundingBox ();
      bool                            intersects = false;
      QVector<CBoundingBox>::iterator begin      = bnds.begin ();
      QVector<CBoundingBox>::iterator end        = bnds.end ();
      QVector<CBoundingBox>::iterator it;
      for (it = begin; it != end && !intersects; ++it)
      {
        intersects = (*it).intersects (townBnd);
      }

      if (it == end)
      {
        bnds.append (townBnd);
      }
      else
      {
        (*it).add (townBnd);
        for (int i = bnds.size () - 1; i >= 1; --i)
        {
          if (bnds[i].intersects (bnds[i - 1]))
          {
            bnds[i - 1].add (bnds[i]);
            bnds.remove (i);
          }
        }
      }
    }
  }

  CBoundingBox greatestBnd;
  if (bnds.size () > 1)
  {
    auto compare = [] (CBoundingBox const b1, CBoundingBox const b2) -> bool
    {
      return b1.area () < b2.area ();
    };

    QVector<CBoundingBox>::const_iterator end = bnds.cend ();
    QVector<CBoundingBox>::const_iterator it  = std::max_element (bnds.cbegin (), end, compare);
    if (it != end)
    {
      greatestBnd = *it;
    }
  }
  else
  {
    greatestBnd = bnds.first ();
  }

  return greatestBnd;
}

void CMapView::updateBoundingCircle (int value, CAirQuality::EIndexType index)
{
  CBoundingBox bnd    = boundingBox (value, index);
  TVertex      center = bnd.center ();
  double       lat    = center.x ();
  double       lon    = center.y ();

  float   w = std::fabs (bnd.width ());
  float   h = std::fabs (bnd.height ());
  TVertex p = center + TVertex (0.5F * w, 0.5F * h);
  double  r = CGeoConverter::wGS84Distance (p.x (), p.y (), center.x (), center.y ());
  QColor       color (Qt::red);
  QMetaObject::invokeMethod (m_mapQuickItem, "mapCircle", Q_ARG (QVariant, lat), Q_ARG (QVariant, lon),
                             Q_ARG (QVariant, r), Q_ARG (QVariant, color));
}

void CMapView::updateTowns (TTAQs const & aqs)
{
  QMetaObject::invokeMethod (m_mapQuickItem, "removeCircle");
  QVariantMap ccs;
  for (CTown& town : m_towns)
  {
    QString const & code = town.code ();
    CAirQuality     aq   = aqs.value (code);
    town.setAirQuality (aq);
    ccs.insert (code, QColor::fromRgb (aq.color (m_index)));
  }

  QMetaObject::invokeMethod (m_mapQuickItem, "changeColors", Q_ARG (QVariant, ccs));
}

void CMapView::updateIndexes ()
{
  QVariantMap ccs;
  for (CTown const & town : qAsConst (m_towns))
  {
    ccs.insert (town.code (), QColor::fromRgb (town.airQuality ().color (m_index)));
  }

  QMetaObject::invokeMethod (m_mapQuickItem, "changeColors", Q_ARG (QVariant, ccs));
}

QList<CTown> CMapView::related (QString const & townCode, float lat, float lon) const
{
  QList<CTown> towns;
  for (CTown const & town : m_towns)
  {
    if (town.code () != townCode && town.boundingBoxContains (lat, lon))
    {
      towns.append (town);
    }
  }

  return towns;
}

void CMapView::delItems ()
{
  QMetaObject::invokeMethod (m_mapQuickItem, "delMarkers");
}

void CMapView::setVisibleRegion (QGeoRectangle rc)
{
  if (rc.isValid ())
  {
    double const offset = m_minimumVisibleRegion / gcst::metersPerDegree;
    if (rc.width () < offset)
    {
      rc.setWidth (offset);
    }

    if (rc.height () < offset)
    {
      rc.setHeight (offset);
    }

    QQmlProperty::write (m_mapQuickItem, "visibleRegion", QVariant::fromValue<QGeoRectangle> (rc));
  }
}

void CMapView::setVisibilities (QVector<bool> const & visibilities)
{
  QMetaObject::invokeMethod (m_mapQuickItem, "setVisibilities", Q_ARG (QVariant, QVariant::fromValue<QVector<bool>> (visibilities)));
}

void CMapView::updateVisibleRegion (double lat, double lon, double width, double height)
{
  QGeoCoordinate  center (lat, lon);
  QGeoRectangle  rc (center, width, height);
  updateVisibleRegion (rc);
}

void CMapView::updateVisibleRegion (QGeoRectangle const & rc)
{
  QQmlProperty::write (m_mapQuickItem, "visibleRegion", QVariant::fromValue<QGeoRectangle> (rc));
}

void CMapView::adjustVisibleRegion (double lat0, double lon0, double lat1, double lon1)
{
  // Set the center and zoom level to see the selected markers.
  double       dx     = std::fabs (lat1 - lat0) * 0.5;
  double       dy     = std::fabs (lon1 - lon0) * 0.5;
  double const offset = m_minimumVisibleRegion / gcst::metersPerDegree;
  if (dx < offset)
  {
    dx = offset;
  }

  if (dy < offset)
  {
    dy = offset;
  }

  lat0 -= dx;
  lat1 += dx;
  lon0 -= dy;
  lon1 += dy;
  updateVisibleRegion ((lat0 + lat1) / 2.0, (lon0 + lon1) / 2.0, std::fabs (lat1 - lat0), std::fabs (lon1 - lon0));
}

void CMapView::adjustVisibleRegion (double lat, double lon)
{
  double const offset = m_minimumVisibleRegion / gcst::metersPerDegree;
  double       xmin   = lat - offset;
  double       xmax   = lat + offset;
  double       ymin   = lon - offset;
  double       ymax   = lon + offset;
  updateVisibleRegion ((xmin + xmax) / 2.0, (ymin + ymax) / 2.0, std::fabs (xmax - xmin), std::fabs (ymax - ymin));
}

void CMapView::clearTowns ()
{
  QMetaObject::invokeMethod (m_mapQuickItem, "delItems");
}

void CMapView::resizeEvent (QResizeEvent* event)
{
  // When the main window dimensions change, the markers, polygons, ... are not at the good position (Qt 5.12.7).
  // Make two pan to redraw the markers correctly
  QMetaObject::invokeMethod (m_mapQuickItem, "mapRefocuses");
  QQuickView::resizeEvent (event);
}

QDate CMapView::date () const
{
  QDate date;
  if (!m_towns.isEmpty ())
  {
    date = m_towns.first ().airQuality ().deadline ();
  }

  return date;
}

void CMapView::setPolygonOpacity (int opacity)
{
  QMetaObject::invokeMethod (m_mapQuickItem, "setPolygonOpacity", Q_ARG (QVariant, static_cast<double>(opacity) / 100.0));
}
