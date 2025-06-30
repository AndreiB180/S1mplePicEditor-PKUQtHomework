#ifndef CONTRASTDIALOG_H
#define CONTRASTDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

class ContrastDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContrastDialog(QWidget *parent = nullptr);

signals:
    void contrastChanged(double factor); // 对比度变化，0.0 - 3.0

private:
    void setupUI();

    QSlider *slider;
    QLabel *valueLabel;
};

#endif // CONTRASTDIALOG_H
