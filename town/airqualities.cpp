#include "airqualities.hpp"
#include "towns.hpp"
#include "towncodes.hpp"
#include "../qtools/csvparser.hpp"

//#define KEEPCSV 1

static quint32 magicNumber = 0x00001001; // 0x00000001->CAirQuality, 0x00001000->set of CAirQuality
static quint32 version     = 0x00000000;

CAirQualities::CAirQualities (QString const & fileName)
{
  load (fileName);
}

enum fieldIndex { date_ech,
                  code_qual,
                  code_zone,
                  code_pm25,
                  code_pm10,
                  code_no2,
                  code_o3,
                  code_so2,
                  last_index
                };

static QVector<int> fieldIndexes (QVector<QByteArray>& fieldNames, QVector<QByteArray> const & fields)
{
  using        TCIt  = QVector<QByteArray>::const_iterator;
  TCIt         begin = fields.cbegin ();
  TCIt         end   = fields.cend ();
  int          count = fieldNames.size ();
  QVector<int> indexes (count);
  for (int i = 0; i < count; ++i)
  {
    TCIt it = std::find (begin, end, fieldNames[i]);
    if (it != end)
    {
      indexes[i] = static_cast<int>(it - begin);
    }
  }

  return indexes;
}

void CAirQualities::load (QString const & fileName)
{
  if (!fileName.isEmpty ())
  {
    QFile file (fileName);
    if (file.open (QIODevice::ReadOnly))
    {
      QDataStream in (&file);
      quint32 n; in >> n;
      if (n == magicNumber)
      {
        file.seek (0LL);
        in >> *this;
        return;
      }
    }

    CTowns*        towns    = CTowns::gblInst ();
    unsigned const indexMax = CAQLegend::gblInst ()->at (0).m_indexes.size () - 1;
    file.close ();
    CCSVParser parser (fileName);
    if (parser.nextRecord ())
    {
      QVector<QByteArray> header = parser.fields (); // Get the header
      if (!header.isEmpty () && header.size () > last_index)
      {
        QVector<QByteArray> fieldNames = { "date_ech",
                                           "code_qual",
                                           "code_zone",
                                           "code_pm25",
                                           "code_pm10",
                                           "code_no2",
                                           "code_o3",
                                           "code_so2",
                                         };

        QVector<int> indexes = fieldIndexes (fieldNames, header);

        // Downloads can contain duplicate csv records. The 2 variables below are use to filter same towns with same dates
        using TDates = QSet<QDateTime>;
        using TCodes = QHash<QString, TDates>;
        TCodes cds;
        while (parser.nextRecord ())
        {
          QVector<QByteArray> fields = parser.fields ();
          if (fields.size () > indexes[last_index - 1])
          {
            CAirQuality aq;
            QDateTime   dt   = QDateTime::fromString (QString::fromUtf8 (fields[indexes[date_ech]]), Qt::ISODate);
            QString     code = QString::fromUtf8 (fields[indexes[code_zone]]);
            if (cds.contains (code))
            {
              if (cds[code].contains (dt))
              {
                continue;
              }

              cds[code].insert (dt);
            }
            else
            {
              TDates dates;
              dates.insert (dt);
              cds.insert (code, dates);
            }

            aq.setDeadline (dt.date ());
            if (m_townCodes != nullptr)
            {
              QString equivalentCode = equivalentTownCode (code);
              if (!equivalentCode.isEmpty ())
              {
                code = equivalentCode;
              }
            }

            aq.setCode (code);
            aq.setIndex (CAirQuality::Global, static_cast<quint8>(std::min (fields[indexes[code_qual]].toUInt (), indexMax)));
            aq.setIndex (CAirQuality::PM25,   static_cast<quint8>(std::min (fields[indexes[code_pm25]].toUInt (), indexMax)));
            aq.setIndex (CAirQuality::PM10,   static_cast<quint8>(std::min (fields[indexes[code_pm10]].toUInt (), indexMax)));
            aq.setIndex (CAirQuality::NO2,    static_cast<quint8>(std::min (fields[indexes[code_no2]].toUInt (),  indexMax)));
            aq.setIndex (CAirQuality::O3,     static_cast<quint8>(std::min (fields[indexes[code_o3]].toUInt (),   indexMax)));
            aq.setIndex (CAirQuality::SO2,    static_cast<quint8>(std::min (fields[indexes[code_so2]].toUInt (),  indexMax)));
            if (!towns->contains (code))
            {
              QMultiHash<QString, QString>&          epcis = towns->epcis ();
              QMultiHash<QString, QString>::iterator it    = epcis.find (code);
              QMultiHash<QString, QString>::iterator end   = epcis.end ();
              while (it != end && it.key () == code)
              {
                QString code = it.value ();
                if (!cds.contains (code))
                {
                  aq.setCode (it.value ());
                  append (aq);
                }

                ++it;
              }
            }
            else
            {
              append (aq);
            }
          }
        }

        parser.closeFile ();
        auto compare = [] (CAirQuality const & aq1, CAirQuality const & aq2) -> bool
        {
          return aq1.deadline () > aq2.deadline ();
        };

        std::sort (begin (), end (), compare);
#ifdef KEEPCSV
        QFile::rename (fileName, fileName + ".csv");
#endif
        QFile file (fileName);
        if (file.open (QIODevice::WriteOnly))
        {
          QDataStream out (&file);
          out << *this;
          file.close ();
        }
      }
    }
  }
}

TCodeColors CAirQualities::colors (QDate dt, quint32 rgb) const
{
  TCodeColors colors;
  for (CAirQuality const & aq : *this)
  {
    qint64 dayCount = dt.daysTo (aq.deadline ());
    if (dayCount == 0)
    {
      quint32 color = rgb != 0 ? rgb : aq.color ();
      colors.insert (aq.code (), color);
    }
    else if (dayCount < 0)
    {
      break;
    }
  }

  return colors;
}

TTAQs CAirQualities::airQualities (QDate const & dt) const
{
  TTAQs airQualities;
  for (CAirQuality const & aq : *this)
  {
    qint64 dayCount = dt.daysTo (aq.deadline ());
    if (dayCount == 0)
    {
      airQualities.insert (aq.code (), aq);
    }
    else if (dayCount < 0)
    {
      break;
    }
  }

  return airQualities;
}

QList<QDate> CAirQualities::dates () const
{
  QSet<QDate> set;
  for (CAirQuality const & aq : *this)
  {
    set.insert (aq.deadline ());
  }

  QList<QDate> dates = set.values ();
  std::sort (dates.begin (), dates.end ());
  return dates;
}

CAirQuality CAirQualities::airQuality (QString const & code, QDate const & date) const
{
  auto predicat = [&date, &code] (CAirQuality const & airQuality) -> bool
  {
    return airQuality.deadline () == date && airQuality.code () == code;
  };

  CAirQualities::const_iterator end = cend ();
  CAirQualities::const_iterator it  = std::find_if (cbegin (), end, predicat);
  return it != end ? *it : CAirQuality ();
}

QList<QDate> CAirQualities::uniqDeadlines () const
{
  QSet<QDate> set;
  for (CAirQuality const & aq : qAsConst (*this))
  {
    set.insert (aq.deadline ());
  }

  return set.values ();
}

QList<QPair<quint8, quint8>> CAirQualities::averageMaxs (CAirQuality::EIndexType index, int dateCount) const
{
  struct SSum
  {
    void reset () { m_max = m_sum = m_count = 0; }

    int    m_sum   = 0;
    int    m_count = 0;
    quint8 m_max   = 0;
  };

  QList<QPair<quint8, quint8>> avs;
  if (!isEmpty ())
  {
    QVector<SSum> sums;
    if (dateCount == 0)
    {
      dateCount = uniqDeadlines ().size ();
    }

    if (dateCount != 0)
    {
      sums.reserve (dateCount);
      QDate date = last ().deadline ();
      SSum  sum;
      for (QVector<CAirQuality>::const_reverse_iterator it = crbegin (), end = crend (); it != end; ++it)
      {
        CAirQuality const & aq    = *it;
        quint8              value = aq.index (index);
        if (value > 0 && value < 7)
        {
          if (date != aq.deadline ())
          {
            sums.append (sum);
            date = aq.deadline ();
            sum.reset ();
          }

          sum.m_sum += value;
          if (sum.m_max < value)
          {
            sum.m_max = value;
          }

          ++sum.m_count;
        }
      }

      sums.append (sum);
      avs.reserve (sums.size ());
      for (SSum const & sum : qAsConst (sums))
      {
        quint8 av = static_cast<quint8>(::qRound (static_cast<double>(sum.m_sum) / sum.m_count));
        QPair<quint8, quint8> avm (av, sum.m_max);
        avs.append (avm);
      }
    }
  }

  return avs;
}

QList<CAirQuality> CAirQualities::airQualities (QString const & code) const
{
  QList<CAirQuality> aqs;
  for (QVector<CAirQuality>::const_reverse_iterator it = crbegin (), end = crend (); it != end; ++it)
  {
    CAirQuality const & aq = *it;
    if (aq.code () == code)
    {
      aqs.append (aq);
    }
  }

  return aqs;
}

int CAirQualities::airQualityCount (QString const & code) const
{
  int count = 0;
  for (CAirQuality const & aq : *this)
  {
    if (aq.code () == code)
    {
      ++count;
    }
  }

  return count;
}

QString CAirQualities::equivalentTownCode (QString const & code) const
{
  return m_townCodes != nullptr ? m_townCodes->value (code) : QString ();
}

QDataStream& operator >> (QDataStream& in, CAirQualities& aqs)
{
  quint32 magicNumber; in >> magicNumber;
  quint32 version;     in >> version;
  quint32 size;        in >> size;
  for (quint32 i = 0; i < size; ++i)
  {
    CAirQuality aq; in >> aq;
    aqs.append (aq);
  }

  return in;
}

QDataStream& operator << (QDataStream& out, CAirQualities const & aqs)
{
  out << magicNumber;
  out << version;
  out << static_cast<quint32>(aqs.size ());
  for (CAirQuality const & aq : aqs)
  {
    out << aq;
  }

  return out;
}


