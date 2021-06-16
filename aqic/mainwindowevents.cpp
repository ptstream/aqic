#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "mapview.hpp"
#include "downloaderdata.hpp"
#include "imageexporterparams.hpp"
#include "displaydata.hpp"
#include "../wtools/magglass.hpp"
#include "../qtools/lastcession.hpp"
#include "../qtools/downloader.hpp"
#include "../qtools/applocations.hpp"
#include "../qtools/downloadcache.hpp"
#include <QDir>

void CMainWindow::closeEvent (QCloseEvent*)
{
  m_newDataTimer.stop ();
  pausePlayer ();
  CDownloader::abortAll ();
  if (!CStatus::contains (FactoryResetOnClose))
  {
    SLastCession lastCession (true);
    double lat, lon, zl;
    m_mapView->mapProperties (lat, lon, zl);
    lastCession.setReal      ("location", "map_latitude",   lat);
    lastCession.setReal      ("location", "map_longitude",  lon);
    lastCession.setReal      ("location", "map_zoom_level", zl);
    lastCession.setByteArray ("location", "region",         m_regionCode.toUtf8 ());
    lastCession.setByteArray ("ui",       "main_window",    saveGeometry ());
    lastCession.setInt       ("ui",       "simplify_coef",  m_simplifyCoef);
    m_downloaderData->save (lastCession);
    m_imageExporterParams->save (lastCession);
    m_displayData->save (lastCession);
    if (m_downloaderData->m_cacheClean)
    {
      removeDownloadData ();
    }
  }
  else
  {
    removeSessionData ();
  }

  m_downloaderCache->clearDebugFiles ();
}

void CMainWindow::changeEvent (QEvent* event)
{
  if (event->type () == QEvent::LanguageChange)
  {
    ui->retranslateUi (this); // Retranslate designer forms
  }

  QWidget::changeEvent (event); // Remember to call base class implementation
}

void CMainWindow::keyPressEvent (QKeyEvent* event)
{
  static QMetaMethod const downloadFinishedSignal = QMetaMethod::fromSignal(&CDownloader::downloadFinished);
  if (!isSignalConnected (downloadFinishedSignal))
  {
    int modifiers = event->modifiers ();
    if (modifiers == Qt::NoModifier || modifiers == Qt::ControlModifier)
    {
      int key = event->key ();
      if (key == Qt::Key_F5 || (key == Qt::Key_R && modifiers == Qt::ControlModifier))
      {
        downloadAirQualities (m_regionCode, FromRegion, true);
      }
    }
  }

  QMainWindow::keyPressEvent (event);
}
