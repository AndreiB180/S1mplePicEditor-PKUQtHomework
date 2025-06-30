#include "watermarkdialog.h"
#include <QFormLayout>
#include <QColorDialog>
#include <QHBoxLayout>

WatermarkDialog::WatermarkDialog(QWidget *parent)
    : QDialog(parent),selectedColor(Qt::white)
{
    setWindowTitle("Add Watermark");


    setStyleSheet(R"(
        QWidget {
            background-color: white;
        }
        QLineEdit {
            color: black;
            background-color: white;
            border: 1px solid black;
        }
        QSpinBox {
            color: black;
            background-color: white;
            border: 1px solid black;
        }
        QSpinBox::up-button, QSpinBox::down-button {
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



    textEdit = new QLineEdit;
    fontSizeSpin = new QSpinBox;
    fontSizeSpin->setRange(8, 72);
    fontSizeSpin->setValue(20);

    colorButton = new QPushButton("Choose");
    opacitySlider = new QSlider(Qt::Horizontal);
    opacitySlider->setRange(0, 255);
    opacitySlider->setValue(128);

    connect(colorButton, &QPushButton::clicked, this, &WatermarkDialog::chooseColor);

    QFormLayout *form = new QFormLayout;
    form->addRow("Watermark Text", textEdit);
    form->addRow("Text size", fontSizeSpin);
    form->addRow("Text color", colorButton);
    form->addRow("Transparency", opacitySlider);

    QPushButton *yesButton = new QPushButton("Yes");
    QPushButton *noButton  = new QPushButton("No");

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(yesButton);
    btnLayout->addWidget(noButton);
    btnLayout->addStretch();

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);

    connect(yesButton, &QPushButton::clicked, this, &WatermarkDialog::onYes);
    connect(noButton, &QPushButton::clicked, this, &WatermarkDialog::onNo);

    connect(textEdit, &QLineEdit::textChanged, this, &WatermarkDialog::emitPreview);
    connect(fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &WatermarkDialog::emitPreview);
    connect(opacitySlider, &QSlider::valueChanged, this, &WatermarkDialog::emitPreview);
}

void WatermarkDialog::chooseColor()
{
    QColor c = QColorDialog::getColor(selectedColor, this, "Choose color");
    if (c.isValid()) {
        selectedColor = c;
        colorButton->setStyleSheet(QString("background-color: %1; color: black;").arg(c.name()));
        emitPreview();
    }

}

void WatermarkDialog::onYes()
{
    QFont font;
    font.setPointSize(fontSizeSpin->value());
    emit confirmed(textEdit->text(), font, selectedColor, opacitySlider->value());
    // **这里不调用 close()，外部决定是否关闭**
}

void WatermarkDialog::onNo()
{
    emit cancelled();
    // **这里也不调用 close()，外部决定关闭**
}

void WatermarkDialog::emitPreview()
{
    QFont font;
    font.setPointSize(fontSizeSpin->value());
    emit previewUpdated(textEdit->text(), font, selectedColor, opacitySlider->value());
}
