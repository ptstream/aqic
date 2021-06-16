#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "../town/airquality.hpp"
#include "../tools/status.hpp"
#include <QMainWindow>
#include <QTimer>
#include <QGeoCoordinate>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE

class QComboBox;
class QLabel;
class CMapView;
class CDownloadCache;
class CUrls;
class CWaitWidget;
class CPlayer;
class CTownCodes;
class CDownloader;
struct SDownloaderData;
struct SLastCession;
struct SMovieData;
struct SImageExporterParams;
struct SDisplayData;

class CMainWindow : public QMainWindow, CStatus<quint32>
{
  Q_OBJECT

public:
  enum EDownloadFrom { FromTown,
                       FromRegion
                     };

  enum EProviderInfo { RegionName,
                       Download,
                       NewDataAvailable
                     };

  enum EStatus : quint32 { FactoryResetOnClose = 0x00000001,
                         };

  CMainWindow (QWidget* parent = nullptr);
  ~CMainWindow ();

protected slots:
  void updateExternalIPAddr (int, bool);
  void updateAppLocation (int, bool);
  void downloadAirQualitiesFinished (int ms, bool readFromCache);
  void downloadAirQualitiesError ();
  void downloadAirQualitiesTimeout ();
  void dateChanged (int index);
  void indexChanged (int index);
  void regionChanged (int index);
  void showProviderSite (QString const & link);
  void abortDownload ();
  void newDataCheck ();
  void newDataCheckFinished ();

  void defaultDimensions (bool = false);
  void preferences (bool);
  void downloadStatistics (bool);
  void cleanCache (bool);
  void factoryReset (bool);
  void about (bool);
  void fitInView (bool);

  void aqiUnavailable (bool);
  void aqiGood (bool);
  void aqiMedium (bool);
  void aqiDegraded (bool);
  void aqiBad (bool);
  void aqiVeryBad (bool);
  void aqiExtremelyBad (bool);
  void aqiEvent (bool);
  void aqiGlobalLegend (bool);

  void frameRateChanged (int);
  void playStateChanged (bool = false);
  void forwardClicked (bool);
  void backwardClicked (bool);
  void changeFrame (int position);
  void pausePlayer ();
  void startPlayer ();
  void highestAverage ();
  void highestMax ();
  void lowestAverage ();
  void lowestMax ();

//  void mapClicked (double, double);
//  void markerClicked (int index);
//  void markerEntered (int index);
//  void markerExited (int index);
  void townClicked (QString code, double lat, double lon);

  void exportMap (bool);
  void exportMovie (bool);

protected:
  void closeEvent (QCloseEvent*) override;
  void changeEvent (QEvent* event) override;
  void keyPressEvent (QKeyEvent* event) override;

private :
  void mapViewCreateLayout ();
  CDownloader* newDownloader (bool data);
  void removeSessionData ();
  void removeDownloadData ();

  // Toobar controls
  void createToolBarControls ();
  void createMainMenu ();
  void createRegionsComboBox ();
  void createDatesComboBox ();
  void createIndexesComboBox ();
  void createFitInViewButton ();
  void createProviderInfo ();
  void createLegendButton ();
  void createPlayer ();
  void setToolBarActiveRegion (QString const & region);
  void updatePlayerTickColors (QList<QDate> const & deadlines);
  void updateToolBarDateComboBox ();

  void initializeMap ();
  void downloadAirQualities (QString code, EDownloadFrom from, bool renewCache = false);
  void restoreInitialParams (SLastCession& lastCession);
  void prepareMovie (CAirQuality::EIndexType index);
  void movieNextFrame ();
  void movieUpdateFrame ();
  QSize defaultDims ();
  void mainMenuActionSetEnabled (bool enabled);
  void setProvider (EProviderInfo info);
  void exportImages (bool one);

private:
  Ui::CMainWindow* ui;
  CMapView*        m_mapView;
  QWidget*         m_mapViewContainer = nullptr;
  QTimer           m_timer; // The timer use to delayed initialization to leave time to polish ui at start.
  QTimer           m_newDataTimer;
  QString          m_newDataUrl = "http://new-data-available";
  QString          m_externalIPAddr; // External ip address at launch. May be unused if the last cession ini file exists.
  QGeoCoordinate   m_initialMapCenter; // Map center at start. May be unused if the last cession ini file exists.
  QString          m_townName;
  QString          m_regionCode, m_regionCodeBeforeDownload;
  CDownloadCache*  m_downloaderCache;
  int              m_simplifyCoef = 50;

  CUrls*                m_urls;
  SDownloaderData*      m_downloaderData      = nullptr;
  CTownCodes*           m_townCodes           = nullptr;
  SImageExporterParams* m_imageExporterParams = nullptr;
  SDisplayData*         m_displayData         = nullptr;

  QComboBox*       m_regionsComboBox = nullptr;
  QComboBox*       m_datesComboBox   = nullptr;
  QComboBox*       m_indexesComboBox = nullptr;

  QLabel*          m_provider   = nullptr;
  CWaitWidget*     m_waitWidget = nullptr;
  SMovieData*      m_movieData  = nullptr;
  CPlayer*         m_player     = nullptr;
  QElapsedTimer    m_frameRate;

  quint32 m_downloadTime = 0;
};
#endif // MAINWINDOW_HPP
