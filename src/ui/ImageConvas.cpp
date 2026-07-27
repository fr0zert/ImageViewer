#include "ImageConvas.hpp"

ImageConvas::ImageConvas(QWidget *parent) : QWidget(parent) { setAttribute(Qt::WA_OpaquePaintEvent, false); }

bool ImageConvas::event(QEvent *event) {
  if (image_.isNull()) {
    event->ignore();
    return true;
  }
  return QWidget::event(event);
}

void ImageConvas::setImage(const QPixmap &new_image) {
  zoom_ = 1.0;
  offset_ = { 0, 0 };
  image_ = new_image;

  fitImage();
  update();
}

void ImageConvas::clearImage() {
  image_ = QPixmap();
  zoom_ = 1.0;
  offset_ = { 0, 0 };
  update();
}

void ImageConvas::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);

  painter.translate(offset_);
  painter.scale(zoom_, zoom_);
  painter.drawPixmap(0, 0, image_);
}

void ImageConvas::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton) {
    return;
  }

  QRectF image_rect(offset_, QSizeF(image_.width() * zoom_, image_.height() * zoom_));

  if (image_rect.contains(event->position())) {
    dragging_ = true;
    last_mouse_pos_ = event->position();
    event->accept();
  }
}

void ImageConvas::mouseMoveEvent(QMouseEvent *event) {
  if (dragging_) {
    QPointF current_pos = event->position();
    offset_ += (current_pos - last_mouse_pos_);
    last_mouse_pos_ = current_pos;
    update();
    event->accept();
  }
}

void ImageConvas::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    dragging_ = false;
    event->accept();
  }
}

void ImageConvas::wheelEvent(QWheelEvent *event) {
  QPointF mouse_pos = event->position();

  QRectF image_rect(offset_, QSizeF(image_.width() * zoom_, image_.height() * zoom_));

  QPointF zoom_anchor;
  if (image_rect.contains(mouse_pos)) {
    zoom_anchor = mouse_pos;
  } else {
    zoom_anchor = image_rect.center();
  }

  double factor = event->angleDelta().y() > 0 ? 1.1 : 1.0 / 1.1;
  double new_scale = std::clamp(zoom_ * factor, 0.05, 20.0);
  factor = new_scale / zoom_;
  offset_ = zoom_anchor - (zoom_anchor - offset_) * factor;
  zoom_ = new_scale;

  update();
  event->accept();
}

void ImageConvas::resizeEvent(QResizeEvent * /*event*/) { fitImage(); }

void ImageConvas::fitImage() {
  constexpr double MARGIN = 0.85;
  double scale_x = width() / static_cast<double>(image_.width());
  double scale_y = height() / static_cast<double>(image_.height());
  zoom_ = std::min(scale_x, scale_y) * MARGIN;
  QSizeF scaled_size = QSizeF(image_.width(), image_.height()) * zoom_;
  offset_ = QPointF((width() - scaled_size.width()) / 2.0, (height() - scaled_size.height()) / 2.0);
  update();
}
