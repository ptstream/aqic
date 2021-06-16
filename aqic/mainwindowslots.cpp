#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "mapview.hpp"
#include "moviedata.hpp"
#include "towntooltip.hpp"
#include "networktimeout.hpp"
#include "downloaderdata.hpp"
#include "../wtools/waitwidget.hpp"
#include "../wtools/player.hpp"
#include "../town/towns.hpp"
#include "../town/airqualities.hpp"
#include "../qtools/externalipaddr.hpp"
#include "../qtools/ipaddrposition.hpp"
#include "../qtools/downloader.hpp"
#include "../qtools/downloadcache.hpp"
#include "../qtools/applocations.hpp"
#include "../qtools/urls.hpp"
#include <QComboBox>
#include <QToolButton>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QLabel>

void CMainWindow::updateExternalIPAddr (int, bool)
{
  auto downloader  = static_cast<CDownloader*>(sender ());
  m_externalIPAddr = CExternalIPAddr::addr (downloader->data ());
  downloader->deleteLater ();
  if (!m_externalIPAddr.isEmpty ())
  {
    initializeMap ();
  }
}

void CMainWindow::updateAppLocation (int, bool)
{
  auto            downloader = static_cast<CDownloader*>(sender ());
  CIPAddrPosition ipAddrPos (downloader->data ());
  downloader->deleteLater ();
  if (ipAddrPos.state () == CIPAddrPosition::Done)
  {
    m_initialMapCenter = ipAddrPos.location ();
    m_townName         = ipAddrPos.town ();
  }
  else
  {
    m_initialMapCenter.setLatitude (48.856614);
    m_initialMapCenter.setLongitude (2.3522219);
    m_townName = "Paris";
  }

  initializeMap ();
}

void CMainWindow::downloadAirQualitiesError ()
{
  auto downloader = static_cast<CDownloader*>(sender ());
  if (!downloader->contains (CDownloader::Timeout) && !downloader->contains (CDownloader::Aborted))
  {
    QString text  = "<p><b>" + tr ("On error occurred while downloading air quality data.") + "</b></p>";
    text         += QString ("%1: %2").arg (tr ("Message"), downloader->message ()) + "<br/>";
    text         += QString ("%1: %2").arg (tr ("Region"),  m_urls->airQualityRegionName (m_regionCode));
    QMessageBox::warning (this, CAppLocations::location (CAppLocations::AppName), text, QMessageBox::Close);
  }

  m_regionCode = m_regionCodeBeforeDownload;
  setProvider (RegionName);
  mainMenuActionSetEnabled (true);
}

void CMainWindow::downloadAirQualitiesTimeout ()
{
  auto            downloader = static_cast<CDownloader*>(sender ());
  int             ms         = downloader->serverTimeout ();
  CNetworkTimeout nt (ms, this);
  int             ret = nt.exec ();
  if (ret == QDialog::Accepted)
  {
    m_downloaderData->m_serverTimeout = 2 * ms;
    downloadAirQualities (m_regionCode, FromRegion, true);
  }

  setProvider (RegionName);
  mainMenuActionSetEnabled (true);
}

void CMainWindow::downloadAirQualitiesFinished (int ms, bool readFromCache)
{
  if (!readFromCache)
  {
    m_downloadTime = ms;
  }

  setToolBarActiveRegion (m_regionCode);
  pausePlayer ();
  m_player->restoreAMM ();
  delete m_waitWidget;
  m_waitWidget = nullptr;

  auto downloader = static_cast<CDownloader*>(sender ());
  bool ok         = !downloader->contains (CDownloader::Aborted | CDownloader::Timeout);
  if (ok)
  {
    CAirQualities::deleteGlobalInstance ();
    CAirQualities* aqs = new CAirQualities;
    aqs->setTownCodes (m_townCodes);

    QString fileName = CDownloadCache::extractFileName (downloader->data ());
    QApplication::setOverrideCursor (Qt::WaitCursor);
    aqs->load (fileName);
    QApplication::restoreOverrideCursor ();
    ok = !aqs->isEmpty ();
    if (ok)
    {
      QDate date         = QDate::currentDate ();
      TTAQs airQualities = aqs->airQualities (date);
      if (airQualities.isEmpty ())
      {
        date          = aqs->firstAvailableDate (); // Always valid due to airQualities not empty
        airQualities  = aqs->airQualities (date);
        QString sdate = QLocale::system ().toString (date, QLocale::ShortFormat);
        QString text  = "<p>" + tr ("No air quality available for today.") + "</p>" +
                        "<p>" + tr ("The first available date is: ") + "<b>" + sdate + "</b></p>";
        QMessageBox::information (this, CAppLocations::location (CAppLocations::AppName), text, QMessageBox::Close);
      }

      QMap<QString, CTown> towns = CTowns::gblInst ()->updateAirQualities (airQualities);
      m_mapView->addTowns (towns);
      QFileInfo fi (fileName);

      qint64 diff    = QDateTime::currentDateTime ().msecsTo (fi.fileTime (QFile::FileModificationTime));
      int    timeout = diff < -60000 ? 10000 : m_downloaderData->m_newDataDelay;
      m_newDataTimer.start (timeout);
    }
    else
    {
      m_mapView->clearTowns ();
      QString text = "<b>" + tr ("No air quality available") + "</b>";
      QMessageBox::warning (this, CAppLocations::location (CAppLocations::AppName), text, QMessageBox::Close);
    }

    updateToolBarDateComboBox ();
    setProvider (RegionName);
    m_regionCodeBeforeDownload.clear ();
    m_indexesComboBox->setCurrentIndex (0);
  }

  downloader->deleteLater ();
  CAirQualities* aqs = CAirQualities::gblInst ();
  m_player->setEnabled (aqs != nullptr && !aqs->isEmpty ());
  mainMenuActionSetEnabled (true);
}

void CMainWindow::dateChanged (int index)
{
  pausePlayer ();
  auto  cb        = static_cast<QComboBox*> (sender ());
  int   dayOffset = cb->itemData (index).toInt ();
  QDate date      = QDate::currentDate ().addDays (dayOffset);
  TTAQs aqs       = CAirQualities::gblInst ()->airQualities (date);
  m_mapView->updateTowns (aqs);
  m_player->setFrame (m_datesComboBox->count () - index - 1);
  m_player->restoreAMM ();
}

// Warning index is from legend not CAirQuality::EIndex
void CMainWindow::indexChanged (int index)
{
  m_movieData->m_frameCount = 0LL;
  m_frameRate.restart ();
  m_mapView->setIndex (static_cast<CAirQuality::EIndexType>(index));
  QList<QDate> const & dates = m_player->dates ();
  updatePlayerTickColors (dates);
  m_mapView->updateIndexes ();
  m_player->restoreAMM ();
}

void CMainWindow::abortDownload ()
{
  CDownloader::abortAll ();
}

void CMainWindow::regionChanged (int index)
{
  QString region = m_regionsComboBox->itemData (index).toString ();
  downloadAirQualities (region, FromRegion);
}

void CMainWindow::newDataCheck  ()
{
  auto downloader = new CDownloader (nullptr, this);
  connect (downloader, &CDownloader::downloadFinished, this, &CMainWindow::newDataCheckFinished, Qt::QueuedConnection);
  QString url = m_urls->airQualityUrl (m_regionCode);
  if (!url.isEmpty ())
  {
    QUrl qurl (url);
    downloader->head (qurl);
  }
}

void CMainWindow::newDataCheckFinished  ()
{
  m_newDataTimer.stop ();
  auto    downloader = static_cast<CDownloader*>(sender ());
  QString url        = m_urls->airQualityUrl (m_regionCode);
  if (!url.isEmpty ())
  {
    QDateTime lastModifiedDateTime = downloader->lastModifiedHeader ();
    QString   fileName             = m_downloaderCache->fileName (url);

    QFileInfo fi (fileName);
    QDateTime fileBirthTime = fi.fileTime (QFileDevice::FileBirthTime);
    if (lastModifiedDateTime > fileBirthTime)
    {
      setProvider (NewDataAvailable);
    }
  }

  downloader->deleteLater ();
  m_newDataTimer.start (m_downloaderData->m_newDataDelay);
}
