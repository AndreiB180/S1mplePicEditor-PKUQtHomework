#include "mosaicdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

MosaicDialog::MosaicDialog(int initBlockSize, int initPenWidth, QWidget *parent)
    : QDialog(parent)
{
    setupUI();

    blockSpinBox->setValue(initBlockSize);
    penSpinBox->setValue(initPenWidth);

    // 设置统一样式
    setStyleSheet(R"(
        QWidget {
            background-color: white;
        }
        QLabel {
            color: black;
        }
        QSpinBox {
            color: black;
            background-color: white;
            border: 1px solid black;
            padding: 2px 6px;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            subcontrol-origin: border;
            width: 15px;
            background-color: white;
            border-left: 1px solid black;
        }
        QSpinBox::up-arrow, QSpinBox::down-arrow {
            width: 8px;
            height: 8px;
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

    connect(blockSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
        emit blockSizeChanged(value);
    });

    connect(penSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
        emit penWidthChanged(value);
    });

    connect(yesButton, &QPushButton::clicked, this, [=](){
        emit confirmed(blockSpinBox->value(), penSpinBox->value());
        accept();
    });

    connect(noButton, &QPushButton::clicked, this, [=](){
        emit cancelled();
        reject();
    });
}

void MosaicDialog::setupUI()
{
    blockLabel = new QLabel("Block Size:");
    penLabel   = new QLabel("Pen Width:");

    blockSpinBox = new QSpinBox;
    blockSpinBox->setRange(1, 100);
    blockSpinBox->setValue(10);

    penSpinBox = new QSpinBox;
    penSpinBox->setRange(1, 100);
    penSpinBox->setValue(20);

    yesButton = new QPushButton("Yes");
    noButton  = new QPushButton("No");

    QFormLayout *form = new QFormLayout;
    form->addRow(blockLabel, blockSpinBox);
    form->addRow(penLabel, penSpinBox);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(yesButton);
    btnLayout->addWidget(noButton);
    btnLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
}
