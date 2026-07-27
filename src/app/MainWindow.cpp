#include "MainWindow.hpp"

static const QColor BACKGROUND_COLOR = QColor(0, 0, 0, 128);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), image_view_(new ImageConvas(this)), image_controller_(new ImageController(this)) {
  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground, true);
  showMaximized();

  setCentralWidget(image_view_);

  connect(image_controller_, &ImageController::imageReady, image_view_, &ImageConvas::setImage);
  connect(image_controller_, &ImageController::imageFailed, image_view_, &ImageConvas::clearImage);

  // TODO
  // setupUi();
  // setupConnections();
}

void MainWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(rect(), BACKGROUND_COLOR);
}

void MainWindow::openImage(QString path) { image_controller_->openImage(std::move(path)); }

// void MainWindow::setupUi() {}

// void MainWindow::setupConnections() {}
