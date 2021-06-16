#include "sliderstyle.hpp"
#include "slider.hpp"
#include <QPainter>
#include <QStyleOptionComplex>
#include <QStyleOptionSlider>
#include <QDebug>
#include <array>

void CSliderStyle::init (CSliderStyle const * other)
{
  m_colors            = other->m_colors;
  m_handleSize        = other->m_handleSize;
  m_padding           = other->m_padding;
  m_spacingTicksLimit = other->m_spacingTicksLimit;
}

static inline QColor rgb2Gray (QColor const & color)
{
  int gray = 21 * color.red () / 100 + 72 * color.green () / 100 + 4 * color.blue () / 100;
  return QColor (gray, gray, gray);
}

void CSliderStyle::drawComplexControl (QStyle::ComplexControl, QStyleOptionComplex const * option, QPainter* painter,
                                       QWidget const * widget) const
{
  if (!m_colors.isEmpty ())
  {
    auto styleOptionSlider = qstyleoption_cast<QStyleOptionSlider const *>(option);
    if (styleOptionSlider->sliderValue >= styleOptionSlider->minimum && styleOptionSlider->sliderValue <= styleOptionSlider->maximum)
    {
      bool enabled = (styleOptionSlider->state & QStyle::State_Enabled) != 0;
      painter->setRenderHint (QPainter::Antialiasing, false);

      // Draw frame.
      QPen  pen           = painter->pen ();
      QRect effectiveRect = option->rect;
      effectiveRect.adjust (m_padding, m_padding, -m_padding, -widget->height () / 4 - m_padding);

      // Draw colors.
      QSize effectiveSize = effectiveRect.size ();
      int   h             = effectiveSize.height ();
      float x             = effectiveRect.x ();
      painter->setPen (Qt::NoPen);
      int rectCount = styleOptionSlider->maximum - styleOptionSlider->minimum + 1;
      // Solution of equation: w = dx * (n - 2) + 2 * h / 2 + dx / 2)  where
      // w  = width of slider.
      // dx = width of rectangle to draw.
      // n  = number of slider ticks.
      // h  = width of handle.
      float dx = (static_cast<float>(effectiveSize.width ()) - m_handleSize) / (rectCount - 1);
      int   xx         = 0;
      int   iw         = 0;
      int   colorIndex = styleOptionSlider->minimum;
      for (int i = 0; i < rectCount; ++i, ++colorIndex)
      {
        QColor color = enabled ? m_colors[colorIndex] : rgb2Gray (m_colors[colorIndex]);
        QBrush brush (color);
        painter->setBrush (brush);
        // First rectangle start at half handle width and last rectangle end at half handle width.
        float w = i == 0 ? (m_handleSize + dx) / 2.0F : dx;
        iw      = std::max (1, ::qRound (w) + 1);
        int xr  = ::qRound (x);
        xx      = xr;
        if (i == 0)
        {
          ++xx;
        }

        if (i == rectCount - 1)
        {
          iw = effectiveSize.width () - xr;
        }

        painter->drawRect (xx, m_padding + 1, iw, h - 1);
        x     += w;
      }

      painter->setPen (pen);
      painter->setBrush (Qt::NoBrush);
      QRect rc = effectiveRect;
      // The effective rectangle is not exactly the rectangle to be drawn due to the truncation of the position of the tick marks.
      rc.setWidth (xx + iw - rc.x ());
      painter->drawRect (rc);

      // Draw ticks.
      float effectiveWidth = effectiveSize.width () - m_handleSize;
      x                    = m_handleSize / 2.0F;
      dx                   = effectiveWidth / (styleOptionSlider->maximum - styleOptionSlider->minimum);
      int y2               = effectiveSize.height () / 2;
      if (dx > m_spacingTicksLimit)
      {
        int y1 = m_padding;
        for (int i = styleOptionSlider->minimum; i <= styleOptionSlider->maximum; ++i)
        {
          int rx = ::qRound (x);
          painter->drawLine (rx, y1, rx, y2);
          x += dx;
        }
      }

      // Draw handle.
      CSlider const * slider = static_cast<CSlider const *>(widget);
      int             value  = slider->isClone () ? slider->originalModel ()->value () : styleOptionSlider->sliderValue;
      if (value <= styleOptionSlider->maximum)
      {
        dx     = (static_cast<float>(effectiveSize.width ()) - m_handleSize) / (rectCount - 1);
        int rx = effectiveRect.x () + m_handleSize / 2 + (styleOptionSlider->sliderValue - styleOptionSlider->minimum)* dx;
        h      = m_handleSize / 2.0F;
        std::array<QPoint, 5> pts;
        pts[0] = QPoint (rx, y2);
        pts[1] = QPoint (pts[0].x () + m_handleSize / 2, pts[0].y () + h);
        pts[2] = QPoint (pts[1].x (), option->rect.height ());
        pts[3] = QPoint (pts[0].x () - m_handleSize / 2, option->rect.height ());
        pts[4] = QPoint (pts[3].x (), pts[1].y ());
        painter->setBrush (enabled ? Qt::blue : Qt::gray);
        painter->setPen (Qt::NoPen);
        painter->setRenderHint (QPainter::Antialiasing, true);
        painter->drawPolygon (pts.data (), 5);
      }
    }
  }
}
