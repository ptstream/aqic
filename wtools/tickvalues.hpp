#ifndef TICKVALUES_HPP
#define TICKVALUES_HPP

#include <QWidget>
#include <QDate>

/*! \brief The class is used to show the date of ticks. */
class CTickValues : public QWidget
{
public:
  /*! Default constructor. */
  CTickValues (QWidget* parent = nullptr);

  /*! Sets the dates. */
  void setDates (QList<QDate> const & dates) { m_dates = dates; }

  /*! Sets the index min of dates. */
  void setMin (int min) { m_min = min; }

  /*! Sets the index max of dates. */
  void setMax (int max) { m_max = max; }

  /*! Sets offset of date text. */
  void setOffset (int offset) { m_offset = offset; }

  /*! Sets padding of date text. */
  void setPadding (int padding) { m_padding = padding; }

  /*! Sets format of dates. The format is the format defined by QLocal::toString (date, format). */
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
