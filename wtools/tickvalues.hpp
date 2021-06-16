#ifndef TICKVALUES_HPP
#define TICKVALUES_HPP

#include <QWidget>
#include <QDate>

class CTickValues : public QWidget
{
public:
  CTickValues (QWidget* parent = nullptr);

  void setDates (QList<QDate> const & dates) { m_dates = dates; }
  void setMin (int min) { m_min = min; }
  void setMax (int max) { m_max = max; }
  void setOffset (int offset) { m_offset = offset; }
  void setPadding (int padding) { m_padding = padding; }
  void setFormatDate (QString format) { m_formatDate = format; }

protected:
  void paintEvent (QPaintEvent*) override;

protected:
  QList<QDate> m_dates;
  QString      m_formatDate;
  int          m_min     = 0;
  int          m_max     = 0;
  int          m_offset  = 0;
  int          m_padding = 2;
};

#endif // TICKVALUES_HPP
