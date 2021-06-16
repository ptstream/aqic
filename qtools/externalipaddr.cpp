#include "externalipaddr.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

QString CExternalIPAddr::addr (QByteArray const & data)
{
  QString addr;
  if (!data.isEmpty ())
  {
    QJsonParseError     error;
    QJsonDocument       doc (QJsonDocument::fromJson (data, &error));
    QJsonObject const & json = doc.object ();
    if (error.error == QJsonParseError::NoError)
    {
      addr = json["ip"].toString ();
    }
    else
    {
      qWarning () << "External IP address error: " << error.errorString ();
    }
  }
  else
  {
    qWarning () << "Could not decode external IP address from empty data";
  }

  return addr;
}
