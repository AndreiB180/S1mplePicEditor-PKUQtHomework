#include "drawmosaicdialog.h"
#include "processimage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QPainter>
#include <QDebug>

DrawMosaicDialog::DrawMosaicDialog(const QPixmap &pixmap, QWidget *parent)
    : QDialog(parent), originalPixmap(pixmap), baseImage(pixmap.toImage().convertToFormat(QImage::Format_ARGB32))
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Draw Mosaic");

    this->setStyleSheet(R"(
        DrawMosaicDialog {
            background-color: white;
        }
        QLabel {
            color: black;
        }
        QPushButton {
            color: black;
            background-color: white;
            border: 1px solid gray;
            padding: 5px 15px;
            border-radius: 4px;
            min-width: 70px;
        }
        QPushButton:hover {
            background-color: #d3d3d3;
        }
        QSlider::groove:horizontal {
            border: 1px solid #999999;
            height: 8px;
            background: black;
            margin: 2px 0;
        }
        QSlider::handle:horizontal {
            background: black;
            border: 1px solid black;
            width: 18px;
            margin: -2px 0;
            border-radius: 3px;
        }
    )");

    int targetWidth = 600;
    scaleFactor = double(targetWidth) / pixmap.width();

    maskImage = QImage(baseImage.size(), QImage::Format_ARGB32);
    maskImage.fill(Qt::transparent);

    setupUI();
    updatePreview();
}


void DrawMosaicDialog::applyMosaicToBaseImage()
{
    QImage mosaicFull = ProcessImage::mosaic(QPixmap::fromImage(baseImage), blockSize).toImage();

    for (int y = 0; y < baseImage.height(); ++y) {
        for (int x = 0; x < baseImage.width(); ++x) {
            if (qAlpha(maskImage.pixel(x, y)) > 0) {
                baseImage.setPixel(x, y, mosaicFull.pixel(x, y));
            }
        }
    }

    maskImage.fill(Qt::transparent);
}

void DrawMosaicDialog::setPenWidth(int width) {
    penWidth = width;
}

void DrawMosaicDialog::setBlockSize(int size) {
    blockSize = size;
}

void DrawMosaicDialog::setupUI()
{
    previewLabel = new QLabel(this);
    previewLabel->setAlignment(Qt::AlignCenter);

    QPushButton *yesButton = new QPushButton("Yes");
    QPushButton *noButton = new QPushButton("No");

    yesButton->setFixedSize(80, 30);
    noButton->setFixedSize(80, 30);

    connect(yesButton, &QPushButton::clicked, this, [=]() {
        applyMosaicToBaseImage();
        emit mosaicConfirmed(QPixmap::fromImage(baseImage));
        close();
    });

    connect(noButton, &QPushButton::clicked, this, [=]() {
        maskImage.fill(Qt::transparent);
        updatePreview(); // 清除未应用的绘制区域
    });

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);

    mainLayout->addWidget(previewLabel);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(yesButton);
    btnLayout->addWidget(noButton);
    btnLayout->addStretch();
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(10, 0, 10, 10);

    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
}


void DrawMosaicDialog::updatePreview()
{
    QImage temp = baseImage;
    QImage mosaicFull = ProcessImage::mosaic(QPixmap::fromImage(temp), blockSize).toImage();

    QPainter p(&temp);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    for (int y = 0; y < temp.height(); ++y) {
        for (int x = 0; x < temp.width(); ++x) {
            if (qAlpha(maskImage.pixel(x, y)) > 0) {
                temp.setPixel(x, y, mosaicFull.pixel(x, y));
            }
        }
    }

    QPixmap preview = QPixmap::fromImage(temp).scaledToWidth(600, Qt::SmoothTransformation);
    previewLabel->setPixmap(preview);
}

void DrawMosaicDialog::mousePressEvent(QMouseEvent *event)
{
    QPoint pt = event->pos() - previewLabel->pos();
    int x = pt.x() / scaleFactor;
    int y = pt.y() / scaleFactor;

    QPainter painter(&maskImage);
    painter.setPen(QPen(Qt::white, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(x, y);
    updatePreview();
}

void DrawMosaicDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint pt = event->pos() - previewLabel->pos();
        int x = pt.x() / scaleFactor;
        int y = pt.y() / scaleFactor;

        QPainter painter(&maskImage);
        painter.setPen(QPen(Qt::white, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPoint(x, y);
        updatePreview();
    }
}

QPixmap DrawMosaicDialog::getResultPixmap() const
{
    return QPixmap::fromImage(baseImage);
}
