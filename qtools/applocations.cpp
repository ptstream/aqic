#include "applocations.hpp"
#include "globals.hpp"
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

QString CAppLocations::appLocation ()
{
  QFileInfo fileInfo (QCoreApplication::applicationFilePath ());
  return fileInfo.absolutePath ();
}

QString CAppLocations::location (EType type)
{
  QString name;
  switch (type)
  {
    case AppName :
    {
      QFileInfo fi (QCoreApplication::applicationFilePath ());
      name    = fi.completeBaseName ();
      name[0] = name[0].toUpper ();
      break;
    }

    case AppFilePath :
      name = QCoreApplication::applicationFilePath ();
      break;

    case AppFileName :
    {
      QFileInfo fi (QCoreApplication::applicationFilePath ());
      name = fi.fileName ();
      break;
    }

    case AppLocation :
      name = appLocation ();
      break;

    case ConfigLocation :
    {
      QDir dir (appLocation ());
      dir.cd ("config");
      name = dir.absolutePath ();
      break;
    }

    case DataLocation :
      // MacOs
      //   ~/Library/Application Support/<APPNAME> (used)
      //   Library/Application Support/<APPNAME>
      //   <APPDIR>/../Resources

      // Windows
      //   C:/Users/<USER>/AppData/Local/<APPNAME> (used)
      //   C:/ProgramData/<APPNAME>"/
      //   <APPDIR>
      //   <APPDIR>/data/
      //   <APPDIR>/data/<APPNAME>

      // Linux
      //   ~/.local/share/<APPNAME> (used)
      //   /usr/local/share/<APPNAME>
      //   /usr/share/<APPNAME>
      name = QStandardPaths::writableLocation (QStandardPaths::AppDataLocation);
      break;

    case CacheLocation :
      // MacOs
      //  ~/Library/Caches/<APPNAME> (used)
      //  /Library/Caches/<APPNAME>

      // Windows
      //   C:/Users/<USER>/AppData/Local/<APPNAME>/cache (used)

      // Linux
      //   ~/.cache/<APPNAME> (used)
      name = QStandardPaths::writableLocation (QStandardPaths::CacheLocation);
      break;

    case Documents :
      name = QStandardPaths::writableLocation (QStandardPaths::DocumentsLocation);
      break;

    case Images :
      name = QStandardPaths::writableLocation (QStandardPaths::PicturesLocation);
      break;

    case Temp :
      name = QStandardPaths::writableLocation (QStandardPaths::TempLocation);
      break;

    default :
      qWarning () << "Unknown location. Return an empty location.";
  }

  return name;
}

QString CAppLocations::localeConfigFilePath (QString const & fileName)
{
  QString   localePath;
  QString   lcc = ::localeCountryCode ();
  QFileInfo fi (fileName);
  QString   suffix   = fi.suffix ();
  QString   baseName = fi.completeBaseName ();
  QDir      dir (CAppLocations::location (CAppLocations::ConfigLocation));
  localePath= dir.absoluteFilePath (QString ("%1_%2.%3").arg (baseName, lcc, suffix));
  if (!QFile::exists (localePath))
  {
    localePath = dir.absoluteFilePath (fileName);
  }

  return localePath;
}

