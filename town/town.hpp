#ifndef TOWN_HPP
#define TOWN_HPP

#include "../town/airquality.hpp"
#include "../qtools/boundingbox.hpp"
#include <QPair>
#include <QString>
#include <QVector>
#include <QSharedDataPointer>

struct STownData;

/*! \brief The is the town container. */
class CTown
{
public:
  /*! The town polygon. */
  using TPath  = QVector<TVertex>;

  /*! The town list of polygons. */
  using TPaths = QVector<TPath>;

  /*! Default constructor. */
  CTown () : m_d (new STownData) {}

  /*! Copy constructor. */
  CTown (CTown const & rhs) : m_d (rhs.m_d) {}

  /*! Default destructor. */
  ~CTown () = default;

  /*! Assignation operator. */
  inline CTown& operator = (CTown const & rhs);

  /*! Equality operator. */
  inline bool operator == (CTown const & rhs) const;

  /*! Returns the town code (insee). */
  QString const & code () const { return m_d->m_code; }

  /*! Returns the town name. */
  QString const & name () const { return m_d->m_name; }

  /*! Returns the town region code. */
  QString const & region () const { return m_d->m_region; }

  /*! Returns the polygons. */
  TPaths& paths () { return m_d->m_paths; }

  /*! Returns the polygons. */
  TPaths const & paths () const { return m_d->m_paths; }

  /*! Returns the bounding box of the polygons. */
  CBoundingBox boundingBox () const { return m_d->m_boundingBox; };

  /*! Returns the actual air quality. */
  CAirQuality const & airQuality () const { return m_d->m_airQuality; }

  /*! Returns the actual air quality. */
  CAirQuality& airQuality () { return m_d->m_airQuality; }

  /*! Returns the area in m2. */
  float area () const { return m_d->m_area; }

  /*! Sets the town code (insee). */
  void setCode (QString const & code) { m_d->m_code = code; }

  /*! Sets the town name. */
  void setName (QString const & name) { m_d->m_name = name; }

  /*! Sets the town region code. */
  void setRegion (QString const & region) { m_d->m_region = region; }

  /*! Sets the polygons. */
  void setPaths (TPaths const & paths, bool boundingBox);

  /*! Sets the bounding box of the polygons. */
  void setBoundingBox (CBoundingBox const & bb) { m_d->m_boundingBox = bb; };

  /*! Sets the actual air quality. */
  void setAirQuality (CAirQuality const & airQuality) { m_d->m_airQuality = airQuality; }

  /*! Sets the area in m2. */
  void setArea (float area) { m_d->m_area = area; }

  /*! Returns true if the polygons contain the point (latitude, longitude). */
  bool contains (float x, float y) const;

  /*! Returns true if the bounding box contains the point (latitude, longitude). */
  bool boundingBoxContains (float x, float y) const { return m_d->m_boundingBox.contains (x, y); }

  /*! Adds paths. */
  void appendPaths (TPaths const & paths);

private:
  struct STownData : public QSharedData
  {
    QString      m_code;
    QString      m_name;
    QString      m_region;
    float        m_area = 0.0F;
    TPaths       m_paths;
    CBoundingBox m_boundingBox;
    CAirQuality  m_airQuality;
  };

  QSharedDataPointer<STownData> m_d;
};

CTown& CTown::operator = (CTown const & rhs)
{
  if (this != &rhs)
  {
    m_d.operator = (rhs.m_d);
  }

  return *this;
}

bool CTown::operator == (CTown const & rhs) const
{
  return code () == rhs.code ();
}

QDataStream& operator << (QDataStream& out, CTown::TPath const & path);
QDataStream& operator >> (QDataStream& in, CTown::TPath& path);

QDataStream& operator << (QDataStream& out, CTown::TPaths const & paths);
QDataStream& operator >> (QDataStream& in, CTown::TPaths& paths);

QDataStream& operator << (QDataStream& out, CTown const & town);
QDataStream& operator >> (QDataStream& in, CTown& town);
#endif // TOWN_HPP
