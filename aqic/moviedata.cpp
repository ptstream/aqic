#include "moviedata.hpp"
#include "../qtools/boundingbox.hpp"
#include <QDebug>

void SAQs::updateIndexStats ()
{
  quint32 averageIndex = 0;
  quint8  maxIndex     = 0;
  quint32 count        = 0;
  for (CAirQuality const & aq : qAsConst (*this))
  {
    quint8 index = aq.index (m_index);
    if (index > CAQLegend::Unavailable && index < CAQLegend::Event)
    {
      averageIndex += index;
      maxIndex      = std::max (maxIndex, index);
      ++count;
    }
  }

  m_averageIndex = static_cast<float>(averageIndex) / count;
  m_max          = maxIndex;
}

int SMovieData::greatestAverageFrame () const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_averageIndex < aqs2.m_averageIndex;
  };

  QVector<SAQs>::const_iterator it = std::max_element (cbegin (), cend (), compare);
  return std::distance (cbegin (), it);
}

int SMovieData::smallestAverageFrame () const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_averageIndex < aqs2.m_averageIndex;
  };

  QVector<SAQs>::const_iterator it = std::min_element (cbegin (), cend (), compare);
  return std::distance (cbegin (), it);
}

int SMovieData::greatestTownsNumberFrame (quint8 index, CAirQuality::EIndexType indexType) const
{
  int      image = 0;
  unsigned cmax  = 0;
  for (int i = 0, end = size (); i < end; ++i)
  {
    SAQs const & aqs      = at (i);
    quint8       indexMax = aqs.m_max;
    if (indexMax == index)
    {
      unsigned count = 0;
      for (CAirQuality const & aq : qAsConst (at (i)))
      {
        quint8 aqIndex = aq.index (indexType);
        if (index == aqIndex)
        {
          ++count;
        }
      }

      if (count != 0 && count > cmax)
      {
        cmax  = count;
        image = i;
      }
    }
  }

  return image;
}

int SMovieData::smallestTownsNumberFrame (quint8 index, CAirQuality::EIndexType indexType) const
{
  int      image = 0;
  unsigned cmin  = std::numeric_limits<unsigned>::max ();
  for (int i = 0, end = size (); i < end; ++i)
  {
    SAQs const & aqs      = at (i);
    quint8       indexMax = aqs.m_max;
    if (indexMax == index)
    {
      unsigned count = 0;
      for (CAirQuality const & aq : qAsConst (aqs))
      {
        quint8 aqIndex = aq.index (indexType);
        if (index == aqIndex)
        {
          ++count;
        }
      }

      if (count != 0 && count < cmin)
      {
        cmin  = count;
        image = i;
      }
    }
  }

  return image;
}

int SMovieData::greatestMaxFrame (quint8& value, CAirQuality::EIndexType& index) const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_max < aqs2.m_max;
  };

  QVector<SAQs>::const_iterator it  = std::max_element (cbegin (), cend (), compare);
  SAQs const &                  aqs = *it;
  value                             = aqs.m_max;
  index                             = aqs.m_index;
  return greatestTownsNumberFrame (value, index);
}

int SMovieData::smallestMaxFrame (quint8& value, CAirQuality::EIndexType& index) const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_max < aqs2.m_max;
  };

  QVector<SAQs>::const_iterator it  = std::min_element (cbegin (), cend (), compare);
  SAQs const &                  aqs = *it;
  value                             = aqs.m_max;
  index                             = aqs.m_index;
  return smallestTownsNumberFrame (value, index);
}
