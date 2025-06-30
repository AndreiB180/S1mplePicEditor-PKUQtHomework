#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "importwindow.h"
#include "imagelabel.h"
#include "processimage.h"
#include "rotatedialog.h"
#include "adjustdialog.h"
#include "mosaicdialog.h"
#include "lightnessdialog.h"
#include "contrastdialog.h"
#include "saturationdialog.h"
#include "cropwindow.h"
#include "clickimagedialog.h"
#include "watermarkdialog.h"
#include "pensettingdialog.h"
#include "penwindow.h"
#include "tempdialog.h"
#include "sharpendialog.h"
#include "drawmosaicdialog.h"

#include <QJsonArray>
#include <QJsonDocument>
#include<QPainter>
#include<QPaintEvent>
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QWidget>
#include<QFileDialog>
#include<QStandardPaths>


QVector<QString> MainWindow::recentFiles;
QString MainWindow::recentFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/recent_files.json";





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("S1mplePicEditor");
    this->setFixedSize(800,600);
    autoResize = true;
    ui->resizeButton->setChecked(true);
    ui->resizeButton->setCheckable(true); // 自动缩放比例

    connect(ui->resizeButton, &QPushButton::toggled, this, [=](bool checked){
        autoResize = !checked;
        updateImageDisplay();
    });


    ui->functionalarea->setCurrentWidget(ui->default_page);

    loadRecentFiles();


    initButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setButtonStyle(QPushButton* button,const QString& filename){
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
void MainWindow::initButtons(){
    setButtonStyle(ui->rotateimage,":/icons/rotate.png");
    setButtonStyle(ui->rotate90,":/icons/rotate.png");
    setButtonStyle(ui->rotateimage,":/icons/rotate.png");
    setButtonStyle(ui->freerotate,":/icons/freerotate.png");
    setButtonStyle(ui->vertical_mirroring,":/icons/vertical.png");
    setButtonStyle(ui->horizonal_mirroring,":/icons/horizonal.png");
    setButtonStyle(ui->cutimage,":/icons/cut.png");
    setButtonStyle(ui->cut,":/icons/cut.png");
    setButtonStyle(ui->adjust,":/icons/adjust.png");
    setButtonStyle(ui->resizeButton,":/icons/resize.png");
    setButtonStyle(ui->reverse,":/icons/reverse.png");
    setButtonStyle(ui->mosaic,":/icons/mosaic.png");
    setButtonStyle(ui->mosaicsetting,":/icons/setting.png");
    setButtonStyle(ui->mosaic_button,":/icons/draw.png");
    setButtonStyle(ui->fullmosaic,":/icons/mosaic.png");
    setButtonStyle(ui->color,":/icons/color.png");
    setButtonStyle(ui->lightness_button,":/icons/lightness.png");
    setButtonStyle(ui->saturation_button,":/icons/saturation.png");
    setButtonStyle(ui->contrast_button,":/icons/contrast.png");
    setButtonStyle(ui->textlogo,":/icons/textlogo.png");
    setButtonStyle(ui->text_button,":/icons/textlogo.png");
    setButtonStyle(ui->greyscale,":/icons/greyscale.png");
    setButtonStyle(ui->grey,":/icons/greyscale.png");
    setButtonStyle(ui->binarize,":/icons/binarize.png");
    setButtonStyle(ui->exit,":/icons/exit.png");
    setButtonStyle(ui->undo,":/icons/undo.png");
    setButtonStyle(ui->save,":/icons/save.png");
    setButtonStyle(ui->cancel,":/icons/cancel.png");
    setButtonStyle(ui->confirm,":/icons/confirm.png");
    setButtonStyle(ui->drawbutton,":/icons/draw.png");
    setButtonStyle(ui->setting,":/icons/setting.png");
    setButtonStyle(ui->pen,":/icons/draw.png");
    setButtonStyle(ui->colorbutton,":/icons/temp.png");
    setButtonStyle(ui->sharpbutton,":/icons/sharpen.png");


    setButtonTip(ui->rotateimage, "旋转图片");
    setButtonTip(ui->rotate90, "旋转90度");
    setButtonTip(ui->freerotate, "自由旋转");
    setButtonTip(ui->vertical_mirroring, "垂直镜像");
    setButtonTip(ui->horizonal_mirroring, "水平镜像");
    setButtonTip(ui->cutimage, "剪裁图片");
    setButtonTip(ui->cut, "剪裁工具");
    setButtonTip(ui->adjust, "尺寸调节");
    setButtonTip(ui->resizeButton, "自适应显示");
    setButtonTip(ui->reverse, "颜色反转");
    setButtonTip(ui->mosaic, "马赛克");
    setButtonTip(ui->mosaicsetting,"马赛克设置");
    setButtonTip(ui->fullmosaic,"全屏马赛克");
    setButtonTip(ui->mosaic_button, "绘制马赛克");
    setButtonTip(ui->color, "颜色设置");
    setButtonTip(ui->lightness_button, "亮度调整");
    setButtonTip(ui->saturation_button, "饱和度调整");
    setButtonTip(ui->contrast_button, "对比度调整");
    setButtonTip(ui->textlogo, "添加文字水印");
    setButtonTip(ui->text_button, "添加水印");
    setButtonTip(ui->greyscale, "风格化处理");
    setButtonTip(ui->grey, "灰度图");
    setButtonTip(ui->binarize, "图像二值化");
    setButtonTip(ui->exit, "退出编辑");
    setButtonTip(ui->undo, "撤销操作");
    setButtonTip(ui->save, "保存图像");
    setButtonTip(ui->cancel, "取消操作");
    setButtonTip(ui->confirm, "确认操作");
    setButtonTip(ui->drawbutton,"画笔");
    setButtonTip(ui->setting,"画笔设置");
    setButtonTip(ui->pen,"绘画");
    setButtonTip(ui->colorbutton,"调节色温");
    setButtonTip(ui->sharpbutton,"调节锐度");

}

void MainWindow::setButtonTip(QPushButton* button,const QString& buttonword){//加载按钮文字

    button->setToolTip(buttonword);

}

void MainWindow::paintEvent(QPaintEvent *Event)//加载背景图
{
    QPainter painter(this);
    //注意下一行路径为背景图路径
    painter.drawPixmap(rect(),QPixmap(":/icons/whitebg.jpg"),QRect());
}

void MainWindow::loadImage(const QString& filePath){
    while(!historyStack.empty()){
        historyStack.pop();
    }
    QPixmap pix(filePath);
    if (pix.isNull()) {
        QMessageBox::warning(this, "打开失败", "无法加载该图片文件！");
        ImportWindow* imp=new ImportWindow();
        imp->show();
        this->close();
        return;
    }
    MainWindow::addToRecentFiles(filePath);
    originalPixmap = pix;//原始图像
    currentPixmap = pix;
    updateImageDisplay();  // 适应窗口大小显示

}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!autoResize && !currentPixmap.isNull()) {
        QPoint newOffset = offset;

        switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            newOffset.setY(newOffset.y() - moveStep);
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            newOffset.setY(newOffset.y() + moveStep);
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            newOffset.setX(newOffset.x() - moveStep);
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            newOffset.setX(newOffset.x() + moveStep);
            break;
        }

        // 限制 offset 不超过图像范围
        newOffset.setX(std::clamp(newOffset.x(), 0, std::max(0, currentPixmap.width() - ui->imagedisplay->width())));
        newOffset.setY(std::clamp(newOffset.y(), 0, std::max(0, currentPixmap.height() - ui->imagedisplay->height())));

        offset = newOffset;
        dynamic_cast<ImageLabel*>(ui->imagedisplay)->setOffset(offset);
    }
}


void MainWindow::updateImageDisplay() {
    if (currentPixmap.isNull()) return;

    auto *imgLabel = dynamic_cast<ImageLabel*>(ui->imagedisplay);
    imgLabel->setImage(currentPixmap);
    imgLabel->enableManualMode(!autoResize);

    if (autoResize) {
        offset = QPoint(0, 0); // 复位
    }
    imgLabel->setOffset(offset);
}

void MainWindow::on_exit_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("退出编辑");
    msgBox.setText("Save picture before exit?");

    // 清空默认按钮
    msgBox.setStandardButtons(QMessageBox::NoButton);

    // 手动创建按钮，保证顺序和文本
    QPushButton *saveBtn = msgBox.addButton(QStringLiteral("Save"), QMessageBox::AcceptRole);
    QPushButton *discardBtn = msgBox.addButton(QStringLiteral("Don't save"), QMessageBox::DestructiveRole);
    QPushButton *cancelBtn = msgBox.addButton(QStringLiteral("Cancel"), QMessageBox::RejectRole);

    msgBox.setDefaultButton(saveBtn);

    // 设置样式表，所有文字黑色，背景白色，按钮也黑字白底带悬停
    msgBox.setStyleSheet(R"(
        QMessageBox {
            background-color: white;
            color: black;
            font-size: 13px;
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
    )");

    msgBox.exec();

    QAbstractButton *clickedBtn = msgBox.clickedButton();

    if (clickedBtn == saveBtn) {
        QString filePath = QFileDialog::getSaveFileName(
            this,
            tr("保存图像"),
            "",
            tr("PNG 文件 (*.png);;BMP 文件 (*.bmp);;JPEG 文件 (*.jpg *.jpeg)")
            );

        if (!filePath.isEmpty()) {
            if (!currentPixmap.save(filePath)) {
                QMessageBox::warning(this, tr("保存失败"), tr("无法保存图像到 %1").arg(filePath));
                return; // 保存失败，不退出
            } else {
                QMessageBox::information(this, tr("保存成功"), tr("图像已保存到：\n%1").arg(filePath));
            }
        } else {
            return; // 用户取消保存对话框，不退出
        }

        ImportWindow *imp = new ImportWindow();
        imp->show();
        this->close();
    }
    else if (clickedBtn == discardBtn) {
        // 不保存，直接退出
        ImportWindow *imp = new ImportWindow();
        imp->show();
        this->close();
    }
    else if (clickedBtn == cancelBtn) {
        // 取消，不关闭窗口
        // 什么都不做
    }
}




void MainWindow::on_rotateimage_clicked()
{
    ui->functionalarea->setCurrentWidget(ui->rotate_page);
}

void MainWindow::on_cutimage_clicked()
{
    ui->functionalarea->setCurrentWidget(ui->cut_page);
}


void MainWindow::on_greyscale_clicked()
{
    ui->functionalarea->setCurrentWidget(ui->greyscale_page);
}


void MainWindow::on_mosaic_clicked()
{
    ui->functionalarea->setCurrentWidget(ui->mosaic_page);
}

void MainWindow::on_color_clicked()
{
    ui->functionalarea->setCurrentWidget(ui->color_page);
}


void MainWindow::on_textlogo_clicked()
{
    ui->functionalarea->setCurrentWidget(ui->textlogo_page);
}

void MainWindow::on_drawbutton_clicked()
{
    ui->functionalarea->setCurrentWidget(ui->draw_page);
}


void MainWindow::on_undo_clicked()
{
    if (historyStack.empty()) {
        return;
    }

    currentPixmap = historyStack.top();
    originalPixmap = historyStack.top();
    historyStack.pop();
    updateImageDisplay();
}


void MainWindow::on_save_clicked()
{
    // 弹出保存对话框
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("保存图像"),
        "",
        tr("PNG 文件 (*.png);;BMP 文件 (*.bmp);;JPEG 文件 (*.jpg *.jpeg)")
        );

    // 如果用户取消保存，filePath 会是空的
    if (filePath.isEmpty())
        return;

    // 尝试保存
    if (!currentPixmap.save(filePath)) {
        QMessageBox::warning(this, tr("保存失败"), tr("无法保存图像到 %1").arg(filePath));
    } else {
        QMessageBox::information(this, tr("保存成功"), tr("图像已保存到：\n%1").arg(filePath));
    }
}


void MainWindow::on_cancel_clicked()
{
    currentPixmap = originalPixmap;
    ui->imagedisplay->setImage(currentPixmap);
}


void MainWindow::on_confirm_clicked()
{
    historyStack.push(originalPixmap); // 先保存
    originalPixmap = currentPixmap;
}


void MainWindow::on_horizonal_mirroring_clicked()
{
    originalPixmap=currentPixmap;
    QPixmap preview = ProcessImage::flipHorizontal(currentPixmap);
    currentPixmap=preview;
    ui->imagedisplay->setImage(preview);
}


void MainWindow::on_rotate90_clicked()
{
    originalPixmap=currentPixmap;
    QPixmap preview = ProcessImage::rotate90(currentPixmap);
    currentPixmap = preview;
    ui->imagedisplay->setImage(currentPixmap);  // 更新显示
}


void MainWindow::on_vertical_mirroring_clicked()
{
    originalPixmap=currentPixmap;
    QPixmap preview = ProcessImage::flipVertical(currentPixmap);
    currentPixmap=preview;
    ui->imagedisplay->setImage(preview);
}


void MainWindow::on_freerotate_clicked()
{
    if (currentPixmap.isNull())
        return;

    RotateDialog *dialog = new RotateDialog(this);


    connect(dialog, &RotateDialog::angleChanged, this, [=](int angle){
        currentPixmap = ProcessImage::rotate(originalPixmap, angle);

        updateImageDisplay();

    });

    dialog->exec(); // 显示旋转对话框
    delete dialog;

}




void MainWindow::on_adjust_clicked()
{
    if (currentPixmap.isNull())
        return;

    AdjustDialog *dialog = new AdjustDialog(this);


    connect(dialog, &AdjustDialog::factorChanged, this, [=](double factor){
        currentPixmap = ProcessImage::scale(originalPixmap, factor);

        updateImageDisplay();

    });

    dialog->exec(); // 显示对话框
    delete dialog;
}


void MainWindow::on_cut_clicked()
{
    CropWindow *cropWin = new CropWindow(currentPixmap, nullptr);

    connect(cropWin, &CropWindow::croppedPixmap, this, [=](const QPixmap &croppedPix) {
        currentPixmap = croppedPix;
        updateImageDisplay();
        cropWin->deleteLater();
    });

    cropWin->show();
}



void MainWindow::on_grey_clicked()
{
    QPixmap src = currentPixmap; // 当前显示的图
    currentPixmap = ProcessImage::grayscale(src);
    updateImageDisplay();
}

void MainWindow::on_binarize_clicked()
{
    QPixmap src = currentPixmap;
    currentPixmap = ProcessImage::binarize(src, 128);
    updateImageDisplay();
}

void MainWindow::on_reverse_clicked()
{
    QPixmap src = currentPixmap;
    currentPixmap = ProcessImage::invert(src);
    updateImageDisplay();
}


void MainWindow::on_mosaic_button_clicked()
{
    DrawMosaicDialog *dialog = new DrawMosaicDialog(currentPixmap, this);
    dialog->setPenWidth(mosaicPenWidth);
    dialog->setBlockSize(mosaicBlockSize);


    connect(dialog, &DrawMosaicDialog::mosaicConfirmed, this, [=](const QPixmap &pix){
        currentPixmap = pix;
        updateImageDisplay();
    });

    dialog->exec();
}


void MainWindow::on_mosaicsetting_clicked()
{
    MosaicDialog *dialog = new MosaicDialog(mosaicBlockSize, mosaicPenWidth, this);

    connect(dialog, &MosaicDialog::confirmed, this, [=](int blockSize, int penWidth){
        mosaicBlockSize = blockSize;
        mosaicPenWidth = penWidth;
        qDebug() << "Mosaic config updated:" << blockSize << penWidth;
    });

    dialog->exec();
}




void MainWindow::on_fullmosaic_clicked()
{
    // 使用ProcessImage::mosaic对currentPixmap进行马赛克处理
    QPixmap mosaiced = ProcessImage::mosaic(currentPixmap, mosaicBlockSize);

    // 更新当前图片
    currentPixmap = mosaiced;

    // 更新显示界面
    updateImageDisplay();
}














void MainWindow::on_lightness_button_clicked()
{
    if (currentPixmap.isNull())
        return;

    LightnessDialog *dialog = new LightnessDialog(this);
    connect(dialog, &LightnessDialog::lightnessChanged, this, [=](int delta){
        currentPixmap = ProcessImage::adjustLightness(originalPixmap, delta);
        updateImageDisplay();
    });
    dialog->exec();
    delete dialog;
}





void MainWindow::on_saturation_button_clicked()
{
    if (currentPixmap.isNull())
        return;

    SaturationDialog *dialog = new SaturationDialog(this);
    connect(dialog, &SaturationDialog::saturationChanged, this, [=](int delta){
        currentPixmap = ProcessImage::adjustSaturation(originalPixmap, delta);
        updateImageDisplay();
    });
    dialog->exec();
    delete dialog;
}


void MainWindow::on_contrast_button_clicked()
{
    if (currentPixmap.isNull())
        return;

    ContrastDialog *dialog = new ContrastDialog(this);
    connect(dialog, &ContrastDialog::contrastChanged, this, [=](int delta){
        currentPixmap = ProcessImage::adjustContrast(originalPixmap, delta);
        updateImageDisplay();
    });
    dialog->exec();
    delete dialog;
}




void MainWindow::on_text_button_clicked()
{
    ClickImageDialog *dialog = new ClickImageDialog(currentPixmap, this);
    connect(dialog, &ClickImageDialog::watermarkConfirmed, this, [=](const QPixmap &pix){
        currentPixmap = pix;
        updateImageDisplay();
        dialog->close(); // 主动关闭对话框
    });
    connect(dialog, &ClickImageDialog::watermarkCancelled, this, [=]() {
        // 取消水印时的额外处理
        // 对话框保持打开
    });

    dialog->exec();
}


void MainWindow::onWatermarkPositionSelected(QPoint pos) {
    watermarkPos = pos;
    // 弹出设置对话框
    WatermarkDialog *dialog = new WatermarkDialog(this);
    connect(dialog, &WatermarkDialog::confirmed, this, &MainWindow::onWatermarkConfirmed);
    dialog->exec();
}


void MainWindow::onWatermarkConfirmed(QString text, QFont font, QColor color, int opacity) {
    QImage img = currentPixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(font);

    QColor textColor = color;
    textColor.setAlpha(opacity);
    painter.setPen(textColor);

    painter.drawText(watermarkPos, text);
    painter.end();

    currentPixmap = QPixmap::fromImage(img);
    updateImageDisplay();
}

void MainWindow::loadRecentFiles() {
    QFile file(recentFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        recentFiles.clear();  // 清空
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray array = doc.array();
        for (const auto& val : array)
            recentFiles.append(val.toString());
    }
}

void MainWindow::saveRecentFiles() {
    QJsonArray array;
    for (const auto& path : recentFiles)
        array.append(path);

    QJsonDocument doc(array);
    QDir().mkpath(QFileInfo(recentFilePath).absolutePath());

    QFile file(recentFilePath);
    if (file.open(QIODevice::WriteOnly))
        file.write(doc.toJson());
}

void MainWindow::addToRecentFiles(const QString& filePath) {
    recentFiles.removeAll(filePath);  // 去重
    recentFiles.prepend(filePath);    // 插入最前
    while (recentFiles.size() > MAX_RECENT)
        recentFiles.removeLast();
    saveRecentFiles(); // 添加文件后立即保存
}





void MainWindow::on_setting_clicked()
{
    PenSettingDialog *dialog = new PenSettingDialog(penWidth, penColor, penOpacity, this);

    connect(dialog, &PenSettingDialog::settingsConfirmed, this, [=](int width, QColor color, int opacity) {
        this->penWidth = width;
        this->penColor = color;
        this->penOpacity = opacity;
    });

    dialog->exec();
    dialog->deleteLater();
}


void MainWindow::on_pen_clicked()
{
    PenWindow *penWin = new PenWindow(currentPixmap, penColor, penWidth, penOpacity, this);

    connect(penWin, &PenWindow::drawingConfirmed, this, [=](const QPixmap &result){
        currentPixmap = result;
        updateImageDisplay();
        penWin->deleteLater();
    });

    connect(penWin, &PenWindow::drawingCancelled, penWin, &PenWindow::deleteLater);

    penWin->exec();  //show()
}



void MainWindow::on_colorbutton_clicked()
{
    if (currentPixmap.isNull())
        return;

    TempDialog *dialog = new TempDialog(this);
    connect(dialog, &TempDialog::temperatureChanged, this, [=](int delta){
        currentPixmap = ProcessImage::adjustColorTemperature(originalPixmap, delta);
        updateImageDisplay();
    });
    dialog->exec();
    delete dialog;
}



void MainWindow::on_sharpbutton_clicked()
{
    if (currentPixmap.isNull())
        return;

    SharpenDialog *dialog = new SharpenDialog(this);
    connect(dialog, &SharpenDialog::sharpnessChanged, this, [=](int strength){
        currentPixmap = ProcessImage::sharpen(originalPixmap, strength);
        updateImageDisplay();
    });
    dialog->exec();
    delete dialog;
}






