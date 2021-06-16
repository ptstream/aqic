#include "downloaderdata.hpp"
#include "../qtools/lastcession.hpp"

void SDownloaderData::save (SLastCession& lastCession) const
{
  lastCession.setInt   ("network", "timeout",        m_serverTimeout);
  lastCession.setInt   ("network", "new_data",       m_newDataDelay);
  lastCession.setBool  ("network", "clean_cache",    m_cacheClean);
  lastCession.setInt64 ("network", "cache_size",     m_cacheSize);
  lastCession.setInt64 ("network", "cache_validity", m_cacheValidity);
}

void SDownloaderData::restore (SLastCession& lastCession)
{
  m_serverTimeout = lastCession.getInt   ("network", "timeout",        m_serverTimeout); // 5mn
  m_newDataDelay  = lastCession.getInt   ("network", "new_data",       m_newDataDelay);  // 10 minutes
  m_cacheClean    = lastCession.getBool  ("network", "clean_cache",    m_cacheClean);
  m_cacheSize     = lastCession.getInt64 ("network", "cache_size",     m_cacheSize);     // 512 Mo
  m_cacheValidity = lastCession.getInt64 ("network", "cache_validity", m_cacheValidity); // 12 hours
}
