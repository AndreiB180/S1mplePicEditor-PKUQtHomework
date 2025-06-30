#ifndef ADJUSTDIALOG_H
#define ADJUSTDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

class AdjustDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdjustDialog(QWidget *parent = nullptr);

signals:
    void factorChanged(double factor); // 实时发出旋转角度信号

private:
    void setupUI();  // 新增初始化界面函数

    QSlider *slider;
    QLabel *factorLabel;
};

#endif
