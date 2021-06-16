#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <algorithm>
#include <cmath>
#include <cassert>

namespace spart
{
#define Invalid_method_for_this_vector_size "Invalid method for this vector size"

  /*! \brief This template allows to create vector of any dimension of any precision.
   *
   *  This object is normally created on the stack and does not generate memory allocation. The member functions
   *  are very optimized.
   */
  template<typename TYPE, unsigned SIZE>
  class CVector
  {
  public :
    /*! Vector dimension. */
    enum  EDim { Dim = SIZE };
    using value_type = TYPE;

    /*! Default constructor.
     *  Coordinates are not initialized.
     */
    inline CVector ();

    /*! Constructor for 2 dimensional vector. */
    inline CVector (TYPE x, TYPE y);

    /*! Constructor for 3 dimension vector. */
    inline CVector (TYPE x, TYPE y, TYPE z);

    /*! Constructor for 4 dimension vector. */
    inline CVector (TYPE x, TYPE y, TYPE z, TYPE w);

    /*! Returns the coordinate at index. */
    inline TYPE operator [] (unsigned index) const;

    /*! Returns the coordinate reference at index. */
    inline TYPE& operator [] (unsigned index);

    /*! Add to this another vector and return the reference of this. */
    inline CVector& operator += (CVector const & other);

    /*! Subtracts to this another vector and return the reference of this. */
    inline CVector& operator -= (CVector const & other);

    /*! Subtracts coef at each coordinate and return the reference of this. */
    inline CVector& operator -= (TYPE coef);

    /*! Adds coef at each coordinate and return the reference of this. */
    inline CVector& operator += (TYPE coef);

    /*! Multiplies each coordinate by coef and return the reference of this. */
    inline CVector& operator *= (TYPE coef);

    /*! Divides each coordinate by coef and return the reference of this. */
    inline CVector& operator /= (TYPE coef);

    /*! Offsetes x by dx and return the reference of this. */
    inline CVector& xOffset (TYPE dx);

    /*! Offsetes y by dy and return the reference of this. */
    inline CVector& yOffset (TYPE dy);

    /*! Returns a vector build by this with x offsetted. */
    inline CVector xOffseted (TYPE dx) const;

    /*! Returns a vector build by this with y offsetted. */
    inline CVector yOffseted (TYPE dy) const;

    /*! Returns x coordinate. */
    inline TYPE x () const;

    /*! Returns y coordinate. */
    inline TYPE y () const;

    /*! Returns z coordinate. */
    inline TYPE z () const;

    /*! Returns w coordinate. */
    inline TYPE w () const;

    /*! Returns the reference on x coordinate. */
    inline TYPE& x ();

    /*! Returns the reference on y coordinate. */
    inline TYPE& y ();

    /*! Returns the reference on z coordinate. */
    inline TYPE& z ();

    /*! Returns the reference on w coordinate. */
    inline TYPE& w ();

    /*! Returns the dimension. */
    inline unsigned dim () const { return Dim; }

private:
    TYPE m_c[Dim] = {}; //!< Coordinates
  };

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>::CVector ()
  {
    static_assert (SIZE > 1, Invalid_method_for_this_vector_size);
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>::CVector (TYPE x, TYPE y)
  {
    static_assert (SIZE > 1, Invalid_method_for_this_vector_size);
    m_c[0] = x;
    m_c[1] = y;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>::CVector (TYPE x, TYPE y, TYPE z)
  {
    static_assert (SIZE > 1, Invalid_method_for_this_vector_size);
    m_c[0] = x;
    m_c[1] = y;
    m_c[2] = z;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>::CVector (TYPE x, TYPE y, TYPE z, TYPE w)
  {
    static_assert (SIZE > 1, Invalid_method_for_this_vector_size);
    m_c[0] = x;
    m_c[1] = y;
    m_c[2] = z;
    m_c[3] = w;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::operator += (CVector<TYPE, SIZE> const & other)
  {
    for (unsigned i = 0; i < SIZE; ++i)
    {
      m_c[i] += other.m_c[i];
    }

    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::operator -= (CVector<TYPE, SIZE> const & other)
  {
    for (unsigned i = 0; i < SIZE; ++i)
    {
      m_c[i] -= other.m_c[i];
    }

    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::xOffset (TYPE dx)
  {
    x () += dx;
    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::yOffset (TYPE dy)
  {
    y () += dy;
    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> CVector<TYPE, SIZE>::xOffseted (TYPE dx) const
  {
    return CVector (x () + dx, y ());
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> CVector<TYPE, SIZE>::yOffseted (TYPE dy) const
  {
    return CVector (x (), y () + dy);
  }

  template<typename TYPE, unsigned SIZE>
  TYPE CVector<TYPE, SIZE>::operator [] (unsigned index) const
  {
    assert (index < SIZE);
    return m_c[index];
  }

  template<typename TYPE, unsigned SIZE>
  TYPE& CVector<TYPE, SIZE>::operator [] (unsigned index)
  {
    assert (index < SIZE);
    return m_c[index];
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::operator -= (TYPE coef)
  {
    for (unsigned i = 0; i < SIZE; ++i)
    {
      m_c[i] -= coef;
    }

    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::operator += (TYPE coef)
  {
    for (unsigned i = 0; i < SIZE; ++i)
    {
      m_c[i] += coef;
    }

    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::operator *= (TYPE coef)
  {
    for (unsigned i = 0; i < SIZE; ++i)
    {
      m_c[i] *= coef;
    }

    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE>& CVector<TYPE, SIZE>::operator /= (TYPE coef)
  {
    for (unsigned i = 0; i < SIZE; ++i)
    {
      m_c[i] /= coef;
    }

    return *this;
  }

  template<typename TYPE, unsigned SIZE>
  TYPE CVector<TYPE, SIZE>::x () const
  {
    return m_c[0];
  }

  template<typename TYPE, unsigned SIZE>
  TYPE CVector<TYPE, SIZE>::y () const
  {
    return m_c[1];
  }

  template<typename TYPE, unsigned SIZE>
  TYPE& CVector<TYPE, SIZE>::x ()
  {
    return m_c[0];
  }

  template<typename TYPE, unsigned SIZE>
  TYPE& CVector<TYPE, SIZE>::y ()
  {
    return m_c[1];
  }

  /*! \brief Return the dot product of 2 vectors. */
  template<typename TYPE, unsigned SIZE>
  TYPE dot (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    TYPE d = p0[0] * p1[0];
    for (unsigned i = 0; i < SIZE - 1; ++i)
    {
      d += p0[1] * p1[1];
    }

    return d;
  }

  /*! \brief Returns the square of the Euclidean distance between 2 points. */
  template<typename TYPE, unsigned SIZE>
  TYPE len2 (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    CVector<TYPE, SIZE> v = p1 - p0;
    return dot (v, v);
  }

  /*! \brief Returns the Euclidean distance between 2 points. */
  template<typename TYPE, unsigned SIZE>
  TYPE len (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    float d = len2 (p0, p1);
    if (d != 0.0F)
    {
      d = std::sqrt (d);
    }

    return d;
  }

  /*! \brief Returns the vector defined by 2 points. */
  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> operator - (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    CVector<TYPE, SIZE> tmp = p0;
    tmp                    -= p1;
    return tmp;
  }

  /*! \brief Returns sum of 2 vectors. */
  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> operator + (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    CVector<TYPE, SIZE> tmp = p0;
    tmp                   += p1;
    return tmp;
  }

  /*! Returns the vector whose coordinates are multiplied by coef. */
  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> operator * (CVector<TYPE, SIZE> const & p, TYPE coef)
  {
    CVector<TYPE, SIZE> tmp = p;
    tmp                    *= coef;
    return tmp;
  }

  /*! Returns the vector whose coordinates are divided by coef. */
  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> operator / (CVector<TYPE, SIZE> const & p, TYPE coef)
  {
    CVector<TYPE, SIZE> tmp = p;
    tmp                    /= coef;
    return tmp;
  }

  /*! Returns the smallest of coordinates. */
  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> c_min (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    CVector<TYPE, SIZE> vcmin;
    for (unsigned i = 0; i < SIZE; ++i)
    {
      vcmin[i] = std::min (p0[i], p1[i]);
    }

    return vcmin;
  }

  /*! Returns the greatest of coordinates. */
  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> c_max (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    CVector<TYPE, SIZE> vcmax;
    for (unsigned i = 0; i < SIZE; ++i)
    {
      vcmax[i] = std::max (p0[i], p1[i]);
    }

    return vcmax;
  }

  /*! Returns the mid points of 2 points. */
  template<typename TYPE, unsigned SIZE>
  CVector<TYPE, SIZE> mid (CVector<TYPE, SIZE> const & p0, CVector<TYPE, SIZE> const & p1)
  {
    return (p0 + p1) * static_cast<TYPE>(0.5);
  }

  using TVector2i  = CVector<int, 2>; //<! Fast way for integer coordinates of 2 dimensions vector
  using TVector2l  = CVector<long, 2>; //<! Fast way for long coordinates of 2 dimensions vector
  using TVector2ll = CVector<long long, 2>; //<! Fast way for long long coordinates of 2 dimensions vector
  using TVector2f  = CVector<float, 2>; //<! Fast way for float coordinates of 2 dimensions vector
  using TVector2d  = CVector<double, 2>; //<! Fast way for double coordinates of 2 dimensions vector

} // Namespace

#endif // VERTEX_HPP
