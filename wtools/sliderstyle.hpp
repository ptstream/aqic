#ifndef SLIDERSTYLE_HPP
#define SLIDERSTYLE_HPP

#include <QProxyStyle>

class CSliderStyle : public QProxyStyle
{
public:
  CSliderStyle () = default;

  void init (CSliderStyle const * other);

  QVector<QColor> const & colors () const { return m_colors; }
  void setColors (QVector<QColor> const & colors) { m_colors = colors; }

  int padding () const { return m_padding; }
  void setPadding (int padding) { m_padding = padding; }

  int handleSize () const { return m_handleSize; }
  void setHandleSize (int size) { m_handleSize = size; }

  int spacingTicksLimit () const { return m_spacingTicksLimit; }
  void setSpacingTicksLimit (int spacing) { m_spacingTicksLimit = spacing; }

  void drawComplexControl (QStyle::ComplexControl, QStyleOptionComplex const * option, QPainter* painter,
                           QWidget const * widget) const override;
protected:
  QVector<QColor> m_colors;
  int             m_handleSize        = 11;
  int             m_padding           = 2;
  int             m_spacingTicksLimit = 3;
};

#endif // SLIDERSTYLE_HPP
