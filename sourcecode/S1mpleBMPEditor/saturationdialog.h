#ifndef SATURATIONDIALOG_H
#define SATURATIONDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

class SaturationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaturationDialog(QWidget *parent = nullptr);

signals:
    void saturationChanged(double factor); // 饱和度变化，0.0 - 3.0

private:
    void setupUI();

    QSlider *slider;
    QLabel *valueLabel;
};

#endif // SATURATIONDIALOG_H
