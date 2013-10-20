#include <QDesktopWidget>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include "../alarm_settings.h"
#include "settings_dlg.h"
#include "ui_settings_dlg.h"

SettingsDlg::SettingsDlg(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDlg) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/settings.svg"));
}

SettingsDlg::~SettingsDlg() {
  delete ui;
}

void SettingsDlg::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_ENABLED) {
    ui->alarm_enabled->setChecked(value.toBool());
  }
  if (key == OPT_TIME) {
    ui->time_edit->setTime(value.value<QTime>());
  }
  if (key == OPT_SIGNAL_TYPE) {
    SignalType st = (SignalType)value.toInt();
    ui->st_file->setChecked(st == ST_FILE);
    ui->st_stream->setChecked(st == ST_STREAM);
  }
  if (key == OPT_FILENAME) {
    QString file = value.toString();
    QFileInfo info(file);
    last_file_path_ = file.isEmpty() ? "." : info.absolutePath();
    if (info.exists()) {
      ui->signal_label->setText(info.baseName());
      ui->signal_label->setToolTip(QDir::toNativeSeparators(file));
    }
  }
  if (key == OPT_STREAM_URL) {
    QString stream = value.toString();
    ui->stream_url_edit->setText(stream);
    ui->stream_url_edit->setToolTip(stream);
  }
  if (key == OPT_SHOW_NOTIFY) {
    ui->notification_enabled->setChecked(value.toBool());
  }
  if (key == OPT_NOTIFY_TEXT) {
    ui->message_edit->setPlainText(value.toString());
  }
}

void SettingsDlg::showEvent(QShowEvent* e) {
  e->accept();

  int scrn = 0;
  const QWidget *w = window();

  if (w)
    scrn = QApplication::desktop()->screenNumber(w);
  else if (QApplication::desktop()->isVirtualDesktop())
    scrn = QApplication::desktop()->screenNumber(QCursor::pos());
  else
    scrn = QApplication::desktop()->screenNumber(this);

  QRect desk(QApplication::desktop()->availableGeometry(scrn));
  move((desk.width() - frameGeometry().width()) / 2,
       (desk.height() - frameGeometry().height()) / 2);
}

void SettingsDlg::on_time_edit_timeChanged(const QTime& time) {
  emit OptionChanged(OPT_TIME, time);
}

void SettingsDlg::on_alarm_enabled_toggled(bool checked) {
  emit OptionChanged(OPT_ENABLED, checked);
}

void SettingsDlg::on_browse_btn_clicked() {
  QString sound_file = QFileDialog::getOpenFileName(this, tr("Select sound file"),
                       last_file_path_,
                       tr("MP3 Files (*.mp3)"));
  if (!sound_file.isEmpty()) {
    ui->signal_label->setText(QFileInfo(sound_file).baseName());
    ui->signal_label->setToolTip(QDir::toNativeSeparators(sound_file));
    emit OptionChanged(OPT_FILENAME, sound_file);
    last_file_path_ = QFileInfo(sound_file).absolutePath();
  }
}

void SettingsDlg::on_notification_enabled_toggled(bool checked) {
  emit OptionChanged(OPT_SHOW_NOTIFY, checked);
}

void SettingsDlg::on_message_edit_textChanged() {
  emit OptionChanged(OPT_NOTIFY_TEXT, ui->message_edit->toPlainText());
}

void SettingsDlg::on_st_file_clicked() {
  emit OptionChanged(OPT_SIGNAL_TYPE, (int)ST_FILE);
}

void SettingsDlg::on_st_stream_clicked() {
  emit OptionChanged(OPT_SIGNAL_TYPE, (int)ST_STREAM);
}

void SettingsDlg::on_stream_url_edit_textEdited(const QString& arg1) {
  emit OptionChanged(OPT_STREAM_URL, arg1);
}
