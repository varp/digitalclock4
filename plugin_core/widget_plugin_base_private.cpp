/*
    Digital Clock: plugin core library
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "widget_plugin_base_private.h"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "plugin_settings.h"

#include "text_skin.h"

#include "widget_plugin_base.h"

namespace plugin {

WidgetPluginBasePrivate::WidgetPluginBasePrivate(WidgetPluginBase* obj, QObject* parent) :
  QObject(parent),
  main_layout_(nullptr), main_wnd_(nullptr),
  clock_customization_(::skin_draw::SkinDrawer::CT_COLOR),
  clock_color_(0, 170, 255),
  last_text_("-----"),
  drawer_(new ::skin_draw::SkinDrawer(this)),
  obj_(obj)
{
}

void WidgetPluginBasePrivate::InitBaseSettingsDefaults(QSettings::SettingsMap* defaults)
{
  QMap<WidgetPluginOption, QVariant> base_defaults;
  InitDefaults(&base_defaults);
  for (auto iter = base_defaults.begin(); iter != base_defaults.end(); ++iter)
    defaults->insert(OptionKey(iter.key(), obj_->plg_name_), iter.value());
}

int WidgetPluginBasePrivate::CalculateAvailableSpace() const
{
  int w_loc = obj_->settings_->GetOption(OptionKey(OPT_WIDGET_LOCATION, obj_->plg_name_)).toInt();
  if (static_cast<WidgetLocation>(w_loc) == WidgetLocation::WL_RIGHT) {
    return main_layout_->itemAtPosition(0, 0)->sizeHint().height();
  } else {
    return main_layout_->itemAtPosition(0, 0)->sizeHint().width();
  }
}

void WidgetPluginBasePrivate::onBaseOptionChanged(const WidgetPluginOption opt, const QVariant& value)
{
  obj_->settings_->SetOption(OptionKey(opt, obj_->plg_name_), value);
}

void WidgetPluginBasePrivate::SettingsChangeListener(const QString& key, const QVariant& value)
{
  if (!plg_widget_) return;  // not started

  if (key == OptionKey(OPT_USE_CLOCK_FONT, obj_->plg_name_)) {
    QString font_key = OptionKey(OPT_CUSTOM_FONT, obj_->plg_name_);
    SettingsChangeListener(font_key, value.toBool() ?
                           clock_font_ : obj_->settings_->GetOption(font_key).value<QFont>());
  }
  if (key == OptionKey(OPT_CUSTOM_FONT, obj_->plg_name_)) {
    font_ = value.value<QFont>();
    if (!obj_->settings_->GetOption(OptionKey(OPT_USE_CLOCK_SKIN, obj_->plg_name_)).toBool())
        ApplySkin(CreateTextSkin(font_));
  }
  if (key == OptionKey(OPT_ZOOM_MODE, obj_->plg_name_)) {
    switch (static_cast<ZoomMode>(value.toInt())) {
      case ZoomMode::ZM_NOT_ZOOM:
        drawer_->SetZoom(1.0);
        break;

      case ZoomMode::ZM_AUTOSIZE:
        obj_->avail_width_ = CalculateAvailableSpace();
        drawer_->SetZoom(obj_->CalculateZoom(last_text_));
        break;
    }
  }
  if (key == OptionKey(OPT_SPACE_PERCENT, obj_->plg_name_)) {
    int c_zoom_mode = obj_->settings_->GetOption(OptionKey(OPT_ZOOM_MODE, obj_->plg_name_)).toInt();
    if (static_cast<ZoomMode>(c_zoom_mode) == ZoomMode::ZM_AUTOSIZE) {
      drawer_->SetZoom(obj_->CalculateZoom(last_text_));
    }
  }
  if (key == OptionKey(OPT_WIDGET_LOCATION, obj_->plg_name_)) {
    main_layout_->removeWidget(plg_widget_);
    switch (static_cast<WidgetLocation>(value.toInt())) {
      case WidgetLocation::WL_BOTTOM:
        obj_->avail_width_ = main_layout_->itemAtPosition(0, 0)->sizeHint().width();
        main_layout_->addWidget(plg_widget_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
        break;

      case WidgetLocation::WL_RIGHT:
        obj_->avail_width_ = main_layout_->itemAtPosition(0, 0)->sizeHint().height();
        main_layout_->addWidget(plg_widget_, 0, main_layout_->columnCount(), 1, 1);
        break;
    }

    int c_zoom_mode = obj_->settings_->GetOption(OptionKey(OPT_ZOOM_MODE, obj_->plg_name_)).toInt();
    if (static_cast<ZoomMode>(c_zoom_mode) == ZoomMode::ZM_AUTOSIZE) {
      drawer_->SetZoom(obj_->CalculateZoom(last_text_));
    }
  }
  if (key == OptionKey(OPT_ALIGNMENT, obj_->plg_name_)) {
    QLabel* lbl = qobject_cast<QLabel*>(plg_widget_.data());
    if (lbl) lbl->setAlignment(static_cast<Qt::Alignment>(value.toInt()));
  }
  if (key == OptionKey(OPT_USE_CUSTOM_COLOR, obj_->plg_name_)) {
    drawer_->SetString(QString());    // set empty string to do not redraw twice
    if (value.toBool()) {
      drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_COLOR);
      drawer_->SetColor(obj_->settings_->GetOption(OptionKey(OPT_CUSTOM_COLOR, obj_->plg_name_)).value<QColor>());
    } else {
      drawer_->SetCustomizationType(clock_customization_);
      drawer_->SetColor(clock_color_);
    }
    if (last_text_ != QString("-")) drawer_->SetString(last_text_);
  }
  if (key == OptionKey(OPT_CUSTOM_COLOR, obj_->plg_name_)) {
    if (obj_->settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR, obj_->plg_name_)).toBool()) {
      drawer_->SetColor(value.value<QColor>());
    }
  }
  if (key == OptionKey(OPT_USE_CLOCK_SKIN, obj_->plg_name_)) {
    ApplySkin(value.toBool() ? clock_skin_ : CreateTextSkin(font_));
  }
}

skin_draw::ISkin::SkinPtr WidgetPluginBasePrivate::CreateTextSkin(const QFont &fnt)
{
    skin_draw::ISkin::SkinPtr txt_skin(new ::skin_draw::TextSkin(fnt));
    txt_skin->SetDevicePixelRatio(main_wnd_->devicePixelRatioF());
    return txt_skin;
}

void WidgetPluginBasePrivate::ApplySkin(skin_draw::ISkin::SkinPtr skin)
{
    drawer_->SetString(QString());
    drawer_->ApplySkin(skin);
    last_text_ = "-";             // reset last date to recalculate zoom
    obj_->TimeUpdateListener();   // force redraw
}

} // namespace plugin
