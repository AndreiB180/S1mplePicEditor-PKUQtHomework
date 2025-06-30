#ifndef CROPWINDOW_H
#define CROPWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QRubberBand>
#include <QPoint>
#include <QPushButton>

class CropWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CropWindow(const QPixmap &pixmap, QWidget *parent = nullptr);

signals:
    void croppedPixmap(const QPixmap &croppedPixmap);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onYesClicked();
    void onNoClicked();

private:
    QPixmap originalPixmap;
    QPixmap displayPixmap;
    QRubberBand *rubberBand = nullptr;
    QPoint origin;

    double scaleFactor;  // 映射比例 = displayPixmap.width()/originalPixmap.width()

    QRect currentSelection; // 当前选区矩形

    QWidget* imageArea = nullptr; // 仅显示图片和接收鼠标事件的区域
    QPushButton* yesBtn = nullptr;
    QPushButton* noBtn = nullptr;
};

#endif // CROPWINDOW_H
