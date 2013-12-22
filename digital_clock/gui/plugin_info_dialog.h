#ifndef PLUGIN_INFO_DIALOG_H
#define PLUGIN_INFO_DIALOG_H

#include "centered_dialog.h"
#include "../core/plugin_info.h"

namespace digital_clock {

namespace Ui {
class PluginInfoDialog;
}

class PluginInfoDialog : public CenteredDialog {
  Q_OBJECT

public:
  PluginInfoDialog(QWidget* parent = 0);
  ~PluginInfoDialog();

public slots:
  void SetInfo(const TPluginInfo& info);

private:
  Ui::PluginInfoDialog* ui;
};

} // namespace digital_clock

#endif // PLUGIN_INFO_DIALOG_H
