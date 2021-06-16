#include "globallegend.hpp"
#include "ui_globallegend.h"
#include <QDesktopServices>
#include <QUrl>

CGlobalLegend::CGlobalLegend (QWidget* parent) : QDialog (parent), ui (new Ui::CGlobalLegend), m_legend (CAQLegend::gblInst ())
{
  ui->setupUi (this);
  setWindowFlag (Qt::WindowContextHelpButtonHint, false);

  m_text = QString ("<p align=\"center\"><b>%1</b></p>")
           .arg (tr ("Sub-index scales depending on the concentration of each pollutant"));
  ui->m_title->setText (m_text);

  m_text = QString (R"(<table border="1" cellpadding="2" width="100%" align="center">)");
  addRow (0, true);
  for (int i = 0, count = m_legend->size (); i < count; ++i)
  {
    addRow (i, false);
  }

  m_text += "</table>";
  ui->m_text->setText (m_text);
  connect (ui->m_source, &QLabel::linkActivated, this, &CGlobalLegend::linkActived);
}

CGlobalLegend::~CGlobalLegend ()
{
  delete ui;
}

void CGlobalLegend::addRow (int pollutant, bool header)
{
  SPollutantLegend const & pl  = m_legend->at (pollutant);
  m_text                      += "<tr>";
  QString fmt = QString ("<td align=\"center\">%1</td><td align=\"center\">%2</td>");
  m_text     += header ? fmt.arg (tr ("Metric"), tr ("Pollutant"))
                       : fmt.arg (pl.m_metric, pl.m_name);
  for (int index = CAQLegend::Unavailable + 1; index < CAQLegend::Event; ++index)
  {
    SPollutantLegend::TIndexes const & inds      = pl.m_indexes;
    QString const &                    color     = inds.at (index).m_color;
    QString const &                    textColor = inds.at (index).m_text;
    QString                            si;
    if (header)
    {
      si = inds.at (index).m_wording;
    }
    else
    {
      SPollutantLegend::TInterval const & itv = inds.at (index).m_interval;
      si = itv.first == 9999 ? QStringLiteral (">") : QString::number (itv.first) + QLatin1Char ('-');
      si +=  QString::number (itv.second);
    }

    m_text += QString (R"(<td style="background:%1;color:%2" align=center>%3</td>)")
              .arg (color, textColor, si);
  }

  m_text += "</tr>";
}

void CGlobalLegend::linkActived (QString const & link)
{
  QUrl url = QUrl::fromLocalFile (link);
  QDesktopServices::openUrl (url);
}
