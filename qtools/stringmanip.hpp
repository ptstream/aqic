#ifndef STRINGMANIP_HPP
#define STRINGMANIP_HPP

#include <QString>

/*! \brief This class allows to manipulate string. */
class CStringManip
{
public:
 enum EConversion { NoConversion, Upper, Lower };

 /*! Returns s without diacritics. */
 static QString removeDiacritics (QString s, EConversion conv = NoConversion);
};

#endif // STRINGMANIP_HPP
