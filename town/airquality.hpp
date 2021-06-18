#ifndef AIRQUALITY_HPP
#define AIRQUALITY_HPP

#include "aqlegend.hpp"
#include <QSharedDataPointer>
#include <QDate>
#include <QVector>
#include <QIODevice>
#include <array>

/*! \brief This class is the container of the air quality. */
class CAirQuality
{
public:
  /*! The index global or by pollutant. */
  enum EIndexType { Global, //!< Global index (the maximum of the other).
                    PM25,   //!< Particles with a diameter of less than 2.5 microns.
                    PM10,   //!< Particles with a diameter of less than 10 microns.
                    NO2,    //!< Nitrogen dioxide.
                    O3,     //!< Ozone.
                    SO2,    //!< Sulphur dioxide/
                    LastIndex
                  };

  /*! Array of index. */
  using TIndexes = std::array<quint8, CAirQuality::LastIndex>;

  /*! Default constructor. */
  CAirQuality () : m_d (new SAirQualityData) {}

  /*! Copy constructor. */
  CAirQuality (CAirQuality const & rhs) : m_d (rhs.m_d) {}

  /*! Destructor. */
  ~CAirQuality () = default;

  /*! Equality operator. */
  inline CAirQuality& operator = (CAirQuality const & other);

  /*! Returns true if the air quality is valid. */
  bool isValid () const { return !m_d->m_code.isEmpty () && m_d->m_deadline.isValid (); }

  /*! Returns the town code (insee). */
  QString const & code () const { return m_d->m_code; }

  /*! Returns the deadline. */
  QDate const & deadline () const { return m_d->m_deadline; }

  /*! Returns the value for the index. */
  quint8 index (EIndexType index = Global) const { return m_d->m_indexes[index]; }

  /*! Returns the indexes. */
  TIndexes const & indexes () const { return m_d->m_indexes; }

  /*! Sets the town code (insee). */
  void setCode (QString const & code) { m_d->m_code = code; }

  /*! Sets the deadline. */
  void setDeadline (QDate const & date) { m_d->m_deadline = date; }

  /*! Sets the value for the index. */
  void setIndex (EIndexType index, quint8 value) { m_d->m_indexes[index] = value; }

  /*! Sets the indexes. */
  void setIndexes (TIndexes const & indexes) { m_d->m_indexes = indexes; }

  /*! Returns the color (rgb), from index. */
  inline quint32 color (EIndexType index = Global) const;

  /*! Returns the color (rgb) from air quality and from index. */
  static inline quint32 color (CAirQuality const & aq, EIndexType index = Global);

  /*! Returns the color (rgb), from index value. */
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
