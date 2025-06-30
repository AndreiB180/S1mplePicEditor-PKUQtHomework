#ifndef WATERMARKDIALOG_H
#define WATERMARKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QSlider>
#include <QColor>
#include <QFont>
#include <QVBoxLayout>

class WatermarkDialog : public QDialog {
    Q_OBJECT
public:
    explicit WatermarkDialog(QWidget *parent = nullptr);

signals:
    void confirmed(QString text, QFont font, QColor color, int opacity);
    void previewUpdated(QString text, QFont font, QColor color, int opacity);
    void cancelled();

private slots:
    void chooseColor();
    void onYes();
    void onNo();
    void emitPreview();

private:
    QLineEdit *textEdit;
    QSpinBox *fontSizeSpin;
    QPushButton *colorButton;
    QSlider *opacitySlider;
    QColor selectedColor = Qt::black;
    QVBoxLayout *mainLayout;
};

#endif // WATERMARKDIALOG_H
