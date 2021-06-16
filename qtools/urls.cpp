#include "urls.hpp"
#include "../qtools/applocations.hpp"
#include <QSettings>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CUrls::CUrls ()
{
  initialize ();
}

void CUrls::initialize ()
{
  QDir dir;
  dir.setPath (CAppLocations::location (CAppLocations::ConfigLocation));
  QString iniFile = dir.absoluteFilePath ("config.ini");
  if (QFile::exists (iniFile))
  {
    QSettings settings (iniFile, QSettings::IniFormat);
    settings.setIniCodec ("UTF-8");
    settings.beginGroup ("urls");
    m_externalIPAddr = settings.value ("externalIPAddr", "https://api.ipify.org/?format=json").toString ();
    m_ipAddrPosition = settings.value ("ipAddrPosition", "http://ip-api.com/json/").toString ();
    settings.endGroup ();
  }
  else
  {
    m_externalIPAddr = "https://api.ipify.org/?format=json";
    m_ipAddrPosition = "http://ip-api.com/json/";
  }

  iniFile = dir.absoluteFilePath ("regions.json");
  QFile file (iniFile);
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
        QJsonArray jsonRegions = doc.array ();
        for (QJsonValue const & jsonValue : qAsConst (jsonRegions))
        {
          QJsonObject jsonRegion           = jsonValue.toObject ();
          QString     region               = jsonRegion.value ("code").toString ();
          QString     provider             = jsonRegion.value ("provider").toString ();
          QString     name                 = jsonRegion.value ("name").toString ();
          QJsonObject airQuality           = jsonRegion.value ("airquality").toObject ();
          QString     url                  = airQuality.value ("url").toString ();
          m_airQualities[region].m_strings = { url, provider, region, name};
        }
      }
    }
  }
}

QString CUrls::biggestProviderStringLen () const
{
  QString biggest;
  int     sizeMax = 0;
  for (TAirQualities::const_iterator it = m_airQualities.cbegin (), end = m_airQualities.cend (); it != end; ++it)
  {
    QString name  = it.value ().m_strings.at (Provider);
    int     index = name.indexOf (QLatin1Char ('>'));
    if (index != -1)
    {
      name = name.mid (index + 1);
    }

    name.remove (QRegExp ("</a>"));
    int size = name.size ();
    if (size > sizeMax)
    {
      sizeMax = size;
      biggest = name;
    }
  }

  return biggest;
}

