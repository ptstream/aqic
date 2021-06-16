#include "airquality.hpp"
#include <QDataStream>
#include <QDebug>

static quint32 magicNumber = 0x00000001; // 0x00000001->CAirQuality
static quint32 version     = 0x00000000;

QDataStream& operator << (QDataStream& out, CAirQuality const & aq)
{
  out << magicNumber;
  out << version;
  out << aq.code ();
  out << aq.deadline ();
  for (quint8 index : aq.indexes ())
  {
    out << index;
  }

  return out;
}

QDataStream& operator >> (QDataStream& in, CAirQuality& aq)
{
  quint32               id;      in >> id;
  quint32               version; in >> version;
  QString               code;    in >> code; aq.setCode (code);
  QDate                 date;    in >> date; aq.setDeadline (date);
  CAirQuality::TIndexes indexes = aq.indexes ();
  for (size_t i = 0, count = indexes.size (); i < count; ++i)
  {
    quint8 index; in >> index;
    aq.setIndex (static_cast<CAirQuality::EIndexType>(i), index);
  }

  return in;
}
