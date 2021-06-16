#include "downloadstatistics.hpp"
#include "ui_downloadstatistics.h"
#include "../qtools/globals.hpp"

CDownloadStatistics::CDownloadStatistics (QWidget* parent) : QDialog (parent), ui(new Ui::CDownloadStatistics)
{
  ui->setupUi (this);
  setWindowFlag (Qt::WindowContextHelpButtonHint, false);
}

CDownloadStatistics::~CDownloadStatistics ()
{
  delete ui;
}

void CDownloadStatistics::setStats (SStats const & stats)
{
  QLocale locale = QLocale::system ();
  QString text;
  QString fmt (R"(<h2 align="center">%1</h2>)");
  text += fmt.arg (stats.m_title);

  fmt = R"(<tr><td width="50%" align="center">%1</td><td width="50%" align="center">%2</td></tr>)";
  text += R"(<table border="1" cellpadding="2" width="100%" align="center">)";
  text += fmt.arg (tr ("Download date"), locale.toString (stats.m_downloadedDateTime, QLocale::LongFormat));
  text += fmt.arg (tr ("Availability date"), locale.toString (stats.m_serverDateTime, QLocale::LongFormat));
  text += fmt.arg (tr ("First date"), locale.toString (stats.m_lastDate, QLocale::LongFormat));
  text += fmt.arg (tr ("Last date"), locale.toString (stats.m_firstDate, QLocale::LongFormat));
  text += fmt.arg (tr ("Data downloaded")).arg (stats.m_airQualityCount);
  text += fmt.arg (tr ("Number of towns")).arg (stats.m_townCount);
  text += fmt.arg (tr ("Download time"), ::elapsedTimeToString (stats.m_downloadTime));
  text += "</table>";
  ui->m_stats->setText (text);
}
