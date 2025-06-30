#include "adjustdialog.h"
#include<cmath>

AdjustDialog::AdjustDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();

    // 信号连接，滑块值变化实时发出角度信号
    connect(slider, &QSlider::valueChanged, this, [=](int value){
        double scaleLog = std::log10(5.0); // 最大放大倍数对应的log值
        double delta = (value - 50) / 50.0; // 中点对称归一化范围 [-1, 1]
        double factor = std::pow(10.0, delta * scaleLog);

        factorLabel->setText(QString::number(factor, 'f', 2));
        emit factorChanged(factor);
    });
}

void AdjustDialog::setupUI()
{
    // 创建控件
    factorLabel = new QLabel("1.0", this);
    slider = new QSlider(Qt::Horizontal, this);

    slider->setRange(0,100);
    slider->setValue(50);

    // 设置字体颜色为黑色
    factorLabel->setStyleSheet("color: black;");

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
    layout->addWidget(factorLabel);
    layout->addWidget(slider);

    setLayout(layout);

    // 设置对话框背景为白色
    this->setStyleSheet("background-color: white;");
}
