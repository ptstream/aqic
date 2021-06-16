#include "tickvalues.hpp"
#include <QPainter>
#include <QLocale>

CTickValues::CTickValues (QWidget* parent) : QWidget (parent), m_formatDate ("dd/MM")
{
}

void CTickValues::paintEvent (QPaintEvent*)
{
  QLocale  locale = QLocale::system ();
  QPainter painter (this);
  painter.setClipping (false);
  painter.setRenderHint (QPainter::TextAntialiasing);
  int            count      = m_max - m_min + 1;
  int            frameWidth = width () - m_padding - m_padding - m_offset - m_offset;
  float          dx         = (static_cast<float>(frameWidth)) / (count - 1);
  float          x          = m_padding + m_offset;
  int            dx2        = ::qRound (0.5F * dx);
  int            w          = ::qRound (dx);
  int            h          = height ();
  painter.setBrush (Qt::blue);
  painter.drawRect (m_padding, 0, width () - m_padding - m_padding - 1, h - 1);

  QPen pen (Qt::white);
  painter.setPen (pen);
  for (int i = 0; i < count; ++i)
  {
    QString       date = locale.toString (m_dates[m_min + i], m_formatDate);
    Qt::Alignment alignment;
    int     rx   = ::qRound (x);
    if (i == 0)
    {
      alignment = Qt::AlignLeft | Qt::AlignVCenter;
      rx       -= m_offset - 2;
    }
    else if (i == count - 1)
    {
      alignment = Qt::AlignRight | Qt::AlignVCenter;
      rx -= dx - m_offset;
    }
    else
    {
      alignment = Qt::AlignHCenter | Qt::AlignVCenter;
      rx -= dx2 + m_offset;
    }

    QRect rc (rx, 0, w, h);
    painter.drawText (rc, alignment, date);
    x += dx;
  }
}
