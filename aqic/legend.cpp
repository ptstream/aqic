#include "legend.hpp"
#include "ui_legend.h"
#include "../qtools/applocations.hpp"
#include <QDesktopServices>
#include <QUrl>

static CAQLegend::EPollutant const pollutant = CAQLegend::PM25;

CLegend::CLegend (CAQLegend const * legend, CAQLegend::EIndex index, QWidget* parent) : QDialog (parent),
  ui (new Ui::CLegend), m_legend (legend)
{
  ui->setupUi (this);
  setWindowFlag (Qt::WindowContextHelpButtonHint, false);

  SPollutantLegend const &           pl        = m_legend->at (pollutant);
  SPollutantLegend::TIndexes const & inds      = pl.m_indexes;
  QString const &                    color     = inds.at (index).m_color;
  QString const &                    textColor = inds.at (index).m_text;

  setStyleSheet ("background-color:" + color);
  ui->m_close->setStyleSheet ("color:" + textColor);

  QString format (R"(<p style="text-align:center;background:%1;color:%2">%3)");
  if (index > legend->Unavailable && index < legend->Event)
  {
    format += "<sup>*</sup>";
  }

  format += "</p>";
  m_text  = format.arg ("none", textColor, inds.at (index).m_wording);
  ui->m_index->setText (m_text);
  QFont font = ui->m_index->font ();
  font.setPointSize (2 * font.pointSize ());
  ui->m_index->setFont (font);

  if (index >= CAQLegend::Good && index <= CAQLegend::ExtremlyBad)
  {
    m_text  = R"(<table border="1" cellpadding="2" width="100%" align="center">)";
    m_text += QString (R"(<tr style="background:%1;color:%2">
                        <td width="33%" align="center">%3</td>
                        <td width="33%" align="center">%4</td>
                        <td width="33%" align="center">%5<sup>**</sup></td>
                        </tr>)")
              .arg ("none", textColor, tr ("Metric"), tr ("Pollutant"), tr ("Concentration"));

    for (int i = CAQLegend::PM25; i < CAQLegend::LastPollutant; ++i)
    {
      addColumn (static_cast<CAQLegend::EPollutant>(i), index);
    }

    m_text += "</table>";
    ui->m_text->setText (m_text);
    ui->m_decree->setText (QString (R"(<div style="color:%1"><sup>*</sup>%2</div>)")
                           .arg (textColor, ui->m_decree->text ()));
    ui->m_unit->setText (QString (R"(<div style="color:%1"><sup>**</sup>%2g/m<sup>3</sup></div>)")
                         .arg (textColor).arg (QChar (0xb5)));  }
  else
  {
    ui->m_decree->setHidden (true);
    m_text = QString ("<p style=\"color:%1\">%2</p>")
             .arg (textColor,
                   index == CAQLegend::Unavailable ? tr ("Absence of sufficient data to calculate the town index (no model).") :
                                                     tr ("Incident or accident causing emissions."));
    ui->m_text->setText (m_text);
  }

  ui->m_sourceLabel->setStyleSheet ("color:" + textColor);
  connect (ui->m_source, &QLabel::linkActivated, this, &CLegend::linkActived);
}

CLegend::~CLegend ()
{
  delete ui;
}

void CLegend::addColumn (CAQLegend::EPollutant pollutant, CAQLegend::EIndex index)

{
  SPollutantLegend const &            pl  = m_legend->at (pollutant);
  SPollutantLegend::TIndexes const & inds = pl.m_indexes;

  SPollutantLegend::TInterval const & itv = inds.at (index).m_interval;
  QString                             si  = itv.first == 9999 ? QStringLiteral (">") :
                                                                QString::number (itv.first) +
                                                                QLatin1Char ('-');
  si     +=  QString::number (itv.second);
  m_text += QString (R"(<tr style="background:%1;color:%2">
                        <td width="33%" align="center">%3</td>
                        <td width="33%" align="center">%4</td>
                        <td width="33%" align="center">%5</td>
                        </tr>)")
            .arg ("none", inds.at (index).m_text, pl.m_metric, pl.m_name, si);
}

void CLegend::linkActived (QString const & link)
{
  QUrl url = QUrl::fromLocalFile (link);
  QDesktopServices::openUrl (url);
}
