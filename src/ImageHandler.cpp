#include "ImageHandler.hpp"

ImageHandler::ImageHandler(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_OpaquePaintEvent, false);
}

bool ImageHandler::loadImage(const QString &filePath) {

  sail::image image(filePath.toLocal8Bit().constData());

  if (!image.is_valid()) {
    qWarning() << "Failed to load image via SAIL:" << filePath;
    return false;
  }

  qInfo() << "Image pixel format"
          << sail_pixel_format_to_string(image.pixel_format());

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

  QImage temp(reinterpret_cast<const uchar *>(image.pixels()), image.width(),
              image.height(), image.bytes_per_line(), load_format);
  m_pixmap = QPixmap::fromImage(temp);

  fitImage();

  update();
  return true;
}

bool ImageHandler::event(QEvent *event) {
  if (m_pixmap.isNull()) {
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

  painter.translate(m_offset);
  painter.scale(m_scale, m_scale);
  painter.drawPixmap(0, 0, m_pixmap);
}

void ImageHandler::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton)
    return;

  QRectF imageRect(m_offset, QSizeF(m_pixmap.width() * m_scale,
                                    m_pixmap.height() * m_scale));

  if (imageRect.contains(event->position())) {
    m_isDragging = true;
    m_lastMousePos = event->position();
    event->accept();
  }
}

void ImageHandler::mouseMoveEvent(QMouseEvent *event) {
  if (m_isDragging) {
    QPointF currentPos = event->position();
    m_offset += (currentPos - m_lastMousePos);
    m_lastMousePos = currentPos;
    update();
    event->accept();
  }
}

void ImageHandler::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_isDragging = false;
    event->accept();
  }
}

void ImageHandler::wheelEvent(QWheelEvent *event) {
  QPointF mousePos = event->position();

  QRectF imageRect(m_offset, QSizeF(m_pixmap.width() * m_scale,
                                    m_pixmap.height() * m_scale));

  QPointF zoomAnchor;
  if (imageRect.contains(mousePos)) {
    zoomAnchor = mousePos;
  } else {
    zoomAnchor = imageRect.center();
  }

  double factor = event->angleDelta().y() > 0 ? 1.1 : 1.0 / 1.1;
  double newScale = std::clamp(m_scale * factor, 0.05, 20.0);
  factor = newScale / m_scale;
  m_offset = zoomAnchor - (zoomAnchor - m_offset) * factor;
  m_scale = newScale;

  update();
  event->accept();
}

void ImageHandler::resizeEvent(QResizeEvent *) { fitImage(); }

void ImageHandler::fitImage() {
  constexpr double margin = 0.90;
  double scaleX = width() / static_cast<double>(m_pixmap.width());
  double scaleY = height() / static_cast<double>(m_pixmap.height());
  m_scale = std::min(scaleX, scaleY) * margin;
  QSizeF scaledSize = QSizeF(m_pixmap.width(), m_pixmap.height()) * m_scale;
  m_offset = QPointF((width() - scaledSize.width()) / 2.0,
                     (height() - scaledSize.height()) / 2.0);
  update();
}
