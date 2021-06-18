#ifndef IMAGEEXPORTER_HPP
#define IMAGEEXPORTER_HPP

#include <QImage>

class QWidget;
class QImage;

/*! \brief Data use by image exporter. */
struct SImageExporterParams
{
  /*! Default constructor. */
  SImageExporterParams () = default;

  /*! Copy constructor. */
  SImageExporterParams (SImageExporterParams const & other) = default;

  /*! Equality operator. */
  bool operator == (SImageExporterParams const & other)
  {
    return m_resolution   == other.m_resolution      &&
        m_quality         == other.m_quality         &&
        m_whiteBackground == other.m_whiteBackground &&
        m_format          == other.m_format;

  }

  /*! Inequality operator. */
  bool operator != (SImageExporterParams const & other) { return !(*this == other); }

  int        m_resolution      = 2048;  //!< Resolution in pixels.
  int        m_quality         = -1;    //!< image quality [0, 100]
  QByteArray m_format          = "JPG"; //!< Format of file
  bool       m_whiteBackground = false; //!< Change background color to white for printing.
};

/*! \brief This class provides functions to save on disk the graphic content of a widget. */
class CImageExporter
{
public:
  /*! Default constructor. */
  CImageExporter () = default;

  /*! Returns the resolution in pixels. */
  int resolution () const { return m_params.m_resolution; }

  /*! Returns the white background flag. */
  bool whiteBackground () const { return m_params.m_whiteBackground; }

  /*! Returns the image quality. */
  int quality () const { return m_params.m_quality; }

  /*! Returns the image format (JPG, PNG). */
  QByteArray const & format () const { return m_params.m_format; }

  /*! Returns the image width */
  int imageWidth () const { return m_imageWidth; }

  /*! Returns the image Height */
  int imageHeight () const { return m_imageHeight; }

  /*! Returns the exporter data. */
  SImageExporterParams const & params () const { return m_params; }

  /*! Returns the image file name. */
  QString const & fileName () const { return m_fileName; }

  /*! Sets the resolution in pixels. */
  void setResolution (int resolution) { m_params.m_resolution = resolution; }

  /*! Sets the white background flag. */
  void setWhiteBackground (bool whiteBackground) { m_params.m_whiteBackground = whiteBackground; }

  /*! Sets the image quality. */
  void setQuality (int quality);

  /*! Sets the image format (JPG, PNG). */
  void setFormat (QByteArray const & format) { m_params.m_format = format; }

  /*! Sets the exporter data. */
  void setParams (SImageExporterParams const & params) { m_params = params; }

  /*! Sets the image file name. */
  void setFileName (QString const & fileName) { m_fileName = fileName; }

  /*! Returns the image of the widget content. */
  QImage generate (QWidget* widget);

  /*! Saves image on disk. */
  QString saveImage (QImage const & image);

  /*! Saves image on disk. */
  bool saveImage (QString const & fileName, QImage const & image);

  /*! Saves image on disk. */
  static QString saveImage (QImage const & image, QByteArray const & format, int quality);

  /*! Saves image on disk. */
  static bool saveImage (QString const & fileName, QImage const & image, QByteArray const & format, int quality);

  /*! Scale an image. */
  static void scaleImage (int& width, int& height, int resolution);

protected:
  SImageExporterParams m_params;
  int                  m_imageWidth  = 0;
  int                  m_imageHeight = 0;
  QString              m_fileName;
};

#endif // IMAGEEXPORTER_HPP
