#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "mapview.hpp"
#include "moviedata.hpp"
#include "towntooltip.hpp"
#include "downloaderdata.hpp"
#include "imageexporterparams.hpp"
#include "displaydata.hpp"
#include "../wtools/waitwidget.hpp"
#include "../town/towns.hpp"
#include "../town/airqualities.hpp"
#include "../town/towncodes.hpp"
#include "../qtools/downloader.hpp"
#include "../qtools/downloadcache.hpp"
#include "../qtools/lastcession.hpp"
#include "../qtools/urls.hpp"
#include "../qtools/applocations.hpp"
#include "QVBoxLayout"
#include <QQuickItem>
#include <QComboBox>
#include <QAction>
#include <QSet>
#include <QListView>
#include <QLabel>
#include <QStandardPaths>

CMainWindow::CMainWindow (QWidget* parent) : QMainWindow (parent), ui (new Ui::CMainWindow),
  m_mapView (new CMapView), m_downloaderCache (new CDownloadCache), m_urls (new CUrls),
  m_downloaderData (new SDownloaderData), m_imageExporterParams (new SImageExporterParams),
  m_displayData (new SDisplayData), m_movieData (new SMovieData)
{
  ui->setupUi (this);

  QByteArray cacheLocation = m_downloaderCache->absolutePath ().toUtf8 ();
  qputenv ("QML_DISK_CACHE_PATH", cacheLocation);

  CAQLegend* legend = new CAQLegend;
  legend->load ();

  SLastCession lastCession;
  createToolBarControls ();
  restoreInitialParams (lastCession);
  mapViewCreateLayout ();
  QStringList fileNames;

  fileNames << QString ("towns-%1.bin").arg (m_simplifyCoef)
            << QString ("towns-%1.geojson").arg (m_simplifyCoef);

  QDir townFolder (CAppLocations::location (CAppLocations::ConfigLocation));
  for (QString& fileName : fileNames)
  {
    fileName = townFolder.absoluteFilePath (fileName);
  }

  new CTowns (fileNames);

  QString fileName = CAppLocations::localeConfigFilePath (townFolder.absoluteFilePath ("towncodes.json"));
  if (QFile::exists (fileName))
  {
    m_townCodes = new CTownCodes (fileName);
  }

  connect (&m_newDataTimer, &QTimer::timeout, this, &CMainWindow::newDataCheck);
  // Lauch the last part of initialization.
  connect (&m_timer, &QTimer::timeout, this, &CMainWindow::initializeMap);
  m_timer.setSingleShot (true);
  m_timer.start (m_initialMapCenter.isValid () ? 500 : 0);
}

CMainWindow::~CMainWindow ()
{
  delete m_downloaderCache;
  m_downloaderCache = nullptr;

  delete m_urls;
  m_urls = nullptr;

  CTowns::gblInst ()->deleteGlobalInstance ();
  CAirQualities::gblInst ()->deleteGlobalInstance ();
  CAQLegend::gblInst ()->deleteGlobalInstance ();

  delete m_townCodes;
  m_townCodes = nullptr;

  delete m_imageExporterParams;
  m_imageExporterParams = nullptr;

  delete ui;
}

void CMainWindow::mapViewCreateLayout ()
{
  QWidget* centralWidget = this->centralWidget ();
  m_mapViewContainer     = QWidget::createWindowContainer (m_mapView, centralWidget);
  QVBoxLayout* layout    = new QVBoxLayout;
  layout->addWidget (m_mapViewContainer);
  centralWidget->setLayout (layout);
}

void CMainWindow::restoreInitialParams (SLastCession& lastCession)
{
  // Get the previous map center.
  m_initialMapCenter.setLatitude (lastCession.getReal ("location", "map_latitude", 0.0));
  m_initialMapCenter.setLongitude (lastCession.getReal ("location", "map_longitude", 0.0));

  // Get the previous main window size and position.
  QByteArray geom = lastCession.getByteArray ("ui", "main_window");
  if (!geom.isEmpty ())
  {
    restoreGeometry (geom);
  }
  else
  {
    defaultDimensions ();
  }

  m_regionCode = QString::fromUtf8 (lastCession.getByteArray ("location", "region", ""));
  if (m_regionCode.isEmpty ())
  { // ini file don't exists or is wrong.
    m_initialMapCenter = QGeoCoordinate ();
  }

  m_simplifyCoef = lastCession.getInt ("ui", "simplify_coef", m_simplifyCoef);

  // Set the regions combo box current item.
  for (int i = 0, count = m_regionsComboBox->count (); i < count; ++i)
  {
    if (m_regionsComboBox->itemData (i).toString () == m_regionCode)
    {
      m_regionsComboBox->blockSignals (true);
      m_regionsComboBox->setCurrentIndex (i);
      m_regionsComboBox->blockSignals (false);
      break;
    }
  }

  // Get the network properties.
  m_downloaderData->restore (lastCession);
  m_downloaderCache->setProperties (m_downloaderData->m_cacheClean, m_downloaderData->m_cacheValidity, m_downloaderData->m_cacheSize);
  m_newDataTimer.setInterval (m_downloaderData->m_newDataDelay);
  m_imageExporterParams->restore (lastCession);
  m_displayData->restore (lastCession);
}

CDownloader* CMainWindow::newDownloader (bool data)
{
  CDownloadCache* cache      = data ? m_downloaderCache : nullptr;
  auto            downloader = new CDownloader (cache, this);
  if (data)
  {
    downloader->setServerTimeout (m_downloaderData->m_serverTimeout);
  }

  return downloader;
}

void CMainWindow::initializeMap ()
{
  m_timer.stop ();
  disconnect (&m_timer, &QTimer::timeout, nullptr, nullptr);
  if (m_initialMapCenter.isValid () && m_initialMapCenter.latitude () != 0.0)
  {
    SLastCession lastCession;
    double       zoomLevel = lastCession.getReal ("location", "map_zoom_level", 8.0);
    m_mapView->init (m_initialMapCenter, zoomLevel); // Initialize the map view.
    m_mapView->setPolygonOpacity (m_displayData->m_opacity);

    QQuickItem* rootQuickItem = m_mapView->rootObject ();
//    connect (rootQuickItem, SIGNAL(mouseClicked (double, double)), this, SLOT(mapClicked (double, double)));
//    connect (rootQuickItem, SIGNAL(markerClicked (int)), this, SLOT(markerClicked (int)));
//    connect (rootQuickItem, SIGNAL(markerEntered (int)), this, SLOT(markerEntered (int)));
//    connect (rootQuickItem, SIGNAL(markerExited (int)), this, SLOT(markerExited (int)));
    connect (rootQuickItem, SIGNAL(townClicked (QString, double, double)), this, SLOT(townClicked (QString, double, double)));

    QLayout* layout = centralWidget ()->layout ();
    layout->setContentsMargins (0, 0, 0, 0);

    QString       code;
    EDownloadFrom from;
    if (m_regionCode.isEmpty ())
    {
      code = m_townName;
      from = FromTown;
    }
    else
    {
      code = m_regionCode;
      from = FromRegion;
    }

    downloadAirQualities (code, from);
    m_timer.setSingleShot (false);
    connect (&m_timer, &QTimer::timeout, this, &CMainWindow::movieNextFrame);
  }
  else
  { // It is the case of first launch or if ini file has been removed.
    auto downloader = new CDownloader;
    if (m_externalIPAddr.isEmpty ())
    { // First phase, download the external ip address of the application.
      connect (downloader, &CDownloader::downloadFinished, this, &CMainWindow::updateExternalIPAddr, Qt::QueuedConnection);
      downloader->get (QUrl (m_urls->externalIPAddr ()));
    }
    else
    { // Second phase, download approximate position of the external ip address.
      connect (downloader, &CDownloader::downloadFinished, this, &CMainWindow::updateAppLocation, Qt::QueuedConnection);
      downloader->get (QUrl (m_urls->ipAddrPosition () + m_externalIPAddr));
    }
  }
}

void CMainWindow::setProvider (EProviderInfo info)
{
  QString text;
  switch (info)
  {
    case RegionName:
      text = m_urls->airQualityProvider (m_regionCode);
      break;

    case Download:
    {
      QString url = m_urls->airQualityRegionName (m_regionCode);
      text        = QString ("<div style=\"color:#D84315\">%1: %2</div>").arg (tr ("Downloding"), url);
      break;
    }

    default:
    {
      QString format = R"(<!DOCTYPE html><html><head><style>a:link { color: red; }</style></head>
                       <body><a href="%1" target="_blank">%2</a></body></html>)";
      text = format.arg (m_newDataUrl, tr ("New data are available"));
      break;
    }
  }

  m_provider->setText (text);
}

void CMainWindow::downloadAirQualities (QString code, EDownloadFrom from, bool renewCache)
{
  m_newDataTimer.stop ();
  if (m_regionCodeBeforeDownload.isEmpty ())
  {
    m_regionCodeBeforeDownload = m_regionCode;
  }

  m_regionCode = from == FromTown ? CTowns::gblInst ()->regionFromTownName (code) : code;
  setProvider (Download);
  auto downloader = newDownloader (true);

  connect (downloader, &CDownloader::downloadFinished,  this, &CMainWindow::downloadAirQualitiesFinished, Qt::QueuedConnection);
  connect (downloader, &CDownloader::downloadError,     this, &CMainWindow::downloadAirQualitiesError);
  connect (downloader, &CDownloader::timeout,           this, &CMainWindow::downloadAirQualitiesTimeout);
  bool    cleanCacheEnabled;
  QString url = m_urls->airQualityUrl (m_regionCode);
  if (!url.isEmpty ())
  {
    if (m_waitWidget == nullptr)
    {
      m_waitWidget = new CWaitWidget (this);
      m_waitWidget->setTimeout (downloader->serverTimeout ());
      connect (m_waitWidget, &CWaitWidget::stopEmitted, this, &CMainWindow::abortDownload);
    }

    m_waitWidget->show ();
    m_waitWidget->startAnimation ();
    QUrl qurl (url);
    if (renewCache)
    {
      m_downloaderCache->removeFile (qurl);
    }

    downloader->get (qurl);
    cleanCacheEnabled = false;
  }
  else
  {
    cleanCacheEnabled = true;
  }

  mainMenuActionSetEnabled (cleanCacheEnabled);
}

void CMainWindow::removeDownloadData ()
{
  QStringList entries = m_downloaderCache->entryList (QDir::Files);
  for (QString entry : qAsConst (entries))
  {
    entry = m_downloaderCache->absoluteFilePath (entry);
    QFileInfo fi (entry);
    QString suffix = fi.suffix ();
    if (suffix.isEmpty () || suffix == "csv")
    {
      QFile::remove (fi.absoluteFilePath ());
    }
  }
}

void CMainWindow::removeSessionData ()
{
  if (m_downloaderCache->exists ())
  {
    m_downloaderCache->cdUp ();
    m_downloaderCache->removeRecursively ();
  }

  QDir dataLocation = CAppLocations::location (CAppLocations::DataLocation);
  if (dataLocation.exists ())
  {
    dataLocation.removeRecursively ();
  }

  QDir otherCacheLocation (QStandardPaths::writableLocation (QStandardPaths::GenericCacheLocation));
  otherCacheLocation.cd ("QtLocation");
  if (otherCacheLocation.exists ())
  {
    otherCacheLocation.removeRecursively ();
  }

  otherCacheLocation.cdUp ();
  QStringList subFolders = otherCacheLocation.entryList (QDir::Dirs);
  for (QString const & subFolder : qAsConst (subFolders))
  {
    if (subFolder.startsWith ("qtshadercache"))
    {
      otherCacheLocation.cd (subFolder);
      otherCacheLocation.removeRecursively ();
    }
  }
}
