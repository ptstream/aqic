#ifndef IMAGEEXPORTERPARAMS_HPP
#define IMAGEEXPORTERPARAMS_HPP

#include <QString>

struct SLastCession;

struct SImageExporterParams
{
  void save (SLastCession& lastCession) const;
  void restore (SLastCession& lastCession);

  QString m_folder;
  QString m_dateFont;
  QString m_format        = "JPG";
  int     m_quality       = 50;
  int     m_alignment     = 1;
  int     m_dateColor     = 0x0000FF;
  bool    m_launchViewer  = false;
  bool    m_date          = false;
};

#endif // IMAGEEXPORTERPARAMS_HPP
