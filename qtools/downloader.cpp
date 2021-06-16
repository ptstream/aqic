#include "downloader.hpp"
#include "downloadcache.hpp"
#include "QEventLoop"

QSet<CDownloader*>     CDownloader::m_downloaders;
QNetworkAccessManager* CDownloader::m_networkAccessManager = nullptr;

CDownloader::CDownloader (CDownloadCache* cache, QObject* parent) : QObject (parent), m_cache (cache)
{
  m_downloaders.insert (this);
  m_timer.setInterval (500);
  connect (&m_timer, &QTimer::timeout, this, &CDownloader::progress);
}

CDownloader::~CDownloader ()
{
  m_downloaders.remove (this);
}

void CDownloader::updateNetworkAccessManager (QNetworkAccessManager*& nam)
{
  if (nam == nullptr)
  {
    if (m_networkAccessManager == nullptr)
    {
      m_networkAccessManager = new QNetworkAccessManager ();
    }

    nam = m_networkAccessManager;
  }
}

void CDownloader::error (QNetworkReply::NetworkError err)
{
  m_timer.stop ();
  auto replySender = static_cast<QNetworkReply*>(sender ());
  m_message        = replySender->errorString ();
  qDebug () << QStringLiteral ("CDownloader::error: ") << err << QStringLiteral (" (") << m_message << ')';
  emit downloadError ();
}

QDateTime CDownloader::lastModifiedHeader () const
{
  auto predicat = [] (QNetworkReply::RawHeaderPair const & pair) -> int
  {
    return pair.first.compare ("LAST-MODIFIED", Qt::CaseInsensitive) == 0;
  };

  QDateTime                                           dt;
  QList<QNetworkReply::RawHeaderPair>::const_iterator end = m_rawHeaderPairs.cend ();
  QList<QNetworkReply::RawHeaderPair>::const_iterator it  = std::find_if (m_rawHeaderPairs.cbegin (), end, predicat);
  if (it != end)
  {
    QString lastModified = QString::fromUtf8 ((*it).second);
    dt = QDateTime::fromString (lastModified, Qt::RFC2822Date);
  }

  return dt;
}

void CDownloader::finished ()
{
  m_ellapsedTime += m_timer.interval () - m_timer.remainingTime ();
  m_timer.stop ();
  if (!contains (ReadFromCache))
  {
    if (m_reply != nullptr)
    {
      m_rawHeaderPairs = m_reply->rawHeaderPairs ();
      if (m_cache != nullptr && !contains (Head))
      {
        QDateTime dt = lastModifiedHeader ();
        if (dt.isValid ())
        {
          m_file.setFileTime (dt, QFileDevice::FileBirthTime);
        }

        m_file.close ();
        m_data = CDownloadCache::cacheFileHeader () + m_file.fileName ().toUtf8 ();
        m_cache->update ();
      }

      m_reply->deleteLater ();
      m_reply = nullptr;
    }
  }

  emit downloadFinished (m_ellapsedTime, contains (ReadFromCache));
}

void CDownloader::readData ()
{
  if (m_reply != nullptr)
  {
    if (m_cache != nullptr && !contains (Head))
    {
      m_file.write (m_reply->readAll ());
    }
    else
    {
      m_data += m_reply->readAll ();
    }
  }
}

void CDownloader::progress ()
{
  m_ellapsedTime += m_timer.interval ();
  if (m_ellapsedTime >= m_serverTimeout)
  {
    add (Timeout);
    abort ();
    emit timeout ();
  }
  else
  {
    emit downloadRunning (m_ellapsedTime);
  }
}

void CDownloader::get (QUrl const & url, QNetworkAccessManager* nam)
{
  emit downloadStarted ();
  m_url = url;
  updateNetworkAccessManager (nam);
  remove (ReadFromCache | Aborted | Timeout | Error | Head);
  m_ellapsedTime = 0;
  m_message.clear ();
  m_data.clear ();
  m_rawHeaderPairs.clear ();
  if (m_cache != nullptr && m_cache->hasValid (url))
  {
    m_data = CDownloadCache::cacheFileHeader () + m_cache->fileName (url).toUtf8 ();
    add (ReadFromCache);
    emit downloadFinished (m_ellapsedTime, contains (ReadFromCache));
  }
  else
  {
    m_timer.start ();
    QNetworkRequest nreq (url);
    m_reply = nam->get (nreq);
    if (m_reply != nullptr)
    {
      m_reply->setReadBufferSize (m_bufferSize);
      if (m_cache != nullptr)
      {
        m_file.setFileName (m_cache->fileName (url));
        m_file.open (QIODevice::WriteOnly);
      }

      connect (m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
               this, QOverload<QNetworkReply::NetworkError>::of(&CDownloader::error));
      connect (m_reply, &QNetworkReply::finished,
               this, QOverload<>::of(&CDownloader::finished));
      connect (m_reply, &QIODevice::readyRead, this, &CDownloader::readData);
    }
  }
}

void CDownloader::head (QUrl const & url, QNetworkAccessManager* nam)
{
  emit downloadStarted ();
  m_url = url;
  updateNetworkAccessManager (nam);
  remove (ReadFromCache | Aborted | Timeout | Error);
  add (Head);
  m_ellapsedTime = 0;
  m_message.clear ();
  m_data.clear ();
  m_rawHeaderPairs.clear ();
  m_timer.start ();
  QNetworkRequest nreq (url);
  m_reply = nam->head (nreq);
  if (m_reply != nullptr)
  {
    connect (m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
             this, QOverload<QNetworkReply::NetworkError>::of(&CDownloader::error));
    connect (m_reply, &QNetworkReply::finished,
             this, QOverload<>::of(&CDownloader::finished));
    connect (m_reply, &QNetworkReply::readyRead, this, &CDownloader::readData);
  }
}

void CDownloader::abort ()
{
  m_timer.stop ();
  if (m_reply != nullptr && m_reply->isRunning ())
  {
    add (Aborted);
    m_reply->abort ();
    if (m_cache != nullptr && contains (ReadFromCache))
    {
      m_file.close ();
      m_file.remove ();
    }
  }
}

void CDownloader::abortAll ()
{
  for (CDownloader* downloader : qAsConst (m_downloaders))
  {
    downloader->abort ();
  }
}

QString CDownloader::headerFileName () const
{
  QString fileName;
  for (QNetworkReply::RawHeaderPair const & pair : m_rawHeaderPairs)
  {
    QByteArray temp = pair.first.toLower ();
    if (temp == "content-disposition")
    {
      temp      = pair.second.toLower ();
      int index = temp.indexOf ("filename");
      if (index != -1)
      {
        temp = temp.mid (index + 10);
        temp.replace (' ', nullptr);
        temp.replace ('"', nullptr);
        temp.replace ('=', nullptr);
        fileName = QString::fromUtf8 (temp);
      }

      break;
    }
  }

  return fileName;
}
