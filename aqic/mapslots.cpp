#include "mainwindow.hpp"
#include "mapview.hpp"
#include "towndetails.hpp"
#include "../town/towns.hpp"
#include "../qtools/urls.hpp"
#include <QDebug>

/*
void CMainWindow::mapClicked (double, double)
{

}

void CMainWindow::markerClicked (int index)
{

}

void CMainWindow::markerEntered (int index)
{

}

void CMainWindow::markerExited (int index)
{

}
*/

static void setTemporalColors (CTownDetails& tds, QList<CAirQuality> const & aqs)
{
  QList<QDate> dates;
  int          dateCount = aqs.size ();
  dates.reserve (dateCount);
  for (int index = CAirQuality::Global; index < CAirQuality::LastIndex; ++index)
  {
    CTownDetails::TColors colors;
    colors.reserve (dateCount);
    for (CAirQuality const & aq : qAsConst (aqs))
    {
      CTownDetails::TColor color;
      color.second = aq.color (static_cast<CAirQuality::EIndexType>(index));
      colors.append (color);
      if (dates.size () < dateCount)
      {
        dates.append (aq.deadline ());
      }
    }

    tds.setTemporalColors (static_cast<CAirQuality::EIndexType>(index), colors);
  }

  tds.setDates (dates);
}

static bool setDailyProfilColors (CTownDetails& tds, QList<CAirQuality> const & aqs)
{
  using TSum   = std::array<int, 7>;
  using TCount = std::array<int, 7>;

  int  aqCount = aqs.size ();
  bool ok      = aqCount >= 7;
  if (ok)
  {
    int  last = aqCount - aqCount / 7 * 7;
    for (int index = CAirQuality::Global; index < CAirQuality::LastIndex; ++index)
    {
      TSum sums;
      sums.fill (0);
      TCount count;
      count.fill (0);

      int c = 0;
      for (int i = aqCount - 1; i >= last; --i)
      {
        int    day   = aqs[i].deadline ().dayOfWeek () - 1;
        quint8 value = aqs[i].index (static_cast<CAirQuality::EIndexType>(index));
        if (value > CAQLegend::Unavailable && value < CAQLegend::Event)
        {
          sums[day] += value;
          ++count[day];
          ++c;
        }
      }

      CTownDetails::TColors colors;
      colors.reserve (7);
      for (int day = 0; day < 7; ++day)
      {
        quint8               index = static_cast<quint8>(::qRound (static_cast<float>(sums[day]) / count[day]));
        CTownDetails::TColor color;
        color.second = CAirQuality::color (index);
        colors.append (color);
      }

      tds.setDailyProfileColors (static_cast<CAirQuality::EIndexType>(index), colors);
    }
  }

  return ok;
}

void CMainWindow::townClicked (QString code, double lat, double lon)
{
  m_mapView->setTooltipActivated (false);
  CTownDetails tds (this);

  tds.setTown (code);
  tds.setRegionName (m_urls->airQualityRegionName (m_regionCode));
  tds.setRegionCode (m_regionCode);
  tds.setLocation (lat, lon);

  QList<CAirQuality> aqs = CAirQualities::gblInst ()->airQualities (code);
  setTemporalColors (tds, aqs);
  setDailyProfilColors (tds, aqs);
  tds.setInitialDate (m_mapView->date ());
  tds.setInitilDayOfWeek ();

  tds.exec ();
  m_mapView->setTooltipActivated (true);
}
