/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tray_control.h"

#include <QMenu>
#include <QIcon>
#include <QApplication>
#include <QSettings>

#include "logger.h"

CLOCK_DECLARE_LOGGING_CATEGORY(clock_gui_widgets)

namespace digital_clock {
namespace gui {

TrayControl::TrayControl(QWidget* parent) : QObject(parent)
{
  cTraceFunction(clock_gui_widgets);
  QMenu* tray_menu = new QMenu(parent);
  // *INDENT-OFF*
  show_hide_action_ = tray_menu->addAction(QIcon(":/clock/images/clock.svg"), tr("&Hide Clock"),
                                           this, SLOT(ShowHideHandler()));
  // *INDENT-ON*
  show_hide_action_->setData(false);
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/settings.svg"), tr("&Settings"),
                       this, SIGNAL(ShowSettingsDlg()));
  tray_menu->addAction(QIcon(":/clock/images/about.svg"), tr("&About"),
                       this, SIGNAL(ShowAboutDlg()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/update.svg"), tr("&Update"),
                       this, SIGNAL(CheckForUpdates()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/quit.svg"), tr("&Quit"),
                       this, SIGNAL(AppExit()));
#ifdef Q_OS_MACOS
  // don't show menu icons on macOS
  QList<QAction*> menu_actions = tray_menu->actions();
  for (auto& a : menu_actions) a->setIconVisibleInMenu(false);
#endif

  tray_icon_ = new QSystemTrayIcon(QApplication::windowIcon(), this);
  UpdateTrayIcon();
  tray_icon_->setVisible(true);
  tray_icon_->setContextMenu(tray_menu);
  tray_icon_->setToolTip(QApplication::applicationDisplayName() + " " + QApplication::applicationVersion());
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayEventHandler(QSystemTrayIcon::ActivationReason)));
}

TrayControl::~TrayControl()
{
  cTraceFunction(clock_gui_widgets);
}

QSystemTrayIcon* TrayControl::GetTrayIcon() const
{
  cTraceFunction(clock_gui_widgets);
  return tray_icon_;
}

QAction* TrayControl::GetShowHideAction() const
{
  cTraceFunction(clock_gui_widgets);
  return show_hide_action_;
}

void TrayControl::UpdateTrayIcon()
{
  cTraceSlot(clock_gui_widgets);
#ifdef Q_OS_MACOS
  // macOS dark panel support
  QSettings s;
  QString curr_mode = s.value("AppleInterfaceStyle").toString();
  if (curr_mode == last_mode_) return;

  if (curr_mode == QString("Dark")) {
    tray_icon_->setIcon(QIcon(":/clock/images/clock-light.svg"));
  } else {
    tray_icon_->setIcon(QIcon(":/clock/images/clock.svg"));
  }

  last_mode_ = curr_mode;
#endif
}

void TrayControl::TrayEventHandler(QSystemTrayIcon::ActivationReason reason)
{
  cTraceSlot(clock_gui_widgets);
  if (reason == QSystemTrayIcon::DoubleClick) emit ShowSettingsDlg();
}

void TrayControl::ShowHideHandler()
{
  cTraceSlot(clock_gui_widgets);
  Q_ASSERT(show_hide_action_);
  bool widget_will_visible = show_hide_action_->data().toBool();
  if (widget_will_visible) {
    show_hide_action_->setText(tr("&Hide Clock"));
  } else {
    show_hide_action_->setText(tr("S&how Clock"));
  }
  show_hide_action_->setData(!widget_will_visible);
  emit VisibilityChanged(widget_will_visible);
}

} // namespace gui
} // namespace digital_clock
