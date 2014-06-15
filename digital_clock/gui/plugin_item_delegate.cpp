#include <QHBoxLayout>
#include <QToolButton>
#include "../core/plugin_item.h"
#include "plugin_info_dialog.h"
#include "plugin_item_delegate.h"

namespace digital_clock {
namespace gui {

ButtonsWidget::ButtonsWidget(QWidget* parent, const QModelIndex& index) : QWidget(parent) {
  QHBoxLayout* l = new QHBoxLayout();
  l->setMargin(2);
  QToolButton* config_btn = new QToolButton(this);
  config_btn->setEnabled(index.data(Qt::EditRole).toBool());
  QToolButton* info_btn = new QToolButton(this);
  connect(info_btn, SIGNAL(clicked()), this, SLOT(DisplayInfo()));
  l->addWidget(config_btn);
  l->addWidget(info_btn);
  setLayout(l);
}

void ButtonsWidget::SetPluginInfo(const TPluginInfo& info) {
  info_ = info;
}

void ButtonsWidget::Configure() {
  emit ConfigureRequested(info_.metadata[PI_NAME]);
}

void ButtonsWidget::DisplayInfo() {
  PluginInfoDialog* dialog = new PluginInfoDialog(this);
  dialog->SetInfo(info_);
  dialog->show();
}


PluginItemDelegate::PluginItemDelegate(QObject* parent) : QItemDelegate(parent) {
}

QWidget* PluginItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                                          const QModelIndex& index) const {
  return new ButtonsWidget(parent, index);
}

QSize PluginItemDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const {
  return QSize(70, 30);
}

void PluginItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
  qobject_cast<ButtonsWidget*>(editor)->SetPluginInfo(index.data(Qt::UserRole).value<TPluginInfo>());
}

} // namespace gui
} // namespace digital_clock
