#ifndef DRAWMOSAICDIALOG_H
#define DRAWMOSAICDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QImage>
#include <QPainterPath>

class DrawMosaicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DrawMosaicDialog(const QPixmap &pixmap, QWidget *parent = nullptr);
    QPixmap getResultPixmap() const;

    void setPenWidth(int width);
    void setBlockSize(int size);

signals:
    void mosaicConfirmed(const QPixmap &pixmap);
    void mosaicCancelled();

private:
    QLabel *previewLabel;
    QPixmap originalPixmap;
    QImage baseImage;
    QImage maskImage;

    void applyMosaicToBaseImage();

    double scaleFactor = 1.0;
    int penWidth = 20;
    int blockSize = 10;

    void setupUI();
    void updatePreview();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // DRAWMOSAICDIALOG_H
