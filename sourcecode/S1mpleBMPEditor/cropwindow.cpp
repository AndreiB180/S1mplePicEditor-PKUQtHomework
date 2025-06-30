#include "cropwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

CropWindow::CropWindow(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent), originalPixmap(pixmap), rubberBand(nullptr)
{
    setWindowTitle("Preview");

    this->setStyleSheet(R"(
    CropWindow {
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

    int w = pixmap.width();
    int h = pixmap.height();

    if (w > h) {
        scaleFactor = 600.0 / w;
    } else {
        scaleFactor = 600.0 / h;
    }

    displayPixmap = originalPixmap.scaled(w * scaleFactor, h * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 设置固定大小为图片显示大小 + 按钮区高度
    setFixedSize(displayPixmap.width(), displayPixmap.height() + 50);

    // 按钮
    QPushButton* yesBtn = new QPushButton("Yes", this);
    QPushButton* noBtn = new QPushButton("No", this);

    yesBtn->setFixedSize(80, 30);
    noBtn->setFixedSize(80, 30);

    connect(yesBtn, &QPushButton::clicked, this, &CropWindow::onYesClicked);
    connect(noBtn, &QPushButton::clicked, this, &CropWindow::onNoClicked);

    // 布局管理：画布 + 按钮
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(5);

    // 一个占位widget只用来放图片和捕获鼠标事件
    imageArea = new QWidget(this);
    imageArea->setFixedSize(displayPixmap.size());
    imageArea->setMouseTracking(true);

    mainLayout->addWidget(imageArea);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();      // 左侧弹簧，推动按钮靠中
    buttonLayout->addWidget(yesBtn);
    buttonLayout->addWidget(noBtn);
    buttonLayout->addStretch();      // 右侧弹簧，推动按钮靠中
    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(10, 0, 10, 10);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);




    // 初始化 rubberBand
    rubberBand = new QRubberBand(QRubberBand::Rectangle, imageArea);

    // 初始化选区为空
    currentSelection = QRect();
}

void CropWindow::mousePressEvent(QMouseEvent *event)
{
    if (!imageArea->rect().contains(event->pos()))
        return;

    origin = event->pos();
    // 相对于 imageArea 的坐标
    QPoint imgOrigin = event->pos();

    rubberBand->setGeometry(QRect(imgOrigin, QSize()));
    rubberBand->show();
}

void CropWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBand->isVisible()) {
        QPoint imgCurrent = event->pos();
        rubberBand->setGeometry(QRect(origin, imgCurrent).normalized());
    }
}

void CropWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (rubberBand->isVisible())
    {
        rubberBand->hide();

        QRect selectedRect = rubberBand->geometry();
        // 保存当前选区
        currentSelection = selectedRect;
        update();
    }
}

void CropWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 绘制背景图到 imageArea 的区域
    painter.drawPixmap(0, 0, displayPixmap);

    // 绘制选区高亮（半透明蓝色覆盖）
    if (currentSelection.isValid()) {
        QColor overlayColor(0, 0, 255, 100); // 半透明蓝
        painter.fillRect(currentSelection, overlayColor);
        painter.setPen(QPen(Qt::blue, 2));
        painter.drawRect(currentSelection);
    }
}

// 按钮槽函数
void CropWindow::onYesClicked()
{
    if (!currentSelection.isValid()) return;

    // 把选区映射回原图
    QRect origRect(
        currentSelection.x() / scaleFactor,
        currentSelection.y() / scaleFactor,
        currentSelection.width() / scaleFactor,
        currentSelection.height() / scaleFactor);

    QRect validRect = origRect.intersected(originalPixmap.rect());
    QPixmap cropped = originalPixmap.copy(validRect);

    emit croppedPixmap(cropped);
    close();
}

void CropWindow::onNoClicked()
{
    // 取消当前选区
    currentSelection = QRect();
    update();
}
