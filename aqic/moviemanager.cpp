#include "mainwindow.hpp"
#include "mapview.hpp"
#include "moviedata.hpp"
#include "imageexporterparams.hpp"
#include "exportmovieresults.hpp"
#include "displaydata.hpp"
#include "../wtools/player.hpp"
#include "../wtools/slider.hpp"
#include "../qtools/applocations.hpp"
#include <QToolButton>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QPainter>

void CMainWindow::movieUpdateFrame ()
{
  if (!m_movieData->isEmpty ())
  {
    m_mapView->updateTowns (m_movieData->at (m_movieData->m_currentFrame));
    m_datesComboBox->blockSignals (true);
    m_datesComboBox->setCurrentIndex (m_movieData->size () - m_movieData->m_currentFrame - 1);
    m_datesComboBox->blockSignals (false);
    m_player->frameNumber ()->blockSignals (true);
    m_player->setFrame (m_movieData->m_currentFrame);
    float fps = 1000.0F * (++m_movieData->m_frameCount) / m_frameRate.elapsed ();
    m_player->setFrameRate (fps);
    m_player->frameNumber ()->blockSignals (false);
  }
}

//void CMainWindow::movieUpdateBoundingCircle ()
//{
//  if (!m_movieData->isEmpty ())
//  {
//  }
//}

void CMainWindow::movieNextFrame ()
{
  m_movieData->incFrameNumber ();
  movieUpdateFrame ();
}

void CMainWindow::prepareMovie (CAirQuality::EIndexType index)
{
  CAirQualities* aqsi  = CAirQualities::gblInst ();
  QList<QDate>   dates = aqsi->dates ();
  m_movieData->clear ();
  m_movieData->reserve (dates.size ());
  for (QDate const & date : qAsConst (dates))
  {
    TTAQs aqs = aqsi->airQualities (date);
    m_movieData->append (SAQs (aqs, index));
  }

  m_movieData->squeeze ();
  m_movieData->m_timerInterval = m_player->frameRate ();
  m_movieData->m_frameCount    = 0LL;

  m_player->setFrameMaximum (m_movieData->size () - 1);
  m_timer.setInterval (m_movieData->m_timerInterval);
}

void CMainWindow::frameRateChanged (int)
{
  m_movieData->m_timerInterval = m_player->frameRate ();
  m_timer.setInterval (m_movieData->m_timerInterval);
  m_movieData->m_frameCount = 0LL;
  m_frameRate.restart ();
}

void CMainWindow::pausePlayer ()
{
  if (m_player->isRunning ())
  {
    m_player->playStateChanged ();
  }

  m_timer.stop ();
  m_mapView->setTooltipActivated (true);
}

void CMainWindow::startPlayer ()
{
  m_mapView->setTooltipActivated (false);
  m_timer.start ();
  m_frameRate.restart ();
}

void CMainWindow::playStateChanged (bool)
{
  if (m_player->isRunning ())
  {
    startPlayer ();
  }
  else
  {
    pausePlayer ();
  }

  m_movieData->m_frameCount = 0LL;
}

void CMainWindow::forwardClicked (bool)
{
  pausePlayer ();
  m_movieData->incFrameNumber ();
  movieUpdateFrame ();
}

void CMainWindow::backwardClicked (bool)
{
  pausePlayer ();
  m_movieData->decFrameNumber ();
  movieUpdateFrame ();
}

void CMainWindow::changeFrame (int position)
{
  m_player->restoreAMM ();
  pausePlayer ();
  m_movieData->m_currentFrame = position;
  movieUpdateFrame ();
}

void CMainWindow::highestAverage ()
{
  pausePlayer ();
  m_movieData->m_currentFrame = m_movieData->greatestAverageFrame ();
  movieUpdateFrame ();
}

void CMainWindow::highestMax ()
{
  pausePlayer ();
  quint8                  value;
  CAirQuality::EIndexType indexType;
  m_movieData->m_currentFrame = m_movieData->greatestMaxFrame (value, indexType);
  movieUpdateFrame ();
  if (m_displayData->m_circle)
  {
    m_mapView->updateBoundingCircle (value, indexType);
  }
}

void CMainWindow::lowestAverage ()
{
  pausePlayer ();

  m_movieData->m_currentFrame = m_movieData->smallestAverageFrame ();
  movieUpdateFrame ();
}

void CMainWindow::lowestMax ()
{
  pausePlayer ();
  quint8                  value;
  CAirQuality::EIndexType indexType;
  m_movieData->m_currentFrame = m_movieData->smallestMaxFrame (value, indexType);
  movieUpdateFrame ();
  if (m_displayData->m_circle)
  {
    m_mapView->updateBoundingCircle (value, indexType);
  }
}

static QString imageExportedFileName (QWidget* parent, QString& folder, QString const & format)
{
  QString suffix;
  QString fileFormat = "*." + format.toLower ();
  if (folder.isEmpty ())
  {
    folder = CAppLocations::location (CAppLocations::Images);
  }

  QString fileName = QFileDialog::getSaveFileName (parent, CMainWindow::tr ("Save the image"), folder, fileFormat, &suffix);
  if (!fileName.isEmpty ())
  {
    QFileInfo fi (fileName);
    folder = fi.absolutePath ();
    QString fileNameSuffix = fi.suffix ();
    if (fileNameSuffix.isEmpty ())
    {
      suffix = suffix.mid (1); // Remove * of *.png
      fileName.append (suffix);
      suffix = suffix.mid (1); // Remove . of .png
    }
    else
    {
      suffix = fileNameSuffix;
    }
  }

  return fileName;
}

static QString imageExportedFolder (QWidget* parent, QString& folder)
{
  QString tmp = folder;
  if (tmp.isEmpty ())
  {
    tmp = CAppLocations::location (CAppLocations::Images);
  }

  tmp = QFileDialog::getExistingDirectory (parent, CMainWindow::tr ("Open Directory"),
                                           tmp, QFileDialog::ShowDirsOnly);
  if (!tmp.isEmpty ())
  {
    folder = tmp;
  }

  return tmp;
}

static void processAllEvents ()
{
  QCoreApplication::sendPostedEvents ();
  QCoreApplication::processEvents ();
}

static void addDate (QImage& image, QString const & date, SImageExporterParams const & params)
{
  QPainter painter (&image);
  QFont font = painter.font ();
  font.fromString (params.m_dateFont);
  painter.setFont (font);
  painter.setPen (QColor::fromRgb (params.m_dateColor));

  Qt::Alignment alignmenst[] = { Qt::AlignLeft    | Qt::AlignTop,
                                Qt::AlignHCenter | Qt::AlignTop,
                                Qt::AlignRight   | Qt::AlignTop,
                                Qt::AlignLeft    | Qt::AlignBottom,
                                Qt::AlignHCenter | Qt::AlignBottom,
                                Qt::AlignRight   | Qt::AlignBottom,
                                Qt::AlignLeft    | Qt::AlignVCenter,
                                Qt::AlignRight   | Qt::AlignVCenter,
                              };

  painter.drawText (image.rect (), alignmenst[params.m_alignment], date);
}

void CMainWindow::exportImages(bool one)
{
  if (one)
  {
    QString fileName = ::imageExportedFileName (this, m_imageExporterParams->m_folder, m_imageExporterParams->m_format);
    if (!fileName.isEmpty ())
    {
      QImage image = m_mapView->grabWindow ();
      bool   ok    = !image.isNull ();
      if (ok)
      {
        if (m_imageExporterParams->m_date)
        {
          QString date = m_datesComboBox->currentText ();
          addDate (image, date, *m_imageExporterParams);
        }

        ok = image.save (fileName, m_imageExporterParams->m_format.toUtf8 ().constData (), m_imageExporterParams->m_quality);
        if (ok && m_imageExporterParams->m_launchViewer)
        {
          QUrl url (fileName);
          QDesktopServices::openUrl (url);
        }
      }
    }
  }
  else
  {
    if (!::imageExportedFolder (this, m_imageExporterParams->m_folder).isEmpty ())
    {
      QDir dir (m_imageExporterParams->m_folder);
      dir.mkdir (m_regionCode);
      dir.cd (m_regionCode);
      int exportedImageCount = 0, imageCount = m_datesComboBox->count ();
      for (int i = 0; i < imageCount; ++i)
      {
        m_movieData->m_currentFrame = i;
        movieUpdateFrame ();
        processAllEvents ();
        QImage image = m_mapView->grabWindow ();
        bool   ok    = !image.isNull ();
        if (ok)
        {
          QString date = m_datesComboBox->currentText ();
          if (m_imageExporterParams->m_date)
          {
            addDate (image, date, *m_imageExporterParams);
          }

          QString fileName = date;
          fileName.replace ('/', '-');
          fileName += (m_imageExporterParams->m_format == "JPG" ? ".jpg" : ".png");
          fileName  = dir.absoluteFilePath (fileName);
          ok        = image.save (fileName, m_imageExporterParams->m_format.toUtf8 ().constData (), m_imageExporterParams->m_quality);
          if (ok)
          {
            ++exportedImageCount;
          }
        }
      }

      CExportMovieResults emr (exportedImageCount, imageCount, dir.absolutePath (), this);
      emr.exec ();
    }
  }
}

