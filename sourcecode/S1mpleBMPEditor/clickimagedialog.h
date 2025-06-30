#ifndef CLICKIMAGEDIALOG_H
#define CLICKIMAGEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include "watermarkdialog.h"

class ClickImageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ClickImageDialog(const QPixmap &pixmap, QWidget *parent = nullptr);
    QPixmap getFinalPixmap() const;

signals:
    void watermarkConfirmed(const QPixmap &pixmap);
    void watermarkCancelled();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel *label;
    QPixmap originalPixmap;
    QPixmap currentPreviewPixmap;
    double scaleFactor;
    QPixmap basePixmap;

    void setupLayout();
    void openWatermarkDialog(QPoint imagePos);
};

#endif // CLICKIMAGEDIALOG_H
