#include "towntooltip.hpp"
#include "mapview.hpp"
#include "../town/towns.hpp"
#include <QGeoCoordinate>
#include <QToolTip>
#include <QApplication>

CTownTooltip::CTownTooltip (QObject* parent) : QObject (parent)
{
  m_styleSheet = QString ("\
QToolTip { border-style:none;\
color: black;\
background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #%1,stop:1 white)\
}");
}

QString CTownTooltip::tooltipText (CTown const & town, float lat, float lon)
{
  QString             line (QLatin1String("%1: %2<br/>"));
  QLocale             locale     = QLocale::system ();
  CAirQuality const & airQuality = town.airQuality ();
  QString             text;
  text += QString (QLatin1String("<h2 style=\"text-align:center\">%1</h2>")).arg (town.name ());
  text += QString (QLatin1String("%1<br/>")).arg (locale.toString (airQuality.deadline ()));
  text += line.arg (tr ("Code"), town.code ());
  text += line.arg (tr ("Region"), town.region ());

  QString r = QString::number (lat, 'f', 6) + charDegree;
  text += line.arg (tr ("Latitude"), r);

  r = QString::number (lon, 'f', 6) + charDegree;
  text += line.arg (tr ("Longitude"), r);

  quint8 globalIndex = airQuality.index (CAirQuality::Global);
  QString tmp;
  text += QLatin1String("<b>") + tr ("Global Index") + QStringLiteral (": ");
  if (globalIndex == 0)
  {
    text += tr ("unavailable");
  }
  else
  {
    tmp   = QString::number (globalIndex);
    text += tmp + " (" + CAQLegend::gblInst ()->at (0).m_indexes[globalIndex].m_wording + ')';
  }

  text += QLatin1String("</b><br/>");
  if (globalIndex != 0)
  {
    for (int i = CAirQuality::Global + 1; i < CAirQuality::LastIndex; ++i)
    {
      QString pollutant, wording;
      QString format = QString (QLatin1String (" - %1: %2 (%3)<br/>"));
      quint8 index   = airQuality.index (static_cast<CAirQuality::EIndexType>(i));
      tmp = QString::number (index);
      if (index == globalIndex)
      {
        format.prepend (QLatin1String ("<b>"));
        format.replace (QLatin1String ("<br/>"), QLatin1String ("</b><br/>"));
      }

      SPollutantLegend const & pl = CAQLegend::gblInst ()->at (i - 1);
      wording                     = pl.m_indexes[index].m_wording;
      pollutant                   = pl.m_name;
      text += format.arg (pollutant).arg (index).arg (wording);
    }
  }

  text += "<h6 style=\"text-align:center\">" + tr ("Click for details") + "</h6>";

  return text;
}

QString CTownTooltip::text (QString const & code, QGeoCoordinate const & loc)
{
  QPoint globalCursorPos = QCursor::pos ();
  if (!code.isEmpty ())
  {
    if (m_mapView->contains (CMapView::AllTownsInserted))
    {
      float x    = static_cast<float>(loc.latitude ());
      float y    = static_cast<float>(loc.longitude ());
      CTown town = m_mapView->town (code);
      bool  in   = town.contains (x, y); // First try if the polygon of town containing the mouse position
      if (!in)
      {
        QList<CTown> towns = m_mapView->related (town.code (), x, y); // Find all town bounding boxes containing the mouse position
        for (CTown const & t : qAsConst (towns))
        {
          in = t.contains (x, y);  // Find which town bounding box containing the mouse position
          if (in)
          {
            town = t;
            break;
          }
        }
      }

      if (in)
      {
        QString color = QString::number (town.airQuality ().color (), 16);
        qApp->setStyleSheet (m_styleSheet.arg (color));
        QString text = tooltipText (town, x, y);
        QToolTip::showText (globalCursorPos, text);
        return text;
      }
    }
  }

  resetTooltip ();
  return QString ();
}

void CTownTooltip::resetTooltip ()
{
  QToolTip::hideText ();
  qApp->setStyleSheet ("QToolTip {}");
}


