#include "displaydata.hpp"
#include "../qtools/lastcession.hpp"

void SDisplayData::save (SLastCession& lastCession) const
{
  lastCession.setInt   ("ui", "opacity", m_opacity);
  lastCession.setBool  ("ui", "circle",  m_circle);
}

void SDisplayData::restore (SLastCession& lastCession)
{
  m_opacity = lastCession.getInt   ("ui", "opacity",  m_opacity);
  m_circle  = lastCession.getBool  ("ui", "circle",   m_circle);
}
