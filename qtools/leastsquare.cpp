#include "leastsquare.hpp"

CLeastSquare::CLeastSquare (QVector<QPointF> const & m_ps) : m_ps (m_ps)
{
}

QPair<double, double> CLeastSquare::lineCoefficients ()
{
  double a = 0.0, b = 0.0;
  if (m_ps.size () > 1)
  {
    double xsum = 0.0, ysum = 0.0, xysum = 0.0, xxsum = 0.0;
    for (QPointF const & p : m_ps)
    {
      double x = p.x ();
      double y = p.y ();
      xsum   += x;
      ysum   += y;
      xysum  += x * y;
      xxsum  += x * x;
    }

    int n = m_ps.size ();
    a     = (n * xysum - xsum * ysum) / (n * xxsum - xsum * xsum);
    b     = (ysum - a * xsum) / n;
  }

  return { a, b };
}

QPair<QPointF, QPointF> CLeastSquare::lineExtremities (double x0, double x1)
{
  QPair<double, double> c  = lineCoefficients ();
  double               y0 = c.first * x0 + c.second;
  double               y1 = c.first * x1 + c.second;
  return { { x0, y0 }, { x1, y1 } };
}

QPair<QPointF, QPointF> CLeastSquare::lineExtremities ()
{
  return lineExtremities (m_ps.first ().x (), m_ps.last ().x ());
}
