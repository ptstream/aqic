#ifndef GLOBALLEGEND_HPP
#define GLOBALLEGEND_HPP

#include "../town/aqlegend.hpp"
#include <QDialog>

namespace Ui {
  class CGlobalLegend;
}

class CGlobalLegend : public QDialog
{
  Q_OBJECT

public:
  explicit CGlobalLegend (QWidget* parent = nullptr);
  ~CGlobalLegend ();

private :
  void addRow (int pollutant, bool header);
  void linkActived (QString const & link);

private:
  Ui::CGlobalLegend* ui;
  CAQLegend const *  m_legend;
  QString            m_text;
};

#endif // GLOBALLEGEND_HPP
