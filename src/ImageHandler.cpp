#include "ImageHandler.hpp"

ImageHandler::ImageHandler(QWidget *parent) : QWidget(parent) { setAttribute(Qt::WA_OpaquePaintEvent, false); }

bool ImageHandler::loadImage(const QString &file_path) {
  sail::image image(file_path.toLocal8Bit().constData());

  if (!image.is_valid()) {
    qWarning() << "Failed to load image via SAIL:" << file_path;
    return false;
  }

  qInfo() << "Image pixel format" << sail_pixel_format_to_string(image.pixel_format());

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
      qWarning() << "Unsupported SAIL pixel format.";
      return false;
    }

    if (image.convert(SAIL_PIXEL_FORMAT_BPP32_RGBA) != SAIL_OK) {
      qWarning() << "Cant convert pixel format to BPP32_RGBA.";
      return false;
    }
    load_format = QImage::Format_RGBA8888;
  }

  QImage temp(static_cast<const uchar *>(image.pixels()), static_cast<int>(image.width()), static_cast<int>(image.height()),
              image.bytes_per_line(), load_format);
  pixmap_ = QPixmap::fromImage(temp);

  fitImage();

  update();
  return true;
}

bool ImageHandler::event(QEvent *event) {
  if (pixmap_.isNull()) {
    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
    case QEvent::Wheel:
    case QEvent::Enter:
    case QEvent::Leave:
      event->ignore();
      return true;
    default:
      break;
    }
  }
  return QWidget::event(event);
}

void ImageHandler::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);

  painter.translate(offset_);
  painter.scale(scale_, scale_);
  painter.drawPixmap(0, 0, pixmap_);
}

void ImageHandler::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton) {
    return;
  }

  QRectF image_rect(offset_, QSizeF(pixmap_.width() * scale_, pixmap_.height() * scale_));

  if (image_rect.contains(event->position())) {
    is_dragging_ = true;
    last_mouse_pos_ = event->position();
    event->accept();
  }
}

void ImageHandler::mouseMoveEvent(QMouseEvent *event) {
  if (is_dragging_) {
    QPointF current_pos { event->position() };
    offset_ += (current_pos - last_mouse_pos_);
    last_mouse_pos_ = current_pos;
    update();
    event->accept();
  }
}

void ImageHandler::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_dragging_ = false;
    event->accept();
  }
}

void ImageHandler::wheelEvent(QWheelEvent *event) {
  QPointF mouse_pos { event->position() };

  QRectF image_rect(offset_, QSizeF(pixmap_.width() * scale_, pixmap_.height() * scale_));

  QPointF zoom_anchor {};
  if (image_rect.contains(mouse_pos)) {
    zoom_anchor = mouse_pos;
  } else {
    zoom_anchor = image_rect.center();
  }
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  double factor { event->angleDelta().y() > 0 ? 1.1 : 1.0 / 1.1 };
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  double new_scale { std::clamp(scale_ * factor, 0.05, 20.0) };
  factor = new_scale / scale_;
  offset_ = zoom_anchor - (zoom_anchor - offset_) * factor;
  scale_ = new_scale;

  update();
  event->accept();
}

void ImageHandler::resizeEvent(QResizeEvent * /*event*/) { fitImage(); }

void ImageHandler::fitImage() {
  constexpr double MARGIN = 0.90;
  double scale_x { width() / static_cast<double>(pixmap_.width()) };
  double scale_y { height() / static_cast<double>(pixmap_.height()) };
  scale_ = std::min(scale_x, scale_y) * MARGIN;
  QSizeF scaled_size { QSizeF(pixmap_.width(), pixmap_.height()) * scale_ };
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  offset_ = QPointF((width() - scaled_size.width()) / 2.0, (height() - scaled_size.height()) / 2.0);
  update();
}
