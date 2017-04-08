/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clock_logger.h"

#include <QSettings>
#include <QStandardPaths>

namespace digital_clock {
namespace core {

QVariant Logger::value(Logger::LoggerOption opt)
{
  QSettings s;
  return s.value(key(opt), defaultValue(opt));
}

void Logger::setValue(Logger::LoggerOption opt, const QVariant& value)
{
  QSettings s;
  s.setValue(key(opt), value);
}

QString Logger::key(LoggerOption opt)
{
  switch (opt) {
    case loLogLevel:      return QString("logger/log_level");
    case loCustomFilter:  return QString("logger/custom_filter");
    case loLogDir:        return QString("logger/log_dir");
    case loDeleteOldLogs: return QString("logger/delete_old_logs");
    case loOldLogsCount:  return QString("logger/old_logs_count");
  }
  Q_ASSERT(false);
  return QString();
}

QVariant Logger::defaultValue(LoggerOption opt)
{
  switch (opt) {
    case loLogLevel:      return static_cast<int>(Disabled);
    case loCustomFilter:  return QStringList();
    case loLogDir:        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs";
    case loDeleteOldLogs: return true;
    case loOldLogsCount:  return 3;
  }
  return QVariant();
}

} // namespace core
} // namespace digital_clock
