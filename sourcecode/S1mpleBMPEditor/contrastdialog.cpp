#include "contrastdialog.h"

ContrastDialog::ContrastDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();

    connect(slider, &QSlider::valueChanged, this, [=](int value){
        double factor = value / 100.0;
        valueLabel->setText(QString::number(factor, 'f', 2));
        emit contrastChanged(value);
    });
}

void ContrastDialog::setupUI()
{
    valueLabel = new QLabel("1.00", this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 300); // 0.00 ~ 3.00
    slider->setValue(100);    // 默认 1.00 倍

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
