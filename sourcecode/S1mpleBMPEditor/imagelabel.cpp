#include "imagelabel.h"
#include<QMouseEvent>

ImageLabel::ImageLabel(QWidget *parent) : QLabel(parent) {}

void ImageLabel::setImage(const QPixmap &pix) {
    image = pix;
    update();
}

void ImageLabel::setOffset(const QPoint &off) {
    if (image.isNull() || !manualMode) return;

    int maxX = std::max(0, image.width() - width());
    int maxY = std::max(0, image.height() - height());

    int newX = std::clamp(off.x(), 0, maxX);
    int newY = std::clamp(off.y(), 0, maxY);

    offset = QPoint(newX, newY);
    update();
}

void ImageLabel::enableManualMode(bool enable) {
    manualMode = enable;
    update();
}

void ImageLabel::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);  // 背景保留
    if (image.isNull()) return;

    QPainter painter(this);
    if (manualMode) {
        // 非自适应模式下，绘制偏移图像
        QRect visibleRect(offset, size());
        painter.drawPixmap(0, 0, image, visibleRect.x(), visibleRect.y(), visibleRect.width(), visibleRect.height());
    } else {
        // 自适应模式：缩放图像
        QPixmap scaled = image.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap((width() - scaled.width()) / 2, (height() - scaled.height()) / 2, scaled);
    }
}

void ImageLabel::mousePressEvent(QMouseEvent *event) {
    if (manualMode && event->button() == Qt::LeftButton) {
        dragging = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor); // 更换鼠标形状，反馈拖动状态
    }
}

void ImageLabel::mouseMoveEvent(QMouseEvent *event) {
    if (manualMode && dragging) {
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();
        setOffset(offset - delta); // 累加偏移
    }
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *event) {
    if (manualMode && event->button() == Qt::LeftButton) {
        dragging = false;
        setCursor(Qt::ArrowCursor); // 恢复正常鼠标形状
    }
}
