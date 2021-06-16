#ifndef KDTREE_HPP
#define KDTREE_HPP

#include "geoconverter.hpp"
#include <vector>

namespace spart
{
  /*! Returns the value squared. */
  template <typename TYPE>
  inline TYPE sqr (TYPE x)
  {
    return x * x;
  }

  /*! Returns the largest power of 2.
   * The function returns 0xffffffff in case of error (x < 1).
   */
  template <typename TYPE>
  inline unsigned largest_power_of_2 (TYPE x)
  {
    if (x < 1)
    {
      return 0;
    }

    int i = 0;
    while (x != 1)
    {
      x >>= 1;
      i++;
    }

    return i;
  }

  /*! \class kdtree
     *  \brief A classic K-dimensional tree.
     * \tparam KEY : It is generally a point object.
     * \tparam VAL : Data associates at the point.
     *
     * A KDtree (K-dimensional tree) is a good data structure for storing points in space and for
     * nearest neighbour queries.\n
     * Traditionaly, kdtrees are split along x, then y, then z then again x, and so on, in a cyclic fashion.
     * Here, k represents the number of dimensions subdivided, which does not have to match the dimensionality
     * of the space used.
     *
     * This object is derived from kdtree of J. Andreas Berentzen.
     *
     * To minimize memory used, the insert function creates kdnode, and the build function construts
     * the kdtree from the list of kdnodes. closest_point and in_sphere functions are use for nearest neighbour queries.
     *
     * The typical code to use kdtree is :
     * \code
     * #include "kdtree.hpp"
     *
     * typedef float comp; // Type of point component
     * typedef usigned data; // Type of data associates at point
     * class point; // Define point object (see spart::SPoint).
     * typedef sppart::kdtree<point, unsigned> kdtree; // Define kdtree object
     *
     * kdtree tree                       // Create an empty kdtree
     * for (unsigned i = 0; i < ......
     * {
     *   tree.insert (p[i], data[i]);   // Insert points with datas
     * }
     *
     * tree.build (); // Build the kdtree. This is mandatory.
     *
     * point    p    = .....;
     * comp     dist = 0.0001f;
     * point    p_find;
     * unsigned i_find;
     * bool = tree.closest_point (p_find, i_find, p, dist); // Search the closest point.
     * \endcode
     *
     * \remarks In KEY class, several member functions and constants must be defined.
     * \li \c dim : Constante which specify the dimension of point (generally 2 or 3).\n
     * \code
     * class KEY
     * {
     *    public :
     *     enum { dim =  };
     * };
     * \endcode
     * \li \c operator - : Defines the subtraction of two points (subtract each coordinate).
     * \code
     * class KEY
     * {
     * };
     * KEY operator - (const KEY& k0, const KEY& k1);
     * \endcode
     * \li \c operator -= : Defines the inplace subtraction of two points (subtract each coordinate).
     * \code
     * class KEY
     * {
     *   public :
     *     KEY& operator -= (const KEY& other);
     * };
     * \endcode
     * \li \c dot : Returns the dot product of two vectors
     * \code
     * class KEY
     * {
     * };
     * KEY::value_type dot (const KEY& k0, const KEY& k1);
     * \endcode
     * \li \c c_min : Returns the vector containing for each coordinate the smallest value from two vectors.
     * \code
     * class KEY
     * {
     * };
     * KEY c_min (const KEY& k0, const KEY& k1);
     * \endcode
     * \li \c c_max : Returns the vector containing for each coordinate the largest value from two vectors.
     * \code
     * class KEY
     * {
     * };
     * KEY c_max (const KEY& k0, const KEY& k1);
     * \endcode
     */
  template<class KEY, typename VAL>
  class kdtree
  {
  public:
    typedef KEY                      key_type;     /*!< Type of KEY */
    typedef typename KEY::value_type scalar_type;  /*!< Type of point components (float, double) */

    /*! Constructs an empty kdtree. */
    inline kdtree ();

    /*! Constructs an kdtree which can receive cpoint number of points (to minimize memory fragmentation). */
    inline kdtree (unsigned cpoint);

    /*! Inserts points in kdtree and create internal kdnode objects. */
    inline void insert (const KEY& key, const VAL& val);

    /*! Build the kdtree after inserting points. */
    inline void build ();

    /*! Returns key of kndone i. */
    inline const KEY& key (unsigned i) const;

    /*! Returns data of kndone i. */
    inline const VAL& val (unsigned i) const;

    /*! Returns key and data of kndone i. */
    inline void get (KEY& key, VAL& val, unsigned i) const;

    /*! Search the closest point of a point.
       * \param k : Key of closest point if found.
       * \param v : Data of closest point if found.
       * \param p : Position of searching.
       * \param dist : Distance of searching.
       * \return true if found.
       */
    inline bool closest_point (KEY& k, VAL& v, const KEY& p, scalar_type dist) const;

    /*! Search the closest point of a point. Special case for double type distance.
       * \param k : Key of closest point if found.
       * \param v : Data of closest point if found.
       * \param p : Position of searching.
       * \param dist : Distance of searching.
       * \return true if found.
       */
    inline bool closest_pointd (KEY& k, VAL& v, const KEY& p, double dist) const;

    /*! Search all points which are in a sphere of center and radius.
       * \param keys : Array of points found.
       * \param vals : Array of point datas found.
       * \param center : Center of sphere.
       * \param radius : Radius of sphere.
       * \return Length of keys and vals.
       */
    inline unsigned in_sphere (std::vector<KEY>& keys, std::vector<VAL>& vals, const key_type& center, scalar_type radius) const;

    /*! Search all points which are in a sphere of center p and radius. Special case for double type radius.
       * \param keys : Array of points found.
       * \param vals : Array of point datas found.
       * \param center : Center of sphere.
       * \param radius : Radius of sphere.
       * \return Length of keys and vals.
       */
    inline unsigned in_sphered (std::vector<KEY>& keys, std::vector<VAL>& vals, const key_type& center, double radius) const;

  private :
    // kdnode struct represents node in KD tree
    struct kdnode
    {
      inline kdnode ();
      inline kdnode (const KEY& _key, const VAL& _val);
      inline scalar_type dist (const key_type& p) const;

      KEY   m_key;
      VAL   m_val;
      short m_dsc;
    };

    typedef std::vector<kdnode> node_vec_type;
    node_vec_type m_init_nodes;
    node_vec_type m_nodes;

    // comp is a class used for comparing two keys. comp is constructed
    // with the discriminator - i.e. the coordinate of the key that is used
    // for comparing keys - comp objects are passed to the sort algorithm.
    struct comp
    {
      comp (unsigned _dsc);
      inline bool operator () (const key_type& k0, const key_type& k1) const;
      inline bool operator () (const kdnode& k0, const kdnode& k1) const;

      unsigned dsc;
    };

  private :
    unsigned m_max_depth; // The greatest depth of KD tree.
    unsigned m_elements;  // Total number of elements in tree
    unsigned m_dim;       // The dimension -- K

  private :
    // Passed a vector of keys, this function will construct an optimal tree.
    // It is called recursively - second argument is level in tree.
    inline void optimize (unsigned, unsigned, unsigned, unsigned);

    // Finde nearest neighbour.
    inline unsigned closest_point_priv (unsigned, const key_type&, scalar_type&) const;

    // Finds the optimal discriminator. There are more ways, but this
    // function traverses the vector and finds out what dimension has
    // the greatest difference between min and max element. That dimension
    // is used for discriminator
    inline unsigned opt_disc (unsigned, unsigned) const;
  };

#include "kdtree_impl.hpp"
} // Namespace
#endif
