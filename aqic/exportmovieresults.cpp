#include "exportmovieresults.hpp"
#include "ui_exportmovieresults.h"
#include <QDesktopServices>
#include <QUrl>

CExportMovieResults::CExportMovieResults(int exportedImageCount, int imageCount, QString const & folder,
                                         QWidget* parent) : QDialog (parent), ui (new Ui::CExportMovieResults)
{
  ui->setupUi (this);
  setWindowFlags (windowFlags () & (~Qt::WindowContextHelpButtonHint));
  ui->m_folder->setText (folder);
  ui->m_imageCount->setText (QString ("%1/%2").arg (exportedImageCount).arg (imageCount));
}

CExportMovieResults::~CExportMovieResults ()
{
  delete ui;
}

void CExportMovieResults::on_m_browser_clicked (bool)
{
  QUrl url (ui->m_folder->text ());
  QDesktopServices::openUrl (url);
}
