#ifndef LEGEND_HPP
#define LEGEND_HPP

#include "../town/aqlegend.hpp"
#include <QDialog>
#include <QMap>

namespace Ui {
  class CLegend;
}

class CLegend : public QDialog
{
  Q_OBJECT
public:
  explicit CLegend (CAQLegend const * legend, CAQLegend::EIndex index, QWidget* parent = nullptr);
  ~CLegend ();

protected slots:
  void linkActived (QString const & link);
  void on_m_close_clicked (bool) { close (); };

private :
  void addColumn (CAQLegend::EPollutant pollutant, CAQLegend::EIndex index);

private:
  Ui::CLegend*      ui;
  CAQLegend const * m_legend;
  QString           m_text;
 };

#endif // LEGEND_HPP
