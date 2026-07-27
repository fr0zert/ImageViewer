#include <QApplication>

#include "app/MainWindow.hpp"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow window;

  if (argc > 1) {
    window.openImage(argv[1]);
    // TODO: exit
  }

  window.show();

  return QApplication::exec();
}
