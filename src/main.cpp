#include "ImageHandler.hpp" // Image "convas"
#include "ImageViewer.hpp"  // Main window (background)
#include <QApplication>
#include <QFileInfo>
#include <chrono> // temp

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  QApplication app(argc, argv);
  ImageViewer mainWindow;
  mainWindow.showMaximized();

  ImageHandler *imageConvas = new ImageHandler(&mainWindow);

  mainWindow.setCentralWidget(imageConvas);

  QString imgPath = QFileInfo(argv[1]).absoluteFilePath();
  auto start = std::chrono::high_resolution_clock::now(); // temp
  imageConvas->loadImage(imgPath);
  auto stop = std::chrono::high_resolution_clock::now(); // temp

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      stop - start); // temp

  qInfo() << "Time taken" << duration.count() << "ms"; // temp

  return app.exec();
}
