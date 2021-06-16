#ifndef WAITWIDGET_HPP
#define WAITWIDGET_HPP

#include <QDialog>
#include <QTimer>

class CWaitWidget : public QDialog
{
  Q_OBJECT
public:
  CWaitWidget (QWidget* parent);
  ~CWaitWidget () { m_timer.stop (); }

  void startAnimation ();
  void stopAnimation () { m_timer.stop (); }
  void show ();

  void setTimeout (int ms) { m_timeout = ms; }
  int timeout () const { return m_timeout; }

signals :
  void stopEmitted ();

protected :
  void paintEvent (QPaintEvent*) override;
  void mouseReleaseEvent (QMouseEvent*) override;

protected slots:
  void changePixmap ();

private :
  QTimer        m_timer;
  int           m_state = 0;
  int           m_timerInterval = 250;
  int           m_timeout       = 240000;
  int           m_totalTime     = 0;
  char const *  m_iconResource  = ":/icons/waitwidget_%1.png";
};

#endif // WAITWIDGET_HPP
