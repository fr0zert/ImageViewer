#include <QApplication>
#include <QFileInfo>
#include <chrono> // temp

#include "ImageHandler.hpp" // Image "convas"
#include "ImageViewer.hpp"  // Main window (background)

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  QApplication app(argc, argv);
  ImageViewer main_window;
  main_window.showMaximized();

  auto *image_convas = new ImageHandler(&main_window); // qt handle dealocation

  main_window.setCentralWidget(image_convas);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  QString img_path { QFileInfo(argv[1]).absoluteFilePath() };
  auto start { std::chrono::high_resolution_clock::now() }; // temp
  image_convas->loadImage(img_path);
  auto stop { std::chrono::high_resolution_clock::now() }; // temp

  auto duration { std::chrono::duration_cast<std::chrono::milliseconds>(stop - start) }; // temp

  qInfo() << "Time taken" << duration.count() << "ms"; // temp

  return QApplication::exec();
}
