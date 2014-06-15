#ifndef DIGITAL_CLOCK_CORE_PLUGINS_MODEL_H
#define DIGITAL_CLOCK_CORE_PLUGINS_MODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "plugin_item.h"

namespace digital_clock {
namespace core {

class PluginsModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit PluginsModel(QObject* parent = 0);

  Qt::ItemFlags flags(const QModelIndex& index) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;

  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

signals:
  void StateChanged(const QString& name, bool checked);

public slots:
  void SetItems(const QVector<PluginItem>& items);

private:
  QVector<PluginItem> items_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_PLUGINS_MODEL_H
