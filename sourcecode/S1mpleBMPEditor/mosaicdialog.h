#ifndef MOSAICDIALOG_H
#define MOSAICDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class MosaicDialog : public QDialog
{
    Q_OBJECT

public:

    explicit MosaicDialog(int initBlockSize, int initPenWidth, QWidget *parent = nullptr);


signals:
    void blockSizeChanged(int blockSize);
    void penWidthChanged(int penWidth);
    void confirmed(int blockSize, int penWidth);
    void cancelled();

private:
    void setupUI();

    QLabel *blockLabel;
    QLabel *penLabel;
    QSpinBox *blockSpinBox;
    QSpinBox *penSpinBox;
    QPushButton *yesButton;
    QPushButton *noButton;
};

#endif // MOSAICDIALOG_H
