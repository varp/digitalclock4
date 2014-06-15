#ifndef DIGITAL_CLOCK_GUI_PLUGIN_ITEM_DELEGATE_H
#define DIGITAL_CLOCK_GUI_PLUGIN_ITEM_DELEGATE_H

#include <QWidget>
#include <QModelIndex>
#include <QItemDelegate>
#include "../core/plugin_info.h"

namespace digital_clock {
namespace gui {

class ButtonsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ButtonsWidget(QWidget *parent = 0, const QModelIndex& index = QModelIndex());

  void SetPluginInfo(const TPluginInfo& info);

signals:
  void ConfigureRequested(const QString& name);

private slots:
  void Configure();
  void DisplayInfo();

private:
  TPluginInfo info_;
};


class PluginItemDelegate : public QItemDelegate {
  Q_OBJECT

public:
  explicit PluginItemDelegate(QObject* parent = 0);

  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex & index) const;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

  void setEditorData(QWidget* editor, const QModelIndex& index) const;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_PLUGIN_ITEM_DELEGATE_H
