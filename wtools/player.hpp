#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QFrame>

class QToolButton;
class QComboBox;
class QSlider;

namespace Ui {
  class CPlayer;
}

/*! \brief This class is used to show the index at different dates. */
class CPlayer : public QFrame
{
  Q_OBJECT

public:
  /*! Default constructor. */
  explicit CPlayer (QWidget* parent = nullptr);

  /*! Destructor. */
  ~CPlayer ();

  /*! Returns the play/pause button. */
  QToolButton* playButton ();

  /*! Returns the forward button. */
  QToolButton* forwardButton ();

  /*! Returns the backward button. */
  QToolButton* backwardButton ();

  /*! Returns the frame rate combobox. */
  QComboBox* frameRateComboBox ();

  /*! Returns the slider used to choose date. */
  QSlider* frameNumber ();

  /*! Returns the player state (paly or pause. */
  bool isRunning () const { return m_running; }

  /*! Returns the list of dates of the CSlider. */
  QList<QDate> const & dates () const;

  /*! Sets the list of dates of the CSlider. */
  void setDates (QList<QDate> const & dates);

  /*! Sets the maximum of dates. */
  void setFrameMaximum (int max);

  /*! Sets the pair of colors. The first of pair is for average, second for max. */
  void setTickColors (QList<QPair<QColor, QColor>> const & colors);

  /*! Returns the current frame rate. */
  int frameRate () const;

  /*! Sets the current frame rate. */
  void setFrameRate (float ms);

  /*! Sets the average of max icon. */
  void restoreAMM ();

public slots:
  /*! Sets the current date. */
  void setFrame (int count);

  /*! The play/pause button has been clicked. */
  void playStateChanged (bool = false);

  /*! Slots for all CToolButton. */
  void buttonClicked (bool);

protected slots:
  /*! average-max button has been clicked. */
  void on_m_averageMax_clicked (bool);

  /*! Highest average colors is selected. */
  void highestMeanDateSelected (bool);

  /*! Highest max colors is selected. */
  void highestMaxDateSelected (bool);

  /*! Lowest average colors is selected. */
  void lowestMeanDateSelected (bool);

  /*! Lowest max colors is selected. */
  void lowestMaxDateSelected (bool);

signals:
  void highestMeanDateChanged ();
  void highestMaxDateChanged ();
  void lowestMeanDateChanged ();
  void lowestMaxDateChanged ();

private:
  void setAverageMaxIcon ();
  void updateAMM (QAction* sender);

private:
  Ui::CPlayer* ui;
  QString      m_ammToolTip;
  bool         m_running = false;
};

#endif // PLAYER_HPP
