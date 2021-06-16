#ifndef EXTERNALIPADDR_HPP
#define EXTERNALIPADDR_HPP

#include <QString>

/*! \brief Tool to get the external IP from the return of https://api.ipify.org/?format=json */
class CExternalIPAddr
{
public:
  /*! Decodes the json return and returns the IP. */
  static QString addr (QByteArray const & data);
};

#endif // EXTERNALIPADDR_HPP
