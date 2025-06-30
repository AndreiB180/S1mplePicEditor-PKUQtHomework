#include "recentfileswindow.h"
#include "clickablewidget.h"
#include "mainwindow.h"

#include <QMessageBox>

#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QPixmap>
#include <QScrollArea>
#include <QGridLayout>
#include <QMouseEvent>
#include <QFile>
#include <QFrame>
#include <QPushButton>



RecentFilesWindow::RecentFilesWindow(const QVector<QString>& recentFiles, QWidget* parent)
    : QDialog(parent)
{
    this->setWindowTitle("Recent Files");
    this->setStyleSheet(R"(
        QDialog {
            background-color: white;
        }
        QLabel {
            color: black;
            font-size: 12px;
        }
    )");
    this->resize(320, 240);

    // 滚动区域
    QScrollArea* scrollArea = new QScrollArea(this);
    QWidget* container = new QWidget;
    QGridLayout* layout = new QGridLayout(container);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    int row = 0, col = 0;
    for (const QString& path : recentFiles) {
        if (!QFile::exists(path)) continue;

        QPixmap pix(path);
        if (pix.isNull()) continue;

        QPixmap thumb = pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QLabel* thumbLabel = new QLabel;
        thumbLabel->setPixmap(thumb);
        thumbLabel->setFixedSize(100, 100);
        thumbLabel->setFrameStyle(QFrame::Box);

        QLabel* nameLabel = new QLabel(QFileInfo(path).fileName());
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setFixedWidth(100);
        nameLabel->setWordWrap(true);
        nameLabel->setStyleSheet("color: black;");

        QVBoxLayout* itemLayout = new QVBoxLayout;
        itemLayout->addWidget(thumbLabel);
        itemLayout->addWidget(nameLabel);

        ClickableWidget* itemWidget = new ClickableWidget(path);
        itemWidget->setLayout(itemLayout);
        itemWidget->setCursor(Qt::PointingHandCursor);

        connect(itemWidget, &ClickableWidget::clicked, this, [=](const QString& selectedPath){
            emit fileSelected(selectedPath);
            this->accept();  // 关闭窗口
        });

        layout->addWidget(itemWidget, row, col);
        if (++col >= 2) { col = 0; row++; }
    }

    container->setLayout(layout);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    scrollArea->viewport()->setStyleSheet("background-color: white;");
    container->setStyleSheet("background-color: white;");

    // 清空按钮
    QPushButton* clearButton = new QPushButton("Clear");
    clearButton->setStyleSheet("background-color: #e53935; color: white; padding: 6px 12px; border: none; border-radius: 5px;");
    clearButton->setFixedHeight(32);
    connect(clearButton, &QPushButton::clicked, this, [=]() {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Confirm Clear");
        msgBox.setText("Sure to clear?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        // 设置样式表，黑色文字，白色背景，适配Mac
        msgBox.setStyleSheet(R"(
        QMessageBox {
            background-color: white;
            color: black;
            font-size: 13px;
        }
        QPushButton {
            color: black;
            background-color: white;
            border: 1px solid gray;
            padding: 5px 15px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #d3d3d3;
        }
    )");

        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes) {
            emit clearRequested();
            this->accept();
        }
        // No 直接返回，什么都不做
    });


    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(clearButton, 0, Qt::AlignCenter);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    setLayout(mainLayout);
}
