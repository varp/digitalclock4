/*
    Digital Clock: clock common library
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

#ifndef CLOCK_LOGGER_H
#define CLOCK_LOGGER_H

#include <QMessageLogger>
#include <QLoggingCategory>

#define CLOCK_LOGGING_CATEGORY(name, category) \
  Q_LOGGING_CATEGORY(name, category) \
  Q_LOGGING_CATEGORY(_trace_##name, category".trace")

#define CLOCK_DECLARE_LOGGING_CATEGORY(name) \
  Q_DECLARE_LOGGING_CATEGORY(name) \
  Q_DECLARE_LOGGING_CATEGORY(_trace_##name)


class AutoTracer
{
public:
  AutoTracer(QMessageLogger::CategoryFunction cat, const char* fn_name, QObject* sender) :
    cat_(cat), fn_name_(fn_name)
  {
    if (sender) {
      qCDebug(cat_) << fn_name_ << "starts. sender:" << sender;
    } else {
      qCDebug(cat_) << fn_name_ << "starts.";
    }
  }

  ~AutoTracer()
  {
    qCDebug(cat_) << fn_name_ << "ends.";
  }

private:
  QMessageLogger::CategoryFunction cat_;
  const char* fn_name_;
};

#define cTraceFunction(log_cat)   AutoTracer tracer_var(_trace_##log_cat, Q_FUNC_INFO, nullptr)
#define cTraceSlot(log_cat)       AutoTracer tracer_var(_trace_##log_cat, Q_FUNC_INFO, sender())

#endif // CLOCK_LOGGER_H
