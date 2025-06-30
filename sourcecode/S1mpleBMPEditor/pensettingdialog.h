#ifndef PENSETTINGDIALOG_H
#define PENSETTINGDIALOG_H

#include <QDialog>
#include <QColor>

class QSpinBox;
class QSlider;
class QPushButton;

class PenSettingDialog : public QDialog {
    Q_OBJECT
public:
    explicit PenSettingDialog(int currentWidth, QColor currentColor, int currentOpacity,QWidget *parent = nullptr);

    int getPenWidth() const;
    QColor getPenColor() const;
    int getPenOpacity() const;

signals:
    void settingsConfirmed(int width, QColor color, int opacity);

private slots:
    void chooseColor();
    void onConfirm();

private:
    QSpinBox *widthSpinBox;
    QSlider *opacitySlider;
    QPushButton *colorButton;
    QColor selectedColor;
};

#endif // PENSETTINGDIALOG_H
