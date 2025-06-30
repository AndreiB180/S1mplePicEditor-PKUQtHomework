#include "tempdialog.h"

TempDialog::TempDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();

    connect(slider, &QSlider::valueChanged, this, [=](int value){
        valueLabel->setText(QString::number(value));
        emit temperatureChanged(value);
    });
}

void TempDialog::setupUI()
{
    valueLabel = new QLabel("0", this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(-100, 100);
    slider->setValue(0);

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
