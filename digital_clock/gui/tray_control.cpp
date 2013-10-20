#include <QApplication>
#include "tray_control.h"

TrayControl::TrayControl(QObject* parent)
  : QObject(parent) {
  CreateActions();
  CreateTrayIcon();
}

QSystemTrayIcon*TrayControl::GetTrayIcon() {
  return tray_icon_;
}

void TrayControl::CreateActions() {
  settings_action_ = new QAction(QIcon(":/images/settings.svg"), tr("&Settings"), this);
  connect(settings_action_, SIGNAL(triggered()), this, SIGNAL(ShowSettingsDlg()));

  about_action_ = new QAction(QIcon(":/images/about.svg"), tr("&About"), this);
  connect(about_action_, SIGNAL(triggered()), this, SIGNAL(ShowAboutDlg()));

  exit_action_ = new QAction(QIcon(":/images/quit.svg"), tr("&Quit"), this);
  connect(exit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void TrayControl::CreateTrayIcon() {
  QMenu* tray_menu = new QMenu();
  tray_menu->addAction(settings_action_);
  tray_menu->addAction(about_action_);
  tray_menu->addSeparator();
  tray_menu->addAction(exit_action_);

  tray_icon_ = new QSystemTrayIcon(this);
  tray_icon_->setContextMenu(tray_menu);
  tray_icon_->setIcon(QIcon(":/images/clock.svg"));
  tray_icon_->setToolTip(qApp->applicationDisplayName() + " " + qApp->applicationVersion());
  tray_icon_->show();
}