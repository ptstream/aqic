#ifndef AQLEGEND_HPP
#define AQLEGEND_HPP

#include "../tools/globalinstance.hpp"
#include <QVector>

struct SPollutantLegend
{
  using TInterval = QPair<int, int>;
  struct SIndex
  {
    QString   m_wording;
    QString   m_text;
    QString   m_color;
    TInterval m_interval;
    quint32   m_rgb = 0;
  };

  using TIndexes = QVector<SIndex>;
  QString  m_name;
  QString  m_metric;
  TIndexes m_indexes;
};

using TPollutantLegends = QVector<SPollutantLegend>;

class CAQLegend : public TPollutantLegends, public CGlobalInstance<CAQLegend>
{
public:
  enum EPollutant { PM25,
                    PM10,
                    NO2,
                    O3,
                    SO2,
                    LastPollutant
                  };

  enum EIndex { Unavailable,
                Good,
                Medium,
                Degraded,
                Bad,
                VeryBad,
                ExtremlyBad,
                Event,
                LastIndex
              };

  CAQLegend () = default;
  ~CAQLegend () override = default;

  void load (QString fileName = QString ());

  inline quint32 rgb (EPollutant pollutant, EIndex index) const;
  inline QString const & color (EPollutant pollutant, EIndex index) const;
  inline QString const & text (EPollutant pollutant, EIndex index) const;

  EIndex index (quint32 rgb) const;

  static CAQLegend* gblInst () { return static_cast<CAQLegend*>(CGlobalInstance<CAQLegend>::gblInst ()); }
};

QString const & CAQLegend::color (EPollutant pollutant, EIndex index) const
{
  Q_ASSERT (pollutant != LastPollutant && index != LastIndex);
  return at (pollutant).m_indexes.at (index).m_color;
}

QString const & CAQLegend::text (EPollutant pollutant, EIndex index) const
{
  Q_ASSERT (pollutant != LastPollutant && index != LastIndex);
  return at (pollutant).m_indexes.at (index).m_text;
}

quint32 CAQLegend::rgb (EPollutant pollutant, EIndex index) const
{
  return at (pollutant).m_indexes.at (index).m_rgb;
}

#endif // AQLEGEND_HPP
