#include "plugins_model.h"

namespace digital_clock {
namespace core {

PluginsModel::PluginsModel(QObject* parent) : QAbstractTableModel(parent) {
}

Qt::ItemFlags PluginsModel::flags(const QModelIndex& index) const {
  switch (index.column()) {
    case 0: return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    case 2: return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    default: return Qt::ItemIsEnabled;
  }
}

QVariant PluginsModel::data(const QModelIndex& index, int role) const {
  switch (index.column()) {
    case 0:
      switch (role) {
        case Qt::DisplayRole:
          return items_[index.row()].info.gui_info.display_name;

        case Qt::CheckStateRole:
          return items_[index.row()].loaded ? Qt::Checked : Qt::Unchecked;

        default:
          return QVariant();
      }
      break;

    case 1:
      switch (role) {
        case Qt::DisplayRole:
          return items_[index.row()].info.metadata[PI_VERSION];

        case Qt::ForegroundRole:
          return QVariant::fromValue<QColor>(Qt::darkGray);

        case Qt::TextAlignmentRole:
          return Qt::AlignCenter;

        default:
          return QVariant();
      }
      break;

    case 2:
      switch (role) {
        case Qt::EditRole:
          return items_[index.row()].configurable;

        case Qt::UserRole:
          return QVariant::fromValue(items_[index.row()].info);

        default:
          return QVariant();
      }
      break;

    default:
      return QVariant();
  }
}

QVariant PluginsModel::headerData(int section, Qt::Orientation orientation, int role) const {
  switch (orientation) {
    case Qt::Vertical:
      if (role == Qt::DisplayRole) return QString::number(section);
      break;

    case Qt::Horizontal:
      switch (section) {
        case 0: return tr("Plugin name");
        case 1: return tr("Version");
      }
      break;
  }
  return QVariant();
}

int PluginsModel::rowCount(const QModelIndex& /*parent*/) const {
  return items_.size();
}

int PluginsModel::columnCount(const QModelIndex& /*parent*/) const {
  return 3;
}

bool PluginsModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  switch (role) {
    case Qt::CheckStateRole:
      items_[index.row()].loaded = value.toInt() == Qt::Checked;
      emit dataChanged(index, index, QVector<int>(1, Qt::CheckStateRole));
      emit StateChanged(items_[index.row()].info.metadata[PI_NAME], items_[index.row()].loaded);
      return true;

    default:
      return QAbstractTableModel::setData(index, value, role);
  }
}

void PluginsModel::SetItems(const QVector<PluginItem>& items) {
  beginRemoveRows(QModelIndex(), 0, rowCount());
  items_.clear();
  endRemoveRows();
  beginInsertRows(QModelIndex(), 0, items.size());
  items_ = items;
  endInsertRows();
}

} // namespace core
} // namespace digital_clock
