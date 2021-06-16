#include "ipaddrposition.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

void CIPAddrPosition::init (QByteArray const & data)
{
  m_state = Fail;
  QGeoCoordinate coords (0.0, 0.0);
  if (!data.isEmpty ())
  {
    QJsonParseError     error;
    QJsonDocument       doc (QJsonDocument::fromJson (data, &error));
    QJsonObject const & json = doc.object ();
    if (error.error == QJsonParseError::NoError)
    {
      if (json["status"].toString () == "success")
      {
        double lat = json["lat"].toDouble (0.0);
        double lon = json["lon"].toDouble (0.0);
        if (lat != 0.0)
        {
          m_location.setLatitude (lat);
          m_location.setLongitude (lon);
          m_state = Done;
        }

        m_town = json["city"].toString ();
      }
      else
      {
        qWarning () << "IP address position. Server return failure";
      }
    }
    else
    {
      qWarning () << "IP address position error: " << error.errorString ();
    }
  }
  else
  {
    qWarning () << "Could not decode external IP address position from empty data";
  }
}
