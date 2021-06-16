#include "about.hpp"
#include "appversion.hpp"
#include "../qtools/urls.hpp"
#include "ui_about.h"
#include <QDesktopServices>
#include <QUrl>

static void setTransparentBackGround (QWidget* widget)
{
  QWidget* parent = widget->parentWidget ();
  if (parent != nullptr)
  {
    QColor   color   = parent->palette ().color (parent->backgroundRole ());
    QPalette palette = widget->palette ();
    palette.setColor (QPalette::Base, color);
    widget->setPalette (palette);
  }
}

static QString providerLink (QString provider)
{
  int index  = provider.indexOf ('"');
  if (index != -1)
  {
    provider = provider.mid (index + 1);
    index    = provider.lastIndexOf ('"');
    if (index != -1)
    {
      provider.truncate (index);
    }
  }

  return provider;
}

CAbout::CAbout (QWidget *parent) : QDialog (parent), ui (new Ui::CAbout)
{
  ui->setupUi (this);
  setWindowFlag (Qt::WindowContextHelpButtonHint, false);
  ui->m_version->setText (APPVERSION);

  CUrls*                       urls = CUrls::gblInst ();
  CUrls::TAirQualities const & aqs= urls->airQualities ();
  for (CUrls::SAirQuality const & aq : aqs)
  {
    if (!aq.m_strings.at (CUrls::Url).isEmpty ())
    {
      QStringList itemTexts;
      itemTexts << aq.m_strings.at (CUrls::Name);
      itemTexts << providerLink (aq.m_strings.at (CUrls::Provider));
      auto item = new QTreeWidgetItem (itemTexts);
      ui->m_regions->addTopLevelItem (item);
    }
  }

  for (QTreeWidget* treeWidget : { ui->m_regions, ui->m_map })
  {
    if (treeWidget != ui->m_map)
    {
      treeWidget->sortItems (0, Qt::AscendingOrder);
    }

    treeWidget->resizeColumnToContents (0);
    treeWidget->resizeColumnToContents (1);
    setTransparentBackGround (treeWidget);
  }
}

CAbout::~CAbout ()
{
  delete ui;
}

void CAbout::on_m_qt_clicked (bool)
{
  QDesktopServices::openUrl (QUrl ("https://www.qt.io/"));
}

void CAbout::on_m_atmoFrance_clicked (bool)
{
  QDesktopServices::openUrl (QUrl ("https://atmo-france.org/"));
}

void CAbout::on_m_regions_itemClicked (QTreeWidgetItem* item, int)
{
  QString url = item->text (1);
  QDesktopServices::openUrl (QUrl (url));
}

void CAbout::on_m_map_itemClicked (QTreeWidgetItem* item, int)
{
  QString url = item->text (1);
  QDesktopServices::openUrl (QUrl (url));
}
