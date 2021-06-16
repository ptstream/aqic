#ifndef GEOCONVERTER_TEMPLATES_HPP
#define GEOCONVERTER_TEMPLATES_HPP

#include<cmath>

namespace geoconverter
{
/*! \brief Returns q.
 * \internal
 */
template<typename T>
static inline T q_internal (T phi, T e, T x1, T x3)
{
  T snlat = std::sin (phi);
  T x2    = snlat / (1.0 - e * e * snlat * snlat);
  T x4    = (1.0 - e * snlat) / (1.0 + e * snlat);
  return x1 * (x2 - x3 * std::log (x4));
};

/*! \brief Returns qp.
 * \internal
 */
template<typename T>
static inline T qp_internal (T x1, T x3, T e)
{
  return x1 * (1.0 / x1 - x3 * std::log ((1.0 - e) / (1.0 + e)));
};

/*! \brief Returns x1.
 * \internal
 */
template<typename T>
inline T x1_internal (T e)
{
  return 1.0 - e * e;
};

/*! \brief Returns x3.
 * \internal
 */
template<typename T>
inline T x3_internal (T e)
{
  return 0.5 / e;
};

/*!  \brief Returns beta1.
 * \internal
 */
template<typename T>
inline T beta1_internal (T q1, T qp)
{
  return std::asin (q1 / qp);
}

/*! \brief Returns rq.
 * \internal
 */
template<typename T>
inline T rq_internal (T a, T qp)
{
  return a * std::sqrt (qp / 2.0);
}

/*! \brief Returns m1.
 * \internal
 */
template<typename T>
inline T m1_internal (T phi1, T e, T snphi1)
{
  return std::cos (phi1) / std::sqrt (1.0 - e * e * snphi1 * snphi1);
}

/*! \brief Returns d.
 * \internal
 */
template<typename T>
inline T d_internal (T a, T m1, T rq, T csbeta1)
{
  return a * m1 / (rq * csbeta1);
}

/* \brief Converts radian to degree.
 * \param rad : Angle in radian.
 * \return Angle in degree.
 */
template<typename T>
inline T rdDg (T rad) { return rad * 180.0 / M_PI; }

/* \brieg Converts degree to radian.
 * \param degree: Angle in degree.
 * \return Angle in radian.
 */
template<typename T>
inline T dgRd (T degree) { return degree * M_PI / 180.0; }

/*! \brief Converts WGS84 coordinates to Lambert Azimuthal Equal Area (LAEA).
 *
 * This function transforms the WGS84 (latitude, longitude) to x, y Lambert Azimuthal Equal Area (LAEA).
 * The conversion is coded from https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0072.pdf.
 * The algorithm name is ALG0072.
 * For minimum precision T must be double.
 *
 * \param phi: Latitude in degrees.
 * \param lambda: Longitude in degrees
 * \param a: Ellipsoid half long axis
 * \param e: Ellipsoid first eccentricity
 * \param lambda0: Origin point longitude relative to the meridian origin
 * \param phi1: Latitude of the origin point
 * \param x0: Offset on x axis
 * \param y0: Offset on y axis
 * \return The coordinates x, y in meters.
 */
template<typename T>
std::pair<T, T> TWgs84ToLaea (T phi, T lambda, T a, T e, T lambda0, T phi1, T x0, T y0)
{
  T const eps    = 1.0e-10;
  T const x1     = x1_internal (e);
  T const x3     = x3_internal (e);
  T const qp     = qp_internal (x1, x3, e);
  T const snphi1 = std::sin (phi1);

  lambda  = dgRd (lambda);
  phi     = dgRd (phi);
  T q1    = q_internal (phi1, e, x1, x3);
  T q     = q_internal (phi, e, x1, x3);
  T csll0 = std::cos (lambda - lambda0);
  T snll0 = std::sin (lambda - lambda0);
  T x, y;
  if (std::fabs (phi1 - M_PI_2) < eps) // phi1=pi/2
  { // Particular case (1)
    T ro = a * std::sqrt (qp - q);
    x    = +ro * snll0 + x0;
    y    = -ro * csll0 + y0;
  }
  else if (std::fabs (phi1 + M_PI_2) < eps) // phi1=-pi/2
  { // Particular case (2)
    T ro = a * std::sqrt (qp - q);
    x    = ro * snll0 + x0;
    y    = ro * csll0 + y0;
  }
  else
  { // Normal case.
    T beta1   = beta1_internal (q1, qp);
    T rq      = rq_internal (a, qp);
    T beta    = std::asin (q / qp);
    T m1      = m1_internal (phi1, e, snphi1);
    T csbeta1 = std::cos (beta1);
    T snbeta1 = std::sin (beta1);
    T csbeta  = std::cos (beta);
    T snbeta  = std::sin (beta);
    T d       = d_internal (a, m1, rq, csbeta1);
    T b       = rq * std::sqrt (2.0 / (1.0 + snbeta1 * snbeta + csbeta1 * csbeta * csll0));
    x         = b * d * csbeta * snll0 + x0;
    y         = b * (csbeta1 * snbeta - snbeta1 * csbeta * csll0) / d + y0;
  }

  return { x, y };
}

/*! \brief Converts Lambert Azimuthal Equal Area (LAEA) coordinates to WGS84.
 *
 * This function transforms x, y Lambert Azimuthal Equal Area (LAEA) to WGS84 (latitude, longitude).
 * The conversion is coded from https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0073.pdf.
 * The algorithm name is ALG0073.
 * For minimum precision T must be double.
 *
 * \param x: Abscissa in meters
 * \param y: Ordinate in meters.
 * \param a: Ellipsoid half long axis
 * \param e: Ellipsoid first eccentricity
 * \param lambda0: Origin point longitude relative to the meridian origin
 * \param phi1: Latitude of the origin point
 * \param x0: Offset on x axis
 * \param y0: Offset on y axis
 * \return The latitude (first) and the longitude (second) in degrees.
 */
template<typename T>
std::pair<T, T> TLaeaToWgs84 (T x, T y, T a, T e, T lambda0, T phi1, T x0, T y0)
{
  std::pair<T, T> ll;

  T const eps     = 1.0e-11;
  T const snphi1  = std::sin (phi1);

  x          -= x0;
  y          -= y0;
  T const x1  = x1_internal (e); // 1 - e * e
  T const x3  = x3_internal (e); // 1 / (2 * e)
  T const qp  = qp_internal (x1, x3, e);

  // Refines the latitude from phi0.
  auto fphii = [e, x1, x3, eps] (T phii1, T q) -> double
  {
    T phii;
    do
    {
      phii          = phii1;
      T snphii = std::sin (phii); // sin(phhi)
      T csphii = std::cos (phii); // cos(phii)
      T x2     = 1.0 - e * e * snphii * snphii; // 1-e*e*sin(phii)*sin(phii)
      T x4     = 0.5 * x2 * x2 / csphii; // (1-e*e*sin(phii)*sin(phii)/(2*cos(phi1))
      T x5     = q / x1; // q/(1-e*e)
      T x6     = snphii / x2; // sin(phi1)/(1-e*e*sin(phii)*sin(phii)
      T x7     = x3 * std::log ((1.0 - e * snphii) / (1.0 + e * snphii)); // (1/2e)*ln((1-e*sin(phii) /(1+e*sin(phii))
      phii1    = phii + x4 * (x5 - x6 + x7);

    } while (std::fabs (phii1 - phii) > eps);

    return phii;
  };

  if (std::fabs (phi1 - M_PI_2) < eps) // phi1=pi/2
  { // Particular case (1)
    T ro      = std::sqrt (x * x + y * y);
    T roa     = ro / a;
    T q       = qp - (roa * roa);
    if (std::fabs (q - qp) < eps || std::fabs (q + qp) < eps) // q=+-qp
    { // Particular case (4)
      ll.second = M_PI_2 * q / std::fabs (q);
    }
    else
    {
      T phi0    = std::asin (0.5 * q);
      T phii    = fphii (phi0, q);
      ll.first  = phii;
      ll.second = lambda0 + std::atan2 (x, -y); // lambda
    }
  }
  else if (std::fabs (phi1 + M_PI_2) < eps) // phi1=-pi/2
  { // Particular case (2)
    T ro      = std::sqrt (x * x + y * y);
    T roa     = ro / a;
    T q       = (roa * roa) - qp;
    if (std::fabs (q - qp) < eps || std::fabs (q + qp) < eps) // q=+-qp
    { // Particular case (4)
      ll.second = M_PI_2 * q / std::fabs (q);
    }
    else
    {
      T phi0    = std::asin (0.5 * q);
      T phii    = fphii (phi0, q);
      ll.first  = phii;
      ll.second = lambda0 + std::atan2 (x, y); // lambda
    }
  }
  else
  { // Normal case.
    T q1        = q_internal (phi1, e, x1, x3);
    T beta1     = beta1_internal (q1, qp);
    T rq        = rq_internal (a, qp);
    T m1        = m1_internal (phi1, e, snphi1);
    T csbeta1   = std::cos (beta1);
    T snbeta1   = std::sin (beta1);
    T d         = d_internal (a, m1, rq, csbeta1);
    T ro        = std::sqrt ((x / d) * (x / d) + (d * y) * (d * y));
    T ce        = 2.0 * std::asin (0.5 * ro / rq);
    T csce      = std::cos (ce);
    T snce      = std::sin (ce);
    if (std::fabs (ro) < eps)
    { // Particular case (3)
      ll.first  = phi1;
      ll.second = lambda0;
    }
    else
    {
      T q = qp * (csce * snbeta1 + d * y * snce * csbeta1 / ro);
      if (std::fabs (q - qp) < eps || std::fabs (q + qp) < eps) // q=+-qp
      { // Particular case (4)
        ll.second = M_PI_2 * q / std::fabs (q);
      }
      else
      {
        T lambda  = lambda0 + std::atan (x * snce / (d * ro * csbeta1 * csce - d * d * y * snbeta1 * snce));
        T phi0    = std::asin (q / 2.0);
        T phii    = fphii (phi0, q);
        ll.first  = phii;
        ll.second = lambda;
      }
    }
  }

  ll.first  = rdDg (ll.first); // latitude
  ll.second = rdDg (ll.second); // longitude
  return ll;
}

/*! \brief Converts WGS84 coordinates to pseudo Mercator.
 *
 * This function transforms the WGS84 (latitude, longitude) to x, y pseudo Mercato.
 * The conversion is coded from https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0076.pdf.
 * The algorithm name is ALG0076.
 * For minimum precision T must be double.
 *
 * \param phi: Latitude in degrees.
 * \param lambda: Longitude in degrees
 * \param a: Ellipsoid half long axis
 * \param lambda0: Origin point longitude relative to the meridian origin
 * \param x0: Offset on x axis
 * \param y0: Offset on y axis
 * \return The coordinates x, y in meters.
 */
template<typename T>
std::pair<T, T> TWgs84ToPseudoMercator (T phi, T lambda, T a, T lambda0, T x0, T y0)
{
  phi     = dgRd (phi);
  lambda  = dgRd (lambda);
  T x     = a * (lambda - lambda0) + x0;
  T snphi = std::sin (phi);
  T y     = 0.5 * a * std::log ((1.0 + snphi) / (1.0 - snphi)) + y0;
  return { x, y };
}

/*! \brief Converts pseudo Mercator coordinates to WGS84.
 *
 * This function transforms x, y pseudo Mercator to WGS84 (latitude, longitude).
 * The conversion is coded from https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0077.pdf.
 * The algorithm name is ALG0077.
 * For minimum precision T must be double.
 *
 * \param x: Abscissa in meters
 * \param y: Ordinate in meters.
 * \param a: Ellipsoid half long axis
 * \param lambda0: Origin point longitude relative to the meridian origin
 * \param x0: Offset on x axis
 * \param y0: Offset on y axis
 * \return The latitude (first) and the longitude (second) in degrees.
 */
template<typename T>
std::pair<T, T> TPseudoMercatorToWgs84 (T x, T y, T a, T lambda0, T x0, T y0)
{
  std::pair<T, T> ll;
  ll.first  = std::atan (std::sinh ((y - y0) / a));
  ll.second = (x - x0) / a + lambda0;
  ll.first  = rdDg (ll.first); // latitude
  ll.second = rdDg (ll.second); // longitude
  return ll;
}
} // Namespace
#endif // GEOCONVERTER_TEMPLATES_HPP
