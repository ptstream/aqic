#ifndef DOWNLOADSTATISTICS_HPP
#define DOWNLOADSTATISTICS_HPP

#include <QDialog>
#include <QDateTime>

namespace Ui {
  class CDownloadStatistics;
}

class CDownloadStatistics : public QDialog
{
  Q_OBJECT
public:
  struct SStats
  {
    QString   m_title;
    QString   m_url;
    QDateTime m_downloadedDateTime;
    QDateTime m_serverDateTime;
    QDate     m_firstDate;
    QDate     m_lastDate;
    int       m_airQualityCount = 0;
    int       m_townCount       = 0;
    int       m_downloadTime    = 0;
  };

  explicit CDownloadStatistics (QWidget* parent = nullptr);
  ~CDownloadStatistics ();

  void setStats (SStats const & stats);

private:
  Ui::CDownloadStatistics* ui;
};

#endif // DOWNLOADSTATISTICS_HPP
