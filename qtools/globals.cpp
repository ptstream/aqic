#include "globals.hpp"

#include <QLocale>
#include <QTime>

//CStatus<quint32> m_globalStatus;

QString localeCountryCode ()
{
  QString systemName = QLocale::system ().name ();
  return systemName.section ('_', 0, 0);
}

QString elapsedTimeToString (int ms, bool hour)
{
  QTime   time   = QTime (0, 0).addMSecs (ms);
  QString format = !hour && time.hour () == 0 ? "mm:ss" : "hh:mm:ss";
  return time.toString (format);
}
