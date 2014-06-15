#ifndef PLUGIN_ITEM_H
#define PLUGIN_ITEM_H

#include "plugin_info.h"

namespace digital_clock {
namespace core {

struct PluginItem {
  TPluginInfo info;
  bool configurable;
  bool loaded;
};

} // namespace core
} // namespace digital_clock

Q_DECLARE_METATYPE(TPluginGUIInfo)
Q_DECLARE_METATYPE(FPluginInfo)
Q_DECLARE_METATYPE(TPluginInfo)

#endif // PLUGIN_ITEM_H
