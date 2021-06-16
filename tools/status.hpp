#ifndef STATUS_HPP
#define STATUS_HPP

/*! \brief Holds information to simulate a set of bits.
 *
 * Each value, 0 or 1 indicates if the bit exists.
 */

template<typename T>
class CStatus
{
public:
  /*! Default constructor. */
  CStatus () : m_status (static_cast<T>(0)) {}

  /*! Copy constructor. */
  CStatus (CStatus<T> const & other) { m_status = other; }

  /*! Constructs with a st value. */
  CStatus (T bits) { m_status = bits; }

  /*! Equal operator. */
  T& operator = (CStatus<T> const & other) { m_status = other.m_status; return m_status; }

  /*! Equality operator. */
  bool operator == (CStatus<T> const & other) const {return m_status == other.m_status; }

  /*! Inequality operator. */
  bool operator != (CStatus<T> const & other) const {return m_status != other.m_status; }

  /*! Equal operator with value. */
  T& operator = (T other) { m_status = other; return m_status; }

  /*! Equality operator with value. */
  bool operator == (T other) const { return m_status == other; }

  /*! Inequality operator with value. */
  bool operator != (T other) const { return m_status != other; }

  /*! Adds a set of bits. */
  T& add (CStatus<T> const &  other) { m_status |= other.m_status; return m_status; }

  /*! Removes a set of bits. */
  T& remove (CStatus<T> const &  other) { m_status &= ~other.m_status; return m_status; }

  /*! Sets a set of bits. */
  T& set (CStatus<T> const &  other) { m_status = other.m_status; return m_status; }

  /*! Sets or removes a set of bits. */
  T& set (CStatus<T> const &  other, bool set) { return set ? add (other) : rem (other); return m_status;  }

  /*! Adds a set of bits. */
  T& add (T bits) { m_status |= bits; return m_status; }

  /*! Removes a set of bits. */
  T& remove (T bits) { m_status &= ~bits; return m_status; }

  /*! Sets a set of bits. */
  T& set (T bits) { m_status = bits; return m_status; }

  /*! Sets or removes a set of bits. */
  T& set (T bits, bool set) { return set ? add (bits) : remove (bits); }

  /*! Return the value corresponding of set of bits. */
  T bitValue () const { return m_status; }

  /*! Has at least one bit of other. */
  bool contains (CStatus<T> const &  other) const { return (m_status & other.m_status) != static_cast<T>(0); }

  /*! Has at least one bit of bits. */
  bool contains (T bits) const { return (m_status & bits) != static_cast<T>(0); }

protected:
  T m_status; //!< The bits of status.
};

/*! \brief Related non-members equality operator. */
template<typename T>
inline bool operator == (CStatus<T> const & s1, CStatus<T> const & s2)
{
  return s1.status () == s2.status ();
}

/*! \brief Related non-members inequality operator. */
template<typename T>
inline bool operator != (CStatus<T> const & s1, CStatus<T> const & s2)
{
  return s1.status () != s2.status ();
}

/*! \brief Related non-members equality operator. */
template<typename T>
inline bool operator == (T bits, CStatus<T> const & s2)
{
  return bits == s2.status ();
}

/*! \brief Related non-members inequality operator. */
template<typename T>
inline bool operator != (T bits, CStatus<T> const & s2)
{
  return bits != s2.status ();
}

/*! \brief Related non-members equality operator. */
template<typename T>
inline bool operator == (CStatus<T> const & s1, T bits)
{
  return s1.status () == bits;
}

/*! \brief Related non-members inequality operator. */
template<typename T>
inline bool operator != (CStatus<T> const & s1, T bits)
{
  return s1.status () != bits;
}

#endif // STATUS_HPP
