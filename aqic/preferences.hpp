#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP

#include "downloaderdata.hpp"
#include "imageexporterparams.hpp"
#include "displaydata.hpp"
#include <QDialog>

namespace Ui {
  class CPreferences;
}

class QAbstractButton;

class CPreferences : public QDialog
{
  Q_OBJECT

public:
  explicit CPreferences (SDownloaderData& downloaderData,
                         SImageExporterParams& imageExporterParams,
                         SDisplayData& displayData,
                         QWidget* parent = nullptr);
  ~CPreferences ();

protected slots:
  void buttonClicked (QAbstractButton* button);
  void on_m_pageSelector_itemSelectionChanged ();
  void on_m_quality_valueChanged (int value);
  void on_m_folderBrowser_clicked (bool);
  void on_m_font_clicked (bool);
  void on_m_chooseColor_clicked (bool);
  void positionClicked (bool);

private :
  void initialize ();
  void setColorPosition ();

private:
  Ui::CPreferences* ui;
  SDownloaderData&      m_downloaderData;
  SDownloaderData       m_downloaderDataBackup;
  SImageExporterParams& m_imageExporterParams;
  SImageExporterParams  m_imageExporterParamsBackup;
  SDisplayData&         m_displayData;
  SDisplayData          m_displayDataBackup;
};

#endif // PREFERENCES_HPP
