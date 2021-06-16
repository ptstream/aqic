#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QFrame>

class QToolButton;
class QComboBox;
class QSlider;

namespace Ui {
  class CPlayer;
}

class CPlayer : public QFrame
{
  Q_OBJECT

public:
  explicit CPlayer (QWidget* parent = nullptr);
  ~CPlayer ();

  QToolButton* playButton ();
  QToolButton* forwardButton ();
  QToolButton* backwardButton ();
  QComboBox* frameRateComboBox ();
  QSlider* frameNumber ();

  bool isRunning () const { return m_running; }

  QList<QDate> const & dates () const;
  void setDates (QList<QDate> const & dates);

  void setFrameMaximum (int max);
  void setTickColors (QList<QPair<QColor, QColor>> const & colors);
  int frameRate () const;
  void setFrameRate (float ms);
  void restoreAMM ();

public slots:
  void setFrame (int count);
  void playStateChanged (bool = false);
  void buttonClicked (bool);

protected slots:
  void on_m_averageMax_clicked (bool);
  void highestMeanDateSelected (bool);
  void highestMaxDateSelected (bool);
  void lowestMeanDateSelected (bool);
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
