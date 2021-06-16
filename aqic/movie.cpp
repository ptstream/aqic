#include "movie.hpp"
#include "mapview.hpp"
#include "../town/aqlegend.hpp"
#include "../town/airqualities.hpp"
#include "../wtools/player.hpp"
#include "../wtools/slider.hpp"

void SAQs::updateIndexStats ()
{
  quint32 averageIndex = 0;
  quint8  maxIndex     = 0;
  for (CAirQuality const & aq : qAsConst (*this))
  {
    quint8 index  = aq.index (CAirQuality::Global);
    averageIndex += index;
    if (index < CAQLegend::Event)
    {
      maxIndex = std::max (maxIndex, index);
    }
  }

  m_averageIndex = static_cast<float>(averageIndex) / size ();
  m_max          = maxIndex;
}

CMovie::CMovie (CMapView* mapView, QWidget* parent) : CPlayer (parent), m_mapView (mapView)
{
}

int CMovie::highestAverageFrame () const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_averageIndex < aqs2.m_averageIndex;
  };

  QVector<SAQs>::const_iterator it = std::max_element (m_aqs.begin (), m_aqs.end (), compare);
  return std::distance (m_aqs.begin (), it);
}

int CMovie::lowestAverageFrame () const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_averageIndex < aqs2.m_averageIndex;
  };

  QVector<SAQs>::const_iterator it = std::min_element (m_aqs.begin (), m_aqs.end (), compare);
  return std::distance (m_aqs.begin (), it);
}

int CMovie::mostTownIndexFrame (quint8 index) const
{
  int      frame = 0;
  unsigned cmax  = 0;
  for (int i = 0, end = m_aqs.size (); i < end; ++i)
  {
    unsigned count = 0;
    for (CAirQuality const & aq : qAsConst (m_aqs[i]))
    {
      quint8 aqIndex = aq.index (CAirQuality::Global);
      if (index == aqIndex)
      {
        ++count;
      }
    }

    if (count != 0 && count > cmax)
    {
      cmax  = count;
      frame = i;
    }
  }

  return frame;
}

int CMovie::highestMaxFrame () const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_max < aqs2.m_max;
  };

  QVector<SAQs>::const_iterator it = std::max_element (m_aqs.begin (), m_aqs.end (), compare);
  return mostTownIndexFrame ((*it).m_max);
}

int CMovie::lowestMaxFrame () const
{
  auto compare = [] (SAQs const aqs1, SAQs const aqs2) -> bool
  {
    return aqs1.m_max < aqs2.m_max;
  };

  QVector<SAQs>::const_iterator it = std::min_element (m_aqs.begin (), m_aqs.end (), compare);
  return mostTownIndexFrame ((*it).m_max);
}

void CMovie::prepare ()
{
  CAirQualities* aqsi = CAirQualities::gblInst ();
  if (m_aqs.isEmpty ())
  {
    QList<QDate> dates = aqsi->dates ();
    m_aqs.clear ();
    m_aqs.reserve (dates.size ());
    for (QDate const & date : qAsConst (dates))
    {
      TTAQs aqs = aqsi->airQualites (date);
      m_aqs.append (SAQs (aqs, 0));
    }

    m_aqs.squeeze ();
    m_timerInterval = frameRate ();
    m_currentFrame  = 0;
    m_frameCount    = 0LL;

    setFrameMaximum (m_aqs.size () - 1);
    m_timer.setInterval (m_timerInterval);
  }
}

void CMovie::updateFrame ()
{
  m_mapView->updateTowns (m_aqs[m_currentFrame]);
  emit frameUpdated (m_aqs.size () - m_currentFrame - 1);
  frameNumber ()->blockSignals (true);
  setFrame (m_currentFrame);
  ++m_frameCount;
  float fps = 1000.0F * m_frameCount / m_frameRate.elapsed ();
  setFrameRate (fps);
  frameNumber ()->blockSignals (false);
}

void CMovie::nextFrame ()
{
  updateFrame ();
  ++m_currentFrame;
  if (m_currentFrame == m_aqs.size ())
  {
    m_currentFrame = 0;
  }
}

void CMovie::frameRateChanged (int)
{
  m_timerInterval = frameRate ();
  m_timer.setInterval (m_timerInterval);
  m_frameCount = 0LL;
  m_frameRate.restart ();
}

void CMovie::pausePlayer ()
{
  if (isRunning ())
  {
    playStateChanged ();
  }

  m_timer.stop ();
  m_mapView->setTooltipActivated (true);
}

void CMovie::startPlayer ()
{
  prepare ();
  m_mapView->setTooltipActivated (false);
  m_timer.start ();
  m_frameRate.restart ();
}

void CMovie::playStateChanged (bool)
{
  if (isRunning ())
  {
    startPlayer ();
  }
  else
  {
    pausePlayer ();
  }

  m_frameCount = 0LL;
}

void CMovie::forwardClicked (bool)
{
  if (isRunning ())
  {
    pausePlayer ();
  }

  prepare ();
  ++m_currentFrame;
  if (m_currentFrame == m_aqs.size ())
  {
    m_currentFrame = 0;
  }

  updateFrame ();
}

void CMovie::backwardClicked (bool)
{
  if (isRunning ())
  {
    pausePlayer ();
  }

  prepare ();
  --m_currentFrame;
  if (m_currentFrame < 0)
  {
    m_currentFrame = m_aqs.size () - 1;
  }

  updateFrame ();
}

void CMovie::changeFrame (int position)
{
  if (isRunning ())
  {
    pausePlayer ();
  }

  prepare ();
  m_currentFrame = position;
  updateFrame ();
}

void CMovie::updateFrame (int action)
{
  if (action == QSlider::SliderSingleStepAdd || action == QSlider::SliderSingleStepSub ||
      action == QSlider::SliderPageStepAdd   || action == QSlider::SliderPageStepSub)
  {
    prepare ();
    pausePlayer ();
    auto slider = static_cast<CSlider*>(sender ());
    slider->jumpToMousePosition (action);
    m_currentFrame = slider->value ();
    updateFrame ();
  }
}

void CMovie::highestAverage (bool)
{
  if (isRunning ())
  {
    pausePlayer ();
  }

  prepare ();
  m_currentFrame = highestAverageFrame ();
  updateFrame ();
}

void CMovie::highestMax (bool)
{
  if (isRunning ())
  {
    pausePlayer ();
  }

  prepare ();
  m_currentFrame = highestMaxFrame ();
  updateFrame ();
}

void CMovie::lowestAverage (bool)
{
  if (isRunning ())
  {
    pausePlayer ();
  }

  prepare ();
  m_currentFrame = lowestAverageFrame ();
  updateFrame ();
}

void CMovie::lowestMax (bool)
{
  if (isRunning ())
  {
    pausePlayer ();
  }

  prepare ();
  m_currentFrame = lowestMaxFrame ();
  updateFrame ();
}

