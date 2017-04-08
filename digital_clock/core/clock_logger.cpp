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

#include <QDateTime>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QLoggingCategory>

namespace digital_clock {
namespace core {

QFile Logger::log_file_;

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

void Logger::init()
{
  QSettings s;
  // create log dir
  QString log_path = s.value(key(loLogDir), defaultValue(loLogDir)).toString();
  Q_ASSERT(!log_path.isEmpty());
  QDir log_dir = QDir::home();
  if (!log_dir.mkpath(log_path)) return;
  log_dir.cd(log_path);

  // delete old logs if required
  if (s.value(key(loDeleteOldLogs), defaultValue(loDeleteOldLogs)).toBool()) {
    int max_logs = s.value(key(loOldLogsCount), defaultValue(loOldLogsCount)).toInt();
    QStringList log_files = log_dir.entryList(QDir::Files, QDir::Name);
    while (log_files.size() > max_logs) {
      QFile::remove(log_dir.absoluteFilePath(log_files.first()));
      log_files.pop_front();
    }
  }

  // set log filters
  LogLevel log_level = static_cast<LogLevel>(s.value(key(loLogLevel), defaultValue(loLogLevel)).toInt());
  switch (log_level) {
    case Disabled:
      QLoggingCategory::setFilterRules("clock.*=false");
      break;

    case VeryBasic:
      QLoggingCategory::setFilterRules("clock.*.debug=false");
      break;

    case BasicDebug:
      QLoggingCategory::setFilterRules("*.trace.debug=false");
      break;

    case FullDebug:
      QLoggingCategory::setFilterRules("*.timer.trace.debug=false");
      break;

    case ExtraDebug:
      QLoggingCategory::setFilterRules("clock.*=true");
      break;

    case CustomFilter:
      QLoggingCategory::setFilterRules(s.value(key(loCustomFilter), defaultValue(loCustomFilter)).toStringList().join('\n'));
      break;
  }

  if (log_level == Disabled) return;

  // create log file
  QDateTime now = QDateTime::currentDateTime();
  QString filename = QString("digital_clock_%1.log").arg(now.toString("dd-MM-yyyy_HH-mm-ss"));
  log_file_.setFileName(log_dir.absoluteFilePath(filename));
  if (!log_file_.open(QIODevice::WriteOnly | QIODevice::Text)) return;

  qSetMessagePattern("[%{type}] %{if-category}%{category}: %{endif}%{message}");
  qInstallMessageHandler(&Logger::logFileOutput);
}

void Logger::logFileOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
  if (!log_file_.isOpen()) return;

  QString data = qFormatLogMessage(type, context, msg) + '\n';
  log_file_.write(data.toUtf8());
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
    case loCustomFilter:  return QStringList() << "*.timer.trace=false" << "clock.core.config.trace=false";
    case loLogDir:        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs";
    case loDeleteOldLogs: return true;
    case loOldLogsCount:  return 3;
  }
  return QVariant();
}

} // namespace core
} // namespace digital_clock
