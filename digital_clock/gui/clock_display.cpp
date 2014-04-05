#include <QTime>
#include <QLocale>
#include <QRegExp>
#include "clock_display.h"

namespace digital_clock {
namespace gui {

ClockDisplay::ClockDisplay(QWidget* parent)
  : QLabel(parent), sep_visible_(false), sep_flashes_(true) {
  setAlignment(Qt::AlignCenter);
  timer_.setInterval(500);
  connect(&timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
  timer_.start();
}

ClockDisplay::~ClockDisplay() {
  timer_.stop();
}

void ClockDisplay::DrawImage(const QImage& image) {
  setPixmap(QPixmap::fromImage(image));
  parentWidget()->adjustSize();
}

void ClockDisplay::SetSeparatorFlash(bool set) {
  sep_flashes_ = set;
  sep_visible_ = !set;
}

void ClockDisplay::SetTimeFormat(const QString& format) {
  time_format_ = format;
  seps_ = format;
  seps_.remove(QRegExp("[hmszap]", Qt::CaseInsensitive));
  emit SeparatorsChanged(seps_);
}

void ClockDisplay::TimeoutHandler() {
  if (time_format_.isEmpty()) SetTimeFormat(QLocale::system().timeFormat());
  QString str_time = QTime::currentTime().toString(time_format_);

  QList<int> seps_pos;
  for (int i = 0; i < str_time.length(); ++i) {
    if (seps_.contains(str_time[i], Qt::CaseInsensitive)) seps_pos.append(i);
  }

  if (sep_flashes_) {
    for (auto& sep_pos : seps_pos) {
      if (!sep_visible_) str_time[sep_pos] = ' ';
    }
    sep_visible_ = !sep_visible_;
  }
  emit ImageNeeded(str_time);
}

} // namespace gui
} // namespace digital_clock