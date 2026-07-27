#pragma once

#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QtConcurrent>
#include <sail-c++/sail-c++.h>
#include <sail-common/common_serialize.h>

class ImageLoader final : public QObject {
  Q_OBJECT

public:
  explicit ImageLoader(QObject *parent = nullptr);

public slots:
  void load(const QString &path);

signals:
  void imageLoaded(QPixmap image);
  void imageLoadFailed(const QString &path);
};
