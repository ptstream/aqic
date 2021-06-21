#ifndef SLIDERSTYLE_HPP
#define SLIDERSTYLE_HPP

#include <QProxyStyle>

/*! \brief This class is used to replace standard CSlider representation by colors representation. */
class CSliderStyle : public QProxyStyle
{
public:
  /*! Default constructor. */
  CSliderStyle () = default;

  /*! Initializes from another style. */
  void init (CSliderStyle const * other);

  /*! Returns the current colors. */
  QVector<QColor> const & colors () const { return m_colors; }

  /*! Sets the current colors. */
  void setColors (QVector<QColor> const & colors) { m_colors = colors; }

  /*! Returns the padding in pixels. The padding is the position of the first or last tick from extremities. */
  int padding () const { return m_padding; }

  /*! Sets the padding in pixels.  The padding is the position of the first or last tick from extremities. */
  void setPadding (int padding) { m_padding = padding; }

  /*! Returns the handle size in pixels. */
  int handleSize () const { return m_handleSize; }

  /*! Sets the handle size in pixels. */
  void setHandleSize (int size) { m_handleSize = size; }

  /*! Returns the spacing ticks limit. If tick spacing is less that this limit, the ticks are hidden. */
  int spacingTicksLimit () const { return m_spacingTicksLimit; }

  /*! Sets the spacing ticks limit. If tick spacing is less that this limit, the ticks are hidden. */
  void setSpacingTicksLimit (int spacing) { m_spacingTicksLimit = spacing; }

  /*! Draws the slider (colors, ticks and handle. */
  void drawComplexControl (QStyle::ComplexControl, QStyleOptionComplex const * option, QPainter* painter,
                           QWidget const * widget) const override;
protected:
  QVector<QColor> m_colors;
  int             m_handleSize        = 11;
  int             m_padding           = 2;
  int             m_spacingTicksLimit = 3;
};

#endif // SLIDERSTYLE_HPP
