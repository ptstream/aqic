#ifndef URLS_HPP
#define URLS_HPP

#include "../tools/globalinstance.hpp"
#include <QString>
#include <QMap>
#include <array>

class QUrl;

/*! \brief This class provides url.
 *
 *  The urls are hard coded for external IP and location from IP.
 *  The urls of air quality are read from region_xx.json
 */
class CUrls : public CGlobalInstance<CUrls>
{
public:
  struct SAirQuality
  {
    std::array<QString, 4> m_strings;
    int                    m_update;
  };

  using TAirQualities = QMap<QString, SAirQuality>;

  enum EAQIndex { Url, Provider, Region, Name };

  CUrls ();

  void initialize ();

  QString const & externalIPAddr () const { return m_externalIPAddr; }
  QString const & ipAddrPosition () const { return m_ipAddrPosition; }

  inline QString airQualityString (QString const & region, EAQIndex index) const;
  inline QString airQualityUrl (QString const & region) const;
  inline QString airQualityRegionCode (QString const & region) const;
  inline QString airQualityRegionName (QString const & region) const;
  inline QString airQualityProvider (QString const & region) const;

  TAirQualities const & airQualities () const { return m_airQualities; }
  QString biggestProviderStringLen () const;

  static CUrls* gblInst () { return static_cast<CUrls*>(CGlobalInstance<CUrls>::gblInst ()); }

private :
  QString       m_externalIPAddr;
  QString       m_ipAddrPosition;
  TAirQualities m_airQualities;
};

QString CUrls::airQualityString (QString const & region, EAQIndex index) const
{
  return m_airQualities.value (region).m_strings[index];
}

QString CUrls::airQualityUrl (QString const & region) const
{
  return airQualityString (region, Url);
}

QString CUrls::airQualityRegionCode (QString const & region) const
{
  return airQualityString (region, Region);
}

QString CUrls::airQualityRegionName (QString const & region) const
{
  return airQualityString (region, Name);
}

QString CUrls::airQualityProvider (QString const & region) const
{
  return airQualityString (region, Provider);
}
#endif // URLS_HPP
