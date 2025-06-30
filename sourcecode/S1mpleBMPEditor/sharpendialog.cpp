#include "sharpendialog.h"

SharpenDialog::SharpenDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();

    connect(slider, &QSlider::valueChanged, this, [=](int value){
        double strength = value / 10.0;
        valueLabel->setText(QString::number(strength, 'f', 1));
        emit sharpnessChanged(strength);  // 发射信号
    });
}

void SharpenDialog::setupUI()
{
    valueLabel = new QLabel("1.0", this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 50);       // 对应 0.0 到 5.0
    slider->setValue(10);          // 默认值 1.0

    valueLabel->setStyleSheet("color: black;");
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

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(valueLabel);
    layout->addWidget(slider);
    setLayout(layout);
    this->setStyleSheet("background-color: white;");
}
