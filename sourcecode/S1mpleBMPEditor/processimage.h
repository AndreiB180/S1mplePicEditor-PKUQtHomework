#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H

#include <QPixmap>

class ProcessImage {
public:
    // 右旋转90度
    static QPixmap rotate90(const QPixmap &src);

    // 水平镜像
    static QPixmap flipHorizontal(const QPixmap &src);

    // 垂直镜像
    static QPixmap flipVertical(const QPixmap &src);

    //任意角度旋转
    static QPixmap rotate(const QPixmap &src, int angle);

    //缩放
    static QPixmap scale(const QPixmap &src, double factor);

    //灰度化
    static QPixmap grayscale(const QPixmap &src);

    //反色
    static QPixmap invert(const QPixmap &src);

    //二值化
    static QPixmap binarize(const QPixmap &src, int threshold = 128);

    //马赛克
    static QPixmap mosaic(const QPixmap &src, int blockSize = 10);

    //亮度
    static QPixmap adjustLightness(const QPixmap &src, int delta);

    //对比度
    static QPixmap adjustContrast(const QPixmap &src, int delta);

    //饱和度
    static QPixmap adjustSaturation(const QPixmap &src, int delta);

    // 色温调整
    static QPixmap adjustColorTemperature(const QPixmap &src, int delta);

    // 锐化处理
    static QPixmap sharpen(const QPixmap &src, double strength = 1.0);


};

#endif // PROCESSIMAGE_H
