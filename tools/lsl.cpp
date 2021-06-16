#include "lsl.hpp"

bool CLsl::coefficients (double const * const ps, int count)
{
  bool error = true;
  m_a        = m_b = 0.0;
  if (ps != nullptr && count > 3)
  {
    double xsum = 0.0, ysum = 0.0, xysum = 0.0, xxsum = 0.0;
    for (int i = 0; i < count; i += 2)
    {
      double x = ps[i];
      double y = ps[i + 1];
      xsum    += x;
      ysum    += y;
      xysum   += x * y;
      xxsum   += x * x;
    }

    m_a   = (count * xysum - xsum * ysum) / (count * xxsum - xsum * xsum);
    m_b   = (ysum - m_a * xsum) / count;
    error = false;
  }

  return error;
}

bool CLsl::extremities (double const * const ps, int count)
{
  bool error = true;
  m_x1       = 0.0, m_y1 = 0.0, m_x2 = 0.0, m_y2 = 0.0;
  if (m_a == 0.0 && m_b == 0.0)
  {
    error = coefficients (ps, count);
  }

  if (!error)
  {
    m_x1 = ps[0];
    m_y1 = m_a * m_x1 + m_b;
    m_x2 = ps[count - 1];
    m_y2 = m_a * m_x2 + m_b;
  }
  else if (count == 4)
  {
    m_x1 = ps[0];
    m_y1 = ps[1];
    m_x2 = ps[2];
    m_y2 = ps[3];
  }

  return error;
}
