#ifndef PLUGIN_LIST_WIDGET_H
#define PLUGIN_LIST_WIDGET_H

#include <QWidget>
#include "../core/plugin_info.h"

namespace Ui {
class PluginListWidget;
}

class PluginListWidget : public QWidget {
  Q_OBJECT

public:
  explicit PluginListWidget(QWidget* parent = 0);
  ~PluginListWidget();

  QString GetName() const;

public slots:
  void SetName(const QString& name);
  void SetType(const QString& type);
  void SetVersion(const QString& version);
  void SetConfigurable(bool configable);
  void SetChecked(bool checked);
  void SetInfo(const TPluginInfo& info);

signals:
  void StateChanged(const QString& name, bool new_state);
  void ConfigureRequested(const QString& name);

private slots:
  void on_name_check_box_toggled(bool checked);
  void on_config_btn_clicked();
  void on_info_btn_clicked();

private:
  Ui::PluginListWidget* ui;
  TPluginInfo info_;
};

#endif // PLUGIN_LIST_WIDGET_H
