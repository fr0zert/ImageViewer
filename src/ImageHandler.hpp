#pragma once
#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QWheelEvent>
#include <QWidget>
#include <sail-c++/sail-c++.h>
#include <sail-common/common_serialize.h>

class ImageHandler : public QWidget {
  Q_OBJECT

public:
  explicit ImageHandler(QWidget *parent = nullptr);
  bool loadImage(const QString &filePath);

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

  QPixmap m_pixmap;
  QPointF m_offset;
  double m_scale = 1.0;
  QPointF m_lastMousePos;
  bool m_isDragging = false;
};
