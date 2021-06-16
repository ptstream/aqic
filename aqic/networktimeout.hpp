#ifndef NETWORKTIMEOUT_HPP
#define NETWORKTIMEOUT_HPP

#include <QDialog>

namespace Ui {
  class CNetworkTimeout;
}

class CNetworkTimeout : public QDialog
{
  Q_OBJECT

public:
  explicit CNetworkTimeout(int ms, QWidget* parent = nullptr);
  ~CNetworkTimeout ();

private:
  Ui::CNetworkTimeout* ui;
};

#endif // NETWORKTIMEOUT_HPP
