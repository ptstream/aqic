#ifndef APPLOCATIONS_HPP
#define APPLOCATIONS_HPP

#include <QString>

/*! \brief The CAppLocations class provides methods for accessing common paths.
 *
 * This class contains functions to query standard locations on the local filesystem, for common tasks such as
 * user-specific directories or system-wide configuration directories linked at the application.
 *
 * AppFilePath is the absolute file path of the executable file. Using standard installers:
 * \li Windows: C:\Program Files\<appname>\<appname>.exe
 * \li Linux: /opt/<appname>/<appname>
 * \li MacOS: /Applications/<appname>.app/Contents/MacOS/<appname>
 *
 * AppFileName is the file name of the executable.
 * \li Windows: <appname>.exe
 * \li Linux: <appname>
 * \li MacOS: <appname>
 *
 * AppLocation is the folder containing the executable.
 * \li Windows: C:\Program Files\<appname>
 * \li Linux: /opt/<appname>
 * \li MacOS: /Applications/<appname>.app/Contents/MacOS
 *
 * Config location is the subfolder "config" of the AppLocation.
 *
 * DataLocation is the folder containing for example <appname>.ini.
 * \li Windows: C:/Users/%USER%/AppData/Local/<appname>
 * \li Linux: ~/.local/share/<appname>
 * \li MacOS: ~/Library/Application Support/<appname>
 *
 * CacheLocation is the folder containing the cache of data.
 * \li Windows: C:/Users/%USER%/AppData/Local/<appname>/cache
 * \li Linux: ~/.cache/<appname>
 * \li MacOS: ~/Library/Caches/<appname>
 */
class CAppLocations
{
public:
  /*! This enumeration defines the common name uses by this class. */
  enum EType { AppName,        //!< The appication name.
               AppFilePath,    //!< The absolute file path of the executable file.
               AppFileName,    //!< The file name of the executable.
               AppLocation,    //!< The folder containing the executable.
               ConfigLocation, //!< The subfolder "config" of the AppLocation.
               DataLocation,   //!< The folder containing for example <appname>.ini.
               CacheLocation,  //!< The folder containing the cache of data.
               Documents,      //!< My documents folder.
               Images,         //!< My images folder.
               Temp,           //!< Temp foler.
             };

  /*! Returns all the directories or file names where files of type belong. */
  static QString location (EType type);

  /*! Returns the  folder containing the executable. */
  static QString appLocation ();

  /*! Returns the fileName prefixed by locale code. */
  static QString localeConfigFilePath (QString const & fileName);
};

#endif // APPLOCATIONS_HPP
