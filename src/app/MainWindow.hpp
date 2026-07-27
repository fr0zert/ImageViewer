#pragma once

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QSplitter>

#include "../core/ImageController.hpp"
#include "../ui/ImageConvas.hpp"

class ImageConvas;
class ShortcutManager;

class ImageController;

class MainWindow final : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  void openImage(QString path);

private:
  // Methods
  // void setupUi();
  // void setupConnections();

  // UI
  ImageConvas *image_view_ = nullptr;

  // Logic
  ImageController *image_controller_ = nullptr;

protected:
  void paintEvent(QPaintEvent *event) override;
};
