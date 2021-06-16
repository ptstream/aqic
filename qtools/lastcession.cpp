#include "lastcession.hpp"
#include "../qtools/applocations.hpp"
#include "../qtools/datastream.hpp"
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QGeoAddress>

SLastCession::SLastCession (bool save)
{
  QString appName     = CAppLocations::location (CAppLocations::AppName);
  QString appLocation = CAppLocations::location (CAppLocations::DataLocation);
  QString iniFile     = QDir (appLocation).absoluteFilePath (appName + QStringLiteral (".ini"));
  if (save || QFile::exists (iniFile))
  {
    m_settings = new QSettings (iniFile, QSettings::IniFormat);
  }
}

SLastCession::~SLastCession ()
{
  delete m_settings;
  m_settings = nullptr;
}

double SLastCession::getReal (QString const & group, QString const & key, double defaultValue) const
{
  double value = defaultValue;
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    value = m_settings->value (key, defaultValue).toDouble ();
    m_settings->endGroup ();
  }

  return value;
}

void SLastCession::setReal (QString const & group, QString const & key, double value)
{
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    m_settings->setValue (key, value);
    m_settings->endGroup ();
  }
}

int SLastCession::getInt (QString const & group, QString const & key, int defaultValue) const
{
  int value = defaultValue;
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    value = m_settings->value (key, defaultValue).toInt ();
    m_settings->endGroup ();
  }

  return value;
}

void SLastCession::setInt (QString const & group, QString const & key, int value)
{
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    m_settings->setValue (key, value);
    m_settings->endGroup ();
  }
}

qint64 SLastCession::getInt64 (QString const & group, QString const & key, qint64 defaultValue) const
{
  qint64 value = defaultValue;
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    value = m_settings->value (key, defaultValue).toLongLong ();
    m_settings->endGroup ();
  }

  return value;
}

void SLastCession::setInt64 (QString const & group, QString const & key, qint64 value)
{
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    m_settings->setValue (key, value);
    m_settings->endGroup ();
  }
}

int SLastCession::getBool (QString const & group, QString const & key, bool defaultValue) const
{
  bool value = defaultValue;
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    value = m_settings->value (key, defaultValue).toBool ();
    m_settings->endGroup ();
  }

  return value;
}

void SLastCession::setBool (QString const & group, QString const & key, bool value)
{
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    m_settings->setValue (key, value);
    m_settings->endGroup ();
  }
}

QByteArray SLastCession::getByteArray (QString const & group, QString const & key, QByteArray const & defaultValue) const
{
  QByteArray value = defaultValue;
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    value = m_settings->value (key, defaultValue).toByteArray ();
    m_settings->endGroup ();
  }

  return value;
}

void SLastCession::setByteArray (QString const & group, QString const & key, QByteArray const & bytes)
{
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    m_settings->setValue (key, bytes);
    m_settings->endGroup ();
  }
}

QByteArray SLastCession::getChars (QString const & group, QString const & key, QByteArray const & defaultValue) const
{
  QByteArray value = defaultValue;
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    value = m_settings->value (key, defaultValue).toByteArray ();
    m_settings->endGroup ();
  }

  return value;
}

void SLastCession::setChars (QString const & group, QString const & key, QByteArray const & chars)
{
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    m_settings->setValue (key, chars.constData ());
    m_settings->endGroup ();
  }
}

QGeoLocation SLastCession::getGeoLocation (QString const & group, QString const & key)
{
  QGeoLocation value;
  if (m_settings != nullptr)
  {
    QByteArray   bytes = getByteArray (group, key);
    CDataStream  stream (&bytes, QIODevice::ReadOnly);
    stream >> value;
  }

  return value;
}

void SLastCession::setGeoLocation (QString const & group, QString const & key, QGeoLocation const & loc)
{
  if (m_settings != nullptr && !loc.address ().isEmpty ())
  {
    QByteArray bytes;
    CDataStream stream (&bytes, QIODevice::WriteOnly);
    stream << loc;
    setByteArray (group, key, bytes);
  }
}

QList<QGeoLocation> SLastCession::getGeoLocations (QString const & group)
{
  QList<QGeoLocation> locs;
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    for (int i = 0, count = m_settings->allKeys ().count (); i < count; ++i)
    {
      QGeoLocation loc = getGeoLocation (group, QString::number (i));
      if (!loc.address ().isEmpty ())
      {
        locs.append (loc);
      }
    }

    m_settings->endGroup ();
  }

  return locs;
}

void SLastCession::setGeoLocations (QString const & group, QList<QGeoLocation> const & locs)
{
  if (m_settings != nullptr)
  {
    m_settings->beginGroup (group);
    QStringList keys = m_settings->allKeys ();
    for (QString const & key : qAsConst (keys))
    {
      m_settings->remove (key);
    }

    for (int i = 0, count = locs.size (); i < count; ++i)
    {
      setGeoLocation ("addresses", QString::number (i), locs[i]);
    }

    m_settings->endGroup ();
  }
}

void SLastCession::remove ()
{
  QDir dir (CAppLocations::location (CAppLocations::DataLocation));
  dir.removeRecursively ();
}
