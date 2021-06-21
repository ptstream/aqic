#ifndef WAITWIDGET_HPP
#define WAITWIDGET_HPP

#include <QDialog>
#include <QTimer>

/*! \brief This class is used to show a progress widget during download data from server. */
class CWaitWidget : public QDialog
{
  Q_OBJECT
public:
  /*! Default constructor. */
  CWaitWidget (QWidget* parent);

  /*! Destructor. */
  ~CWaitWidget () { m_timer.stop (); }

  /*! Start the animation. The animation shows different pixmap. */
  void startAnimation ();

  /*! Stop animation. */
  void stopAnimation () { m_timer.stop (); }

  /*! Just call QDialog show. */
  void show ();

  /*! Sets timeout between 2 pixmaps. By default 250ms. */
  void setTimeout (int ms) { m_timeout = ms; }

  /*! Returns timeout between 2 pixmaps. By default 250ms. */
  int timeout () const { return m_timeout; }

signals :
  /*! Signal emitted when mouse release event. */
  void stopEmitted ();

protected :
  void paintEvent (QPaintEvent*) override;
  void mouseReleaseEvent (QMouseEvent*) override;

protected slots:
  /*! Changes pixmap after timeout. */
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
