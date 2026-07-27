#pragma once
#include <sail-c++/sail-c++.h>
#include <sail-common/common_serialize.h>

#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QWheelEvent>
#include <QWidget>

class ImageHandler : public QWidget {
  Q_OBJECT

public:
  explicit ImageHandler(QWidget *parent = nullptr);
  bool loadImage(const QString &file_path);

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

  QPixmap pixmap_;
  QPointF offset_;
  double scale_ { 1.0 };
  QPointF last_mouse_pos_;
  bool is_dragging_ { false };
};
