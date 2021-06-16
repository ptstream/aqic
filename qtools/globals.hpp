#ifndef TOWNGLOBALS_HPP
#define TOWNGLOBALS_HPP

#include "../tools/vector.hpp"
#include "../tools/status.hpp"
#include <QString>
#include <QHash>
#include <QDataStream>

/*! Polygon vertex. */
using TVertex = spart::TVector2f; // x = latitude, y = longitude.

/*! Set of towns color. */
using TCodeColors = QHash<QString, quint32>;

/*! Unicode degree character. */
QChar const charDegree (0xB0);

/*! Write a vertex. */
inline QDataStream& operator << (QDataStream& out, TVertex const & vertex)
{
  out << vertex.x ();
  out << vertex.y ();
  return out;
}

/*! Read a vertex. */
inline QDataStream& operator >> (QDataStream& in, TVertex& vertex)
{
  in >> vertex.x ();
  in >> vertex.y ();
  return in;
}

/*! Returns country code. Example for France the function returns fr. */
QString localeCountryCode ();

/*! Converts a number of miliseconds in string. */
QString elapsedTimeToString (int ms, bool hour = true);

#endif // TOWNGLOBALS_HPP
