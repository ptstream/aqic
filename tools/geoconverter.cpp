#include "geoconverter.hpp"
#include "geoconverter_templates.hpp"

static double latitudeFromLatitudeISO (double lat_iso)
{
  double const eps = 1.0e-10;
  auto phi = [lat_iso] (double phi_0) -> double
  {
    double const e   = 0.08181919106;
    double       sp0 = std::sin (phi_0);
    return 2.0 * std::atan (std::pow ((1.0 + e * sp0) /
           (1.0 - e * sp0), e / 2.0) * lat_iso) - M_PI_2;
  };

  double phi_0 = 2.0 * std::atan (std::exp (lat_iso)) - M_PI_2;
  double phi_i = phi (phi_0);
  double delta;
  while (static_cast<void>(delta = std::fabs (phi_i - phi_0)), delta > eps)
  {
    phi_0 = phi_i;
    phi_i = phi (phi_0);
  }

  return phi_i;
}

std::pair<double, double> CGeoConverter::lambert93ToWgs84 (double x, double y)
{
  double const lon_merid  = 3.0 * M_PI / 180.0;
  double const lambert_n  = 0.7256077650;
  double const lambert_c  = 11754255.426;
  double const lambert_xs = 700000.0;
  double const lambert_ys = 12655612.05;

  double x0      = x - lambert_xs;
  double y0      = y - lambert_ys;
  double r       = std::sqrt (x0 * x0 + y0 * y0);
  double gamma   = std::atan (x0 / (-y0));
  double lon     = lon_merid + gamma / lambert_n;
  double lat_iso = -1.0 / lambert_n * std::log (std::fabs (r / lambert_c));
  lat_iso        = std::exp (lat_iso);
  double lat     = latitudeFromLatitudeISO (lat_iso);
  return { geoconverter::rdDg (lat), geoconverter::rdDg (lon) };
}

std::pair<double, double> CGeoConverter::wgs84ToLambert93 (double lat, double lon)
{
  double a    = 6378137.0; // Ellipsoid half long axis (m)
  double e    = 0.08181919106; //  Ellipsoid first eccentricity
  double l0   = (M_PI / 180.0) * 3.0;
  double lc   = l0;
  double phi0 = (M_PI / 180.0) * 46.5; // Original latitude in radians
  double lat1 = (M_PI / 180.0) * 44.0; // First parallel automecoic
  double phi2 = (M_PI / 180.0) * 49.0; // Second parallel automecoic

  double x0 = 700000.0; // Origin coordinate
  double y0 = 6600000.0; // Origin coordinate

  double phi = geoconverter::dgRd (lat);
  double l   = geoconverter::dgRd (lon);

  // Large normal Calculations
  auto fgn = [a, e] (double w) -> double
  {
    double snw = std::sin (w);
    return a / std::sqrt (1.0 - e * e * snw * snw);
  };

  double gN1 = fgn (lat1);
  double gN2 = fgn (phi2);

  // Isometric latitude calculations
  auto fgl = [e] (double w) -> double
  {
    double snw = std::sin (w);
    return std::log (std::tan (M_PI_4 + w / 2.0) * std::pow ((1.0 - e * snw) / (1.0 + e * snw), e / 2.0));
  };

  double gl1 = fgl (lat1);
  double gl2 = fgl (phi2);
  double gl0 = fgl (phi0);
  double gl  = fgl (phi);

  // Exponent calculation of the projection
  double n = std::log ((gN2 * std::cos (phi2)) / (gN1 * std::cos (lat1))) / (gl1 - gl2);

  // Constant of the projection
  double c = ((gN1 * std::cos (lat1)) / n) * std::exp (n * gl1);

  // Coordinates
  double ys = y0 + c * std::exp (-1.0 * n * gl0);

  double exp  = std::exp (-1.0 * n * gl);
  double nllc = n * (l - lc);
  double x93  = x0 + c * exp * std::sin (nllc);
  double y93  = ys - c * exp * std::cos (nllc);
  return { x93, y93 };
}

std::pair<double, double> CGeoConverter::wgs84ToEtrs89Laea (double phi, double lambda)
{
  double const a       = 6378137.0;      // Ellipsoid half long axis
  double const e       = 0.081819191043; // Ellipsoid first eccentricity
  double const lambda0 = 0.174532925199; // 10° origin point longitude relative to the meridian origin
  double const phi1    = 0.907571211037; // 52° latitude of the origin point
  double const x0      = 4321000.0;      // Constante on x axis
  double const y0      = 3210000.0;      // Constante on y axis
  return geoconverter::TWgs84ToLaea (phi, lambda, a, e, lambda0, phi1, x0, y0);
}

std::pair<double, double> CGeoConverter::etrs89LaeaToWgs84 (double x, double y)
{
  double const a       = 6378137.0;      // Ellipsoid half long axis
  double const e       = 0.081819191043; // Ellipsoid first eccentricity
  double const lambda0 = 0.174532925199; // 10° origin point longitude relative to the meridian origin
  double const phi1    = 0.907571211037; // 52° latitude of the origin point
  double const x0      = 4321000.0;      // Constante on x axis
  double const y0      = 3210000.0;      // Constante on y axis
  return geoconverter::TLaeaToWgs84 (x, y, a, e, lambda0, phi1, x0, y0);
}

std::pair<double, double> CGeoConverter::wgs84ToPseudoMercator (double lat, double lon)
{
  double const a       = 6378137.0;
  double const lambda0 = 0.0;
  double const x0      = 0.0;
  double const y0      = 0.0;
  return geoconverter::TWgs84ToPseudoMercator (lat, lon, a, lambda0, x0, y0);
}

std::pair<double, double> CGeoConverter::pseudoMercatorToWgs84 (double x, double y)
{
  double const a       = 6378137.0;
  double const lambda0 = 0.0;
  double const x0      = 0.0;
  double const y0      = 0.0;
  return geoconverter::TPseudoMercatorToWgs84 (x, y, a, lambda0, x0, y0);
}

void CGeoConverter::etrs89LaeaBounds (std::pair<double, double>& p0, std::pair<double, double>& p1)
{
  p0 = { 1896628.62, 1507846.05 };
  p1 = { 4662111.45, 6829874.45 };
}

void CGeoConverter::wgs84OfEtrs89LaeaBounds (std::pair<double, double>& ll0, std::pair<double, double>& ll1)
{
  ll0 = { 32.88, -16.1  };
  ll1 = { 84.17,  40.18 };
}

void CGeoConverter::lambert93Bounds (std::pair<double, double>& p0, std::pair<double, double>& p1)
{
  p0 = { -378305.81, 6093283.21 };
  p1 = { 1212610.74, 7186901.68 };
}

void CGeoConverter::wgs84OfLambert93Bounds (std::pair<double, double>& ll0, std::pair<double, double>& ll1)
{
  ll0 = { 41.15, -9.86 };
  ll1 = { 51.56, 10.38 };
}

void CGeoConverter::pseudoMercatorBounds (std::pair<double, double>& p0, std::pair<double, double>& p1)
{
  p0 = { -20026376.39, -20048966.10 };
  p1 = {  20026376.39,  20048966.10 };
}

void CGeoConverter::wgs84OfPseudoMercatorBounds (std::pair<double, double>& ll0, std::pair<double, double>& ll1)
{
  ll0 = { -180.0, -85.06 };
  ll1 = {  180.0,  85.06 };
}

double CGeoConverter::wGS84Distance (double lat1, double lon1, double lat2, double lon2)
{
  double const earthMeanRadius = 6371007.2;
  lat1                        *= M_PI / 180.0;
  lon1                        *= M_PI / 180.0;
  lat2                        *= M_PI / 180.0;
  lon2                        *= M_PI / 180.0;
  return earthMeanRadius * std::acos (std::sin (lat1) * std::sin (lat2) + std::cos(lat1) * std::cos(lat2) * std::cos (lon2 - lon1));
}

double CGeoConverter::wGS84Distance (double lat, double lon, bool init)
{
  static double sn0, cs0, lon0;
  double        d = 0.0;
  if (init)
  {
    lat *= M_PI / 180.0;
    sn0  = std::sin (lat);
    cs0  = std::cos (lat);
    lon0 = lon * M_PI / 180.0;
  }
  else
  {
    double const earthMeanRadius = 6371007.2;
    lat                         *= M_PI / 180.0;
    lon                         *= M_PI / 180.0;
    d                            =  earthMeanRadius * std::acos (sn0 * std::sin (lat) + cs0 * std::cos(lat) * std::cos (lon - lon0));
  }

  return d;
}
