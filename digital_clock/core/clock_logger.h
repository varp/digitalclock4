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

#ifndef DIGITAL_CLOCK_CORE_CLOCK_LOGGER_H
#define DIGITAL_CLOCK_CORE_CLOCK_LOGGER_H

#include <QFile>
#include <QString>
#include <QVariant>

namespace digital_clock {
namespace core {

class Logger
{
public:
  enum LogLevel {
    Disabled,
    VeryBasic,
    BasicDebug,
    FullDebug,
    ExtraDebug,
    CustomFilter
  };

  enum LoggerOption {
    loLogLevel,
    loCustomFilter,
    loLogDir,
    loDeleteOldLogs,
    loOldLogsCount
  };

  static QVariant value(LoggerOption opt);
  static void setValue(LoggerOption opt, const QVariant& value);

  static void init();

private:
  static void logFileOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

  static QString key(LoggerOption opt);
  static QVariant defaultValue(LoggerOption opt);

  static QFile log_file_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_LOGGER_H
