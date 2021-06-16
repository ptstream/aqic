#include "downloadcache.hpp"
#include <QUrl>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QDateTime>

CDownloadCache* CDownloadCache::m_cache = nullptr;

CDownloadCache::CDownloadCache (QString const & path)
{
  if (path.isEmpty ())
  {
    QStringList paths = QStandardPaths::standardLocations (QStandardPaths::CacheLocation);
    QString     path  = paths.first ();
    setPath (path);
    if (!exists (path))
    {
      mkpath (path);
    }
  }
  else
  {
    setPath (path);
  }
}

CDownloadCache::~CDownloadCache ()
{
  if (m_cleaned)
  {
    clear ();
  }
}

QString CDownloadCache::fileName (QUrl const & url) const
{
  QByteArray urlBytes = url.toString ().toUtf8 ();
  QByteArray md5      = QCryptographicHash::hash (urlBytes, QCryptographicHash::Md5).toHex ();
  QString    fileName = QString::fromLatin1 (md5);
  return absoluteFilePath (fileName);
}

void CDownloadCache::removeFile (QUrl const & url)
{
  QString fileName = this->fileName (url);
  QFile::remove (fileName);
}

bool CDownloadCache::hasValid (QUrl const & url)
{
  QString fileName = this->fileName (url);
  QFile   file (fileName);
  bool    valid = file.exists ();
  if (valid)
  {
    valid = file.size () != 0;
    if (valid)
    {
      QDateTime  modificationDateTime = file.fileTime (QFileDevice::FileModificationTime);
      QDateTime  currentDateTime      = QDateTime::currentDateTime ();
      qint64     diff                 = modificationDateTime.msecsTo (currentDateTime);
      valid = diff <= m_validity;
    }
  }

  return valid;
}

static qint64 adjust (QFileInfoList const & fis, qint64 maxCacheSize)
{
  qint64 totalSize = 0;
  if (maxCacheSize > 0)
  { // Get the total size of cache
    for (QFileInfo const & fi : fis)
    {
      qint64 size = fi.size ();
      totalSize  += size;
    }

    // Delete the oldest ones first until the cache size is less than the max cache size
    for (QFileInfoList::const_reverse_iterator it = fis.crbegin (), end = fis.crend ();
         it != end  && totalSize > maxCacheSize; ++it)
    {
      QFileInfo const fi = *it;
      totalSize         -= fi.size ();
      QFile::remove (fi.absoluteFilePath ());
    }
  }

  return totalSize;
}

qint64 CDownloadCache::adjust ()
{
  qint64 totalSize = 0;
  if (m_maxCacheSize > 0)
  {
    QFileInfoList fis = entryInfoList (QDir::Time);
    totalSize = ::adjust (fis, m_maxCacheSize);
  }

  return totalSize;
}

void CDownloadCache::update ()
{
  QDateTime     currentDateTime = QDateTime::currentDateTime ();
  QFileInfoList fis             = entryInfoList (QDir::Time);
  if (fis.size () > 1)
  {
    for (QFileInfo const & fi : qAsConst (fis))
    {
      QDateTime  modificationDateTime = fi.fileTime (QFileDevice::FileModificationTime);
      qint64     diff                 = modificationDateTime.msecsTo (currentDateTime);
      if (diff >= m_validity)
      {
        QFile::remove (fi.absoluteFilePath ());
      }
    }

    ::adjust (fis, m_maxCacheSize);
  }
}

QFileInfoList CDownloadCache::entryInfoList (QDir::SortFlags sort) const
{
  QFileInfoList fis = QDir::entryInfoList (QStringList (), QDir::Files | QDir::NoDotAndDotDot, sort);
  for (int i = fis.size () - 1; i >= 0; --i)
  {
    if (!fis[i].suffix ().isEmpty ())
    {
      fis.removeAt (i);
    }
  }

  return fis;
}

void CDownloadCache::clear ()
{
  QFileInfoList fis = entryInfoList (QDir::Time);
  for (QFileInfo const & fi : qAsConst (fis))
  {
    QString filePath = absoluteFilePath (fi.baseName ());
    QFile::remove (filePath);
  }
}

void CDownloadCache::clearDebugFiles ()
{
  QStringList filters ("*.csv");
  QFileInfoList fis = QDir::entryInfoList (filters, QDir::Files | QDir::NoDotAndDotDot);
  for (QFileInfo const & fi : qAsConst (fis))
  {
    QFile::remove (fi.absoluteFilePath ());
  }
}

CDownloadCache* CDownloadCache::createGlobalInstance (const QString &path)
{
  if (m_cache == nullptr)
  {
    m_cache = new CDownloadCache (path);
  }

  return m_cache;
}

QString CDownloadCache::extractFileName (QByteArray data)
{
  int     len      = CDownloadCache::cacheFileHeader ().size ();
  QString fileName = QString::fromUtf8 (data.remove (0, len));
  return fileName;
}

void CDownloadCache::setValidity (qint64 ms)
{
  if (m_validity != ms)
  {
    m_validity = ms;
    update ();
  }
}

void CDownloadCache::setSize (qint64 size)
{
  if (m_maxCacheSize != size)
  {
    m_maxCacheSize = size;
    update ();
  }
}

void CDownloadCache::setProperties (bool cleaned, qint64 validity, qint64 size)
{
  m_cleaned = cleaned;
  if (validity != 0LL)
  {
    setValidity (validity);
  }

  if (size != 0LL)
  {
    setSize (size);
  }
}

