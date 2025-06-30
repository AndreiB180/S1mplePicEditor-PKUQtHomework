#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include<QString>
#include<QPushButton>
#include<QDialog>
#include<stack>
#include<QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void paintEvent(QPaintEvent *Event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadImage(const QString& filePath);
    friend class ImportWindow;
    void updateImageDisplay();

    int getPenWidth() const { return penWidth; }
    QColor getPenColor() const { return penColor; }
    int getPenOpacity() const { return penOpacity; }

    void setPenWidth(int w) { penWidth = w; }
    void setPenColor(QColor c) { penColor = c; }
    void setPenOpacity(int o) { penOpacity = o; }


private slots:
    void on_exit_clicked();

    void on_rotateimage_clicked();

    void on_cutimage_clicked();

    void on_greyscale_clicked();

    void on_mosaic_clicked();

    void on_contrast_button_clicked();

    void on_lightness_button_clicked();

    void on_textlogo_clicked();

    void on_undo_clicked();

    void on_save_clicked();

    void on_cancel_clicked();

    void on_confirm_clicked();

    void on_horizonal_mirroring_clicked();

    void on_rotate90_clicked();

    void on_vertical_mirroring_clicked();

    void on_freerotate_clicked();

    void on_adjust_clicked();

    void on_cut_clicked();

    void on_grey_clicked();

    void on_binarize_clicked();

    void on_reverse_clicked();

    void on_mosaic_button_clicked();

    void on_color_clicked();

    void on_saturation_button_clicked();

    void on_text_button_clicked();


    void on_drawbutton_clicked();

    void on_setting_clicked();

    void on_pen_clicked();

    void on_colorbutton_clicked();

    void on_sharpbutton_clicked();

    void on_mosaicsetting_clicked();

    void on_fullmosaic_clicked();

private:

    QPixmap originalPixmap;
    QPixmap currentPixmap;
    bool autoResize = false;
    void refreshimage();
    QPoint offset = QPoint(0, 0);  // 图像左上角相对于 label 的偏移
    QPoint watermarkPos;
    const int moveStep = 10;       // 每次移动的像素数量
    void keyPressEvent(QKeyEvent *event);
    Ui::MainWindow *ui;
    void setButtonStyle(QPushButton* button,const QString& filename);

    void setButtonTip(QPushButton* button,const QString& buttonword);

    void initButtons();
    void onWatermarkPositionSelected(QPoint pos);
    void onWatermarkConfirmed(QString text, QFont font, QColor color, int opacity);
    std::stack<QPixmap> historyStack;
    static const int MAX_RECENT = 20;
    static QVector<QString> recentFiles;
    static QString recentFilePath;

    static void loadRecentFiles();
    static void saveRecentFiles();
    static void addToRecentFiles(const QString& filePath);

    int penWidth = 5;             // 默认画笔宽度
    QColor penColor = Qt::black;  // 默认画笔颜色
    int penOpacity = 255;         // 默认不透明

    int mosaicPenWidth = 20;      // 马赛克画笔粗细
    int mosaicBlockSize = 10;     // 马赛克格子大小


};


#endif // MAINWINDOW_H
