#include "magglass.hpp"
#include "tickvalues.hpp"
#include "../wtools/slider.hpp"
#include "../wtools/sliderstyle.hpp"
#include <QVBoxLayout>
#include <QEvent>
#include <QScreen>
#include <QResizeEvent>
#include <QDebug>

CMagGlass::CMagGlass (CSlider* slider, int value, QWidget* parent) :
  QDialog (parent, Qt::CustomizeWindowHint)
{
  setAttribute (Qt::WA_DeleteOnClose);
  auto layout = new QVBoxLayout (this);
  layout->setContentsMargins (0, 0, 0, 0);
  layout->setSpacing (0);

  auto* dates = new CTickValues (this);
  layout->addWidget (dates);
  QFontMetrics fms (dates->font ());
  QString values ("  88/88   ");
  int     width = fms.horizontalAdvance (values);
  width         = width * (m_interval + 2);
  dates->setMinimumWidth (width);

  CSlider* clone = slider->clone (this);
  layout->addWidget (clone);
  connect (clone, &CSlider::sliderMoved,  slider, &CSlider::sliderMoved);
  connect (clone, &CSlider::valueChanged, slider, &CSlider::valueChanged);

  int min = clone->minimum ();
  int max = clone->maximum ();
  if (m_interval > max - min - 1)
  {
    m_interval = max - min;
  }

  if (value - m_interval / 2 < min)
  {
    max = min + m_interval;
  }
  else if (value + m_interval / 2 > max)
  {
    min = max - m_interval;
  }
  else
  {
    min = std::max (value - m_interval / 2 - 1, min);
    max = min + m_interval;
  }

  clone->blockSignals (true);
  clone->setMinimum (min);
  clone->setMaximum (max);
  clone->blockSignals (false);

  dates->setDates (clone->dates ());
  dates->setMin (min);
  dates->setMax (max);
  CSliderStyle* sliderStyle = slider->style ();
  dates->setPadding (sliderStyle->padding ());
  dates->setOffset (sliderStyle->handleSize () / 2);

  // Initialize the minimum widget size.
  QWidget* mainWindow = this->mainWindow ();
  QSize    size (m_width, m_height);
  if (mainWindow != nullptr)
  {
    size  = mainWindow->size ();
    int m = 1;
    int d = 2;
    if (size.width () <= 1024)
    {
      m = 3;
      d = 4;
    }

    size = QSize (m * mainWindow->width () / d, slider->height () * 2);
  }

  resize (size);
  dates->resize (size.width (), dates->height ());
  show ();
}

CMagGlass::~CMagGlass ()
{
  CSlider::m_magGlass = nullptr;
}

QWidget* CMagGlass::mainWindow () const
{
  QWidget* mainWindow = parentWidget ();
  while (mainWindow != nullptr && !mainWindow->inherits ("QMainWindow"))
  {
    mainWindow = mainWindow->parentWidget ();
  }

  return mainWindow;
}

bool CMagGlass::event (QEvent* e)
{
  switch (e->type ())
  {
    case QEvent::WindowDeactivate:
      close ();
      break;

    default:
      break;
  }

  return QDialog::event (e);
}


void CMagGlass::resizeEvent (QResizeEvent* event)
{
  // Adjust the position from the screen.
  QSize    size        = event->size ();
  QPoint   pos         = QCursor::pos (); // Screen coordinates
  QPoint   magGlassPos = pos - QPoint (size.width () / 2, size.height () / 2);
  QWidget* mainWindow  = this->mainWindow ();
  if (mainWindow != nullptr)
  {
    QScreen* screen     = mainWindow->screen ();
    QSize    screenSize = screen->availableSize ();
    if ((pos.x () + size.width () / 2 >= screenSize.width ()))
    {
      magGlassPos.setX (screenSize.width () - size.width () - 4);
    }

    if (pos.x () - size.width () / 2 < 0)
    {
      magGlassPos.setX (4);
    }
  }

  move (magGlassPos);
  setFixedSize (size);
}

