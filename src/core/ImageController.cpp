#include "ImageController.hpp"

#include "ImageLoader.hpp"

ImageController::ImageController(QObject *parent) : QObject(parent), loader_(new ImageLoader(this)) {
  connect(loader_, &ImageLoader::imageLoaded, this, &ImageController::imageReady);
  connect(loader_, &ImageLoader::imageLoadFailed, this, &ImageController::imageFailed);
}

void ImageController::openImage(QString path) { loader_->load(path); }
