#ifndef IMAGELABEL_H
#define IMAGELABEL_H


#include <QLabel>
#include <QPainter>

class ImageLabel : public QLabel {
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = nullptr);

    void setImage(const QPixmap &pix);
    void setOffset(const QPoint &off);
    void enableManualMode(bool enable);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool dragging = false;
    QPoint lastMousePos;
    QPixmap image;
    QPoint offset;
    bool manualMode = false;
};

#endif // IMAGELABEL_H
