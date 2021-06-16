#ifndef EXPORTMOVIERESULTS_HPP
#define EXPORTMOVIERESULTS_HPP

#include <QDialog>

namespace Ui {
  class CExportMovieResults;
}

class CExportMovieResults : public QDialog
{
  Q_OBJECT

public:
  explicit CExportMovieResults (int exportedImageCount, int imageCount, QString const & folder, QWidget* parent = nullptr);
  ~CExportMovieResults ();

protected slots:
  void on_m_browser_clicked (bool);

private:
  Ui::CExportMovieResults* ui;
};

#endif // EXPORTMOVIERESULTS_HPP
