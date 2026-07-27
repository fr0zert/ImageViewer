#pragma once

#include <QObject>
#include <QPixmap>

class ImageLoader;

class ImageController final : public QObject {
  Q_OBJECT

public:
  explicit ImageController(QObject *parent = nullptr);

public slots:
  void openImage(QString path);

signals:
  void imageReady(QPixmap image);
  void imageFailed(const QString &path);

private:
  ImageLoader *loader_;
};
