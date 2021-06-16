#ifndef TOWN_HPP
#define TOWN_HPP

#include "../town/airquality.hpp"
#include "../qtools/boundingbox.hpp"
#include <QPair>
#include <QString>
#include <QVector>
#include <QSharedDataPointer>

struct STownData;

class CTown
{
public:
  using TPath  = QVector<TVertex>;
  using TPaths = QVector<TPath>;

  CTown () : m_d (new STownData) {}
  CTown (CTown const & rhs) : m_d (rhs.m_d) {}
  ~CTown () = default;

  inline CTown& operator = (CTown const & rhs);
  inline bool operator == (CTown const & rhs) const;

  QString const & code () const { return m_d->m_code; }
  QString const & name () const { return m_d->m_name; }
  QString const & region () const { return m_d->m_region; }
  TPaths& paths () { return m_d->m_paths; }
  TPaths const & paths () const { return m_d->m_paths; }
  CBoundingBox boundingBox () const { return m_d->m_boundingBox; };
  CAirQuality const & airQuality () const { return m_d->m_airQuality; }
  CAirQuality& airQuality () { return m_d->m_airQuality; }
  float area () const { return m_d->m_area; }

  void setCode (QString const & code) { m_d->m_code = code; }
  void setName (QString const & name) { m_d->m_name = name; }
  void setRegion (QString const & region) { m_d->m_region = region; }
  void setPaths (TPaths const & paths, bool boundingBox);
  void setBoundingBox (CBoundingBox const & bb) { m_d->m_boundingBox = bb; };
  void setAirQuality (CAirQuality const & airQuality) { m_d->m_airQuality = airQuality; }
  void setArea (float area) { m_d->m_area = area; }

  bool contains (float x, float y) const;
  bool boundingBoxContains (float x, float y) const { return m_d->m_boundingBox.contains (x, y); }
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
