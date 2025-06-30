#include "rotatedialog.h"

RotateDialog::RotateDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();

    // 信号连接，滑块值变化实时发出角度信号
    connect(slider, &QSlider::valueChanged, this, [=](int value){
        angleLabel->setText(QString::number(value) + "°");
        emit angleChanged(value);
    });
}

void RotateDialog::setupUI()
{
    // 创建控件
    angleLabel = new QLabel("0°", this);
    slider = new QSlider(Qt::Horizontal, this);

    slider->setRange(-180, 180);
    slider->setValue(0);

    // 设置字体颜色为黑色
    angleLabel->setStyleSheet("color: black;");

    // 设置slider黑色样式
    slider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #999999;"
        "    height: 8px;"
        "    background: black;"
        "    margin: 2px 0;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: black;"
        "    border: 1px solid black;"
        "    width: 18px;"
        "    margin: -2px 0;"
        "    border-radius: 3px;"
        "}"
        );

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(angleLabel);
    layout->addWidget(slider);

    setLayout(layout);

    // 设置对话框背景为白色
    this->setStyleSheet("background-color: white;");
}
