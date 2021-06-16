#include "player.hpp"
#include "ui_player.h"
#include <QLineEdit>
#include <QMenu>
#include <array>

CPlayer::CPlayer (QWidget* parent) : QFrame (parent), ui (new Ui::CPlayer)
{
  ui->setupUi (this);

  ui->m_frameNumber->setCloneAccepted (true);
  setFrameShape (QFrame::Box);
  auto le = ui->m_frameRate->lineEdit ();
  le->setReadOnly (true);

  auto menu = new QMenu (this);
  ui->m_amm->setMenu (menu);
  auto group = new QActionGroup (menu);
  m_ammToolTip = ui->m_amm->toolTip ();

  std::array<QString, 4> icons = { ":/icons/hm.png",
                                   ":/icons/lm.png",
                                   ":/icons/ha.png",
                                   ":/icons/la.png",
                                 };

  std::array<QString, 4> titles = { tr ("Date on which the max is highest"),
                                    tr ("Date on which the max is lowest"),
                                    tr ("Date on which the average is highest"),
                                    tr ("Date on which the average is lowest"),
                                  };

  using TFunctor = void (CPlayer::*) (bool);
  std::array<TFunctor, 4> functors = { &CPlayer::highestMaxDateSelected,
                                       &CPlayer::lowestMaxDateSelected,
                                       &CPlayer::highestMeanDateSelected,
                                       &CPlayer::lowestMeanDateSelected,
                                     };

  for (size_t i = 0, count = icons.size (); i < count; ++i)
  {
    QAction* action = menu->addAction (QIcon (icons[i]), titles[i], this, functors[i]);
    action->setCheckable (action);
    group->addAction (action);
  }

  setAverageMaxIcon ();
  QList<QToolButton*> buttons = findChildren<QToolButton*> ();
  for (QToolButton* button : qAsConst (buttons))
  {
    if (button != ui->m_amm)
    {
      connect (button, &QToolButton::clicked, this, &CPlayer::buttonClicked);
    }
  }
}

CPlayer::~CPlayer ()
{
  delete ui;
}

void CPlayer::updateAMM (QAction* action)
{
  ui->m_amm->setIcon (action->icon ());
  ui->m_amm->setToolTip (action->text ());
}

void CPlayer::restoreAMM ()
{
  ui->m_amm->setIcon (QIcon (":/icons/amm"));
  ui->m_amm->setToolTip (m_ammToolTip);
}

void CPlayer::highestMeanDateSelected (bool)
{
  updateAMM (static_cast<QAction*>(sender ()));
  emit highestMeanDateChanged ();
}

void CPlayer::highestMaxDateSelected (bool)
{
  updateAMM (static_cast<QAction*>(sender ()));
  emit highestMaxDateChanged ();
}

void CPlayer::lowestMeanDateSelected (bool)
{
  updateAMM (static_cast<QAction*>(sender ()));
  emit lowestMeanDateChanged ();
}

void CPlayer::lowestMaxDateSelected (bool)
{
  updateAMM (static_cast<QAction*>(sender ()));
  emit lowestMaxDateChanged ();
}

QToolButton* CPlayer::playButton ()
{
  return ui->m_play;
}

QToolButton* CPlayer::forwardButton ()
{
  return ui->m_forward;
}

QToolButton* CPlayer::backwardButton ()
{
  return ui->m_backward;
}

QComboBox* CPlayer::frameRateComboBox ()
{
  return ui->m_frameRate;
}

QSlider* CPlayer::frameNumber ()
{
  return ui->m_frameNumber;
}

void CPlayer::setFrameMaximum (int max)
{
  ui->m_frameNumber->setMaximum (max);
}

void CPlayer::setDates (QList<QDate> const & dates)
{
  ui->m_frameNumber->setDates (dates);
}

QList<QDate> const & CPlayer::dates () const
{
  return ui->m_frameNumber->dates ();
}

void CPlayer::playStateChanged (bool)
{
  m_running = !m_running;
  QIcon icon (m_running ? ":/icons/player_pause.png" : ":/icons/player_play.png");
  ui->m_play->setIcon (icon);
}

void CPlayer::setFrame (int value)
{
  bool signalBlocked = ui->m_frameNumber->signalsBlocked ();
  if (!signalBlocked)
  {
    ui->m_frameNumber->blockSignals (true);
  }

  ui->m_frameNumber->setValue (value);
  if (!signalBlocked)
  {
    ui->m_frameNumber->blockSignals (false);
  }
}

int CPlayer::frameRate () const
{
  std::array<int, 9> intervals = { 1000 / 24, 1000 / 12, 1000 / 8, 1000 / 6, 1000 / 4, 1000 / 2, 1000, 2000, 4000 };
  int                index     = ui->m_frameRate->currentIndex ();
  return intervals[index];
}

void CPlayer::setFrameRate (float fps)
{
  int prec;
  if (fps > 10.0F)
  {
    prec = 0;
  }
  else if (fps > 1.0F)
  {
    prec = 1;
  }
  else
  {
    prec = 2;
  }

  ui->m_frameRate->setCurrentText (QString ("%1 fps").arg (fps, 0, 'f', prec));
}

void CPlayer::on_m_averageMax_clicked (bool)
{
  ui->m_frameNumber->toggleAverage ();
  setAverageMaxIcon ();
}

void CPlayer::buttonClicked (bool)
{
  QToolButton* button = static_cast<QToolButton*>(this->sender ());
  if (button != ui->m_amm)
  {
    restoreAMM ();
  }

  if (button == ui->m_play)
  {
    playStateChanged ();
  }
}

void CPlayer::setTickColors (QList<QPair<QColor, QColor>> const & colors)
{
  ui->m_frameNumber->setTickColors (colors);
}

void CPlayer::setAverageMaxIcon ()
{
  QString toolTip;
  QString icon;
  if (ui->m_frameNumber->average ())
  {
    toolTip = tr ("The colors of the slider represent the average");
    icon    = ":/icons/xbar.png";
  }
  else
  {
    toolTip = tr ("The colors of the slider represent the maximum");
    icon    = ":/icons/max.png";
  }

  ui->m_averageMax->setIcon (QIcon (icon));
  ui->m_averageMax->setToolTip (toolTip);
}



