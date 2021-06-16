#ifndef MAGGLASS_HPP
#define MAGGLASS_HPP

#include <QDialog>

class CSlider;

class CMagGlass : public QDialog
{
  Q_OBJECT
public:
  CMagGlass (CSlider* slider, int value, QWidget* parent = nullptr);
  ~CMagGlass ();

protected:
  bool event (QEvent* e) override;
  void resizeEvent (QResizeEvent* event) override;

  QWidget* mainWindow () const;

protected:
  int m_width    = 400;
  int m_height   = 32;
  int m_interval = 16;
};

#endif // MAGGLASS_HPP
