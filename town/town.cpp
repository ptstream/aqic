#include "town.hpp"
#include <QDebug>

static quint32 magicNumber    = 0x00000002; // 0x00000002->CTown
static quint32 version        = 0x00000000;
static float   enlargePercent = 0.02F;

void CTown::setPaths (TPaths const & paths, bool boundingBox)
{
  m_d->m_paths = paths;
  if (boundingBox)
  {
    m_d->m_area = 0.0F;
    float coef  = 0.5F;
    m_d->m_boundingBox.init ();
    for (TPath const & path : qAsConst (m_d->m_paths))
    {
      int vertexCount = path.size ();
      for (int i = 0; i < vertexCount; ++i)
      {
        float x = path[i].x ();
        float y = path[i].y ();
        m_d->m_boundingBox.add (x, y);
        int i1       = i == vertexCount - 1 ? 0 : i + 1;
        m_d->m_area += coef * (path[i1].y () + y) * (path[i1].x () - x);   //S = [ (B+b) x h ] : 2
      }

      coef = -coef;
    }

    m_d->m_boundingBox.enlarge (enlargePercent);
    float const epsilon = 1.0e-10F;
    if (m_d->m_area < epsilon)
    {
      m_d->m_area = epsilon;
    }
  }
}

void CTown::appendPaths (TPaths const & paths)
{
  float coef = 0.5F;
  m_d->m_paths.append (paths);
  for (TPath const & path : qAsConst (paths))
  {
    int vertexCount = path.size ();
    for (int i = 0; i < vertexCount; ++i)
    {
      float x = path[i].x ();
      float y = path[i].y ();
      m_d->m_boundingBox.add (x, y);
      int i1       = i == vertexCount - 1 ? 0 : i + 1;
      m_d->m_area += coef * (path[i1].y () + y) * (path[i1].x () - x);   //S = [ (B+b) x h ] : 2
    }

    coef = -coef;
  }

  m_d->m_boundingBox.enlarge (enlargePercent);
  float const epsilon = 1.0e-10F;
  if (m_d->m_area < epsilon)
  {
    m_d->m_area = epsilon;
  }
}

static bool rayCrossesSegment(float x, float y, TVertex const & a, TVertex const & b)
{
  float px = y, py = x;
  float ax = a.y (), ay = a.x (), bx = b.y (), by = b.x ();
  if (ay > by)
  {
    ax = b.y ();
    ay = b.x ();
    bx = a.y ();
    by = a.x ();
  }

  // alter longitude to cater for 180 degree crossings
  if (px < 0.0F)
  {
    px += 360.0F;
  }
  if (ax < 0.0F)
  {
    ax += 360.0F;
  }

  if (bx < 0.0F)
  {
    bx += 360.0F;
  }

  if (py == ay || py == by)
  {
    py += 0.00000001F;
  }

  if ((py > by || py < ay) || (px > std::max (ax, bx)))
  {
    return false;
  }

  if (px < std::min (ax, bx))
  {
    return true;
  }

  float red  = (ax != bx) ? ((by - ay) / (bx - ax)) : std::numeric_limits<float>::max ();
  float blue = (ax != px) ? ((py - ay) / (px - ax)) : std::numeric_limits<float>::max ();
  return (blue >= red);
}

// https://gis.stackexchange.com/questions/42879/check-if-lat-long-point-is-within-a-set-of-polygons-using-google-maps/46720#46720
bool CTown::contains (float x, float y) const
{
  bool contains = false;
  if (!m_d->m_paths.isEmpty () && boundingBoxContains (x, y))
  {
    for (TPath const & path : m_d->m_paths)
    {
      int crossings = 0;
      for (int i = 0, count = path.size (); i < count; ++i)
      {
        TVertex const & a = path.at (i);
        int             j = i == count - 1 ? 0 : i + 1;
        TVertex const & b = path.at (j);
        if (rayCrossesSegment (x, y, a, b))
        {
          ++crossings;
        }
      }

      // odd number of crossings?
      contains = crossings % 2 == 1;
      if (contains)
      {
        break;
      }
    }
  }

  return contains;
}

QDataStream& operator << (QDataStream& out, CTown::TPath const & path)
{
  out << static_cast<quint32>(path.size ());
  for (TVertex const & vertex : path)
  {
    out <<  vertex;
  }

  return out;
}

QDataStream& operator >> (QDataStream& in, CTown::TPath& path)
{
  quint32 count; in >> count;
  path.reserve (count);
  for (quint32 i = 0; i < count; ++i)
  {
    TVertex vertex; in >> vertex;
    path.append (vertex);
  }

  return in;
}

QDataStream& operator << (QDataStream& out, CTown::TPaths const & paths)
{
  out << static_cast<quint32>(paths.size ());
  for (CTown::TPath const & path : paths)
  {
    out << path;
  }

  return out;
}

QDataStream& operator >> (QDataStream& in, CTown::TPaths& paths)
{
  quint32 count; in >> count;
  paths.reserve (count);
  for (quint32 i = 0; i < count; ++i)
  {
    CTown::TPath path; in >> path;
    paths.append (path);
  }

  return in;
}

QDataStream& operator << (QDataStream& out, CTown const & town)
{
  out << magicNumber;
  out << version;
  out << town.code ();
  out << town.name ();
  out << town.region ();
  out << town.paths ();
  out << town.area ();
  out << town.boundingBox ();
  return out;
}

QDataStream& operator >> (QDataStream& in, CTown& town)
{
  quint32          magicNumber; in >> magicNumber;
  quint32          version;     in >> version;
  QString          code;        in >> code;   town.setCode (code);
  QString          name;        in >> name;   town.setName (name);
  QString          region;      in >> region; town.setRegion (region);
  CTown::TPaths    paths;       in >> paths;  town.setPaths (paths, false);
  float            area;        in >> area;   town.setArea (area);
  CBoundingBox     bb;          in >> bb;     town.setBoundingBox (bb);
  return in;
}
