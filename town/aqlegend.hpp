#ifndef AQLEGEND_HPP
#define AQLEGEND_HPP

#include "../tools/globalinstance.hpp"
#include <QVector>

/*! The container of the legend for one pollutant. */
struct SPollutantLegend
{
  using TInterval = QPair<int, int>;
  struct SIndex
  {
    QString   m_wording; //!< For example degraded or bad.
    QString   m_text;    //!< html text color.
    QString   m_color;   //!< html color.
    TInterval m_interval; //!< The concentration interval.
    quint32   m_rgb = 0;  //!< The color (rgb).
  };

  using TIndexes = QVector<SIndex>;
  QString  m_name;   //!< The pollutant name.
  QString  m_metric;  //!< The concentration metric.
  TIndexes m_indexes; //!< Data for each index.
};

using TPollutantLegends = QVector<SPollutantLegend>;

/*! \brief This is the container for the legend.
 *
 *  Actually, it is assumed the wording, text color, color are the same for all pollutant for the same index.
 */
class CAQLegend : public TPollutantLegends, public CGlobalInstance<CAQLegend>
{
public:
  /*! Pollutants. */
  enum EPollutant { PM25,
                    PM10,
                    NO2,
                    O3,
                    SO2,
                    LastPollutant
                  };

  /*! Wordings. */
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

  /*! Default constructor. */
  CAQLegend () = default;

  /*! Destructor. */
  ~CAQLegend () override = default;

  /*! Load from file. If fileName is empty, the file name is legend_xx.json. xx represents the country. */
  void load (QString fileName = QString ());

  /*! Returns the color (rgb) from a pollutant and an index. */
  inline quint32 rgb (EPollutant pollutant, EIndex index) const;

  /*! Returns the html color from a pollutant and an index. */
  inline QString const & color (EPollutant pollutant, EIndex index) const;

  /*! Returns the html text color from a pollutant and an index. */
  inline QString const & text (EPollutant pollutant, EIndex index) const;

  /*! Returns the index from the color. */
  EIndex index (quint32 rgb) const;

  /*! Return the single instance of the legend. */
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
