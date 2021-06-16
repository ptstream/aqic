#ifndef AIRQUALITIES_HPP
#define AIRQUALITIES_HPP

#include "airquality.hpp"
#include "../qtools/globals.hpp"
#include "../tools/globalinstance.hpp"
#include <QMap>

class CTownCodes;

using TTAQs = QHash<QString, CAirQuality>;

/*! \brieg This is the container of air quality.*/
class CAirQualities : public QVector<CAirQuality>, public CGlobalInstance<CAirQualities>
{
public:
  /*! Default constructor. */
  CAirQualities () = default;

  /*! Constructor with file name. */
  CAirQualities (QString const & fileName);

  /*! Destructor. */
  ~CAirQualities () override = default;

  /*! Load air qualities from a file. */
  void load (QString const & fileName);

  /*! Returns a set of couple town-color. */
  TCodeColors colors (QDate dt, quint32 rgb = 0) const;

  /*! Returns number of air quality from a town. */
  int airQualityCount (QString const & code) const;

  /*! Returns a list of air quality from a town. */
  QList<CAirQuality> airQualities (QString const & code) const;

  /*! Returns the air qualities from a date. */
  TTAQs airQualities (QDate const & dt) const;

  /*! Returns the air qualitiy from a town and a date. */
  CAirQuality airQuality (QString const & code, QDate const & date) const;

  /*! Returns the list of dates. */
  QList<QDate> dates () const;

  /*! Returns the first dates available. */
  QDate firstAvailableDate () const { return isEmpty () ? QDate () : first ().deadline (); }

  /*! Sets the town codes. */
  void setTownCodes (CTownCodes* townCodes) { m_townCodes = townCodes; }

  /*! Returns the average or max index value. */
  QList<QPair<quint8, quint8>> averageMaxs (CAirQuality::EIndexType index, int dateCount) const;

  /*! Returns the uniqu deadlines. */
  QList<QDate> uniqDeadlines () const;

  /*! Returns the equivalent code from CTownCodes. */
  QString equivalentTownCode (QString const & code) const;

  /*! Returns the global instance. */
  static CAirQualities* gblInst () { return static_cast<CAirQualities*>(CGlobalInstance<CAirQualities>::gblInst ()); }

private:
  CTownCodes* m_townCodes = nullptr;
};

QDataStream& operator << (QDataStream& out, CAirQualities const & aqs);
QDataStream& operator >> (QDataStream& in, CAirQualities& aqs);

#endif // AIRQUALITIES_HPP
