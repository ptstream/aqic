#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include "globals.hpp"
#include <array>
#include <limits>
#include <algorithm>

using TBoundingBoxData = QPair<TVertex, TVertex>;

/*! \brief The CBoundingBox class provides a simple rectangular box.
 *
 *  This bounding box is very similar than CRetF but they use float coordinates.
 */

class CBoundingBox : public TBoundingBoxData
{
public:
  /*! Default constructor. */
  CBoundingBox () { init (); }

  /*! Returns bottom left corner. */
  inline TVertex const & bl () const;

  /*! Returns bottom right corner. */
  inline TVertex br () const;

  /*! Returns top right corner. */
  inline TVertex const & tr () const;

  /*! Returns top left corner. */
  inline TVertex tl () const;

  /*! Returns the latitude min. */
  float latMin () const { return first.x (); }

  /*! Returns the longitude min. */
  float lonMin () const { return first.y (); }

  /*! Returns the latitude max. */
  float latMax () const { return second.x (); }

  /*! Returns the longitude max. */
  float lonMax () const { return second.y (); }

  /*! Returns the width. */
  float width () const { return second.x () - first.x (); }

  /*! Returns the height. */
  float height () const { return second.y () - first.y (); }

  /*! Returns true if the box contains the point. */
  inline bool contains (float lat, float lon) const;

  /*! Returns true if the box intersetcs other. */
  bool intersects (CBoundingBox const & other) const;

  /*! Returns the center of the box. */
  inline TVertex center () const;

  /*! Returns the radius of the box. It is the circle centered at center and tangent at the biggest dimension. */
  inline float radius () const;

  /*! Returns true is width > 0 and height > 0. */
  inline bool isValid () const;

  /*! Returns the diagonal. */
  inline float diag () const;

  /*! Returns the area. */
  inline float area () const;

  /*! Initializes the box. The box is invalid. */
  inline void init ();

  /*! Expands the box to contain the point. */
  inline void add (float lat, float lon);

  /*! Expands the box to contain the point. */
  inline void add (TVertex vertex);

  /*! Expands the box to contain another box. */
  inline void add (CBoundingBox const & other);

  /*! Enlarges the box of x precent. */
  inline void enlarge (float percent);
};

bool CBoundingBox::isValid () const
{
  return first.x () < second.x () && first.y () < second.y ();
}

float CBoundingBox::diag () const
{
  return len (first, second);
}

void CBoundingBox::init ()
{
  first.x ()  = std::numeric_limits<float>::max ();
  first.y ()  = first.x ();
  second.x () = -first.x ();
  second.y () = second.x ();
}

void CBoundingBox::add (float lat, float lon)
{
  first.x ()  = std::min (lat, first.x ());
  first.y ()  = std::min (lon, first.y ());
  second.x () = std::max (lat, second.x ());
  second.y () = std::max (lon, second.y ());
}

void CBoundingBox::add (TVertex vertex)
{
  add (vertex.x (), vertex.y ());
}

inline void CBoundingBox::add (CBoundingBox const & other)
{
  TVertex const & bl = other.bl ();
  TVertex const & tr = other.tr ();
  add (bl);
  add (tr);
}

TVertex const & CBoundingBox::bl () const
{
  return first;
}

TVertex CBoundingBox::br () const
{
  return first.xOffseted (second.x () - first.x ());
}

TVertex const & CBoundingBox::tr () const
{
  return second;
}

TVertex CBoundingBox::tl () const
{
  return second.xOffseted (first.x () - second.x ());
}

bool CBoundingBox::contains (float lat, float lon) const
{
  return lat > first.x () && lat < second.x () && lon > first.y () && lon < second.y ();
}

void CBoundingBox::enlarge (float percent)
{
  if (percent != 0.0F)
  {
    float dx     = this->width ()  * percent;
    float dy     = this->height () * percent;
    first.x ()  -= dx;
    second.x () += dx;
    first.y ()  -= dy;
    second.y () += dy;
  }
}

float CBoundingBox::area () const
{
  return std::fabs (width ()) * std::fabs (height ());
}

TVertex CBoundingBox::center () const
{
  return mid (first, second);
}

float CBoundingBox::radius () const
{
  float w = std::fabs (width ());
  float h = std::fabs (height ());
  return 0.5F * (w < h ? h : w);
}

inline QDataStream& operator << (QDataStream& out, CBoundingBox const & bb)
{
  out << bb.first;
  out << bb.second;
  return out;
}

inline QDataStream& operator >> (QDataStream& in, CBoundingBox& bb)
{
  in >> bb.first;
  in >> bb.second;
  return in;
}

#endif // BOUNDINGBOX_HPP
