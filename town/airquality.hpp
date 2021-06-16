#ifndef AIRQUALITY_HPP
#define AIRQUALITY_HPP

#include "aqlegend.hpp"
#include <QSharedDataPointer>
#include <QDate>
#include <QVector>
#include <QIODevice>
#include <array>

class CAirQuality
{
public:
  enum EIndexType { Global,
                    PM25,
                    PM10,
                    NO2,
                    O3,
                    SO2,
                    LastIndex
                  };

  using TIndexes = std::array<quint8, CAirQuality::LastIndex>;

  CAirQuality () : m_d (new SAirQualityData) {}
  CAirQuality (CAirQuality const & rhs) : m_d (rhs.m_d) {}
  ~CAirQuality () = default;

  inline CAirQuality& operator = (CAirQuality const & other);

  bool isValid () const { return !m_d->m_code.isEmpty () && m_d->m_deadline.isValid (); }
  QString const & code () const { return m_d->m_code; }
  QDate const & deadline () const { return m_d->m_deadline; }
  quint8 index (EIndexType index = Global) const { return m_d->m_indexes[index]; }
  TIndexes const & indexes () const { return m_d->m_indexes; }

  void setCode (QString const & code) { m_d->m_code = code; }
  void setDeadline (QDate const & date) { m_d->m_deadline = date; }
  void setIndex (EIndexType index, quint8 value) { m_d->m_indexes[index] = value; }
  void setIndexes (TIndexes const & indexes) { m_d->m_indexes = indexes; }

  inline quint32 color (EIndexType index = Global) const;
  static inline quint32 color (CAirQuality const & aq, EIndexType index = Global);
  static inline quint32 color (quint8 index);

private:
  struct SAirQualityData : public QSharedData
  {
    QString  m_code;
    QDate    m_deadline;
    TIndexes m_indexes = {};
  };

  QSharedDataPointer<SAirQualityData> m_d;
};

CAirQuality& CAirQuality::operator = (CAirQuality const & rhs)
{
  if (this != &rhs)
  {
    m_d.operator = (rhs.m_d);
  }

  return *this;
}

quint32 CAirQuality::color (EIndexType index) const
{
  return color (m_d->m_indexes[index]);
}

quint32 CAirQuality::color (CAirQuality const & aq, EIndexType index)
{
  return color (aq.m_d->m_indexes[index]);
}

quint32 CAirQuality::color (quint8 index)
{
  return CAQLegend::gblInst ()->rgb (CAQLegend::PM25, static_cast<CAQLegend::EIndex>(index));
}

QDataStream& operator << (QDataStream& out, CAirQuality const & aq);
QDataStream& operator >> (QDataStream& in, CAirQuality& aq);

#endif // AIRQUALITY_HPP
