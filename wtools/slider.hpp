#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "../tools/status.hpp"
#include <QSlider>
#include <QLocale>
#include <QDate>

class CSliderStyle;
class CMagGlass;

/*! \brief This slider is the widget for controlling a bounded of days with possible two colors
 *  Each day is represented by a color on either side of a tick. Two colors are possible.
 *  They are use to show the average and the max. Each tick represents a day at 12h PM.
 *  This class changes the page up and page down QSlider mechanism.
 *  When page up or page down is clicked, the value goes to the position of the cursor. By default
 *  the value is incremented or decremented by pageStep.
 *  A zoom is activated by the shift key.
 */
class CSlider : public QSlider
{
  Q_OBJECT
  friend CMagGlass;
public:
  /*! Default constructor. */
  CSlider (QWidget* parent = nullptr);

  /*! Sets the QSlider value at the cursor position. */
  void jumpToMousePosition (int action);
  int mousePositionNearestValue () const;

  int handleWidth () const { return m_handleWidth; }
  void setHandleWidth (int width)  { m_handleWidth = width; }

  int handleHeight () const { return m_handleHeight; }
  void setHandleHeight (int height) { m_handleHeight = height; }

  QList<QDate> const & dates () { return m_dates; }
  void setDates (QList<QDate> const & dates) { m_dates = dates; }

  QList<QPair<QColor, QColor>> const & tickColors () const { return m_colors; }
  void setTickColors (QList<QPair<QColor, QColor>> const & colors);

  bool isToolTipAccepted () const { return m_toolTipAccepted; }
  void setToolTipAccepted (bool accepted) { m_toolTipAccepted = accepted; }

  void toggleAverage ();
  bool average () const { return m_average; }
  void setAverage (bool set) { m_average = set; }

  bool isClone () const { return m_original != nullptr; }
  CSlider* originalModel () const { return m_original; }
  bool cloneAccepted () const { return m_cloneAccepted; }
  void setCloneAccepted (bool accepted) { m_cloneAccepted = accepted; }

  CSliderStyle* style () { return m_style; }
  CSliderStyle const * style () const { return m_style; }

  CSlider* clone (QWidget* parent);

protected:
  void mousePressEvent (QMouseEvent* event) override;
  void mouseReleaseEvent (QMouseEvent* event) override;
  bool event (QEvent* event) override;
  void keyPressEvent (QKeyEvent* event) override;
  void keyReleaseEvent (QKeyEvent* event) override;
  void enterEvent (QEvent* event) override;
  void leaveEvent (QEvent* event) override;

  void setStyleColors ();

protected:
  CSliderStyle*                m_style        = nullptr;
  int                          m_handleWidth  = 0;
  int                          m_handleHeight = 0;
  QLocale                      m_locale;
  QList<QDate>                 m_dates;
  QList<QPair<QColor, QColor>> m_colors;
  bool                         m_average         = false;
  bool                         m_cloneAccepted   = true;
  bool                         m_toolTipAccepted = true;
  CSlider*                     m_original        = nullptr;

  static CMagGlass* m_magGlass;
};

#endif // SLIDER_HPP
