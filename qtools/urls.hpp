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
  /*! Struct for air quality urls. */
  struct SAirQuality
  {
    std::array<QString, 4> m_strings; //!< [0] Url of data, [1] Url of provider, [2] Region code, [3] Region name
    int                    m_update; //!< Not use.
  };

  /*! Map of air qualities ordered by code. */
  using TAirQualities = QMap<QString, SAirQuality>;

  /*! Index of string. */
  enum EAQIndex { Url, Provider, Region, Name };

  /*! Default constructor. */
  CUrls ();

  /*! Initialize urls.
   *  External IP and IP address position are hard coded.
   *  Air quality urls are read from regions.json.
   */
  void initialize ();

  /*! Returns the external IP of the router. */
  QString const & externalIPAddr () const { return m_externalIPAddr; }

  /*! Returns the approximative position of the external IP. */
  QString const & ipAddrPosition () const { return m_ipAddrPosition; }

  /*! Returns the air quality string value.
   *  \param region: The region code.
   *  \param index: The string. See SAirQuality.
   */
  inline QString airQualityString (QString const & region, EAQIndex index) const;

  /*! Returns the air quality data url. */
  inline QString airQualityUrl (QString const & region) const;

  /*! Returns region code. In fact this fonction return parameter region. */
  inline QString airQualityRegionCode (QString const & region) const;

  /*! Returns region name. */
  inline QString airQualityRegionName (QString const & region) const;

  /*! Returns the provider url. */
  inline QString airQualityProvider (QString const & region) const;

  /*! Returns the map of air qualities. */
  TAirQualities const & airQualities () const { return m_airQualities; }

  /*! Returns the biggest provider url length. This function is used to initialize the combobox size. */
  QString biggestProviderStringLen () const;

  /*! Returns the single instance of CUrls. */
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
