#include "translators.hpp"
#include "applocations.hpp"
#include <QTranslator>
#include <QLocale>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QDir>

CTranslators::CTranslators ()
{
  installTranslators ();
}

CTranslators::~CTranslators ()
{
  removeTranslators ();
}

void CTranslators::installTranslators ()
{
  m_translators.clear ();
  QTranslator* translator = nullptr;
  QString      systemName = QLocale::system ().name ();
  QString      locale     = systemName.section ('_', 0, 0);
  QDir         dir (CAppLocations::appLocation ());
  dir.cd ("config");
  QStringList filters (QStringLiteral ("*_") + locale + QStringLiteral (".qm"));
  QStringList files = dir.entryList (filters, QDir::Files | QDir::NoDotAndDotDot);
  for (QString const & file : qAsConst (files))
  {
    QString path = dir.absoluteFilePath (file);
    translator   = new QTranslator;
    translator->load (path);
    QCoreApplication::installTranslator (translator);
    m_translators.append (translator);
  }

  translator = new QTranslator;
  if (!translator->load ("qt_" + systemName, QLibraryInfo::location (QLibraryInfo::TranslationsPath)))
  { // Qt is not installed. Try to use qt_translation local folder.
    dir.cdUp ();
    dir.cd ("qt_translations");
    translator->load ("qt_" + systemName, dir.absolutePath ());
  }

  QCoreApplication::installTranslator (translator);
  m_translators.append (translator);
}

void CTranslators::removeTranslators ()
{
  for (QTranslator* translator : qAsConst (m_translators))
  {
    QCoreApplication::removeTranslator (translator);
    delete translator;
  }

  m_translators.clear ();
}

