#ifndef DOWNLOADERDATA_HPP
#define DOWNLOADERDATA_HPP

#include <qglobal.h>

struct SLastCession;

struct SDownloaderData
{
  void save (SLastCession& lastCession) const;
  void restore (SLastCession& lastCession);

  bool   m_cacheClean    = false;
  int    m_serverTimeout = 5 * 60 * 1000;          //!< 5mn. 0 = Default cache server timeout.
  int    m_newDataDelay  = 10 * 60 * 1000;          //!< Check for new data every 10mn.
  qint64 m_cacheValidity = 12LL * 3600LL * 1000LL;  //!< 12 hours. 0 = Default cache validity.
  qint64 m_cacheSize     = 512LL * 1024LL * 1024LL; //!< 512 Mo. 0 = Default cache size.
};

#endif // DOWNLOADERDATA_HPP
