#ifndef DOWNLOADER_HPP
#define DOWNLOADER_HPP

#include "../tools/status.hpp"
#include <QNetworkReply>
#include <QTimer>
#include <QFile>

class CDownloadCache;

/*!
 * \brief The CDownloader class provides functionalities to download data from a server.
 *
 * This object has two operating modes.
 * \li Without cache data. The data returned by the server are stored internally.
 * The fonction data returns the address of the data returned. The get function return an empty data.
 * \li With cache. The data returned from the server are witten in a cache file. The data contains the
 * cache file header returned by CDownloadCahche::cacheFileHeader followed by the file name. In this case,
 * the get function returns also the data.
 *
 * In case of transfert error, the function message returns a none empty string containing the error.
 * A timer is used to visualize the state of transfert. By default the signal running is emitted at each interval.
 * By default the interval is 1/2 second. The setTimeIntervel allows to change the interval.
 *
 * The function get initialize the download.
 * It is possible to abort the transfert by calling abort. The function isAborted returns true in this case.
 * It is possible to abort all downloads by calling abortAll.
 *
 * Several signals are emitted during the transfert.
 */
class CDownloader : public QObject, public CStatus<quint32>
{
  Q_OBJECT
public:
  /*! \brief The type of downloaders actually created. */
  using TDownloaders = QSet<CDownloader*>;

  /*! \brief The downloader status. */
  enum EStatus : quint32 { ReadFromCache = 0x01, //!< The data are read from the cache.
                           Aborted       = 0x02, //!< The abort function was called.
                           Timeout       = 0x04, //!< The download timeout reached.
                           Error         = 0x08, //!< on error accured.
                           Head          = 0x10, //!< The download timeout reached.
                         };

  /*! \brief The constructor.
   * \param cache: A pointer on a CDownloadCache object.
   * \param parent: The parent object.
   */
  CDownloader (CDownloadCache* cache = nullptr, QObject* parent = nullptr);

  /*! \brief The destructor.*/
  virtual ~CDownloader ();

  /*! \brief Changes the timer interval used to see the progression of the download.
   * \param ms: The interval in milliseconds.
   */
  void setProgressInterval (int ms) { m_timer.setInterval (ms); }

  /*! \brief Returns the timer interval in milliseconds. */
  int progressInterval () const { return m_timer.interval (); }

  /*! \brief Changes the download timeout.
   *  In case of multiple get or head, this value must be consider with caution. The duration of download
   *  depends of the capabilities of the system to manage multiple query.
   *  In fact, the download duration is the sum of all durations and not the individual duration.
   * \param ms: The maximum download duration
   */
  void setServerTimeout (int ms) { m_serverTimeout = ms; }

  /*! \brief Returns the server timeout in milliseconds. By default 2 minutes.*/
  int serverTimeout () const { return m_serverTimeout; }

  /*! \brief Returns the error message. If no error occurs returns an empty string. */
  QString const & message () const { return m_message; }

  /*! \brief Initialize the download.
   * \param url: The url.
   * \param nam: A QNetworkAccessManager. It is sufficient to use a single QNetworkAccessManager for the application.
   */
  void get (QUrl const & url, QNetworkAccessManager* nam = nullptr);

  /*! \brief Initialize the download of head.
   * \param url: The url.
   * \param nam: A QNetworkAccessManager. It is sufficient to use a single QNetworkAccessManager for the application.
   */
  void head (QUrl const & url, QNetworkAccessManager* nam = nullptr);

  /*! \brief Returns the actual downloaded data. Must be null or incomplete.
   * The data is complete only when finished is emitted.
   */
  QByteArray const & data () const { return m_data; }

  /*! \brief Aborts the actual download. */
  void abort ();

  /*! \brief Returns true if abort function was called. */
  bool isAborted () const { return contains (Aborted); }

  /*! \brief Returns the header of the last network access. */
  QList<QNetworkReply::RawHeaderPair> const & rawHeaderPairs () const { return m_rawHeaderPairs; }

  /*! \brief Returns the url passed to get or head. */
  QUrl const & url () const { return m_url; }

  /*! \brief Returns the header file name. */
  QString headerFileName () const;
  QDateTime lastModifiedHeader () const;

  /*! \brief Aborts all download for CDownloader with parent parent.
   * \param parent: The downloader parent.
   */
  static void abortAll ();

signals :
  /*! \brief This signal is emitted when the download was initialized. */
  void downloadStarted ();

  /*! \brief This signal is emitted when the download running.
   * \param ms: The dowload time in milliseconds.
   */
  void downloadRunning (int ms);

  /*! \brief This signal is emitted when the download is finished.
   * \param ms: The duration in milliseconds.
   */
  void downloadFinished (int ms, bool fromCache);

  /*! \brief This signal is emitted when the download is aborted. */
  void downloadAborted ();

  /*! \brief This signal is emitted when an error was generated during download.
   * The error message can be retreive by calling message function.
   */
  void downloadError ();

  /*! \brief This signal is emitted when the download is too long.
   * The signal aborted follow this signal.
   */
  void timeout ();

protected slots :
  void error (QNetworkReply::NetworkError err); //!< Called by the QNetworkReply in case of error.
  void finished ();                             //!< Called by the QNetworkReply.
  void readData ();                             //!< Called by the QNetworkReply to get data from the transfert buffer.
  void progress ();                             //!< Called when the download is too long.

protected:
  static void updateNetworkAccessManager(QNetworkAccessManager*& nam);

protected:
  QString                             m_message;                 //!< Error message.
  QNetworkReply*                      m_reply = nullptr;         //!< The network reply after get function.
  QByteArray                          m_data;                    //!< The data returned.
  QTimer                              m_timer;                   //!< The data returned.
  int                                 m_ellapsedTime  = 0;       //!< The download time.
  int                                 m_bufferSize    = 0x10000; //!< Network transfert buffer size (64ko).
  QFile                               m_file;                    //!< The cache file.
  QList<QNetworkReply::RawHeaderPair> m_rawHeaderPairs;          //!< The raw header of the server answer.
  int                                 m_serverTimeout = 180000;  //!< Maximum timout for the server answer (3mn).
  CDownloadCache*                     m_cache = nullptr;         //!< The cache file.
  QUrl                                m_url;                     //!< The url passed to get or head.

  static TDownloaders m_downloaders; //!< List of current dowloaders.
  static QNetworkAccessManager* m_networkAccessManager; //!< Single network access manager.
};

#endif // DOWNLOADER_HPP
