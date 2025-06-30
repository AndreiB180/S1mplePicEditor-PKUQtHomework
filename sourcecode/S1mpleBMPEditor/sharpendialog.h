#ifndef SHARPENDIALOG_H
#define SHARPENDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

class SharpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SharpenDialog(QWidget *parent = nullptr);

signals:
    void sharpnessChanged(double strength);  // 信号

private:
    void setupUI();

    QSlider *slider;
    QLabel *valueLabel;
};

#endif // SHARPENDIALOG_H
