#include "plugin_list_widget.h"
#include "ui_plugin_list_widget.h"

namespace digital_clock {

PluginListWidget::PluginListWidget(QWidget* parent)
  : QWidget(parent), ui(new Ui::PluginListWidget) {
  ui->setupUi(this);
}

PluginListWidget::~PluginListWidget() {
  delete ui;
}

QString PluginListWidget::GetName() const {
  return ui->name_check_box->text();
}

void PluginListWidget::SetName(const QString& name) {
  ui->name_check_box->setText(name);
}

void PluginListWidget::SetVersion(const QString& version) {
  ui->version_label->setText(version);
}

void PluginListWidget::SetConfigurable(bool configable) {
  ui->config_btn->setEnabled(configable);
}

void PluginListWidget::SetChecked(bool checked) {
  ui->name_check_box->setChecked(checked);
}

void PluginListWidget::SetInfo(const TPluginInfo& info) {
  info_ = info;
  SetName(info[PI_NAME]);
  SetVersion(info[PI_VERSION]);
}

void PluginListWidget::on_name_check_box_toggled(bool checked) {
  emit StateChanged(ui->name_check_box->text(), checked);
}

void PluginListWidget::on_config_btn_clicked() {
  emit ConfigureRequested(ui->name_check_box->text());
}

void PluginListWidget::on_info_btn_clicked() {
  if (dialog_) {
    dialog_->activateWindow();
  } else {
    dialog_ = new PluginInfoDialog(this);
    dialog_->SetInfo(info_);
    dialog_->show();
  }
}

} // namespace digital_clock
