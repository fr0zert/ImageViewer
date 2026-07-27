#include "ImageLoader.hpp"
#include <optional>

ImageLoader::ImageLoader(QObject *parent) : QObject(parent) {}

void ImageLoader::load(const QString &path) {

  auto future = QtConcurrent::run([path]() -> std::optional<QPixmap> {
    sail::image image(path.toLocal8Bit().constData());

    if (!image.is_valid()) {
      qDebug() << "Failed to load image via SAIL:" << path;
      return std::nullopt;
    }

    qDebug() << "Image pixel format" << sail_pixel_format_to_string(image.pixel_format());

    QImage::Format load_format = [format = image.pixel_format()] {
      switch (format) {
      case SAIL_PIXEL_FORMAT_BPP8_INDEXED:
        return QImage::Format_Indexed8;

      case SAIL_PIXEL_FORMAT_BPP8_GRAYSCALE:
        return QImage::Format_Grayscale8;
      case SAIL_PIXEL_FORMAT_BPP16_GRAYSCALE:
        return QImage::Format_Grayscale16;

      case SAIL_PIXEL_FORMAT_BPP16_RGB555:
        return QImage::Format_RGB555;
      case SAIL_PIXEL_FORMAT_BPP16_RGB565:
        return QImage::Format_RGB16;

      case SAIL_PIXEL_FORMAT_BPP24_RGB:
        return QImage::Format_RGB888;
      case SAIL_PIXEL_FORMAT_BPP24_BGR:
        return QImage::Format_BGR888;

      case SAIL_PIXEL_FORMAT_BPP32_RGBX:
        return QImage::Format_RGBX8888;
      case SAIL_PIXEL_FORMAT_BPP32_RGBA:
        return QImage::Format_RGBA8888;
      case SAIL_PIXEL_FORMAT_BPP32_ARGB:
        return QImage::Format_ARGB32;
        // case SAIL_PIXEL_FORMAT_BPP32_RGB32: ???
        //   return QImage::Format_RGB32;

      case SAIL_PIXEL_FORMAT_BPP30_RGB:
        return QImage::Format_RGB30;
      case SAIL_PIXEL_FORMAT_BPP30_BGR:
        return QImage::Format_BGR30;
      case SAIL_PIXEL_FORMAT_BPP32_RGBA_1010102:
        return QImage::Format_A2RGB30_Premultiplied;
      case SAIL_PIXEL_FORMAT_BPP32_BGRA_1010102:
        return QImage::Format_A2BGR30_Premultiplied;

      case SAIL_PIXEL_FORMAT_BPP64_RGBX:
        return QImage::Format_RGBX64;
      case SAIL_PIXEL_FORMAT_BPP64_RGBA:
        return QImage::Format_RGBA64;

      case SAIL_PIXEL_FORMAT_BPP48_RGB_HALF:
        return QImage::Format_RGBX16FPx4;
      case SAIL_PIXEL_FORMAT_BPP64_RGBA_HALF:
        return QImage::Format_RGBA16FPx4;
      case SAIL_PIXEL_FORMAT_BPP96_RGB_FLOAT:
        return QImage::Format_RGBX32FPx4;
      case SAIL_PIXEL_FORMAT_BPP128_RGBA_FLOAT:
        return QImage::Format_RGBA32FPx4;

      case SAIL_PIXEL_FORMAT_BPP32_CMYK:
        return QImage::Format_CMYK8888;

      default:
        return QImage::Format_Invalid;
      }
    }();

    if (load_format == QImage::Format_Invalid) {
      if (!image.can_convert(SAIL_PIXEL_FORMAT_BPP32_RGBA)) {
        qDebug() << "Unsupported SAIL pixel format.";
        return std::nullopt;
      }

      if (image.convert(SAIL_PIXEL_FORMAT_BPP32_RGBA) != SAIL_OK) {
        qDebug() << "Cant convert pixel format to BPP32_RGBA.";
        return std::nullopt;
      }
      load_format = QImage::Format_RGBA8888;
    }

    QImage result(reinterpret_cast<const uchar *>(image.pixels()), static_cast<int>(image.width()), static_cast<int>(image.height()),
                  image.bytes_per_line(), load_format);

    return QPixmap::fromImage(result.copy());
  });

  auto *watcher = new QFutureWatcher<std::optional<QPixmap>>(this);

  connect(watcher, &QFutureWatcher<std::optional<QPixmap>>::finished, this, [this, watcher, path]() {
    auto result = watcher->result();

    if (result.has_value()) {
      emit imageLoaded(result.value());
    } else {
      qDebug() << "Image loading failed";
      emit imageLoadFailed(path);
    }

    watcher->deleteLater();
  });

  watcher->setFuture(future);
}
