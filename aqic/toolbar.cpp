#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "legend.hpp"
#include "moviedata.hpp"
#include "mapview.hpp"
#include "about.hpp"
#include "preferences.hpp"
#include "downloadstatistics.hpp"
#include "globallegend.hpp"
#include "../wtools/player.hpp"
#include "../wtools/slider.hpp"
#include "../qtools/urls.hpp"
#include "../qtools/downloadcache.hpp"
#include "../qtools/downloader.hpp"
#include "../qtools/stringmanip.hpp"
#include "../qtools/applocations.hpp"
#include <QComboBox>
#include <QListView>
#include <QLayout>
#include <QStandardItemModel>
#include <QMenu>
#include <QToolButton>
#include <QLabel>
#include <QDesktopServices>
#include <QMessageBox>

int   const toolBarSpacing  = 4;
int   const listItemSpacing = 2;
QSize const regionIconSize  = QSize (32, 21);
QSize const dateIconSize    = QSize (24, 24);

static void addSpacer (QToolBar* toolBar, int width = 0)
{
  auto spacer = new QWidget (toolBar);
  spacer->setAutoFillBackground (false);
  spacer->setUpdatesEnabled (false);
  if (width != 0)
  {
    spacer->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSize size (width, width);
    spacer->setMinimumSize (size);
  }
  else
  {
    spacer->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Expanding);
  }

  toolBar->addWidget (spacer);
}

static QComboBox* createToobarComboBox (QToolBar* toolBar, QSize const & iconSize = QSize ())
{
  auto comboBox = new QComboBox (toolBar);
  comboBox->setSizeAdjustPolicy (QComboBox::AdjustToContents);
  auto list = static_cast<QListView*>(comboBox->view ());
  list->setSpacing (listItemSpacing);
  list->setUniformItemSizes (true);
  if (!iconSize.isEmpty ())
  {
    list->setIconSize (iconSize);
  }

  QAction* action = toolBar->addWidget (comboBox);
  action->setVisible (true);
  addSpacer (toolBar, toolBarSpacing);
  return comboBox;
}

static void addRegions (QComboBox* comboBox, CUrls const * urls)
{
  CUrls::TAirQualities const & airQualities = urls->airQualities ();
  QStringList                  regions      = airQualities.keys ();

  auto compare = [urls] (QString const & s1, QString const & s2) -> bool
  {
    QString temp1 = CStringManip::removeDiacritics (urls->airQualityRegionName (s1));
    QString temp2 = CStringManip::removeDiacritics (urls->airQualityRegionName (s2));
    return  temp1 < temp2;
  };

  std::sort (regions.begin (), regions.end (), compare);
  auto model          = qobject_cast<QStandardItemModel*>(comboBox->model ());
  int  contentsLength = 0;
  for (QString const & region : qAsConst (regions))
  {
    QIcon   icon (QString (":/icons/%1.png").arg (region));
    QString regionName = urls->airQualityRegionName (region);
    contentsLength     = std::max (contentsLength, regionName.length ());
    comboBox->addItem (icon, regionName, region);
    if (urls->airQualityUrl (region).isEmpty ())
    {
      int            index = comboBox->count () - 1;
      QStandardItem* item  = model->item (index);
      Qt::ItemFlags  flags = item->flags () & ~Qt::ItemIsEnabled;
      item->setFlags (flags);
    }
  }

  comboBox->setMinimumContentsLength (contentsLength);
}

void CMainWindow::createProviderInfo ()
{
  m_provider  = new QLabel (ui->toolBar);
  m_provider->setTextInteractionFlags (Qt::TextBrowserInteraction);
  QAction* action = ui->toolBar->addWidget (m_provider);
  action->setVisible (true);
  m_provider->setObjectName ("m_provider");
  m_provider->setToolTip (tr ("Data source"));
  connect (m_provider, &QLabel::linkActivated, this, &CMainWindow::showProviderSite);
}

void CMainWindow::createFitInViewButton ()
{
  auto toolButton = new QToolButton (ui->toolBar);
  toolButton->setAutoRaise (true);
  toolButton->setIcon (QIcon (":/icons/fitinview.png"));
  toolButton->setToolTip (CMainWindow::tr ("See all towns"));
  QAction* action = ui->toolBar->addWidget (toolButton);
  action->setVisible (true);
  connect (toolButton, &QToolButton::clicked, this, &CMainWindow::fitInView);
}

void CMainWindow::createMainMenu ()
{
  auto toolButton = new QToolButton (ui->toolBar);
  toolButton->setObjectName ("m_mainMenu");
  toolButton->setAutoRaise (true);
  toolButton->setIcon (QIcon (":/icons/mainmenu.png"));
  toolButton->setToolTip (tr ("Main menu"));
  QAction* action = ui->toolBar->addWidget (toolButton);
  action->setVisible (true);

  auto menu = new QMenu;
  toolButton->setMenu (menu);
  toolButton->setPopupMode (QToolButton::InstantPopup);
  menu->addAction (QIcon (":/icons/restoredefaultdimensions.png"), CMainWindow::tr ("Restore the default dimensions"),
                   this, &CMainWindow::defaultDimensions);
  action = menu->addAction (QIcon (":/icons/clearcache.png"), CMainWindow::tr ("Clean cache"), this, &CMainWindow::cleanCache);
  action->setObjectName ("m_cleanCache");
  menu->addSeparator ();
  menu->addAction (QIcon (":/icons/exportmap.png"), CMainWindow::tr ("Export map"), this, &CMainWindow::exportMap);
  menu->addAction (QIcon (":/icons/exportmovie.png"), CMainWindow::tr ("Export movie"), this, &CMainWindow::exportMovie);
  menu->addSeparator ();
  menu->addAction (QIcon (":/icons/downloadstatistics.png"), CMainWindow::tr ("Download statistics"), this, &CMainWindow::downloadStatistics);
  menu->addAction (QIcon (":/icons/preferences.png"), CMainWindow::tr ("Preferences"), this, &CMainWindow::preferences);
  menu->addSeparator ();
  menu->addAction (QIcon (":/icons/about.png"), CMainWindow::tr ("About"), this, &CMainWindow::about);
  menu->addAction (QIcon (":/icons/factoryreset.png"), CMainWindow::tr ("Factory reset and close"), this, &CMainWindow::factoryReset);
  menu->addSeparator ();
  menu->addAction (QIcon (":/icons/close.png"), CMainWindow::tr ("Close"), this, &QMainWindow::close);
  addSpacer (ui->toolBar, toolBarSpacing);
}

void CMainWindow::createLegendButton ()
{
  auto toolButton = new QToolButton (ui->toolBar);
  toolButton->setAutoRaise (true);
  toolButton->setIcon (QIcon (":/icons/legend.png"));
  toolButton->setToolTip (tr ("Legend"));
  QAction* action = ui->toolBar->addWidget (toolButton);
  action->setVisible (true);

  auto menu = new QMenu;
  toolButton->setMenu (menu);
  toolButton->setPopupMode (QToolButton::InstantPopup);

  using TReceiver = void (CMainWindow::*) (bool);
  using TIR       = QPair<CAQLegend::EIndex, TReceiver>;
  TIR   irs[]     = { TIR (CAQLegend::Unavailable, &CMainWindow::aqiUnavailable),
                      TIR (CAQLegend::Good,        &CMainWindow::aqiGood),
                      TIR (CAQLegend::Medium,      &CMainWindow::aqiMedium),
                      TIR (CAQLegend::Degraded,    &CMainWindow::aqiDegraded),
                      TIR (CAQLegend::Bad,         &CMainWindow::aqiBad),
                      TIR (CAQLegend::VeryBad,     &CMainWindow::aqiVeryBad),
                      TIR (CAQLegend::ExtremlyBad, &CMainWindow::aqiExtremelyBad),
                      TIR (CAQLegend::Event,       &CMainWindow::aqiEvent)
                    };

  for (TIR const & ir : irs)
  {
    SPollutantLegend::SIndex const & aqli = CAQLegend::gblInst ()->at (0).m_indexes.at (ir.first);
    QPixmap                          pxm (48, 48);
    pxm.fill (QColor (aqli.m_color));
    action = new QAction (QIcon (pxm), aqli.m_wording);
    if (ir.second != nullptr)
    {
      connect (action, &QAction::triggered, this, ir.second);
    }

    menu->addAction (action);
  }

  menu->addSeparator ();
  action = new QAction (QIcon (":/icons/legend.png"), tr ("Sub-index scales"));
  menu->addAction (action);
  connect (action, &QAction::triggered, this, &CMainWindow::aqiGlobalLegend);
}

void CMainWindow::createPlayer ()
{
  m_player = new CPlayer (ui->toolBar);
  m_player->setMaximumHeight (24);
  m_player->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
  QAction* action = ui->toolBar->addWidget (m_player);
  action->setVisible (true);
  connect (m_player->frameRateComboBox (), QOverload<int>::of(&QComboBox::currentIndexChanged), this,
           &CMainWindow::frameRateChanged);
  connect (m_player->playButton (),     &QToolButton::clicked,            this, &CMainWindow::playStateChanged);
  connect (m_player->forwardButton (),  &QToolButton::clicked,            this, &CMainWindow::forwardClicked);
  connect (m_player->backwardButton (), &QToolButton::clicked,            this, &CMainWindow::backwardClicked);
  connect (m_player->frameNumber (),    &CSlider::sliderMoved,            this, &CMainWindow::changeFrame);
  connect (m_player->frameNumber (),    &CSlider::valueChanged,           this, &CMainWindow::changeFrame);
  connect (m_player,                    &CPlayer::highestMeanDateChanged, this, &CMainWindow::highestAverage);
  connect (m_player,                    &CPlayer::highestMaxDateChanged,  this, &CMainWindow::highestMax);
  connect (m_player,                    &CPlayer::lowestMeanDateChanged,  this, &CMainWindow::lowestAverage);
  connect (m_player,                    &CPlayer::lowestMaxDateChanged,   this, &CMainWindow::lowestMax);
  addSpacer (ui->toolBar, toolBarSpacing);
}

void CMainWindow::createRegionsComboBox ()
{
  m_regionsComboBox = createToobarComboBox (ui->toolBar, regionIconSize);
  m_regionsComboBox->setToolTip (tr ("Regions"));
  m_regionsComboBox->setMaxVisibleItems (m_urls->airQualities ().size () + 1);
  addRegions (m_regionsComboBox, m_urls);
  connect (m_regionsComboBox, QOverload<int>::of(&QComboBox::activated), this, &CMainWindow::regionChanged);
}

void CMainWindow::createDatesComboBox ()
{
  m_datesComboBox = createToobarComboBox (ui->toolBar, dateIconSize);
  m_datesComboBox->setMinimumContentsLength (12);
  connect (m_datesComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CMainWindow::dateChanged);
}

void CMainWindow::createIndexesComboBox ()
{
  m_indexesComboBox = createToobarComboBox (ui->toolBar);
  m_indexesComboBox->setToolTip (tr ("Global index and index by pollutant"));
  m_indexesComboBox->addItem (tr ("Global"));
  CAQLegend* legend = CAQLegend::gblInst ();
  for (SPollutantLegend const & pl : qAsConst (*legend))
  {
    m_indexesComboBox->addItem (pl.m_name);
  }

  connect (m_indexesComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CMainWindow::indexChanged);
}

void CMainWindow::createToolBarControls ()
{
  createMainMenu ();
  createRegionsComboBox ();
  createDatesComboBox ();
  createIndexesComboBox ();
  createPlayer ();
  createFitInViewButton ();

  addSpacer (ui->toolBar);
  createProviderInfo ();
  createLegendButton ();
  if (!m_regionCode.isEmpty ())
  {
    setToolBarActiveRegion (m_regionCode);
  }
}

void CMainWindow::setToolBarActiveRegion (QString const & region)
{
  int index = m_regionsComboBox->currentIndex ();
  if (region != m_regionsComboBox->itemData (index).toString ())
  {
    for (int i = 0, count = m_regionsComboBox->count (); i < count; ++i)
    {
      QString code = m_regionsComboBox->itemData (i).toString ();
      if (code == region)
      {
        m_regionsComboBox->blockSignals (true);
        m_regionsComboBox->setCurrentIndex (i);
        m_regionsComboBox->blockSignals (false);
        break;
      }
    }
  }
}

void CMainWindow::updatePlayerTickColors (QList<QDate> const & deadlines)
{
  CAirQuality::EIndexType      index = m_mapView->index ();
  QList<QPair<quint8, quint8>> avms  = CAirQualities::gblInst ()->averageMaxs (index, deadlines.size ());
  QList<QPair<QColor, QColor>> colors;
  colors.reserve (avms.size ());
  for (QPair<quint8, quint8> const & avm : qAsConst (avms))
  {
    QPair<QColor, QColor> cs;
    cs.first  = CAirQuality::color (avm.first);
    cs.second = CAirQuality::color (avm.second);
    colors.append (cs);
  }

  m_player->setTickColors (colors);
  prepareMovie (index);
}

void CMainWindow::updateToolBarDateComboBox ()
{
  m_datesComboBox->blockSignals (true);
  m_datesComboBox->clear ();
  CAirQualities* aqs = CAirQualities::gblInst ();
  if (!aqs->isEmpty ())
  {
    QLocale      locale    = QLocale::system ();
    QList<QDate> deadlines = aqs->uniqDeadlines ();
    // The comboBox dates is sorted so that the oldest date is last (the reverse of the slider).
    std::sort (deadlines.begin (), deadlines.end (), std::greater<QDate>());
    QDate toDay        = QDate::currentDate ();
    int   currentIndex = -1;
    m_datesComboBox->clear ();
    for (QDate const & deadline : qAsConst (deadlines))
    {
      QString date     = locale.toString (deadline, QLocale::NarrowFormat);
      int     dayCount = toDay.daysTo (deadline);
      QIcon   icon;
      if (dayCount == 0)
      {
        icon = QIcon (":/icons/today.png");
        currentIndex = m_datesComboBox->count ();
      }
      else if (dayCount > 0)
      {
        icon = QIcon (":/icons/todayp.png");
      }
      else
      {
        icon = QIcon (":/icons/todaym.png");
      }

      m_datesComboBox->addItem (icon, date, dayCount);
    }

    if (currentIndex == -1)
    {
      currentIndex = 0;
    }

    // Dates must have the same direction that slider.
    std::reverse (std::begin (deadlines), std::end (deadlines));
    m_player->setDates (deadlines);
    updatePlayerTickColors (deadlines);
    m_player->setFrameMaximum (m_datesComboBox->count () - 1);
    m_datesComboBox->setCurrentIndex (currentIndex);
    QString toolTip = QString ("%1 %2").arg (m_datesComboBox->count ()).arg (tr ("available dates"));
    m_datesComboBox->setToolTip (toolTip);
    m_movieData->m_currentFrame = m_datesComboBox->count () - 1 - currentIndex;
    m_player->setFrame (m_movieData->m_currentFrame);
  }

  m_datesComboBox->blockSignals (false);
}

void CMainWindow::defaultDimensions (bool)
{
  pausePlayer ();
  QSize size = defaultDims ();
  setWindowState (Qt::WindowNoState);
  resize (size);
}

void CMainWindow::preferences (bool)
{
  pausePlayer ();
  CDownloader::abortAll ();
  CPreferences pref (*m_downloaderData, *m_imageExporterParams, *m_displayData, this);
  if (pref.exec () == QDialog::Accepted)
  {
    m_downloaderCache->setProperties (m_downloaderData->m_cacheClean, m_downloaderData->m_cacheValidity,
                                      m_downloaderData->m_cacheSize);
    m_newDataTimer.setInterval (m_downloaderData->m_newDataDelay);
    m_mapView->setPolygonOpacity (m_displayData->m_opacity);
  }
}

void CMainWindow::downloadStatistics (bool)
{
  pausePlayer ();
  CDownloader::abortAll ();
  CDownloadStatistics::SStats stats;
  CAirQualities* aqs           = CAirQualities::gblInst ();
  CUrls*         urls          = CUrls::gblInst ();
  QString        cacheFileName = m_downloaderCache->fileName (urls->airQualityUrl (m_regionCode));
  QFileInfo      fi (cacheFileName);

  if (fi.exists ())
  {
    stats.m_title              = QString ("%1 (%2)").arg (urls->airQualityRegionName (m_regionCode), m_regionCode);
    stats.m_url                = urls->airQualityUrl (m_regionCode);
    stats.m_downloadedDateTime = fi.fileTime (QFileDevice::FileModificationTime);
    stats.m_serverDateTime     = fi.birthTime ();
    stats.m_firstDate          = aqs->first ().deadline ();
    stats.m_lastDate           = aqs->last ().deadline ();
    stats.m_airQualityCount    = aqs->size ();
    QSet<QString> towns;
    for (CAirQuality const & aq : *aqs)
    {
      towns.insert (aq.code ());
    }

    stats.m_townCount    = towns.size ();
    stats.m_downloadTime = m_downloadTime;
  }

  CDownloadStatistics ds (this);
  ds.setStats (stats);
  ds.exec ();
}

void CMainWindow::cleanCache (bool)
{
  pausePlayer ();
  CDownloader::abortAll ();
  m_downloaderCache->clear ();
}

void CMainWindow::factoryReset (bool)
{
  pausePlayer ();
  QString text  = tr ("\
You have chosen to reset Aqic to factory mode. All the data of your cession as well as the \
cached data will be erased. Aqic will be in the state right after installation and before the first launch.\
You will be able to relaunch Aqic without any problem. \
Only your preferences will have to be validated again.\n\n\
Click on Apply to validate this operation. Aqic will close.");
  QMessageBox::StandardButton btn = QMessageBox::warning (this, CAppLocations::location (CAppLocations::AppName), text,
                                                          QMessageBox::Apply | QMessageBox::Cancel);
  if (btn == QMessageBox::Apply)
  {
    CStatus::add (FactoryResetOnClose);
    QMainWindow::close ();
  }
}

void CMainWindow::about (bool)
{
  pausePlayer ();
  CAbout about (this);
  about.exec ();
}

void CMainWindow::showProviderSite (QString const & link)
{
  if (link == m_newDataUrl)
  {
    downloadAirQualities (m_regionCode, FromRegion, true);
  }
  else
  {
    QUrl url (link);
    QDesktopServices::openUrl (url);
  }
}

void CMainWindow::aqiUnavailable (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::Unavailable, this);
  legend.exec ();
}

void CMainWindow::aqiGood (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::Good, this);
  legend.exec ();
}

void CMainWindow::aqiMedium (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::Medium, this);
  legend.exec ();
}

void CMainWindow::aqiDegraded (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::Degraded, this);
  legend.exec ();
}

void CMainWindow::aqiBad (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::Bad, this);
  legend.exec ();
}

void CMainWindow::aqiVeryBad (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::VeryBad, this);
  legend.exec ();
}

void CMainWindow::aqiExtremelyBad (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::ExtremlyBad, this);
  legend.exec ();
}

void CMainWindow::aqiEvent (bool)
{
  CLegend legend (CAQLegend::gblInst (), CAQLegend::Event, this);
  legend.exec ();
}

void CMainWindow::aqiGlobalLegend (bool)
{
  CGlobalLegend legend (this);
  legend.exec ();
}

void CMainWindow::fitInView (bool)
{
  Q_ASSERT (m_mapView != nullptr);
  m_mapView->fitInView ();
}

QSize CMainWindow::defaultDims ()
{
  QLayout*        layout  = ui->toolBar->layout ();
  int             spacing = layout->spacing ();
  QList<QWidget*> widgets = ui->toolBar->findChildren<QWidget*> (QString (), Qt::FindDirectChildrenOnly);
  int             width   = 0, height = 0;
  if (!ui->toolBar->isFloating ())
  {
    if (ui->toolBar->orientation () == Qt::Horizontal)
    {
      width += spacing;
      for (QWidget* widget : qAsConst (widgets))
      {
        int w;
        if (widget->objectName () == "m_provider")
        {
          QString text = m_urls->biggestProviderStringLen ();
          QFont   font = widget->font ();
          QFontMetrics fm (font);
          w = fm.horizontalAdvance (text);
        }
        else if (!widget->updatesEnabled ())
        {
          w = widget->minimumWidth ();
        }
        else
        {
          w = widget->sizeHint ().width ();
          if (w <= 0)
          {
            w = widget->frameSize ().width ();
          }
        }

        width += w + spacing;
      }

      width += spacing;
    }
    else
    {
      height += spacing;
      for (QWidget* widget : qAsConst (widgets))
      {
        int h = widget->sizeHint ().height ();
        if (h == 0)
        {
          h  = widget->frameSize ().height ();
        }

        height += h + spacing;
      }

      height += spacing;
    }
  }

  width  = std::max (width, 900);
  height = std::max (height, 600);
  qDebug () << "Default dimensions: " << width << 'x' << height;
  return QSize (width, height);
}

void CMainWindow::mainMenuActionSetEnabled (bool enabled)
{
  auto            button  = ui->toolBar->findChild<QToolButton*> ("m_mainMenu");
  QMenu*          menu    = button->menu ();
  QList<QAction*> actions = menu->actions ();
  for (QAction* action : qAsConst (actions))
  {
    if (action->objectName () == QStringLiteral ("m_cleanCache"))
    {
      action->setEnabled (enabled);
      break;
    }
  }
}

void CMainWindow::exportMap (bool)
{
  exportImages (true);
}

void CMainWindow::exportMovie (bool)
{
  exportImages (false);
}
