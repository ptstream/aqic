#include "aqlegend.hpp"
#include "../qtools/globals.hpp"
#include "../qtools/applocations.hpp"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static QString uniqString (QMap<QString, QString> cache, QString const & s)
{
  if (!cache.contains (s))
  {
    cache.insert (s, s);
  }

  return cache.value (s);
}

void CAQLegend::load (QString fileName)
{
  if (fileName.isEmpty ())
  {
    fileName = CAppLocations::localeConfigFilePath ("legend.json");
  }

  QFile   file (fileName);
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
        QMap<QString, QString> cache;
        QJsonArray             jsonPollutants = doc.array ();
        reserve (jsonPollutants.size ());
        for (QJsonValue const & jvp : qAsConst (jsonPollutants))
        {
          QJsonObject jsonPollutant = jvp.toObject ();

          struct SPollutantLegend pollutant;
          pollutant.m_name       = uniqString (cache, jsonPollutant["pollutant"].toString ());
          pollutant.m_metric     = uniqString (cache, jsonPollutant["metric"].toString ());
          QJsonArray jsonIndexes = jsonPollutant["indexes"].toArray ();
          SPollutantLegend::TIndexes indexes;
          indexes.reserve (jsonIndexes.size ());
          for (QJsonValue const & jvis : qAsConst (jsonIndexes))
          {
            QJsonObject              jsonIndex = jvis.toObject ();
            SPollutantLegend::SIndex index;
            index.m_wording     = uniqString (cache, jsonIndex["wording"].toString ());
            index.m_text        = uniqString (cache, jsonIndex["text"].toString ());
            index.m_color       = uniqString (cache, jsonIndex["color"].toString ());
            bool ok;
            index.m_rgb         = index.m_color.midRef (1).toUInt (&ok, 16);
            QJsonArray interval = jsonIndex["interval"].toArray ();
            if (interval.size () == 2)
            {
              index.m_interval.first  = interval[0].toInt ();
              index.m_interval.second = interval[1].toInt ();
              pollutant.m_indexes.append (index);
            }
          }

          append (pollutant);
        }
      }
    }
  }
}

CAQLegend::EIndex CAQLegend::index (quint32 rgb) const
{
  auto predicat = [rgb] (SPollutantLegend::SIndex const & index) -> bool
  {
    bool    ok;
    quint32 indexRgb = index.m_color.midRef (1).toUInt (&ok, 16) | 0xFF000000;
    return indexRgb == rgb;
  };

  SPollutantLegend const & pl = first ();
  SPollutantLegend::TIndexes::const_iterator begin = pl.m_indexes.cbegin ();
  SPollutantLegend::TIndexes::const_iterator end   = pl.m_indexes.cend ();
  SPollutantLegend::TIndexes::const_iterator it    = std::find_if (begin, end, predicat);
  return it != end ? static_cast<EIndex>(it - begin) : LastIndex;
}

