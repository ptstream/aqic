#ifndef STRINGMANIP_HPP
#define STRINGMANIP_HPP

#include <QString>

/*! \brief This class allows to manipulate string. */
class CStringManip
{
public:
 enum EConversion { NoConversion, Upper, Lower };

 /*! Returns s without diacritics.
  *  \param s: the input string.
  *  \param conv: To convert in upper or lower cases or keep case.
  */
 static QString removeDiacritics (QString s, EConversion conv = NoConversion);
};

#endif // STRINGMANIP_HPP
