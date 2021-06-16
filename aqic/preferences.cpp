#include "preferences.hpp"
#include "ui_preferences.h"
#include "../qtools/applocations.hpp"
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QDate>

CPreferences::CPreferences (SDownloaderData& downloaderData,
                            SImageExporterParams& imageExporterParams, SDisplayData& displayData,
                            QWidget* parent) : QDialog (parent), ui (new Ui::CPreferences),
                            m_downloaderData (downloaderData),
                            m_imageExporterParams (imageExporterParams),
                            m_displayData (displayData)
{
  ui->setupUi (this);
  setWindowFlags (windowFlags () & (~Qt::WindowContextHelpButtonHint));
  m_downloaderDataBackup      = downloaderData;
  m_imageExporterParamsBackup = imageExporterParams;
  m_displayDataBackup         = displayData;
  initialize ();
  ui->m_pageSelector->item (0)->setSelected (true);
  connect (ui->buttonBox, &QDialogButtonBox::clicked, this, &CPreferences::buttonClicked);
}

CPreferences::~CPreferences ()
{
  delete ui;
}

static void fontText (QLabel* label, QFont const & font)
{
  label->setText (QString ("%1 - %2").arg (font.family ()).arg (font.pointSize ()));
}

void CPreferences::setColorPosition ()
{
  QList<QToolButton*> btns     = ui->m_position->findChildren<QToolButton*> ();
  QColor              defColor = QToolButton ().palette ().color (QPalette::ButtonText);
  for (QToolButton* btn : qAsConst (btns))
  {
    QColor   color   = btn->objectName ().midRef (1).toInt () != m_imageExporterParams.m_alignment ? defColor : Qt::red;
    QPalette palette = btn->palette ();
    palette.setColor (QPalette::ButtonText, color);
    btn->setPalette (palette);
  }
}

static void setDateColor (QFrame* frame, int rgb)
{
  QString styleSheet = QString ("background-color: #%1").arg (rgb, 6, 16, QLatin1Char ('0'));
  frame->setStyleSheet (styleSheet);
}

void CPreferences::initialize ()
{
  ui->m_cleanCache->setChecked (m_downloaderData.m_cacheClean);
  ui->m_cacheSize->setValue (m_downloaderData.m_cacheSize / (1024 * 1024));
  ui->m_serverTimeout->setValue (m_downloaderData.m_serverTimeout / 60000);
  ui->m_newData->setValue (m_downloaderData.m_newDataDelay / 60000);
  ui->m_cacheValidity->setValue (m_downloaderData.m_cacheValidity / 3600000);

  ui->m_folder->setText (m_imageExporterParams.m_folder);
  ui->m_quality->setValue (m_imageExporterParams.m_quality == -1 ? 50 : m_imageExporterParams.m_quality);
  ui->m_qualityValue->setText (QString ("(%1)").arg (ui->m_quality->value ()));
  ui->m_format->setCurrentText (m_imageExporterParams.m_format);
  ui->m_browser->setChecked (m_imageExporterParams.m_launchViewer);
  ui->m_date->setChecked (m_imageExporterParams.m_date);
  setDateColor (ui->m_color, m_imageExporterParams.m_dateColor);

  QFont font;
  font.fromString (m_imageExporterParams.m_dateFont);
  fontText (ui->m_fontLabel, font);

  QList<QToolButton*> btns = ui->m_position->findChildren<QToolButton*> ();
  QString             date   = QLocale::system ().toString (QDate::currentDate (), QLocale::NarrowFormat);
  for (QToolButton* btn : qAsConst (btns))
  {
    connect (btn, &QToolButton::clicked, this, &CPreferences::positionClicked);
    btn->setText (date);
  }

  setColorPosition ();
  ui->m_opacity->setValue (m_displayData.m_opacity);
  ui->m_circle->setChecked (m_displayData.m_circle);
}

void CPreferences::buttonClicked (QAbstractButton* button)
{
  switch (ui->buttonBox->buttonRole (button))
  {
    case QDialogButtonBox::AcceptRole :
    {
      m_downloaderData.m_cacheClean    = ui->m_cleanCache->isChecked ();
      m_downloaderData.m_cacheSize     = ui->m_cacheSize->value () * 1024 * 1024;
      m_downloaderData.m_serverTimeout = ui->m_serverTimeout->value () * 60000;
      m_downloaderData.m_newDataDelay  = ui->m_newData->value () * 60000;
      m_downloaderData.m_cacheValidity = ui->m_cacheValidity->value () * 3600000;

      m_imageExporterParams.m_folder       = ui->m_folder->text ();
      m_imageExporterParams.m_format       = ui->m_format->currentText ();
      m_imageExporterParams.m_quality      = ui->m_quality->value ();
      m_imageExporterParams.m_launchViewer = ui->m_browser->isChecked ();
      m_imageExporterParams.m_date         = ui->m_date->isChecked ();
      m_imageExporterParams.m_dateColor    = ui->m_color->palette ().color (QPalette::Window).rgb () & 0x00FFFFFF;

      m_displayData.m_opacity = ui->m_opacity->value ();
      m_displayData.m_circle  = ui->m_circle->isChecked ();
      break;
    }

    case QDialogButtonBox::ResetRole :
      m_downloaderData      = SDownloaderData ();
      m_imageExporterParams = SImageExporterParams ();
      m_displayData         = SDisplayData ();
      initialize ();
      break;

    default:
      m_downloaderData      = m_downloaderDataBackup;
      m_imageExporterParams = m_imageExporterParamsBackup;
      m_displayData         = m_displayDataBackup;
      break;
  }
}

void CPreferences::on_m_pageSelector_itemSelectionChanged ()
{
  QList<QListWidgetItem*> items = ui->m_pageSelector->selectedItems ();
  int                     row   = ui->m_pageSelector->row (items.first ());
  ui->stackedWidget->setCurrentIndex (row);
}

void CPreferences::on_m_quality_valueChanged (int value)
{
  ui->m_qualityValue->setText (QString ("(%1)").arg (value));
}

void CPreferences::on_m_folderBrowser_clicked (bool)
{
  QString tmp = m_imageExporterParams.m_folder;
  if (tmp.isEmpty ())
  {
    tmp = CAppLocations::location (CAppLocations::Images);
  }

  tmp = QFileDialog::getExistingDirectory (this, tr ("Open Directory"),
                                           tmp, QFileDialog::ShowDirsOnly);
  if (!tmp.isEmpty ())
  {
    m_imageExporterParams.m_folder = tmp;
    ui->m_folder->setText (tmp);
  }
}

void CPreferences::on_m_font_clicked (bool)
{
  bool ok;
  QFont initial;
  initial.fromString (m_imageExporterParams.m_dateFont);
  QFont font = QFontDialog::getFont (&ok, initial, this);
  if (ok)
  {
    fontText (ui->m_fontLabel, font);
    m_imageExporterParams.m_dateFont = font.toString ();
  }
}

void CPreferences::on_m_chooseColor_clicked (bool)
{
  QColor initial (m_imageExporterParams.m_dateColor);
  QColor color = QColorDialog::getColor (initial, this);
  if (color.isValid ())
  {
    setDateColor (ui->m_color, color.rgb () & 0x00FFFFFF);
  }
}

void CPreferences::positionClicked (bool)
{
  m_imageExporterParams.m_alignment = static_cast<QToolButton*> (sender ())->objectName ().midRef (1).toInt ();
  setColorPosition ();
}

