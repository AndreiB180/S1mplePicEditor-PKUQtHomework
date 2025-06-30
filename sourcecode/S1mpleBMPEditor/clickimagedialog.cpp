#include "clickimagedialog.h"
#include "watermarkdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

ClickImageDialog::ClickImageDialog(const QPixmap &pixmap, QWidget *parent)
    : QDialog(parent), originalPixmap(pixmap), basePixmap(pixmap)
{
    setWindowTitle("Preview");
    setAttribute(Qt::WA_DeleteOnClose);

    // ⬇️ 和 CropWindow 一致的样式表
    this->setStyleSheet(R"(
        QWidget {
            background-color: white;
        }
        QMessageBox {
            background-color: white;
            color: black;
            font-size: 13px;
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
    currentPreviewPixmap = originalPixmap.scaledToWidth(targetWidth, Qt::SmoothTransformation);

    label = new QLabel;
    label->setPixmap(currentPreviewPixmap);
    label->setAlignment(Qt::AlignCenter);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label);

    QPushButton *yesButton = new QPushButton("Yes");
    QPushButton *noButton = new QPushButton("No");

    yesButton->setFixedSize(80, 30);  // 和 CropWindow 保持一致
    noButton->setFixedSize(80, 30);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(yesButton);
    btnLayout->addWidget(noButton);
    btnLayout->addStretch();
    btnLayout->setSpacing(10);
    btnLayout->setContentsMargins(10, 0, 10, 10);

    mainLayout->addLayout(btnLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);

    connect(yesButton, &QPushButton::clicked, this, [=]() {
        emit watermarkConfirmed(originalPixmap);
        close();
    });

    connect(noButton, &QPushButton::clicked, this, [=]() {
        originalPixmap = basePixmap;
        currentPreviewPixmap = basePixmap.scaledToWidth(600, Qt::SmoothTransformation);
        label->setPixmap(currentPreviewPixmap);
        emit watermarkCancelled();
    });

    setLayout(mainLayout);
}

void ClickImageDialog::mousePressEvent(QMouseEvent *event)
{
    QPoint labelPos = label->pos();
    QPoint clickedPoint = event->pos() - labelPos;

    int origX = int(clickedPoint.x() / scaleFactor);
    int origY = int(clickedPoint.y() / scaleFactor);
    origX = qBound(0, origX, originalPixmap.width() - 1);
    origY = qBound(0, origY, originalPixmap.height() - 1);

    WatermarkDialog *dialog = new WatermarkDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &WatermarkDialog::previewUpdated, this, [=](QString text, QFont font, QColor color, int opacity) {
        // 将原图转换为 ARGB32 格式的 QImage
        QImage img = originalPixmap.toImage().convertToFormat(QImage::Format_ARGB32);

        // 用 QPainter 在 QImage 上绘制文字
        QPainter painter(&img);
        color.setAlpha(opacity);
        painter.setFont(font);
        painter.setPen(color);
        painter.drawText(QPoint(origX, origY), text);
        painter.end();

        // 再转回 QPixmap，用于预览显示（缩放）
        QPixmap preview = QPixmap::fromImage(img);
        label->setPixmap(preview.scaledToWidth(600, Qt::SmoothTransformation));
    });





    connect(dialog, &WatermarkDialog::confirmed, this, [=](QString text, QFont font, QColor color, int opacity) {
        QImage img = originalPixmap.toImage().convertToFormat(QImage::Format_ARGB32);
        QPainter painter(&img);
        color.setAlpha(opacity);

        painter.setFont(font);
        painter.setPen(color);
        painter.drawText(QPoint(origX, origY), text);
        painter.end();

        originalPixmap = QPixmap::fromImage(img);

        currentPreviewPixmap = originalPixmap.scaledToWidth(600, Qt::SmoothTransformation);
        label->setPixmap(currentPreviewPixmap);

        dialog->close();
    });


    connect(dialog, &WatermarkDialog::cancelled, this, [=]() {
        label->setPixmap(currentPreviewPixmap);
        dialog->close();  // 关闭 WatermarkDialog
    });

    dialog->exec();
}

QPixmap ClickImageDialog::getFinalPixmap() const {
    return originalPixmap;
}
