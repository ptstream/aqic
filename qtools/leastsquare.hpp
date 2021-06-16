#ifndef LEASTSQUARE_HPP
#define LEASTSQUARE_HPP

#include <QVector>
#include <QPointF>

/*! \brief This class allows to compute least square line from a set of points. */
class CLeastSquare
{
public:
  /*! Consructor. */
  CLeastSquare (QVector<QPointF> const & ps);

  /*! Returns the line coefficients a, b like y = ax+b */
  QPair<qreal, qreal> lineCoefficients ();

  /*! Returns the line extremities */
  QPair<QPointF, QPointF> lineExtremities ();

  /*! Returns the line extremities from a first point. */
  QPair<QPointF, QPointF> lineExtremities (double x0, double x1);

private :
  QVector<QPointF> const & m_ps;
};

#endif // LEASTSQUARE_HPP
