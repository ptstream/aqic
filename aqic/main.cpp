#include "mainwindow.hpp"
#include <QApplication>
#include "../qtools/translators.hpp"
#include "../qtools/applocations.hpp"

int main (int argc, char* argv[])
{
  qputenv ("QTLOCATION_OPENGL_ITEMS", "1");
  Q_INIT_RESOURCE (wtools);
  QGuiApplication::setAttribute (Qt::AA_EnableHighDpiScaling);
  auto  application = new QApplication (argc, argv);
  auto  translators = new CTranslators;
  auto  mainWindow  = new CMainWindow;
  QIcon icon (":/icons/aqic.ico");
  mainWindow->setWindowIcon (icon);
  mainWindow->show ();
  int ret = application->exec ();
  delete mainWindow;
  delete translators;
  delete application;
  return ret;
}
