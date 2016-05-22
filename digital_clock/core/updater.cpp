#include "updater.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>

#include "core/build_defs.h"
#include "core/clock_state.h"
#include "core/http_client.h"

#define S_OPT_LAST_UPDATE       "last_update"

namespace digital_clock {
namespace core {

const char c_build_date[] = {
  BUILD_DAY_CH0, BUILD_DAY_CH1,
  '-',
  BUILD_MONTH_CH0, BUILD_MONTH_CH1,
  '-',
  BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
  '\0'
};

Updater::Updater(ClockState* state, QObject* parent) :
  QObject(parent),
  state_(state),
  check_beta_(false), autoupdate_(true), update_period_(3),
  force_update_(false), was_error_(false)
{
  last_update_ = state_->GetVariable(S_OPT_LAST_UPDATE, QDate(2013, 6, 18)).value<QDate>();
  downloader_ = new HttpClient(this);
  connect(downloader_, &HttpClient::ErrorMessage, [=] (const QString& msg) {
    was_error_ = true;
    emit ErrorMessage(msg);
  });
  connect(downloader_, &HttpClient::DataDownloaded,
          [=] (const QByteArray& data) { data_.append(data); });
  connect(downloader_, &HttpClient::finished, this, &Updater::ProcessData);
}

Updater::~Updater() {
  if (downloader_->isRunning()) downloader_->cancel();
}

void Updater::CheckForUpdates() {
  RunCheckForUpdates(true);
}

void Updater::SetCheckForBeta(bool check) {
  check_beta_ = check;
}

void Updater::SetAutoupdate(bool update) {
  autoupdate_ = update;
}

void Updater::SetUpdatePeriod(int period) {
  update_period_ = period;
}

void Updater::TimeoutHandler() {
  if (!autoupdate_ || (downloader_ && downloader_->isRunning())) return;
  if (last_update_.daysTo(QDate::currentDate()) >= update_period_) RunCheckForUpdates(false);
}

void Updater::ProcessData() {
  if (was_error_) return;
  QJsonParseError err;
  QJsonDocument js_doc = QJsonDocument::fromJson(data_, &err);
  if (err.error != QJsonParseError::NoError) {
    was_error_ = true;
    emit ErrorMessage(err.errorString());
    return;
  }
  QJsonObject js_obj = js_doc.object().value("stable").toObject();
  QString latest = js_obj.value("version").toString();
  QDate last_build = QDate::fromString(js_obj.value("timestamp").toString().simplified(), "dd-MM-yyyy");
  QString link = js_obj.value("download").toString();
  if (check_beta_) {
    js_obj = js_doc.object().value("testing").toObject();
    QString t_version = js_obj.value("version").toString("-");
    if (t_version != "-") {
      latest = t_version;
      last_build = QDate::fromString(js_obj.value("timestamp").toString().simplified(), "dd-MM-yyyy");
      link = js_obj.value("download").toString();
    }
  }

  if (latest > QCoreApplication::applicationVersion() ||
      QDate::fromString(QLatin1String(c_build_date), "dd-MM-yyyy") < last_build) {
    latest = QString("%1, %2").arg(latest).arg(last_build.toString(Qt::DefaultLocaleShortDate));
    emit NewVersion(latest, link);
  } else {
    if (force_update_) emit UpToDate();
  }
  data_.clear();
  state_->SetVariable(S_OPT_LAST_UPDATE, last_update_);
}

void Updater::RunCheckForUpdates(bool force)
{
  force_update_ = force;
  was_error_ = false;
  data_.clear();
  last_update_ = QDate::currentDate();
  downloader_->startRequest(QUrl("http://digitalclock4.sourceforge.net/cgi-bin/update.py"));
}

} // namespace core
} // namespace digital_clock
