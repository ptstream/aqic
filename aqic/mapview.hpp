#ifndef MAPVIEW_HPP
#define MAPVIEW_HPP

#include "../town/airqualities.hpp"
#include "../town/town.hpp"
#include "../qtools/boundingbox.hpp"
#include "../tools/vector.hpp"
#include "../tools/status.hpp"
#include <QQuickView>
#include <QGeoLocation>
#include <QDate>

class CTownTooltip;

/*! \brief The CMapView class provides a window for displaying and managing the map.
 */
class CMapView : public QQuickView, public CStatus<quint32>
{
  Q_OBJECT
public:
  enum EStatus : quint32 { AllTownsInserted     = 0x00000001,
                           InsertOnlyValidIndex = 0x00000002,
                         };

  CMapView () = default;
  ~CMapView () = default;

  void init (QGeoCoordinate const & center, double zoomLevel = 8.0, QUrl url = QUrl ());
  void mapProperties (double& latitude, double& longitude, double& zoomLevel);

  QQuickItem* mapQuickItem () { return m_mapQuickItem; }
  CTownTooltip* townTooltip () { return m_townTooltip; }

  QDate date () const;

  void updateBoundingCircle (int value, CAirQuality::EIndexType indexType);

  void fitInView ();
  void addTowns (QMap<QString, CTown> const & towns, bool fitInView = true);
  void updateTowns (TTAQs const & aqs);
  void updateIndexes ();
  CTown town (QString const & code) const { return m_towns.value (code); }

//  void addItems (QStringList const & codes);
  void delItems ();
  void setVisibleRegion (QGeoRectangle rc);
  void setVisibilities (QVector<bool> const & visibilities);
//  void selectMarkers (QStringList const & codes);
  void clearTowns ();

  CAirQuality::EIndexType index () const { return m_index; }
  void setIndex (CAirQuality::EIndexType index) { m_index = index; }
  QList<CTown> related (QString const & townCode, float lat, float lon) const;
  void setTooltipActivated (bool activated);

  void setPolygonOpacity (int opacity);

protected:
  void resizeEvent (QResizeEvent* event) override;

protected:
  void addTown (CTown const & town);
  void adjustVisibleRegion (double lat, double lon);
  void adjustVisibleRegion (double lat0, double lon0, double lat1, double lon1);
  void updateVisibleRegion (double lat, double lon, double width, double height);
  void updateVisibleRegion (QGeoRectangle const & rc);
  CBoundingBox boundingBox (int value, CAirQuality::EIndexType indexType) const;

private :
  QQuickItem*             m_mapQuickItem = nullptr; //!< The map quick item.
  QMap<QString, CTown>    m_towns;
  CTownTooltip*           m_townTooltip = nullptr;
  double                  m_minimumVisibleRegion = 2000.0; // 2km from either side of marker.
  int                     m_vertexCount = 0; // Just for console messages
  CAirQuality::EIndexType m_index = CAirQuality::Global;
  CBoundingBox            m_boundingBox; // Just for fit in view
  QGeoLocation            m_address;
};

#endif // MAPVIEW_HPP
