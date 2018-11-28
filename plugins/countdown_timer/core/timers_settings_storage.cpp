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

#include "timers_settings_storage.h"

#include "settings_storage.h"

#include "settings.h"

namespace countdown_timer {

TimerSettings::TimerSettings() : m_target_time(QDate(QDate::currentDate().year() + 1, 1, 1))
{
}

void TimerSettings::getInterval(int* hours, int* minutes, int* seconds) const
{
  *hours = m_hours;
  *minutes = m_minutes;
  *seconds = m_seconds;
}

void TimerSettings::setInterval(int hours, int minutes, int seconds)
{
  m_hours = hours;
  m_minutes = minutes;
  m_seconds = seconds;
}

TimersSettingsStorage::TimersSettingsStorage(SettingsStorage* backend, const QString& name, QObject* parent)
  : SettingsStorageWrapper(backend, parent)
  , key_prefix_(QString("pugins/%1/timers").arg(name))
{
}

int TimersSettingsStorage::addTimer(const TimerSettings& item)
{
  int id = (items_.isEmpty() ? 0 : items_.lastKey()) + 1;
  items_[id] = item;
  return id;
}

void TimersSettingsStorage::removeTimer(int id)
{
  if (items_.remove(id) > 0)
    this->remove(QString("%1/%2").arg(key_prefix_).arg(id));
}

void TimersSettingsStorage::updateTimer(int id, const TimerSettings& updated)
{
  auto iter = items_.find(id);
  if (iter != items_.end()) *iter = updated;
}

const TimerSettings& TimersSettingsStorage::timerSettings(int id) const
{
  auto iter = items_.find(id);
  return iter != items_.end() ? iter.value() : default_timer_;
}

void TimersSettingsStorage::load()
{
  items_.clear();
  QStringList items_keys = this->GetBackend()->ListChildren(key_prefix_);
  for (auto& key : qAsConst(items_keys)) {
    bool id_ok = false;
    int id = key.toInt(&id_ok);
    if (!id_ok) continue;
    readItem(id, &items_[id]);
  }

  if (items_.isEmpty()) addTimer(default_timer_);
}

void TimersSettingsStorage::save()
{
  for (auto iter = items_.begin(); iter != items_.end(); ++iter)
    writeItem(iter.key(), iter.value());
}

void TimersSettingsStorage::readItem(int id, TimerSettings* item)
{
  Q_ASSERT(item);

  QString item_key = QString("%1/%2").arg(key_prefix_).arg(id);

  item->setInterval(
        this->getValue(QString("%1/%2").arg(item_key, OPT_INTERVAL_HOURS)).toInt(),
        this->getValue(QString("%1/%2").arg(item_key, OPT_INTERVAL_MINUTES)).toInt(),
        this->getValue(QString("%1/%2").arg(item_key, OPT_INTERVAL_SECONDS)).toInt());
  QString time_str = this->getValue(QString("%1/%2").arg(item_key, OPT_TARGET_DATETIME)).toString();
  item->setTargetTime(QDateTime::fromString(time_str, "yyyy-MM-dd HH:mm:ss"));
  item->setUseTargetTime(this->getValue(QString("%1/%2").arg(item_key, OPT_USE_TARGET_TIME)).toBool());
  item->setShowMessage(this->getValue(QString("%1/%2").arg(item_key, OPT_SHOW_MESSAGE)).toBool());
  item->setMessage(this->getValue(QString("%1/%2").arg(item_key, OPT_MESSAGE_TEXT)).toString());
  item->setWidgetVisible(this->getValue(QString("%1/%2").arg(item_key, OPT_WIDGET_IS_VISIBLE)).toBool());
}

void TimersSettingsStorage::writeItem(int id, const TimerSettings& item)
{
  QString item_key = QString("%1/%2").arg(key_prefix_).arg(id);

  int hours, minutes, seconds;
  item.getInterval(&hours, &minutes, &seconds);

  this->setValue(QString("%1/%2").arg(item_key, OPT_INTERVAL_HOURS), hours);
  this->setValue(QString("%1/%2").arg(item_key, OPT_INTERVAL_MINUTES), minutes);
  this->setValue(QString("%1/%2").arg(item_key, OPT_INTERVAL_SECONDS), seconds);
  this->setValue(QString("%1/%2").arg(item_key, OPT_TARGET_DATETIME), item.targetTime().toString("yyyy-MM-dd HH:mm:ss"));
  this->setValue(QString("%1/%2").arg(item_key, OPT_USE_TARGET_TIME), item.useTargetTime());
  this->setValue(QString("%1/%2").arg(item_key, OPT_SHOW_MESSAGE), item.showMessage());
  this->setValue(QString("%1/%2").arg(item_key, OPT_MESSAGE_TEXT), item.message());
  this->setValue(QString("%1/%2").arg(item_key, OPT_WIDGET_IS_VISIBLE), item.isWidgetVisible());
}

} // namespace countdown_timer
