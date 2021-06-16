#include "imageexporterparams.hpp"
#include "../qtools/lastcession.hpp"

void SImageExporterParams::save (SLastCession& lastCession) const
{
  lastCession.setByteArray ("exporter", "folder",          m_folder.toUtf8 ());
  lastCession.setByteArray ("exporter", "format",          m_format.toUtf8 ());
  lastCession.setInt       ("exporter", "quality",         m_quality);
  lastCession.setByteArray ("exporter", "date_font",       m_dateFont.toUtf8 ());
  lastCession.setInt       ("exporter", "alignment",       m_alignment);
  lastCession.setInt       ("exporter", "date_color",      m_dateColor);
  lastCession.setBool      ("exporter", "viewer",          m_launchViewer);
  lastCession.setBool      ("exporter", "date",            m_date);
}

void SImageExporterParams::restore (SLastCession& lastCession)
{
  m_folder        = QString::fromUtf8        (lastCession.getByteArray ("exporter", "folder", m_folder.toUtf8 ()));
  m_format        = QString::fromUtf8        (lastCession.getByteArray ("exporter", "format", m_format.toUtf8 ()));
  m_quality       = lastCession.getInt       ("exporter", "quality",    m_quality);
  m_dateFont      = lastCession.getByteArray ("exporter", "date_font",  m_dateFont.toUtf8 ());
  m_alignment     = lastCession.getInt       ("exporter", "alignment",  m_alignment);
  m_dateColor     = lastCession.getInt       ("exporter", "date_color", m_dateColor);
  m_launchViewer  = lastCession.getBool      ("exporter", "viewer",     m_launchViewer);
  m_date          = lastCession.getBool      ("exporter", "date",       m_date);
}
