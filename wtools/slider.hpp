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

  /*! Returns the nearest value from position. */
  int mousePositionNearestValue () const;

  /*! Returns the handle width. */
  int handleWidth () const { return m_handleWidth; }

  /*! Sets the handle width. */
  void setHandleWidth (int width)  { m_handleWidth = width; }

  /*! Returns the handle height. */
  int handleHeight () const { return m_handleHeight; }

  /*! Sets the handle height. */
  void setHandleHeight (int height) { m_handleHeight = height; }

  /*! Returns the dates. */
  QList<QDate> const & dates () { return m_dates; }

  /*! Sets the dates. */
  void setDates (QList<QDate> const & dates) { m_dates = dates; }

  /*! Returns the color pairs associated at dates. */
  QList<QPair<QColor, QColor>> const & tickColors () const { return m_colors; }

  /*! Sets the color pairs associated at dates. */
  void setTickColors (QList<QPair<QColor, QColor>> const & colors);

  /*! Returns the tooltip state. */
  bool isToolTipAccepted () const { return m_toolTipAccepted; }

  /*! Sets the tooltip state. */
  void setToolTipAccepted (bool accepted) { m_toolTipAccepted = accepted; }

  /*! Toggles from average to date to average. */
  void toggleAverage ();

  /*! Returns true is colors represent average otherwize false if colors represent max. */
  bool average () const { return m_average; }

  /*! Sets true is colors represent average otherwize false if colors represent max. */
  void setAverage (bool set) { m_average = set; }

  /*! Returns true if this is a clone. The clone is the CMagGlass widget. */
  bool isClone () const { return m_original != nullptr; }

  /*! Returns true if this is not a clone. */
  CSlider* originalModel () const { return m_original; }

  /*! Returns true if this accepts cloning. */
  bool cloneAccepted () const { return m_cloneAccepted; }

  /*! Sets true if this accepts cloning. */
  void setCloneAccepted (bool accepted) { m_cloneAccepted = accepted; }

  /*! Returns the style of CSlider. */
  CSliderStyle* style () { return m_style; }

  /*! Returns the style of CSlider. */
  CSliderStyle const * style () const { return m_style; }

  /*! Returns the clone widget or null if this as no clone. */
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
