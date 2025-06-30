#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

class RotateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RotateDialog(QWidget *parent = nullptr);

signals:
    void angleChanged(int angle); // 实时发出旋转角度信号

private:
    void setupUI();  // 新增初始化界面函数

    QSlider *slider;
    QLabel *angleLabel;
};

#endif // ROTATEDIALOG_H
