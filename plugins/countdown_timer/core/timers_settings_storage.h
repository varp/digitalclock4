/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef COUNTDOWN_TIMER_TIMERS_SETTINGS_STORAGE_H
#define COUNTDOWN_TIMER_TIMERS_SETTINGS_STORAGE_H

#include "settings_storage_wrapper.h"

#include <QDateTime>
#include <QMap>

namespace countdown_timer {

class TimerSettings
{
public:
  TimerSettings();

  void getInterval(int* hours, int* minutes, int* seconds) const;
  void setInterval(int hours, int minutes, int seconds);

  bool useTargetTime() const { return m_use_target_time; }
  void setUseTargetTime(bool use) { m_use_target_time = use; }

  const QDateTime& targetTime() const { return m_target_time; }
  void setTargetTime(const QDateTime& tt) { m_target_time = tt; }

  bool showMessage() const { return m_show_message; }
  void setShowMessage(bool show) { m_show_message = show; }

  const QString& message() const { return m_message; }
  void setMessage(const QString& message) { m_message = message; }

  bool isWidgetVisible() const { return m_widget_is_visible; }
  void setWidgetVisible(bool visible) { m_widget_is_visible = visible; }

private:
  int m_hours = 0;
  int m_minutes = 0;
  int m_seconds = 0;
  bool m_use_target_time = true;
  bool m_show_message = false;
  bool m_widget_is_visible = true;
  QString m_message;
  QDateTime m_target_time;
};

class TimersSettingsStorage : public SettingsStorageWrapper
{
  Q_OBJECT
public:
  explicit TimersSettingsStorage(SettingsStorage* backend, const QString& name, QObject* parent = nullptr);

  int addTimer(const TimerSettings& item);
  void removeTimer(int id);
  void updateTimer(int id, const TimerSettings& updated);

  QList<int> knownIds() const { return items_.keys(); }

  const TimerSettings& timerSettings(int id) const;

public slots:
  void load();
  void save();

private:
  void readItem(int id, TimerSettings* item);
  void writeItem(int id, const TimerSettings& item);

  const QString key_prefix_;
  const TimerSettings default_timer_;
  QMap<int, TimerSettings> items_;
};

} // namespace countdown_timer

#endif // COUNTDOWN_TIMER_TIMERS_SETTINGS_STORAGE_H
