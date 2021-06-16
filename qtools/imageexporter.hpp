#ifndef IMAGEEXPORTER_HPP
#define IMAGEEXPORTER_HPP

#include <QImage>

class QWidget;
class QImage;

/*! \brief Data use by image exporter. */
struct SImageExporterParams
{
  SImageExporterParams () = default;
  SImageExporterParams (SImageExporterParams const & other) = default;

  bool operator == (SImageExporterParams const & other)
  {
    return m_resolution   == other.m_resolution      &&
        m_quality         == other.m_quality         &&
        m_whiteBackground == other.m_whiteBackground &&
        m_format          == other.m_format;

  }

  bool operator != (SImageExporterParams const & other) { return !(*this == other); }

  int        m_resolution      = 2048;  //!< Resolution in pixels.
  int        m_quality         = -1;    //!< image quality [0, 100]
  QByteArray m_format          = "JPG"; //!< Format of file
  bool       m_whiteBackground = false; //!< Change background color to white for printing.
};

/*! \brief This class provides functions to save on disk the contentof a widget. */
class CImageExporter
{
public:
  CImageExporter () = default;

  int resolution () const { return m_params.m_resolution; }
  bool whiteBackground () const { return m_params.m_whiteBackground; }
  int quality () const { return m_params.m_quality; }
  QByteArray const & format () const { return m_params.m_format; }
  int imageWidth () const { return m_imageWidth; }
  int imageHeight () const { return m_imageHeight; }
  SImageExporterParams const & params () const { return m_params; }
  QString const & fileName () const { return m_fileName; }

  void setResolution (int resolution) { m_params.m_resolution = resolution; }
  void setWhiteBackground (bool whiteBackground) { m_params.m_whiteBackground = whiteBackground; }
  void setQuality (int quality);
  void setFormat (QByteArray const & format) { m_params.m_format = format; }
  void setParams (SImageExporterParams const & params) { m_params = params; }
  void setFileName (QString const & fileName) { m_fileName = fileName; }

  /*! Returns the image of the widget. */
  QImage generate (QWidget* widget);

  /*! Saves image on disk. */
  QString saveImage (QImage const & image);

  /*! Saves image on disk. */
  bool saveImage (QString const & fileName, QImage const & image);

  /*! Saves image on disk. */
  static QString saveImage (QImage const & image, QByteArray const & format, int quality);

  /*! Saves image on disk. */
  static bool saveImage(QString const & fileName, QImage const & image, QByteArray const & format, int quality);

  /*! Scale an image. */
  static void scaleImage (int& width, int& height, int resolution);

protected:
  SImageExporterParams m_params;
  int                  m_imageWidth  = 0;
  int                  m_imageHeight = 0;
  QString              m_fileName;
};

#endif // IMAGEEXPORTER_HPP
