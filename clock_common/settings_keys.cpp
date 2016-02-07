#include <QFont>
#include <QColor>
#include <QStringList>
#include <QLocale>
#include "settings_keys.h"

QVariant GetDefaultValue(Option opt) {
  switch (opt) {
    // clock settings
    case OPT_OPACITY:               return 0.75;
    case OPT_STAY_ON_TOP:           return true;
    case OPT_TRANSP_FOR_INPUT:      return false;
    case OPT_SEPARATOR_FLASH:       return true;
    case OPT_PLUGINS:               return QStringList("Test plugin");
    case OPT_TIME_FORMAT:
    {
      QString sys_time_format = QLocale::system().timeFormat();
      int sep_pos = sys_time_format.indexOf(':');
      QString time_format = sys_time_format.mid(0, sys_time_format.indexOf(':', sep_pos + 1));

      if (sys_time_format.contains('A', Qt::CaseInsensitive)) {
        time_format += 'A';
      }
      return time_format;
    }
    case OPT_ALIGNMENT:             return static_cast<int>(CAlignment::A_LEFT);
    // skin settings
    case OPT_SKIN_NAME:             return QString("Electronic (default)");
    case OPT_FONT:                  return QFont();
    case OPT_ZOOM:                  return 1.25;
    case OPT_COLOR:                 return QVariant::fromValue<QColor>(Qt::blue);
    case OPT_TEXTURE:               return QString();
    case OPT_TEXTURE_TYPE:          return 1;
    case OPT_TEXTURE_PER_ELEMENT:   return false;
    case OPT_TEXTURE_DRAW_MODE:     return 0;
    case OPT_CUSTOMIZATION:         return 1;
    case OPT_SPACING:               return 4;
    case OPT_COLORIZE_COLOR:        return QVariant::fromValue<QColor>(Qt::darkCyan);
    case OPT_COLORIZE_LEVEL:        return 0.8;
    // updater settings
    case OPT_USE_AUTOUPDATE:        return true;
    case OPT_UPDATE_PERIOD:         return 3;
    case OPT_CHECK_FOR_BETA:        return false;
    default:                        return QVariant();
  }
}
