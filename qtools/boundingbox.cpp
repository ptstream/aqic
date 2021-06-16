#include "boundingbox.hpp"

bool CBoundingBox::intersects (CBoundingBox const & r) const
{
  float xp = first.x ();
  float w  = width ();
  float l1 = xp;
  float r1 = xp;
  if (w < 0.0F)
  {
    l1 += w;
  }
  else
  {
    r1 += w;
  }

  if (l1 == r1) // null rect
  {
    return false;
  }

  xp       = r.first.x ();
  w        = r.width ();
  float l2 = xp;
  float r2 = xp;
  if (w < 0.0F)
  {
    l2 += w;
  }
  else
  {
    r2 += w;
  }

  if (l2 == r2) // null rect
  {
    return false;
  }

  if (l1 >= r2 || l2 >= r1)
  {
    return false;
  }

  float yp = first.y ();
  float h  = height ();
  float t1 = yp;
  float b1 = yp;
  if (h < 0.0F)
  {
    t1 += h;
  }
  else
  {
    b1 += h;
  }

  if (t1 == b1) // null rect
  {
    return false;
  }

  yp       = r.first.y ();
  h        = height ();
  float t2 = yp;
  float b2 = yp;
  if (h < 0.0F)
  {
    t2 += h;
  }
  else
  {
    b2 += h;
  }

  if (t2 == b2) // null rect
  {
    return false;
  }

  return t1 < b2 && t2 < b1;
}
