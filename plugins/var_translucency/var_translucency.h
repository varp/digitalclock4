/*
    Digital Clock: variable translucency plugin
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef VAR_TRANSLUCENCY_VAR_TRANSLUCENCY_H
#define VAR_TRANSLUCENCY_VAR_TRANSLUCENCY_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

namespace var_translucency {

class VarTranslucency : public ISettingsPlugin, public ISettingsPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "var_translucency.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin ISettingsPluginInit)

public:
  VarTranslucency();
  void Init(const QMap<Option, QVariant>& current_settings);

public slots:
  void Start();
  void Stop();
  void TimeUpdateListener();

private:
  qreal old_opacity_;
  qreal cur_opacity_;
  qreal op_stap_;
};

} // namespace var_translucency

#endif // VAR_TRANSLUCENCY_VAR_TRANSLUCENCY_H
