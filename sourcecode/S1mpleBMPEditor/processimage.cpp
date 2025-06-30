#include "processimage.h"
#include <QImage>
#include <QTransform>

QPixmap ProcessImage::rotate90(const QPixmap &src) {
    QTransform transform;
    transform.rotate(90);
    return src.transformed(transform);
}

QPixmap ProcessImage::flipHorizontal(const QPixmap &src) {
    return src.transformed(QTransform().scale(-1, 1));
}

QPixmap ProcessImage::flipVertical(const QPixmap &src) {
    return src.transformed(QTransform().scale(1, -1));
}

QPixmap ProcessImage::rotate(const QPixmap &src, int angle)
{
    QTransform transform;
    transform.rotate(angle);
    return src.transformed(transform, Qt::SmoothTransformation);
}

QPixmap ProcessImage::scale(const QPixmap &src, double factor)
{
    if (factor == 1.0) return src; // 不缩放时直接返回原图

    int newWidth = static_cast<int>(src.width() * factor);
    int newHeight = static_cast<int>(src.height() * factor);

    return src.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}


QPixmap ProcessImage::grayscale(const QPixmap &src) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int w = img.width();
    int h = img.height();

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < w; ++x) {
            QRgb px = line[x];
            if (qAlpha(px) == 0) continue;
            int gray = static_cast<int>(0.299 * qRed(px) + 0.587 * qGreen(px) + 0.114 * qBlue(px));
            line[x] = qRgba(gray, gray, gray, qAlpha(px));
        }
    }

    return QPixmap::fromImage(img);
}


QPixmap ProcessImage::invert(const QPixmap &src) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int w = img.width();
    int h = img.height();

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < w; ++x) {
            QRgb px = line[x];
            if (qAlpha(px) == 0) continue;
            int r = 255 - qRed(px);
            int g = 255 - qGreen(px);
            int b = 255 - qBlue(px);
            line[x] = qRgba(r, g, b, qAlpha(px));
        }
    }

    return QPixmap::fromImage(img);
}


QPixmap ProcessImage::binarize(const QPixmap &src, int threshold) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int w = img.width();
    int h = img.height();

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < w; ++x) {
            QRgb px = line[x];
            if (qAlpha(px) == 0) continue;
            int gray = (qRed(px) + qGreen(px) + qBlue(px)) / 3;
            int val = (gray >= threshold) ? 255 : 0;
            line[x] = qRgba(val, val, val, qAlpha(px));
        }
    }

    return QPixmap::fromImage(img);
}


QPixmap ProcessImage::mosaic(const QPixmap &src, int blockSize) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int width = img.width();
    int height = img.height();

    for (int y = 0; y < height; y += blockSize) {
        for (int x = 0; x < width; x += blockSize) {
            int rSum = 0, gSum = 0, bSum = 0, count = 0;

            // 计算当前块的平均颜色（排除透明像素）
            for (int dy = 0; dy < blockSize && (y + dy) < height; ++dy) {
                QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y + dy));
                for (int dx = 0; dx < blockSize && (x + dx) < width; ++dx) {
                    QRgb px = line[x + dx];
                    if (qAlpha(px) == 0) continue;
                    rSum += qRed(px);
                    gSum += qGreen(px);
                    bSum += qBlue(px);
                    ++count;
                }
            }

            if (count == 0) continue;

            int rAvg = rSum / count;
            int gAvg = gSum / count;
            int bAvg = bSum / count;

            // 设置当前块的颜色（只改非透明像素）
            for (int dy = 0; dy < blockSize && (y + dy) < height; ++dy) {
                QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y + dy));
                for (int dx = 0; dx < blockSize && (x + dx) < width; ++dx) {
                    QRgb px = line[x + dx];
                    if (qAlpha(px) == 0) continue;
                    line[x + dx] = qRgba(rAvg, gAvg, bAvg, qAlpha(px));
                }
            }
        }
    }

    return QPixmap::fromImage(img);
}


// 亮度调整：每个颜色分量加减delta（范围建议[-100,100]）
QPixmap ProcessImage::adjustLightness(const QPixmap &src, int delta) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32); // 保留 alpha
    int w = img.width(), h = img.height();

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < w; ++x) {
            if (qAlpha(line[x]) == 0) continue; // 保留透明像素
            QColor color = QColor::fromRgba(line[x]);
            int r = qBound(0, color.red() + delta, 255);
            int g = qBound(0, color.green() + delta, 255);
            int b = qBound(0, color.blue() + delta, 255);
            int a = color.alpha();
            line[x] = qRgba(r, g, b, a);
        }
    }

    return QPixmap::fromImage(img);
}


// 对比度调整：对每个像素偏离128的位置进行放大或缩小
QPixmap ProcessImage::adjustContrast(const QPixmap &src, int delta) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int w = img.width(), h = img.height();

    double factor = delta / 100.0;

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < w; ++x) {
            if (qAlpha(line[x]) == 0) continue;
            QColor color = QColor::fromRgba(line[x]);
            int r = qBound(0, int((color.red() - 128) * factor + 128), 255);
            int g = qBound(0, int((color.green() - 128) * factor + 128), 255);
            int b = qBound(0, int((color.blue() - 128) * factor + 128), 255);
            int a = color.alpha();
            line[x] = qRgba(r, g, b, a);
        }
    }

    return QPixmap::fromImage(img);
}


// 饱和度调整：转换为HSL后调整saturation
QPixmap ProcessImage::adjustSaturation(const QPixmap &src, int delta) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int w = img.width(), h = img.height();

    double factor = delta / 100.0;

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < w; ++x) {
            if (qAlpha(line[x]) == 0) continue;
            QColor color = QColor::fromRgba(line[x]);
            int h, s, l;
            color.getHsl(&h, &s, &l);
            s = qBound(0, int(s * factor), 255);
            color.setHsl(h, s, l);
            int a = color.alpha();
            line[x] = qRgba(color.red(), color.green(), color.blue(), a);
        }
    }

    return QPixmap::fromImage(img);
}

// 色温调整实现：红蓝通道加减实现简单色温变化
QPixmap ProcessImage::adjustColorTemperature(const QPixmap &src, int delta) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int w = img.width(), h = img.height();

    // delta > 0 → 增红减蓝（偏暖），delta < 0 → 减红增蓝（偏冷）
    int rAdjust = delta;
    int bAdjust = -delta;

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < w; ++x) {
            if (qAlpha(line[x]) == 0) continue;
            QColor color = QColor::fromRgba(line[x]);
            int r = qBound(0, color.red() + rAdjust, 255);
            int g = color.green(); // 不变
            int b = qBound(0, color.blue() + bAdjust, 255);
            int a = color.alpha();
            line[x] = qRgba(r, g, b, a);
        }
    }

    return QPixmap::fromImage(img);
}

// 锐化处理实现：使用简单的拉普拉斯模板
QPixmap ProcessImage::sharpen(const QPixmap &src, double strength) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage result = img.copy();

    int w = img.width(), h = img.height();

    auto getPixel = [&](int x, int y) -> QRgb {
        if (x < 0) x = 0;
        if (x >= w) x = w - 1;
        if (y < 0) y = 0;
        if (y >= h) y = h - 1;
        return img.pixel(x, y);
    };

    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(result.scanLine(y));
        for (int x = 0; x < w; ++x) {
            QRgb center = getPixel(x, y);
            if (qAlpha(center) == 0) continue;

            // 拉普拉斯模板
            int r = 5 * qRed(center)
                    - qRed(getPixel(x - 1, y))
                    - qRed(getPixel(x + 1, y))
                    - qRed(getPixel(x, y - 1))
                    - qRed(getPixel(x, y + 1));

            int g = 5 * qGreen(center)
                    - qGreen(getPixel(x - 1, y))
                    - qGreen(getPixel(x + 1, y))
                    - qGreen(getPixel(x, y - 1))
                    - qGreen(getPixel(x, y + 1));

            int b = 5 * qBlue(center)
                    - qBlue(getPixel(x - 1, y))
                    - qBlue(getPixel(x + 1, y))
                    - qBlue(getPixel(x, y - 1))
                    - qBlue(getPixel(x, y + 1));

            r = qBound(0, int(qRed(center) + strength * (r - qRed(center))), 255);
            g = qBound(0, int(qGreen(center) + strength * (g - qGreen(center))), 255);
            b = qBound(0, int(qBlue(center) + strength * (b - qBlue(center))), 255);

            line[x] = qRgba(r, g, b, qAlpha(center));
        }
    }

    return QPixmap::fromImage(result);
}


