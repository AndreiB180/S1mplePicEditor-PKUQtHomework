#ifndef PENWINDOW_H
#define PENWINDOW_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QColor>

class PenWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PenWindow(const QPixmap &pixmap, QColor color, int width, int opacity, QWidget *parent = nullptr);
    QPixmap getFinalPixmap() const;

signals:
    void drawingConfirmed(const QPixmap &pixmap);
    void drawingCancelled();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPixmap originalPixmap;
    QPixmap currentPreviewPixmap;
    QImage baseImage;      // 在这个图上实际绘图
    QLabel *label;

    double scaleFactor;

    QPoint lastPoint;
    bool drawing = false;

    QColor penColor;
    int penWidth;
    int penOpacity;
};

#endif // PENWINDOW_H
