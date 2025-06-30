#ifndef TEMPDIALOG_H
#define TEMPDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

class TempDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TempDialog(QWidget *parent = nullptr);

signals:
    void temperatureChanged(int delta);

private:
    void setupUI();

    QSlider *slider;
    QLabel *valueLabel;
};

#endif // TEMPDIALOG_H
