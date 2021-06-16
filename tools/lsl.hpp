#ifndef LSL_HPP
#define LSL_HPP

class CLsl
{
public:
  /*! Compute the a and b coefficients of the least squares line of a set of points. */
  bool coefficients (double const * const ps, int count);

  /*! Computes start and end points of the least squares line of a set of points. */
  bool extremities (double const * const ps, int count);

  /*! Returns x coordinate of the least squares line first extremity. */
  double x1 () const { return m_x1; }

  /*! Returns y coordinate of the least squares line first extremity. */
  double y1 () const { return m_y1; }

  /*! Returns x coordinate of the least squares line second extremity. */
  double x2 () const { return m_x2; }

  /*! Returns x coordinate of the least squares line second extremity. */
  double y2 () const { return m_y2; }

protected:
  double m_a = 0.0, m_b = 0.0; //!< Line coefficients
  double m_x1 = 0.0, m_y1 = 0.0, m_x2 = 0.0, m_y2 = 0.0; //!< Extremities
};

#endif // LSL_HPP
