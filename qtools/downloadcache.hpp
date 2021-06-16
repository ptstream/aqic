#ifndef DOWNLOADCACHE_HPP
#define DOWNLOADCACHE_HPP

#include <QDir>

class QUrl;

/*! \brief The CDownloadCache class provides a cache files.
 *
 * One cache object is created for the application. The function createGlobalInstance must be used once.
 * Generally at the end of the application, destroyGlobalInstance is use to free the global instance.
 * setSize allows to set a new size of the cache. By default the limit is 256MB.
 * setCleaned allows to remove all files during the destructor.
 * setValidity allows to change the validity delay of the file. By default the delay is 1 hour.
 *
 * filename function returns file name from an url. For this, the md5 of the url is converted in hexadecimal.
 * hasValid function allows to check if the file in the cache is valid. The return value is false if the file
 * is out of date (modification date is upper the validity or if its size is null or if the file corresponding
 * at the url don't exist.
 *
 * It is the resposability of the user to manage the content of the cache.
 * remove function is used to remove a file in the directory.
 * adjust function is used to resize the cache from the cache size.
 * update function is used to remove the out of date files and resize the cache from the cache size.
 * clear function is used to remove all files.
 */
class CDownloadCache : public QDir
{
public:
  /*! \brief Contructor.
   * \param path: The directory name. If the path is empty, QStandardPaths::CacheLocation is used
   * to define the cahche directory. The path must be exists.
   */
  CDownloadCache (QString const & path = QString ());

  /*! \brief Destructor.
   * If cleaned (true) was called all files are removed.
   */
  ~CDownloadCache ();

  /*! \brief Enables the cache to remove all files a the destruction.
   * \param cleaned: True to remove all files. The default is false.
   */
  void setCleaned (bool cleaned) { m_cleaned = cleaned; }

  /*! \brief Sets the file validity time in milliseconds.
   * \param ms: The number of milliseconds. By default 1hour.
   */
  void setValidity (qint64 ms);

  /*! \brief Returns the validity time in milliseconds. */
  qint64 validity () const { return m_validity; }

  /*! \brief Sets the maximum cache size in bytes. */
  void setSize (qint64 size);

  /*! \brief Returns the cache size in bytes. */
  qint64 size () const { return m_maxCacheSize; }

  void setProperties (bool cleaned, qint64 validity, qint64 size);

  /*! \brief Returns the cache files directory name. */
  QString const & dir () const;

  /*! \brief Returns the file name from the url.
   *
   * The file name is built from the md5 of the url in base 16.
   */
  QString fileName (QUrl const & url) const;

  /*! \brief Removes the file name correspoding at the url.
   *
   * The file name is built from the md5 of the url in base 16.
   */
  void removeFile (QUrl const & url);

  /*! \brief Returns true if the file in the cache correspoding at the url is out of date or
   * if the size is null.
   */
  bool hasValid (QUrl const & url);

  /*! \brief Ajusts the size of the cache.
   * \return the actual cache size.
   */
  qint64 adjust ();

  /*! \brief UPdates the cache.
   *
   * The out of date file are removed and the size is adjusted.
   */
  void update ();

  /*! \brief Clears all files in the cache directory. */
  void clear ();

  /*! \brief Clears all csv files in the cache directory. */
  void clearDebugFiles ();

  /*! \brief Returns list of info of file without suffix to remove this files. */
  QFileInfoList entryInfoList (QDir::SortFlags sort = NoSort) const;

  /*! \brief Returns the standard prefix. */
  static inline QByteArray const & cacheFileHeader ();

  /*! \brief Returns the global instance of the cache. If this fuction is called before createGlobalInstance
   * it returns nullptr.
   */
  static CDownloadCache* globalInstance () { return m_cache; }

  /*! \brief Creates the global instance of the cache.param
   * \param path: The directory name. If path is empty, the QStandardPaths::CacheLocation is used.
   * If the path is not empty, it must be exists.
   * \return the global cache pointer.
   */
  static CDownloadCache* createGlobalInstance (QString const & path = QString ());

  /*! Returns the file name from data begging by header. */
  static QString extractFileName (QByteArray data);

protected:
  qint64 m_validity     = 2 * 3600 * 1000;     //!< File validity 2 hours.
  qint64 m_maxCacheSize = 536870912LL; //!< 512 Mo
  bool   m_cleaned      = false;       //!< Clean the directory at the destruction.

  static CDownloadCache* m_cache; //!< The global cache pointer.
};

inline QByteArray const & CDownloadCache::cacheFileHeader ()
{
  static QByteArray s_cacheFileHeader ("$$cache-file-0.0.0$$");
  return s_cacheFileHeader;
}

#endif // DOWNLOADCACHE_HPP
