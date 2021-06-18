#ifndef TRANSLATORS_HPP
#define TRANSLATORS_HPP

#include <QList>
class QTranslator;

/*! \brief Tool to install or remove translators.
 *
 *  The qm files are searched in install folder.
 */
class CTranslators
{
public:
  /*! Constructor. */
  CTranslators ();

  /*! Destructor. */
  ~CTranslators ();

  /*! Installs translators. */
  void installTranslators ();

  /*! Removes translators. */
  void removeTranslators ();

private :
  QList<QTranslator*> m_translators ;
};

#endif // TRANSLATORS_HPP
