#include "imageexporter.hpp"
#include "applocations.hpp"
#include <QPainter>
#include <QWidget>
#include <QTemporaryFile>
#include <QDir>

static void prepareRendering (QPainter& painter, QRectF const & rect, QWidget* widget)
{
  QRectF pageRect     = rect;
  qreal  pageWidth    = pageRect.width ();
  qreal  pageHeight   = pageRect.height ();
  qreal  widgetWidth  = widget->width ();
  qreal  widgetHeight = widget->height ();

  qreal  xscale = pageWidth  / widgetWidth;
  qreal  yscale = pageHeight / widgetHeight;
  qreal  scale  = qMin (xscale, yscale);

  painter.translate (pageRect.x () + pageWidth / 2.0, pageRect.y () + pageHeight / 2.0);
  painter.scale (scale, scale);
  painter.translate (-widgetWidth / 2.0, -widgetHeight / 2.0);
}

QString CImageExporter::saveImage (QImage const & image)
{
  return saveImage (image, m_params.m_format, m_params.m_quality);
}

bool CImageExporter::saveImage (QString const & fileName, QImage const & image)
{
  return saveImage (fileName, image, m_params.m_format, m_params.m_quality);
}

QString CImageExporter::saveImage (QImage const & image, QByteArray const & format, int quality)
{
  QTemporaryFile file;
  QDir           dir (QDir::tempPath ());
  file.setFileTemplate (dir.absoluteFilePath ("atmpol_image_XXXXXX.") + format.toLower ());
  file.setAutoRemove (false);
  file.open ();
  image.save (&file, format.constData (), quality);
  file.close ();
  return file.fileName ();
}

bool CImageExporter::saveImage (QString const & fileName, QImage const & image, QByteArray const & format, int quality)
{
  return image.save (fileName, format.constData (), quality);
}

void CImageExporter::scaleImage (int& width, int& height, int resolution)
{
  float ratio  = static_cast<float>(width) / static_cast<float>(height);
  int   w, h;
  if (ratio > 1.0F)
  {
    w = resolution;
    h = ::qRound (static_cast<float>(w) / ratio);
  }
  else
  {
    h = resolution;
    w = ::qRound (static_cast<float>(h) * ratio);
  }

  width  = w;
  height = h;
}

QImage CImageExporter::generate (QWidget* widget)
{
  QSize size    = widget->size ();
  m_imageWidth  = size.width ();
  m_imageHeight = size.height ();
  scaleImage (m_imageWidth, m_imageHeight, m_params.m_resolution);
  QImage image (m_imageWidth, m_imageHeight, QImage::Format_RGBA8888);
  QPainter painter;
  painter.begin (&image);
  prepareRendering (painter, QRectF (0.0, 0.0, m_imageWidth, m_imageHeight), widget);
  widget->render (&painter);
  painter.end ();
  return image;
}

void CImageExporter::setQuality (int quality)
{
  if (m_params.m_quality < -1)
  {
    m_params.m_quality = -1;
  }
  else
  {
    m_params.m_quality = std::min (100, quality);
  }
}
