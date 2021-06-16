#ifndef IPADDRPOSITION_HPP
#define IPADDRPOSITION_HPP

#include <QString>
#include <QPair>
#include <QGeoCoordinate>

/*! \brief Tool to get the approxiamtive position of the external IP from
 *  the return of http://ip-api.com/json
 */
class CIPAddrPosition
{
public:
  /*! State of the request. */
  enum EState { NotExecute, Done, Fail};

  /*! Default constructor. */
  CIPAddrPosition () = default;

  /*! Constructor from data. */
  CIPAddrPosition (QByteArray const & data) { init (data); }

  /*! Initializes the class member. */
  void init (QByteArray const & data);

  /*! Returns the state of the request. */
  EState state () const { return m_state; }

  /*! Returns the IP location. */
  QGeoCoordinate const & location () const { return m_location; }

  /*! return the town if the IP. */
  QString const & town () const { return m_town; }

private:
  EState         m_state = NotExecute;
  QGeoCoordinate m_location;
  QString        m_town;
};

#endif // IPADDRPOSITION_HPP
