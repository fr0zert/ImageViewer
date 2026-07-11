#pragma once
#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>

class ImageViewer : public QMainWindow {
  Q_OBJECT

public:
  explicit ImageViewer(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;
};
