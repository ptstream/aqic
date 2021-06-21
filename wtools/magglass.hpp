#ifndef MAGGLASS_HPP
#define MAGGLASS_HPP

#include <QDialog>

class CSlider;

/*! \brief This class shows a part of CSlider with magnification. */
class CMagGlass : public QDialog
{
  Q_OBJECT
public:
  /*! Constructor.
   *  \param slider: CSlider to apply magnification
   *  \param parent: Parent widget
   */
  CMagGlass (CSlider* slider, int value, QWidget* parent = nullptr);

  /*! Destructor. */
  ~CMagGlass ();

protected:
  /*! Event handler. Use only when this widget is not activate. */
  bool event (QEvent* e) override;

  /*! Handler resize event. */
  void resizeEvent (QResizeEvent* event) override;

  /*! Returns the main window or nullptr if parent is null. */
  QWidget* mainWindow () const;

protected:
  int m_width    = 400;
  int m_height   = 32;
  int m_interval = 16;
};

#endif // MAGGLASS_HPP
