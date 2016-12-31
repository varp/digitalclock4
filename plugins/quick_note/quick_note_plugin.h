/*
    Digital Clock: quick note plugin
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

#ifndef QUICK_NOTE_QUICK_NOTE_PLUGIN_H
#define QUICK_NOTE_QUICK_NOTE_PLUGIN_H

#include "widget_plugin_base.h"

class QGridLayout;

namespace quick_note {

class MessageWidget;

class QuickNotePlugin : public ::plugin::WidgetPluginBase
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "quick_note.json")

public:
  QuickNotePlugin();

  void Init(const QMap<Option, QVariant>& current_settings);

public slots:
  void Configure();

  void SettingsListener(Option option, const QVariant& new_value);

protected:
  void InitSettingsDefaults(QSettings::SettingsMap* defaults);
  QWidget* InitWidget(QGridLayout* layout);
  void DisplayImage(const QImage& image);
  QString GetWidgetText();

private slots:
  void SettingsUpdateListener(const QString& key, const QVariant& value);

private:
  qreal CalculateZoom(const QString& text) const;

  MessageWidget* msg_widget_;
  QColor msg_color_;
};

} // namespace quick_note

#endif // QUICK_NOTE_QUICK_NOTE_PLUGIN_H
