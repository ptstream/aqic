#ifndef TOWNDETAILS_HPP
#define TOWNDETAILS_HPP

#include "../town/airquality.hpp"
#include <QDialog>

class QGridLayout;

namespace Ui {
  class CTownDetails;
}

class CSlider;

class CTownDetails : public QDialog
{
  Q_OBJECT
public:
  using TColor  = QPair<QColor, QColor>;
  using TColors = QList<TColor>;

  explicit CTownDetails (QWidget* parent = nullptr);
  ~CTownDetails () override;

  void setLocation (double lat, double lon) { m_lat = lat; m_lon = lon; }
  void setInitialDate (QDate const & date);
  void setInitilDayOfWeek ();
  void setTown (QString const & code) { m_town = code; }
  void setTemporalColors (CAirQuality::EIndexType index, TColors const & colors);
  void setDailyProfileColors (CAirQuality::EIndexType index, TColors const & colors);
  void setDates (QList<QDate> const & dates);
  void setRegionCode (QString const & code) { m_regionCode = code; }
  void setRegionName (QString const & name) { m_regionName = name; }

protected slots:
  void tabChange (int index);
  void temporalSliderChangeValue (int value);
  void dailyProfileSliderChangeValue (int value);
  void backward (bool);
  void forward (bool);
  void highest (bool);
  void lowest (bool);

protected:
  void resizeEvent (QResizeEvent* event) override;

private:
  CSlider* createSlider(QString const & name, int index, QGridLayout* layout, QWidget* parent, bool createButton);
  void createTemporalDates (int index, QGridLayout* layout);
  void createDaysOfWeek (int index, QGridLayout* layout);
  void createProfilSliders ();
  void updateTemporalComment (int sliderValue);
  void updateDailyProfileComment (int sliderValue);
  void createTemporalWidgets ();
  void createDailyProfilWidgets ();
  QString table1 (QString const & date, int dayCount, QString const & number);
  QString table2 (CAirQuality::TIndexes const & indexes);

private:
  Ui::CTownDetails*      ui;
  QString                m_regionCode;
  QString                m_regionName;
  QString                m_town;
  QDate                  m_initialDate;
  double                 m_lat              = 0, m_lon = 0;
  int                    m_splitterSize     = 70; // %
  int                    m_widthFromParent  = 70; // %
  int                    m_heightFromParent = 50; // %
  std::array<QString, 7> m_dayOfWeek;
};

#endif // TOWNDETAILS_HPP
