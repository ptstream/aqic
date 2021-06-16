#ifndef GLOBALINSTANCE_HPP
#define GLOBALINSTANCE_HPP

/*! \brief This template is the base class to create a single instance of a class.
 *
 *  To create a single instance of your class you must inherit your class of this template.
 *  To acced at your single object, you must create it once, and delete once to change it.
 *  The only function to get this single object is throw the gblInst.
 *  Because it is a single object, all functions except the contructor are static.
 *
 */
template<typename TYPE>
class CGlobalInstance
{
public:
  /*! Constructor. */
  CGlobalInstance ();

  /*! Deletes the global object. */
  static inline void deleteGlobalInstance ();

  /*! Returns the global object. */
  static inline CGlobalInstance* gblInst ();

protected:
  virtual ~CGlobalInstance () = default;

protected:
  static CGlobalInstance* m_gblInst; //!< The global object.
};

template<typename TYPE>
CGlobalInstance<TYPE>::CGlobalInstance ()
{
  deleteGlobalInstance ();
  m_gblInst = this;
}

template<typename TYPE>
void CGlobalInstance<TYPE>::deleteGlobalInstance ()
{
  delete m_gblInst;
  m_gblInst = nullptr;
}

template<typename TYPE>
CGlobalInstance<TYPE>* CGlobalInstance<TYPE>::gblInst ()
{
  return m_gblInst;
}

template<typename TYPE>
CGlobalInstance<TYPE>* CGlobalInstance<TYPE>::m_gblInst = nullptr;

#endif // GLOBALINSTANCE_HPP
