#include "importwindow.h"
#include "ui_importwindow.h"
#include "mainwindow.h"
#include "recentfileswindow.h"

#include<QCoreApplication>
#include<QImage>
#include<QPixmap>
#include<QPaintEvent>
#include<QPainter>
#include<QPalette>
#include<QFileDialog>
#include<QMessageBox>

#include<QIcon>
#include<QFile>

ImportWindow::ImportWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImportWindow)
{
    ui->setupUi(this);
    setWindowTitle("S1mplePicEditor");
    this->setFixedSize(320,240);
    ui->label->setStyleSheet("color: black;");

    initButtons();

}

void ImportWindow::setButtonStyle(QPushButton* button,const QString& filename){
    button->setFixedSize(50,50);
    button->setIcon(QIcon(filename));
    button->setIconSize(QSize(button->width(),button->height()));
    button->setStyleSheet("background-color:transparent");

    button->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: none;
        }
        QPushButton:hover {
            background-color: rgba(200, 200, 200, 80); /* 鼠标悬停时变灰 */
        }
    )");
}
void ImportWindow::initButtons(){
    setButtonStyle(ui->newpic,":/icons/newpic.png");
    setButtonStyle(ui->openpic,":/icons/openpic.png");
    setButtonStyle(ui->exit,":/icons/exit.png");

    setButtonTip(ui->newpic, "打开新图片");
    setButtonTip(ui->openpic, "打开最近图片");
    setButtonTip(ui->exit, "退出程序");
}


void ImportWindow::setButtonTip(QPushButton* button,const QString& buttonword){//加载按钮文字

    button->setToolTip(buttonword);

}


ImportWindow::~ImportWindow()
{
    delete ui;
}


void ImportWindow::paintEvent(QPaintEvent *Event)//加载背景图
{
    QPainter painter(this);
    //注意下一行路径为背景图路径
    painter.drawPixmap(rect(),QPixmap(":/icons/mainlogo.jpg"),QRect());
}


void ImportWindow::on_exit_clicked()
{
    exit(0);
}



void ImportWindow::on_newpic_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "打开图片文件",
        QDir::homePath(),
        "图片文件 (*.bmp *.png *.jpg *.jpeg *.gif *.webp *.tiff);;所有文件 (*.*)"
        );

    if (fileName.isEmpty()) {
        return; // 用户取消
    }

    // 打开主界面
    MainWindow *mainWin = new MainWindow();
    mainWin->show();

    // 加载图片文件（loadBMP可以改成loadImage或统一处理多格式）
    mainWin->loadImage(fileName);  // 确保你实现了这个函数，支持不同格式

    this->close(); // 关闭当前导入窗口
}


void ImportWindow::on_openpic_clicked()
{
    // 先加载 recentFiles（如你是在 main() 初始化就无须重复）
    MainWindow::loadRecentFiles();

    // 弹出 recent files 选择窗口
    RecentFilesWindow* win = new RecentFilesWindow(MainWindow::recentFiles, this);

    connect(win, &RecentFilesWindow::fileSelected, this, [=](const QString& path){

        MainWindow::addToRecentFiles(path);


        MainWindow *mainWindowPtr = new MainWindow();
        mainWindowPtr->show();
        mainWindowPtr->loadImage(path);

        this->close();
    });

    connect(win, &RecentFilesWindow::clearRequested, this, [=]() {
        MainWindow::recentFiles.clear();
        QFile::remove(MainWindow::recentFilePath);
        MainWindow::saveRecentFiles();  // 同步保存
    });

    win->exec();
}


