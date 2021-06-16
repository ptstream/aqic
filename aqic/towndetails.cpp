#include "towndetails.hpp"
#include "ui_towndetails.h"
#include "../town/towns.hpp"
#include "../wtools/slider.hpp"
#include "../qtools/urls.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QResizeEvent>
#include <QStyle>
#include <QDebug>

CTownDetails::CTownDetails (QWidget* parent) : QDialog (parent), ui (new Ui::CTownDetails)
{
  ui->setupUi (this);
  setWindowFlag (Qt::WindowContextHelpButtonHint, false);
  connect (ui->m_close, &QPushButton::clicked, this, &CTownDetails::close);
  connect (ui->tabWidget, &QTabWidget::currentChanged, this, &CTownDetails::tabChange);
  createTemporalWidgets ();
  createDailyProfilWidgets ();
  if (parent != nullptr)
  {
    QSize size            = parent->size ();
    int   widthFromParent = m_widthFromParent;
    if (size.width () <= 1024)
    {
      widthFromParent = 85; // %
    }

    size.setWidth (widthFromParent * size.width () / 100);
    size.setHeight (m_heightFromParent * size.height () / 100);
    resize (size);
  }
}

CTownDetails::~CTownDetails ()
{
  delete ui;
}

void CTownDetails::tabChange (int index)
{
  if (index == 1)
  {
    CSlider* slider = ui->m_dailyProfilSliders->findChild<CSlider*> ();
    if (slider != nullptr)
    {
      if (slider->maximum () == 0)
      {
        QList<QWidget*> ws = ui->m_dailyProfilSliders->findChildren<QWidget*> ();
        for (QWidget* w : qAsConst (ws))
        {
          delete w;
        }

        auto   warning = new QLabel (ui->m_dailyProfilSliders);
        QIcon icon     = style()->standardIcon (QStyle::SP_MessageBoxWarning);
        QPixmap pxm    = icon.pixmap (QSize (64, 64));
        warning->setPixmap (pxm);
        warning->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
        auto layout  = static_cast<QGridLayout*>(ui->m_dailyProfilSliders->layout ());
        layout->addWidget (warning, 2, 3);

        auto  message = new QLabel (ui->m_dailyProfilSliders);
        QFont font    = message->font ();
        font.setPointSize (15 * font.pointSize () / 10);
        message->setFont (font);
        message->setAlignment (Qt::AlignHCenter | Qt::AlignTop);
        message->setText (tr ("Not enough data to calculate the daily profile.<br/>Data depth should be greater than one week."));
        layout->addWidget (message, 3, 3);
      }
    }
  }
}

void CTownDetails::createTemporalWidgets ()
{
  QWidget* widgetParent = ui->m_temporalSliders;
  auto     layout       = new QGridLayout (widgetParent);
  layout->setContentsMargins (2, 2, 2, 2);
  int index = 0;
  createTemporalDates (index++, layout);
  TPollutantLegends const & pls = *CAQLegend::gblInst ();
  CSlider* slider = createSlider (tr ("Global"), index++, layout, widgetParent, true);
  connect (slider, &CSlider::valueChanged, this, &CTownDetails::temporalSliderChangeValue);
  for (SPollutantLegend const & pl : pls)
  {
    slider = createSlider (pl.m_name, index++, layout, widgetParent, true);
    connect (slider, &CSlider::valueChanged, this, &CTownDetails::temporalSliderChangeValue);
  }
}

void CTownDetails::createDailyProfilWidgets ()
{
  QWidget* widgetParent = ui->m_dailyProfilSliders;
  auto     layout       = new QGridLayout (widgetParent);
  layout->setContentsMargins (2, 2, 2, 2);
  int index = 0;
  createDaysOfWeek (index++, layout);
  TPollutantLegends const & pls = *CAQLegend::gblInst ();
  CSlider* slider = createSlider (tr ("Global"), index++, layout, widgetParent, false);
  connect (slider, &CSlider::valueChanged, this, &CTownDetails::dailyProfileSliderChangeValue);
  for (SPollutantLegend const & pl : pls)
  {
    slider = createSlider (pl.m_name, index++, layout, widgetParent, false);
    connect (slider, &CSlider::valueChanged, this, &CTownDetails::dailyProfileSliderChangeValue);
  }
}

void CTownDetails::createTemporalDates (int index, QGridLayout* layout)
{
  auto datesLayout = new QHBoxLayout;
  auto date        = new QLabel (ui->m_temporalSliders);
  date->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
  date->setAlignment (Qt::AlignLeft);
  date->setObjectName ("date1");
  datesLayout->addWidget (date, 0, Qt::AlignLeft);

  date = new QLabel (ui->m_temporalSliders);
  date->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
  date->setAlignment (Qt::AlignRight);
  date->setObjectName ("date2");
  datesLayout->addWidget (date, 1, Qt::AlignRight);

  layout->addLayout (datesLayout, index, 2);
}

void CTownDetails::createDaysOfWeek (int index, QGridLayout* layout)
{
  m_dayOfWeek = { tr ("Monday"),
                  tr ("Tuesday"),
                  tr ("Wednesday"),
                  tr ("Thursday"),
                  tr ("Friday"),
                  tr ("Saturday"),
                  tr ("Sunday")
                };

  auto datesLayout = new QHBoxLayout;
  for (int i = 0; i < 7; ++i)
  {
    auto day = new QLabel (ui->m_dailyProfilSliders);
    day->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    Qt::Alignment alignment;
    switch (i)
    {
      case 0 :
        alignment = Qt::AlignLeft;
        break;

      case 6 :
        alignment = Qt::AlignRight;
        break;

      default :
        alignment = Qt::AlignCenter;
        break;
    }

    day->setObjectName ("days");
    day->setAlignment (alignment);
    day->setText (m_dayOfWeek[i]);
    datesLayout->addWidget (day);
    datesLayout->setStretch (i, i == 0 || i == 6 ? 1 : 2);
  }

  layout->addLayout (datesLayout, index, 2);
}

CSlider* CTownDetails::createSlider (QString const & name, int index, QGridLayout* layout, QWidget* parent, bool createButton)
{
  auto label = new QLabel (parent);
  label->setText (name);
  layout->addWidget (label, index, 0);

  QToolButton* btn;
  if (createButton)
  {
    btn = new QToolButton (parent);
    btn->setAutoRepeat (true);
    btn->setIcon (QIcon ((":/icons/player_backward.png")));
    layout->addWidget (btn, index, 1);
    connect (btn, &QToolButton::clicked, this, &CTownDetails::backward);
  }

  auto slider = new CSlider (parent);
  slider->setObjectName (QString::number (index - 1));
  slider->setToolTipAccepted (createButton);
  slider->setCloneAccepted (createButton);
  slider->setMaximum (0);
  layout->addWidget (slider, index, 2);

  if (createButton)
  {
    btn = new QToolButton (parent);
    btn->setAutoRepeat (true);
    btn->setIcon (QIcon ((":/icons/player_forward.png")));
    layout->addWidget (btn, index, 3);
    connect (btn, &QToolButton::clicked, this, &CTownDetails::forward);

    btn = new QToolButton (parent);
    btn->setIcon (QIcon ((":/icons/hm.png")));
    btn->setObjectName (QString::number (index - 1));
    layout->addWidget (btn, index, 4);
    connect (btn, &QToolButton::clicked, this, &CTownDetails::highest);

    btn = new QToolButton (parent);
    btn->setIcon (QIcon ((":/icons/lm.png")));
    btn->setObjectName (QString::number (index - 1));
    layout->addWidget (btn, index, 5);
    connect (btn, &QToolButton::clicked, this, &CTownDetails::lowest);
  }

  return slider;
}

void CTownDetails::setTemporalColors (CAirQuality::EIndexType index, TColors const & colors)
{
  CSlider* slider     = ui->m_temporalSliders->findChild<CSlider*> (QString::number (index));
  int      colorCount = colors.size ();
  slider->setMaximum (colorCount > 0 ? colorCount - 1 : colorCount);
  slider->setTickColors (colors);
}

void CTownDetails::setDailyProfileColors (CAirQuality::EIndexType index, TColors const & colors)
{
  CSlider* slider = ui->m_dailyProfilSliders->findChild<CSlider*> (QString::number (index));
  slider->setMaximum (colors.size () - 1);
  slider->setTickColors (colors);
}

static int findValue (QList<QDate> const & dates, QDate const & date)
{
  int value = -1;
  if (!dates.isEmpty ())
  {
    value = dates.size () - 1;
    while (value > 0 && dates[value] > date)
    {
      --value;
    }
  }

  return value;
}

void CTownDetails::setDates (QList<QDate> const & dates)
{
  if (!dates.isEmpty ())
  {
    QDate date  = !m_initialDate.isValid () ? QDate::currentDate () : m_initialDate;
    int   value = findValue (dates, date);
    if (value >= 0)
    {
      QList<CSlider*> sliders = ui->m_temporalSliders->findChildren<CSlider*> ();
      for (CSlider* slider : qAsConst (sliders))
      {
        slider->setDates (dates);
        slider->setValue (value);
      }

      QLocale locale = QLocale::system ();
      QLabel* date1 = findChild<QLabel*> ("date1");
      date1->setText (locale.toString (dates.first (), QLocale::LongFormat));
      QLabel* date2 = findChild<QLabel*> ("date2");
      date2->setText (locale.toString (dates.last (), QLocale::LongFormat));
    }
  }
}

void CTownDetails::setInitialDate (QDate const & date)
{
  m_initialDate               = date;
  CSlider*             slider = ui->m_temporalSliders->findChild<CSlider*> ();
  QList<QDate> const & dates  = slider->dates ();
  int                  value  = findValue (dates, date);
  if (value < 0)
  {
    value = 0;
  }

  temporalSliderChangeValue (value);
}

void CTownDetails::setInitilDayOfWeek ()
{
  int value = QDate::currentDate ().dayOfWeek () - 1;
  dailyProfileSliderChangeValue (value);
}

void CTownDetails::temporalSliderChangeValue (int value)
{
  QList<CSlider*> sliders = ui->m_temporalSliders->findChildren<CSlider*> ();
  for (CSlider* slider : qAsConst (sliders))
  {
    if (slider->value () != value)
    {
      slider->blockSignals (true);
      slider->setValue (value);
      slider->blockSignals (false);
    }
  }

  updateTemporalComment (value);
}

void CTownDetails::dailyProfileSliderChangeValue (int value)
{
  QList<CSlider*> sliders = ui->m_dailyProfilSliders->findChildren<CSlider*> ();
  for (CSlider* slider : qAsConst (sliders))
  {
    if (slider->value () != value)
    {
      slider->blockSignals (true);
      slider->setValue (value);
      slider->blockSignals (false);
    }
  }

  updateDailyProfileComment (value);
}

void CTownDetails::resizeEvent (QResizeEvent* event)
{
  int width = event->size ().width ();
  QList<int> sizes;
  sizes << m_splitterSize * width / 100;
  sizes << width - sizes.first ();
  QList<QSplitter*> splitters = findChildren<QSplitter*> ();
  for (QSplitter* splitter : qAsConst (splitters))
  {
    splitter->setSizes (sizes);
  }

  QDialog::resizeEvent (event);
}

QString CTownDetails::table1 (QString const & date, int count, QString const & number)
{
  QString fmt    = R"(<tr><td width="50%" align="center">%1</td><td width="50%" align="center">%2</td></tr>)";
  QString text   = "<h2 align=\"center\">" + CTowns::gblInst ()->value (m_town).name () + "</h2><br/>";
  text          += R"(<table border="1" cellpadding="2" width="100%" align="center" color="%1">)";
  text          += fmt.arg (tr ("Date"), date);
  text          += fmt.arg (tr ("Insee"), m_town);
  text          += fmt.arg (tr ("Region"), m_regionName + " (" + m_regionCode + ')');
  QString r      = QString::number (m_lat, 'f', 6) + charDegree;
  text          += fmt.arg (tr ("Lat"), r);
  r              = QString::number (m_lon, 'f', 6) + charDegree;
  text          += fmt.arg (tr ("Lon"), r);
  text          += fmt.arg (number, QString::number (count));
  text          += "</table><br/>";
  return text;
}

QString CTownDetails::table2 (CAirQuality::TIndexes const & indexes)
{
  CAQLegend* legend = CAQLegend::gblInst ();
  QString    text;
  QString    fmt = R"(<tr style="background-color:#%1;color:%2"><td width="50%" align="center">%3</td>
                      <td width="50%" align="center">%4</td></tr>)";
  text += R"(<table border="1" cellpadding="2" width="100%" align="center" color="%1">)";
  quint8  index   = indexes[CAirQuality::Global];
  QString wording = legend->at (0).m_indexes[index].m_wording;
  QString bkgnd   = QString::number (CAirQuality::color (index), 16);
  QString color   = legend->at (0).m_indexes[index].m_text;
  text           += fmt.arg (bkgnd, color, tr ("Global Index"), wording);
  for (int i = 1; i < CAirQuality::LastIndex; ++i)
  {
    quint8 index = indexes[static_cast<CAirQuality::EIndexType>(i)];
    wording      = legend->at (i - 1).m_indexes[index].m_wording;
    color        = legend->at (i - 1).m_indexes[index].m_text;
    bkgnd        = QString::number (CAirQuality::color (index), 16);
    text        += fmt.arg (bkgnd, color, CAQLegend::gblInst ()->at (i - 1).m_name, wording);
  }

  text += "</table>";
  return text;
}

void CTownDetails::updateTemporalComment (int sliderValue)
{
  QLocale        locale = QLocale::system ();
  QList<QDate>   dates  = ui->m_temporalSliders->findChild<CSlider*> ()->dates ();
  QDate          date;
  if (!dates.isEmpty ())
  {
    date = ui->m_temporalSliders->findChild<CSlider*> ()->dates ()[sliderValue];
  }

  CAirQualities* aqs    = CAirQualities::gblInst ();
  CAirQuality    aq     = aqs->airQuality (m_town, date);
  int            count  = aqs->airQualityCount (m_town);
  QString        text   = table1 (locale.toString (date, QLocale::LongFormat), count, tr ("Number of days")) +
                          table2 (aq.indexes ());
  ui->m_temporalComment->setText (text);
}

void CTownDetails::updateDailyProfileComment (int sliderValue)
{
  QList<CSlider*> sliders = ui->m_dailyProfilSliders->findChildren<CSlider*> ();
  if (!sliders.isEmpty ())
  {
    QList<QPair<QColor, QColor>> colors = sliders.first ()->tickColors ();
    if (!colors.isEmpty ())
    {
      CAQLegend*            legend    = CAQLegend::gblInst ();
      QString               dayOfWeek = m_dayOfWeek[sliderValue];
      CAirQualities*        aqs       = CAirQualities::gblInst ();
      int                   count     = aqs->airQualityCount (m_town) / 7;
      CAirQuality::TIndexes indexes;
      for (int i = 0, count = sliders.size (); i < count; ++i)
      {
        QColor color = sliders[i]->tickColors ()[sliders[i]->value ()].second;
        indexes[i]   = legend->index (color.rgb ());
      }

      QString text = table1 (dayOfWeek, count, tr ("Number of weeks")) + table2 (indexes);
      ui->m_dailyProfilComment->setText (text);
    }
  }
}

void CTownDetails::backward (bool)
{
  auto slider = ui->m_temporalSliders->findChild<CSlider*> ();
  int  value  = slider->value () - 1;
  if (value < slider->minimum ())
  {
    value = slider->maximum ();
  }

  temporalSliderChangeValue (value);
}

void CTownDetails::forward (bool)
{
  auto slider = ui->m_temporalSliders->findChild<CSlider*> ();
  int  value  = slider->value () + 1;
  if (value > slider->maximum ())
  {
    value = slider->minimum ();
  }

  temporalSliderChangeValue (value);
}

static int greatestTimeGap (QList<CAirQuality> const & aqs, int value, CAirQuality::EIndexType index)
{
  int im       = -1;
  int deltaMax = -1;
  int start    = -1;
  int count    = aqs.size ();
  for (int i = 0; i < count; ++i)
  {
    int vi = aqs[i].index (index);
    if (vi == value)
    {
      if (start == -1)
      {
        start = i;
      }
    }
    else
    {
      if (start != -1)
      {
        int end   = i;
        int delta = end - start;
        if (delta > deltaMax)
        {
          deltaMax = delta;
          im       = (start + end) / 2;
        }
      }

      start = -1;
    }
  }

  if (im == -1)
  {
    im = start == -1 ? 0 : (count - start) / 2;
  }

  return im;
}

void CTownDetails::highest (bool)
{
  QList<CAirQuality> aqs = CAirQualities::gblInst ()->airQualities (m_town);
  if (!aqs.isEmpty ())
  {
    auto btn   = static_cast<QToolButton*>(sender ());
    auto index = static_cast<CAirQuality::EIndexType>(btn->objectName ().toInt ());
    int  value = aqs.first ().index (index);
    for (int i = 1, count = aqs.size (); i < count; ++i)
    {
      int vi = aqs[i].index (index);
      if (vi > value)
      {
        value = vi;
      }
    }

    int im = greatestTimeGap (aqs, value, index);
    temporalSliderChangeValue (im);
  }
}

void CTownDetails::lowest (bool)
{
  QList<CAirQuality> aqs = CAirQualities::gblInst ()->airQualities (m_town);
  if (!aqs.isEmpty ())
  {
    auto  btn   = static_cast<QToolButton*>(sender ());
    auto  index = static_cast<CAirQuality::EIndexType>(btn->objectName ().toInt ());
    int   value = aqs.first ().index (index);
    for (int i = 1, count = aqs.size (); i < count; ++i)
    {
      int vi = aqs[i].index (index);
      if (vi < value)
      {
        value = vi;
      }
    }

    int im = greatestTimeGap (aqs, value, index);
    temporalSliderChangeValue (im);
  }
}

