#include "penwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QDebug>

PenWindow::PenWindow(const QPixmap &pixmap, QColor color, int width, int opacity, QWidget *parent)
    : QDialog(parent), originalPixmap(pixmap), penColor(color), penWidth(width), penOpacity(opacity)
{
    setWindowTitle("Draw");
    setAttribute(Qt::WA_DeleteOnClose);

    this->setStyleSheet(R"(
        QWidget {
            background-color: white;
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
    )");

    // 缩放到长边为 600 的比例
    int targetLongSide = 600;
    if (pixmap.width() >= pixmap.height()) {
        scaleFactor = double(targetLongSide) / pixmap.width();
    } else {
        scaleFactor = double(targetLongSide) / pixmap.height();
    }

    baseImage = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    currentPreviewPixmap = QPixmap::fromImage(baseImage).scaled(pixmap.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    label = new QLabel;
    label->setPixmap(currentPreviewPixmap);
    label->setAlignment(Qt::AlignCenter);

    QPushButton *yesBtn = new QPushButton("Yes");
    QPushButton *noBtn = new QPushButton("No");
    yesBtn->setFixedSize(80, 30);
    noBtn->setFixedSize(80, 30);

    connect(yesBtn, &QPushButton::clicked, this, [=]() {
        originalPixmap = QPixmap::fromImage(baseImage);
        emit drawingConfirmed(originalPixmap);
        close();
    });

    connect(noBtn, &QPushButton::clicked, this, [=]() {
        // 恢复 baseImage 到原始状态
        baseImage = originalPixmap.toImage().convertToFormat(QImage::Format_ARGB32);

        // 重新生成预览图
        currentPreviewPixmap = QPixmap::fromImage(baseImage).scaled(originalPixmap.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(currentPreviewPixmap);

    });

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(yesBtn);
    btnLayout->addWidget(noBtn);
    btnLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label);
    mainLayout->addLayout(btnLayout);
    mainLayout->setContentsMargins(10, 0, 10,10);
    setLayout(mainLayout);
}

void PenWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint labelPos = label->pos();
    QPoint clickedPoint = event->pos() - labelPos;

    lastPoint = QPoint(int(clickedPoint.x() / scaleFactor), int(clickedPoint.y() / scaleFactor));
    drawing = true;
}

void PenWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!drawing) return;

    QPoint labelPos = label->pos();
    QPoint currentPoint = event->pos() - labelPos;
    QPoint imgPoint = QPoint(int(currentPoint.x() / scaleFactor), int(currentPoint.y() / scaleFactor));

    QPainter painter(&baseImage);
    QPen pen(penColor);
    pen.setWidth(penWidth);
    QColor c = pen.color();
    c.setAlpha(penOpacity);
    pen.setColor(c);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawLine(lastPoint, imgPoint);
    painter.end();

    lastPoint = imgPoint;

    // 更新预览
    currentPreviewPixmap = QPixmap::fromImage(baseImage).scaled(originalPixmap.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(currentPreviewPixmap);
}

QPixmap PenWindow::getFinalPixmap() const {
    return QPixmap::fromImage(baseImage);
}
