#ifndef LASTCESSION_HPP
#define LASTCESSION_HPP

#include <QGeoLocation>

class QSettings;

/*! \brief This class provides function to read or write the current parameters at close or open.
 *
 *  The parameters are generally stored or read from an ini file.
 */
struct SLastCession
{
public:
  /*! Constructor. */
  SLastCession (bool save = false);

  /*! Destructor. */
  ~SLastCession ();

  /*! Gets a byte array from group end key */
  QByteArray getByteArray (QString const & group, QString const & key, const QByteArray& defaultValue = QByteArray ()) const;

  /*! Sets a byte array to group end key */
  void setByteArray (QString const & group, QString const & key, QByteArray const & bytes);

  /*! Gets a real from group end key */
  double getReal (QString const & group, QString const & key, double defaultValue = 0.0) const;

  /*! Sets a real to group end key */
  void setReal (QString const & group, QString const & key, double value);

  /*! Gets an int from group end key */
  int getInt (QString const & group, QString const & key, int defaultValue = 0) const;

  /*! Sets an int to group end key */
  void setInt (QString const & group, QString const & key, int value);

  /*! Gets a long long from group end key */
  qint64 getInt64 (QString const & group, QString const & key, qint64 defaultValue = 0LL) const;

  /*! Sets a long long to group end key */
  void setInt64 (QString const & group, QString const & key, qint64 value);

  /*! Gets a bool from group end key */
  int getBool (QString const & group, QString const & key, bool defaultValue = false) const;

  /*! Sets a bool to group end key */
  void setBool (QString const & group, QString const & key, bool value);

  /*! Gets characters from group end key */
  QByteArray getChars (QString const & group, QString const & string, QByteArray const & defaultValue = QByteArray ()) const;

  /*! Sets a characters to group end key */
  void setChars (QString const & group, QString const & key, QByteArray const & chars);

  /*! Gets a geo-location from group end key */
  QGeoLocation getGeoLocation (QString const & group, QString const & key);

  /*! Sets a geo-location to group end key */
  void setGeoLocation (QString const & group, QString const & key, QGeoLocation const & loc);

  /*! Gets a list of geo-locations from group end key */
  QList<QGeoLocation> getGeoLocations (QString const & group);

  /*! Sets a list of geo-locations to group end key */
  void setGeoLocations (QString const & group, QList<QGeoLocation> const & locs);

  /*! Removes the folder containing ini file. */
  static void remove ();

protected:
  QSettings* m_settings = nullptr;
};

#endif // LASTCESSION_HPP
