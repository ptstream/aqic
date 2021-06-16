#include "towncodes.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

CTownCodes::CTownCodes (QString const & fileName)
{
  if (!fileName.isEmpty ())
  {
    QFile file (fileName);
    if (file.open (QIODevice::ReadOnly))
    {
      QByteArray data = file.readAll ();
      file.close ();
      if (!data.isEmpty ())
      {
        QJsonParseError error = {};
        QJsonDocument   doc (QJsonDocument::fromJson (data, &error));
        if (error.error == QJsonParseError::NoError)
        {
          QJsonArray jsonArray = doc.array ();
          for (QJsonValue const & value : qAsConst (jsonArray))
          {
            QJsonObject jsonObject = value.toObject ();
            QJsonArray  codes      = jsonObject.value ("codes").toArray ();
            QString     equivalent = jsonObject.value ("equivalent").toString ();
            for (QJsonValue const & code : qAsConst (codes))
            {
              insert (code.toString (), equivalent);
            }
          }
        }
      }
    }
  }
}

