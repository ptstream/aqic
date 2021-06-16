#ifndef KDTREE_IMPL_H
#define KDTREE_IMPL_H

template<class KEY, typename VAL>
kdtree<KEY, VAL>::kdnode::kdnode () : m_dsc (0)
{
}

template<class KEY, typename VAL>
kdtree<KEY, VAL>::kdnode::kdnode (const KEY& _key, const VAL& _val) : m_key (_key), m_val (_val), m_dsc(-1)
{
}

template<class KEY, typename VAL>
typename kdtree<KEY, VAL>::scalar_type kdtree<KEY, VAL>::kdnode::dist (const key_type& p) const 
{
  key_type dist_vec = p;
  dist_vec         -= m_key;
  return dot (dist_vec, dist_vec);
}

template<class KEY, typename VAL>
kdtree<KEY, VAL>::comp::comp (unsigned _dsc): dsc(_dsc)
{
}

template<class KEY, typename VAL>
bool kdtree<KEY, VAL>::comp::operator () (const key_type& k0, const key_type& k1) const
{
  for (unsigned i = 0; i < key_type::Dim; ++i)
  {
    unsigned j = (dsc + i) % key_type::Dim;
    if (k0[j] < k1[j])
    {
      return true;
    }

    if (k0[j] > k1[j])
    {
      return false;
    }
  }

  return false;
}

template<class KEY, typename VAL>
bool kdtree<KEY, VAL>::comp::operator () (const kdnode& k0, const kdnode& k1) const
{
  return (*this) (k0.m_key, k1.m_key);
}

template<typename KEY, typename VAL>
unsigned kdtree<KEY,VAL>::opt_disc (unsigned kvec_beg, unsigned kvec_end) const 
{
  key_type vmin = m_init_nodes[kvec_beg].m_key;
  key_type vmax = m_init_nodes[kvec_beg].m_key;
  for (unsigned i = kvec_beg; i < kvec_end; ++i)
  {
    vmin = c_min (vmin, m_init_nodes[i].m_key);
    vmax = c_max (vmax, m_init_nodes[i].m_key);
  }

  unsigned od    = 0;
  key_type ave_v = vmax - vmin;
  for(unsigned i = 1; i < key_type::Dim; ++i)
  {
    if (ave_v[i] > ave_v[od])
    {
      od = i;
    }
  }

  return od;
} 

template<typename KEY, typename VAL>
void kdtree<KEY,VAL>::optimize (unsigned cur, unsigned kvec_beg, unsigned kvec_end, unsigned level)
{
  // Assert that we are not inserting beyond capacity.
  assert (cur < (unsigned)m_nodes.size ());

  // If there is just a single element, we simply insert.
  if (kvec_beg + 1 == kvec_end) 
  {
    m_max_depth        = std::max (level, m_max_depth);
    m_nodes[cur]       = m_init_nodes[kvec_beg];
    m_nodes[cur].m_dsc = -1;
    return;
  }
  
  // Find the axis that best separates the data.
  unsigned disc = opt_disc (kvec_beg, kvec_end);

  // Compute the median element. See my document on how to do this
  // www.imm.dtu.dk/~jab/publications.html
  unsigned n          = kvec_end - kvec_beg;
  unsigned m          = 1U << largest_power_of_2 (n);
  unsigned r          = n - (m - 1);
  unsigned left_size  = (m - 2) / 2;
  unsigned right_size = (m - 2) / 2;
  if (r < m / 2)
  {
    left_size += r;
  }
  else
  {
    left_size  += m / 2;
    right_size += r - m / 2;
  }

  unsigned median = kvec_beg + left_size;

  // Sort elements but use nth_element (which is cheaper) than
  // a sorting algorithm. All elements to the left of the median
  // will be smaller than or equal the median. All elements to the right
  // will be greater than or equal to the median.
  const comp comp (disc);

  unsigned end_real = static_cast<unsigned>(m_init_nodes.size() - 1);
  unsigned end     = end_real < kvec_end ? end_real : kvec_end;
  std::nth_element (&m_init_nodes[kvec_beg], &m_init_nodes[median], &m_init_nodes[end], comp);

  // Insert the node in the final data structure.
  m_nodes[cur]       = m_init_nodes[median];
  m_nodes[cur].m_dsc = (short)disc;

  // Recursively build left and right tree.
  if(left_size > 0)  
    optimize (2 * cur, kvec_beg, median, level + 1);
    
  if(right_size > 0) 
    optimize (2 * cur + 1, median + 1, kvec_end, level + 1);
}

template<typename KEY, typename VAL>
unsigned kdtree<KEY,VAL>::closest_point_priv (unsigned n, const key_type& p, scalar_type& dist) const
{
  unsigned         ret_node  = 0;
  scalar_type this_dist = m_nodes[n].dist (p);

  if (this_dist < dist)
  {
    dist     = this_dist;
    ret_node = n;
  }

  if (m_nodes[n].m_dsc != -1)
  {
    unsigned    dsc      = m_nodes[n].m_dsc;
    scalar_type dsc_dist = sqr (m_nodes[n].m_key[dsc] - p[dsc]);
    bool        left_son = comp(dsc)(p,m_nodes[n].m_key);

    if (left_son || dsc_dist < dist)
    {
      unsigned left_child = 2 * n;
      if (left_child < (unsigned)m_nodes.size ())
      {
        if (unsigned nl = closest_point_priv (left_child, p, dist))
        {
          ret_node = nl;
        }
      }
    }

    if (!left_son || dsc_dist < dist)
    {
      unsigned right_child = 2 * n + 1;
      if(right_child < (unsigned)m_nodes.size ())
      {
        if (unsigned nr = closest_point_priv (right_child, p, dist))
        {
          ret_node = nr;
        }
      }
    }
  }

  return ret_node;
}

template<class KEY, typename VAL>
kdtree<KEY, VAL>::kdtree (): m_max_depth (0), m_elements (0), m_dim (key_type::Dim)
{
}

template<class KEY, typename VAL>
kdtree<KEY, VAL>::kdtree (unsigned cpoint): m_max_depth (0), m_elements (0), m_dim (key_type::Dim)
{
  m_init_nodes.reserve (cpoint);
}

template<class KEY, typename VAL>
void kdtree<KEY, VAL>::insert (const KEY& key, const VAL& val)
{
  m_init_nodes.push_back (kdnode (key, val));
}

template<class KEY, typename VAL>
void kdtree<KEY, VAL>::build ()
{
  m_nodes.resize (m_init_nodes.size () + 1);
  if (!m_init_nodes.empty ())
  {
    optimize (1, 0, static_cast<unsigned>(m_init_nodes.size ()), 0);
  }

  node_vec_type v (0);
  m_init_nodes.swap (v);
}

template<class KEY, typename VAL>
const KEY& kdtree<KEY, VAL>::key (unsigned i) const
{
  return m_init_nodes[i].m_key;
}

template<class KEY, typename VAL>
const VAL& kdtree<KEY, VAL>::val (unsigned i) const
{
  return m_init_nodes[i].m_val;
}

template<class KEY, typename VAL>
void kdtree<KEY, VAL>::get (KEY& key, VAL& val, unsigned i) const
{
  key = m_init_nodes[i].m_key;
  val = m_init_nodes[i].m_val;
}

template<class KEY, typename VAL>
bool kdtree<KEY, VAL>::closest_point (KEY& k, VAL& v, const KEY& p, scalar_type dist) const
{
  dist       = sqr (dist);
  unsigned n = closest_point_priv (1, p, dist);
  if (n != 0)
  {
    k    = m_nodes[n].m_key;
    v    = m_nodes[n].m_val;
    return true;
  }

  return false;
}

template<class KEY, typename VAL>
bool kdtree<KEY, VAL>::closest_pointd (KEY& k, VAL& v, const KEY& p, double dist) const
{
  return closest_point (k, v, p, static_cast<scalar_type>(dist));
}

#endif
