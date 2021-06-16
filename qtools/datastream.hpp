#ifndef DATASTREAM_HPP
#define DATASTREAM_HPP

#include <QDataStream>

class QGeoLocation;

/*! \brief The CDataStream class provides serialization of QGeoLocation to an array of bytes. */
class CDataStream
{
public:
  /*! Constructor. */
  CDataStream (QByteArray* bytes, QIODevice::OpenMode openMode);

  /*! Write QGeoLocation. */
  CDataStream& operator << (QGeoLocation const & loc);

  /*! Read QGeoLocation. */
  CDataStream& operator >> (QGeoLocation& loc);

protected:
  QDataStream m_stream; //!< The array of bytes containing or accepting QGeoLocation.
};

#endif // DATASTREAM_HPP
