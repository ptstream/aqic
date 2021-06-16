#include "datastream.hpp"
#include <QGeoLocation>
#include <QGeoAddress>
#include <QGeoCoordinate>
#include <QGeoRectangle>

CDataStream::CDataStream (QByteArray* bytes, QIODevice::OpenMode openMode) : m_stream (bytes, openMode)
{
}

CDataStream& CDataStream::operator << (QGeoLocation const & loc)
{
  m_stream.setVersion (QDataStream::Qt_5_12);
  quint16 const  majorVersion = 0;
  quint16 const  minorVersion = 0;
  QGeoAddress    addr         = loc.address ();
  m_stream << majorVersion
           << minorVersion
           << loc.coordinate ()
           << loc.boundingBox ()
           << addr.countryCode ()
           << addr.country ()
           << addr.state ()
           << addr.city ()
           << addr.district ()
           << addr.street ()
           << addr.postalCode ()
           << addr.text ();
  return *this;
}

CDataStream& CDataStream::operator >> (QGeoLocation& loc)
{
  quint16        majorVersion, minorVersion;
  QGeoCoordinate coord;
  QGeoRectangle  boundingBox;
  QString        countryCode, country, state, city, district, street, postalCode, text;
  m_stream >> majorVersion
           >> minorVersion
           >> coord
           >> boundingBox
           >> countryCode
           >> country
           >> state
           >> city
           >> district
           >> street
           >> postalCode
           >> text;

  QGeoAddress addr;
  addr.setCountryCode (countryCode);
  addr.setCountry (country);
  addr.setState (state);
  addr.setCity (city);
  addr.setDistrict (district);
  addr.setStreet (street);
  addr.setPostalCode (postalCode);
  addr.setText (text);

  loc.setAddress (addr);
  loc.setCoordinate (coord);
  loc.setBoundingBox (boundingBox);
  return *this;
}
