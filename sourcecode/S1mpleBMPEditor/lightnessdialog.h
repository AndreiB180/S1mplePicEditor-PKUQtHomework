#ifndef LIGHTNESSDIALOG_H
#define LIGHTNESSDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

class LightnessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LightnessDialog(QWidget *parent = nullptr);

signals:
    void lightnessChanged(int delta);

private:
    void setupUI();

    QSlider *slider;
    QLabel *valueLabel;
};

#endif // LIGHTNESSDIALOG_H
