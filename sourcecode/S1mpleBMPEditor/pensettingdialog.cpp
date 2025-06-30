#include "pensettingdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QColorDialog>
#include <QDialogButtonBox>

PenSettingDialog::PenSettingDialog(int currentWidth, QColor currentColor, int currentOpacity,QWidget *parent)
    : QDialog(parent), selectedColor(Qt::black)
{


    setWindowTitle("Pen Settings");


    setStyleSheet(R"(
        QWidget {
            background-color: white;
        }
        QLineEdit, QSpinBox {
            color: black;
            background-color: white;
            border: 1px solid black;
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


    widthSpinBox = new QSpinBox;
    widthSpinBox->setRange(1, 30);
    widthSpinBox->setValue(5);

    opacitySlider = new QSlider(Qt::Horizontal);
    opacitySlider->setRange(0, 255);
    opacitySlider->setValue(255);

    colorButton = new QPushButton("Choose");
    colorButton->setStyleSheet("background-color: black; color: white;");
    connect(colorButton, &QPushButton::clicked, this, &PenSettingDialog::chooseColor);

    QFormLayout *form = new QFormLayout;
    form->addRow("Pen Width", widthSpinBox);
    form->addRow("Transparency", opacitySlider);
    form->addRow("Pen Color", colorButton);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btnBox, &QDialogButtonBox::accepted, this, &PenSettingDialog::onConfirm);
    connect(btnBox, &QDialogButtonBox::rejected, this, &PenSettingDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addWidget(btnBox);

    widthSpinBox->setValue(currentWidth);
    colorButton->setStyleSheet(QString("background-color: %1").arg(currentColor.name()));
    opacitySlider->setValue(currentOpacity);
    selectedColor = currentColor;

    setLayout(mainLayout);
}

void PenSettingDialog::chooseColor()
{
    QColor c = QColorDialog::getColor(selectedColor, this, "Choose Pen Color");
    if (c.isValid()) {
        selectedColor = c;
        colorButton->setStyleSheet(QString("background-color: %1; color: black;").arg(c.name()));
    }
}

void PenSettingDialog::onConfirm()
{
    emit settingsConfirmed(widthSpinBox->value(), selectedColor, opacitySlider->value());
    accept();
}

int PenSettingDialog::getPenWidth() const { return widthSpinBox->value(); }
QColor PenSettingDialog::getPenColor() const { return selectedColor; }
int PenSettingDialog::getPenOpacity() const { return opacitySlider->value(); }
