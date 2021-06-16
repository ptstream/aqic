#ifndef GEOCONVERTER_HPP
#define GEOCONVERTER_HPP

#include <tuple>

class CGeoConverter
{
public:
  /*! \brief Converts WGS84 coordinates to ETRS89-LAEA (EPSG:3035).
   *
   * This function transforms the WGS84 (latitude, longitude) to x, y European Terrestrial Reference System 89
   * Lambert Azimuthal Equal Area (ETRS89-LAEA).
   * The used algorithm is pubished at https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0072.pdf.
   *
   * \param phi: Latitude in degrees.
   * \param lambda: Longitude in degrees
   * \return The coordinates x, y in meters.
   */
  static std::pair<double, double> wgs84ToEtrs89Laea (double phi, double lambda);

  /*! \brief Converts ETRS89-LAEA (EPSG:3035) coordinates to WGS84.
   *
   * This function transforms x, y to the European Terrestrial Reference System 89
   * Lambert Azimuthal Equal Area (ETRS89-LAEA, ) in WGS84 (latitude, longitude).
   * The used algorithm is pubished at https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0073.pdf.
   *
   * \param x: Abscissa in meters
   * \param y: Ordinate in meters.
   * \return The latitude (first) and the longitude (second) in degrees.
   */
  static std::pair<double, double> etrs89LaeaToWgs84 (double x, double y);

  /*! \brief Converts Lambert93 coordinates to WGS84.
   *
   * This function transforms x, y in the Lambert93 (EPSG:2154) coordinate system to WGS84 (latitude, longitude).
   * The used algorithm is pubished at https://codes-sources.commentcamarche.net/source/52427-conversion-systeme-geodesique-mondial-1984-lambert-93.
   *
   * \param x: Abscissa in meters
   * \param y: Ordinate in meters.
   * \return The latitude (first) and the longitude (second) in degrees.
   */
  static std::pair<double, double> lambert93ToWgs84 (double x, double y);

  /*! \brief Converts WGS84 coordinates to Lambert93.
   *
   * This function transforms the WGS84 (latitude, longitude) to x, y to Lambert93 coordinate system (EPSG:2154).
   * The used algorithm is pubished at https://codes-sources.commentcamarche.net/source/52427-conversion-systeme-geodesique-mondial-1984-lambert-93.
   *
   * \param lat: Longitude in degrees
   * \param lon: Latitude in degrees.
   * \return The coordinates x, y in meters.
   */
  static std::pair<double, double> wgs84ToLambert93 (double lat, double lon);

  /*! \brief Converts WGS84 coordinates to pseudo Mercator.
   *
   * This function transforms the WGS84 (latitude, longitude) in x, y to pseudo Mercator coordinate system (EPSG:3857).
   * The conversion is coded from https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0076.pdf.
   *
   * \param lat: Longitude in degrees
   * \param lon: Latitude in degrees.
   * \return The coordinates x, y in meters.
   */
  static std::pair<double, double> wgs84ToPseudoMercator (double lat, double lon);

  /*! \brief Converts pseudo Mercator coordinates to WGS84.
   *
   * This function transforms x, y in the pseudo Mercator (EPSG:3857) coordinate system to WGS84 (latitude, longitude).
   * The conversion is coded from https://geodesie.ign.fr/contenu/fichiers/documentation/algorithmes/alg0077.pdf.
   *
   * \param x: Abscissa in meters
   * \param y: Ordinate in meters.
   * \return The latitude (first) and the longitude (second) in degrees.
   */
  static std::pair<double, double> pseudoMercatorToWgs84 (double x, double y);

  /*! \brief ETRS89-LAEA (EPSG:3035) bounds.
   *
   * \param xy: The coordinates of the bounds in meters.
   */
  static void etrs89LaeaBounds (std::pair<double, double>& p0, std::pair<double, double>& p1);

  /*! \brief WGS84 coordinates of ETRS89-LAEA (EPSG:3035) bounds.
   *
   * \param ll: The latitude and longitude of the bounds in degrees.
  */
  static void wgs84OfEtrs89LaeaBounds (std::pair<double, double>& ll0, std::pair<double, double>& ll1);

  /*! \brief Lambert93 bounds.
   *
   * \param xy: The coordinates of the bounds in meters.
   */
  static void lambert93Bounds (std::pair<double, double>& p0, std::pair<double, double>& p1);

  /*! \brief WGS84 coordinates of Lambert93 bounds.
   *
   * \param ll: The latitude and longitude of the bounds in degrees.
  */
  static void wgs84OfLambert93Bounds (std::pair<double, double>& ll0, std::pair<double, double>& ll1);

  /*! \brief pseudo Mercator bounds.
   *
   * \param xy: The coordinates of the bounds in meters.
   */
  static void pseudoMercatorBounds (std::pair<double, double>& p0, std::pair<double, double>& p1);

  /*! \brief WGS84 coordinates of pseudo Mercator bounds.
   *
   * \param ll: The latitude and longitude of the bounds in degrees.
  */
  static void wgs84OfPseudoMercatorBounds (std::pair<double, double>& ll0, std::pair<double, double>& ll1);

  /*! Returns the distance in meters between 2 WGS84 coordinates. */
  static double wGS84Distance (double lat1, double lon1, double lat2, double lon2);

  /*! Returns the distance in meters between 2 WGS84 coordinates.
   *  The function performes some optimizations on the start position.
   * init = true for the start position.
   * init = false for the successive positions.
   */
  static double wGS84Distance (double lat, double lon, bool init);
};

#endif // GEOCONVERTER_HPP
