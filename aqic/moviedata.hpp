#ifndef MOVIEDATA_HPP
#define MOVIEDATA_HPP

#include "../town/airqualities.hpp"
#include <QDate>

class CBoundingBox;

struct SAQs : public TTAQs
{
  SAQs (TTAQs const & taqs, CAirQuality::EIndexType index) : m_index (index)
  {
    *static_cast<TTAQs*>(this) = taqs;
    updateIndexStats ();
  }

  void updateIndexStats ();

  CAirQuality::EIndexType m_index        = CAirQuality::Global;
  float                   m_averageIndex = 0.0F;
  float                   m_max          = 0.0F;
};

struct SMovieData : public QVector<SAQs>
{
  /*! \brief The frame where the number of cities is the greatest for a given index. */
  int greatestTownsNumberFrame (quint8 index, CAirQuality::EIndexType indexType) const;

  /*! \brief The frame where the number of cities is the smallest for a given index. */
  int smallestTownsNumberFrame (quint8 index, CAirQuality::EIndexType indexType) const;

  /*! \brief The frame where the average is greatest for a given index. */
  int greatestAverageFrame () const;

  /*! \brief The frame where the average is smallest for a given index. */
  int smallestAverageFrame () const;

  /*! \brief The frame where the max is greatest for a given index. */
  int greatestMaxFrame (quint8& value, CAirQuality::EIndexType& index) const;

  /*! \brief The frame where the max is smallest for a given index. */
  int smallestMaxFrame (quint8& value, CAirQuality::EIndexType& index) const;

  void incFrameNumber ();
  void decFrameNumber ();

  qint64 m_frameCount    = 0LL;
  int    m_currentFrame  = 0;
  int    m_timerInterval = 41;
};

inline void SMovieData::incFrameNumber ()
{
  ++m_currentFrame;
  if (m_currentFrame >= size ())
  {
    m_currentFrame = 0;
  }
}

inline void SMovieData::decFrameNumber ()
{
  --m_currentFrame;
  if (m_currentFrame < 0)
  {
    m_currentFrame = size () - 1;
  }
}

#endif // MOVIEDATA_HPP
