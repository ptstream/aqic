#include "networktimeout.hpp"
#include "ui_networktimeout.h"
#include "../qtools/globals.hpp"
#include <QStyle>

CNetworkTimeout::CNetworkTimeout (int ms, QWidget* parent) :  QDialog (parent),  ui (new Ui::CNetworkTimeout)
{
  ui->setupUi (this);
  setWindowFlag (Qt::WindowContextHelpButtonHint, false);
  ui->m_timeout->setText (::elapsedTimeToString (ms));
  ui->m_newTimeout->setText (::elapsedTimeToString (ms * 2));
  QIcon   icon   = style ()->standardIcon (QStyle::SP_MessageBoxWarning);
  QPixmap pixmap = icon.pixmap (QSize (64, 64));
  ui->m_warning->setPixmap (pixmap);
}

CNetworkTimeout::~CNetworkTimeout ()
{
  delete ui;
}

