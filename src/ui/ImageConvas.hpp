#pragma once

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

class ImageConvas final : public QWidget {
  Q_OBJECT

public:
  explicit ImageConvas(QWidget *parent = nullptr);

public slots:
  void setImage(const QPixmap &image);
  void clearImage();

protected:
  bool event(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  void fitImage();

  QPixmap image_;
  double zoom_ { 1.0 };
  QPointF offset_ { 0, 0 };
  bool dragging_ { false };
  QPointF last_mouse_pos_;
};
