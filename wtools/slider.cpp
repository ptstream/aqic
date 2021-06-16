#include "slider.hpp"
#include "sliderstyle.hpp"
#include "magglass.hpp"
#include "../qtools/globals.hpp"
#include <QStyleOptionSlider>
#include <QMouseEvent>
#include <QProxyStyle>
#include <QToolTip>
#include <QPainter>
#include <QApplication>
#include <QDebug>

CMagGlass* CSlider::m_magGlass = nullptr;

CSlider::CSlider (QWidget* parent) : QSlider (Qt::Horizontal, parent), m_locale (QLocale::system ())
{
  setTracking (true);
  QStyleOptionSlider opts;
  opts.initFrom (this);
  if (opts.maximum <= opts.minimum)
  { // for MAcOS only.
    opts.maximum = 99;
    opts.minimum = 0;
    opts.pageStep = 10;
    opts.singleStep = 1;
    opts.sliderValue = 50;
    opts.sliderPosition = 50;
  }

  QStyle* style = this->style ();
  if (style == nullptr)
  {
    style = QApplication::style ();
  }

  QRect rc       = style->subControlRect (QStyle::CC_Slider, &opts, QStyle::SC_SliderHandle, this);
  m_handleWidth  = rc.width ();
  m_handleHeight = rc.height ();
  m_style        = new CSliderStyle;
  m_style->setHandleSize (m_handleWidth);
  setStyle (m_style);
  connect (this, &CSlider::actionTriggered, this, &CSlider::jumpToMousePosition);
}

void CSlider::mousePressEvent (QMouseEvent* event)
{
  QSlider::mousePressEvent (event);
  // Very important because the slider can enter in an infinit loop.
  setRepeatAction (QAbstractSlider::SliderNoAction);
}

void CSlider::mouseReleaseEvent (QMouseEvent* event)
{
  QSlider::mouseReleaseEvent (event);
  // Very important because the slider can enter in an infinit loop.
  setRepeatAction (QAbstractSlider::SliderNoAction);
}

bool CSlider::event (QEvent* event)
{
  if (m_toolTipAccepted && event->type () == QEvent::ToolTip)
  {
    int value    = mousePositionNearestValue ();
    int lastDate = m_dates.size () - 1;
    if (value >= 0 && value <= lastDate)
    {
      QHelpEvent* helpEvent   = static_cast<QHelpEvent*>(event);
      QString     toolTipText = "<b>" + m_locale.toString (m_dates[value]) + "</b>";
      if (!isClone ())
      {
        toolTipText += "<br/>" + tr ("SHIFT key to zoom.");
      }

      QToolTip::showText (helpEvent->globalPos (), toolTipText);
    }

    return true;
  }

  return QSlider::event (event);
}

int CSlider::mousePositionNearestValue () const
{
  float  normalizedPosition;
  QPoint localPos = mapFromGlobal (QCursor::pos ());
  if (orientation () == Qt::Vertical)
  {
    float  halfHandleHeight = 0.5F * m_handleHeight + 0.5F;
    int height             = this->height ();
    int   adaptedPosY      = height - localPos.y ();
    if (adaptedPosY < halfHandleHeight)
    {
      adaptedPosY = halfHandleHeight;
    }

    if (adaptedPosY > height - halfHandleHeight )
    {
      adaptedPosY = height - halfHandleHeight;
    }

    float newHeight    = (height - halfHandleHeight) - halfHandleHeight;
    normalizedPosition = (adaptedPosY - halfHandleHeight)  / newHeight;
  }
  else
  {
    float halfHandleWidth = 0.5F * m_handleWidth + 0.5F;
    int   adaptedPosX     = localPos.x ();
    if (adaptedPosX < halfHandleWidth)
    {
      adaptedPosX = halfHandleWidth;
    }

    int width = this->width ();
    if (adaptedPosX > width - halfHandleWidth)
    {
      adaptedPosX = width - halfHandleWidth;
    }

    float newWidth     = (width - halfHandleWidth) - halfHandleWidth;
    normalizedPosition = (adaptedPosX - halfHandleWidth)  / newWidth;
  }

  int value = ::qRound (minimum () + ((maximum () - minimum ()) * normalizedPosition));
  if (invertedAppearance ())
  {
    value = maximum () - value;
  }

  return value;
}

void CSlider::jumpToMousePosition (int action)
{
  if (!signalsBlocked () && (action == QSlider::SliderSingleStepAdd || action == QSlider::SliderSingleStepSub ||
                             action == QSlider::SliderPageStepAdd   || action == QSlider::SliderPageStepSub))
  {
    int newVal = mousePositionNearestValue ();
    setValue (newVal);
  }
}

void CSlider::setStyleColors ()
{
  QVector<QColor> colors;
  colors.reserve (m_colors.size ());
  for (QPair<QColor, QColor> const & color : qAsConst (m_colors))
  {
    colors.append (m_average ? color.first : color.second);
  }

  m_style->setColors (colors);
}

void CSlider::toggleAverage ()
{
  m_average = !m_average;
  setStyleColors ();
  update ();
}

void CSlider::setTickColors (QList<QPair<QColor, QColor>> const & colors)
{
  m_colors = colors;
  setStyleColors ();
  update ();
}

CSlider* CSlider::clone (QWidget* parent)
{
  CSlider* clone = nullptr;
  if (m_cloneAccepted)
  {
    clone = new CSlider (parent);
    clone->setMinimum (minimum ());
    clone->setMaximum (maximum ());
    clone->setTickPosition (tickPosition ());
    clone->setPageStep (pageStep ());
    clone->setOrientation (orientation ());
    clone->setValue (value ());
    clone->resize (size ());
    clone->m_average = m_average;
    clone->setTickColors (m_colors);
    clone->m_handleWidth      = m_handleWidth;
    clone->m_handleHeight     = m_handleHeight;
    clone->m_locale           = m_locale;
    clone->m_dates            = m_dates;
    clone->m_colors           = m_colors;
    clone->m_original         = this;
    clone->m_style->init (m_style);
   }

  return clone;
}

void CSlider::keyPressEvent (QKeyEvent* event)
{
  if (!isClone () && m_cloneAccepted && event->key () == Qt::Key_Shift)
  {
    if (m_magGlass != nullptr)
    {
      delete m_magGlass;
    }

    int value  = mousePositionNearestValue ();
    m_magGlass = new CMagGlass (this, value, parentWidget ());
  }

  QSlider::keyPressEvent (event);
}

void CSlider::keyReleaseEvent (QKeyEvent* event)
{
  if (m_magGlass != nullptr)
  {
    delete m_magGlass;
    m_magGlass = nullptr;
  }

  QSlider::keyPressEvent (event);
}

void CSlider::enterEvent (QEvent* event)
{
  if (!isClone () && m_cloneAccepted)
  {
    setFocus ();
  }

  QSlider::enterEvent (event);
}

void CSlider::leaveEvent (QEvent* event)
{
  if (!isClone () && m_cloneAccepted)
  {
    clearFocus ();
  }

  QSlider::leaveEvent (event);
}
