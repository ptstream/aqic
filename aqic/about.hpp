#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <QDialog>

namespace Ui {
  class CAbout;
}

class QTreeWidgetItem;

class CAbout : public QDialog
{
  Q_OBJECT
public:
  explicit CAbout (QWidget* parent = nullptr);
  ~CAbout ();

protected slots:
  void on_m_qt_clicked (bool);
  void on_m_atmoFrance_clicked (bool);
  void on_m_regions_itemClicked (QTreeWidgetItem* item, int);
  void on_m_map_itemClicked (QTreeWidgetItem* item, int);

private:
  Ui::CAbout* ui;
};

#endif // ABOUT_HPP
