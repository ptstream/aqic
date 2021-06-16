#include "waitwidget.hpp"
#include "../qtools/globals.hpp"
#include <QPainter>
#include <QDebug>

CWaitWidget::CWaitWidget (QWidget* parent) :
  QDialog (parent, Qt::CustomizeWindowHint | Qt::FramelessWindowHint)
{
  setCursor (Qt::PointingHandCursor);
  setAttribute (Qt::WA_TranslucentBackground);
  QPixmap pxm (QString (m_iconResource).arg (m_state));
  resize (pxm.size ());
  m_timer.setInterval (m_timerInterval);
  connect (&m_timer, &QTimer::timeout, this, &CWaitWidget::changePixmap);
}

void CWaitWidget::paintEvent (QPaintEvent*)
{
  QPainter painter (this);
  painter.setRenderHint (QPainter::Antialiasing, true);
  painter.setRenderHint (QPainter::TextAntialiasing, true);
  QPixmap pxm (QString (m_iconResource).arg (m_state));
  QRect rc = rect ();
  int   dx = (rc.width () - pxm.width ()) / 2;
  int   dy = (rc.height () - pxm.height ()) / 2;
  rc.setX (dx);
  rc.setY (dy);
  rc.setWidth (pxm.width ());
  rc.setHeight (pxm.height ());
  painter.drawPixmap (rc, pxm, pxm.rect ());
  rc.setHeight (rc.height () / 2);
  painter.setPen (Qt::white);
  painter.drawText (rc, Qt::AlignCenter, ::elapsedTimeToString (m_timeout - m_totalTime, false));
}

void CWaitWidget::mouseReleaseEvent (QMouseEvent*)
{
  emit stopEmitted ();
}

void CWaitWidget::changePixmap ()
{
  ++m_state;
  if (m_state > 5)
  {
    m_state = 0;
  }

  m_totalTime += m_timerInterval;
  update ();
}

void CWaitWidget::show ()
{
  QDialog::show ();
}

void CWaitWidget::startAnimation ()
{
  m_timer.start ();
  m_state     = 0;
  m_totalTime = 0;
}

