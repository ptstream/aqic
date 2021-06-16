#include "towns.hpp"
#include "../qtools/stringmanip.hpp"
#include "../qtools/applocations.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

static quint32 magicNumber = 0x00001002; // 0x00000002->CTown; 0x00001000->set of towns
static quint32 version     = 0x00000000;

using namespace spart;

CTowns::CTowns (QStringList const & fileNames)
{
  load (fileNames);
}

static CTown::TPath createPolygon (QJsonArray polygon)
{
  CTown::TPath path;
  if (!polygon.isEmpty ())
  {
    QJsonValue const & location    = polygon.first ();
    QJsonArray         coordinates = location.toArray ();
    for (QJsonArray::const_iterator it = coordinates.constBegin (), end = coordinates.constEnd () - 1; it != end; ++it)
    {
      QJsonArray coordinate = (*it).toArray ();
      if (coordinate.size () == 2)
      {
        float lat = static_cast<float>(coordinate[1].toDouble ());
        float lon = static_cast<float>(coordinate[0].toDouble ());
        path.append (TVertex (lat, lon));
      }
    }
  }

  return path;
}

QDataStream& operator << (QDataStream& out, CTowns const & towns)
{
  out << magicNumber;
  out << version;
  out << static_cast<quint32>(towns.size ());
  for (CTown const & town : towns)
  {
    out << town;
  }

  out << towns.epcis ();
  return out;
}

// https://geoservices.ign.fr/documentation/diffusion/telechargement-donnees-libres.html#admin-express (communes.geojson)
void CTowns::loadPath (QStringList const & fileNames)
{
  if (fileNames.isEmpty ())
  {
    return;
  }

  for (QString const & fileName : fileNames)
  {
    if (!fileName.isEmpty ())
    {
      QByteArray data;
      QFile      file (fileName);
      if (file.open (QIODevice::ReadOnly))
      {
        data = file.readAll ();
        file.close ();

        QJsonParseError error = {};
        QJsonDocument   doc (QJsonDocument::fromJson (data, &error));
        if (error.error == QJsonParseError::NoError)
        {
          QJsonObject json     = doc.object ();
          QJsonArray  features = json.value ("features").toArray ();
          if (!features.isEmpty ())
          {
            reserve (features.size ());
            for (QJsonValue const & feature : qAsConst (features))
            {
              QJsonObject     objFeature  = feature.toObject ();
              QJsonObject     geometry    = objFeature.value ("geometry").toObject ();
              QString         polygonType = geometry.value ("type").toString ();
              QJsonArray      polygons    = geometry.value ("coordinates").toArray ();
              if (!polygons.isEmpty ())
              {
                QJsonObject properties = objFeature.value ("properties").toObject ();
                QString     code, name, region;
                region       = properties.value ("INSEE_REG").toString ();
                code         = properties.value ("INSEE_COM").toString ();
                name         = properties.value ("NOM_COM").toString ();
                QString epci = properties.value ("CODE_EPCI").toString ();
                m_epcis.insert (epci, code);

                if (!name.isEmpty ())
                {
                  CTown::TPaths paths;
                  QJsonArray locations = polygons.first ().toArray ();
                  if (!locations.isEmpty ())
                  {
                    if (polygonType.startsWith (QLatin1Char ('M')))
                    {
                      paths.reserve (polygons.size ());
                      for (QJsonValue const & polygon : qAsConst (polygons))
                      {
                        CTown::TPath path = createPolygon (polygon.toArray ());
                        paths.append (path);
                      }
                    }
                    else
                    {
                      paths.reserve (1);
                      CTown::TPath path = createPolygon (polygons);
                      paths.append (path);
                    }
                  }

                  if (contains (code))
                  {
                    CTown& town = (*this)[code];
                    town.appendPaths (paths);
                  }
                  else
                  {
                    CTown town;
                    town.setCode (code);
                    town.setName (name);
                    town.setRegion (region);
                    town.setPaths (paths, true);
                    insert (code, town);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void CTowns::load (QStringList fileNames)
{
  if (!fileNames.isEmpty ())
  {
    QFileInfo fi (fileNames.first ());
    if (!fi.exists ())
    {
      QElapsedTimer ti;
      ti.start ();
      fileNames.removeFirst ();
      loadPath (fileNames);
      qDebug () << "Town paths decoding: " << ti.elapsed () / 1000.0F << 's';
      int invalidTowns = 0;
      for (CTown const & town : qAsConst (*this))
      {
        if (town.region ().isEmpty ())
        {
          qDebug () << town.name ();
          ++invalidTowns;
        }
      }

      QFile file (fi.absoluteFilePath ());
      if (file.open (QIODevice::WriteOnly))
      {
        QDataStream out (&file);
        out << *this;
        file.close ();
      }

      qDebug () << "Invalid towns: " << invalidTowns;
    }
    else
    {
      qDebug () << "Contours from:" << fileNames.first ();
      QFile file (fi.absoluteFilePath ());
      if (file.open (QIODevice::ReadOnly))
      {
        QDataStream in (&file);
        in >> *this;
        file.close ();
      }
    }
  }
}

QString CTowns::regionFromTownCode (QString const & code) const
{
  QString region;
  for (CTown const & town : qAsConst (*this))
  {
    if (town.code () == code)
    {
      region = town.region ();
      break;
    }
  }

  return region;
}

QString CTowns::regionFromTownName (QString townName) const
{
  townName = CStringManip::removeDiacritics (townName, CStringManip::Upper);
  QString region;
  for (CTown const & town : qAsConst (*this))
  {
    QString temp = CStringManip::removeDiacritics (town.name (), CStringManip::Upper);
    if (temp == townName)
    {
      region = town.region ();
      break;
    }
  }

  return region;
}


QList<CTown> CTowns::towns (QString const & region) const
{
  QList<CTown> towns;
  for (CTown const & town : *this)
  {
    if (town.region () == region)
    {
      towns.append (town);
    }
  }

  return towns;
}

QMap<QString, CTown> CTowns::updateAirQualities (TTAQs const & airQualities)
{
  QMap<QString, CTown> towns;
  for (TTAQs::key_iterator ita = airQualities.keyBegin (), end = airQualities.keyEnd (); ita != end; ++ita)
  {
    QString code = *ita;
    if (contains (code))
    {
      CTown& town = (*this)[code];
      town.setAirQuality (airQualities.value (code));
      towns.insert (code, town);
    }
  }

  return towns;
}

QDataStream& operator >> (QDataStream& in, CTowns& towns)
{
  quint32 magicNumber; in >> magicNumber;
  quint32 version;     in >> version;
  quint32 count;       in >> count;
  towns.reserve (count);
  for (quint32 i = 0; i < count; ++i)
  {
    CTown town; in >> town;
    towns.insert (town.code (), town);
  }

  in >> towns.epcis ();
  return in;
}
