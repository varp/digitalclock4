#ifndef DIGITAL_CLOCK_H
#define DIGITAL_CLOCK_H

#include <QLabel>
#include <QTimer>

class DigitalClock : public QLabel {
  Q_OBJECT
public:
  explicit DigitalClock(QWidget* parent = 0);
  ~DigitalClock();

signals:
  void ImageNeeded(const QString& text);

public slots:
  void DrawImage(const QImage& image);
  void SetSeparatorFlash(bool set);
  void SetDisplayAMPM(bool set);
  void SetTimeFormat(const QString& format);

private slots:
  void TimeoutHandler();

private:
  QTimer* timer_;
  bool sep_visible_;
  bool sep_flashes_;
  bool display_am_pm_;
  QString time_format_;
};

#endif // DIGITAL_CLOCK_H
