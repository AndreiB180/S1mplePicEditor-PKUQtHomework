// ClickableWidget.h
#ifndef CLICKABLEWIDGET_H
#define CLICKABLEWIDGET_H

#include <QWidget>

class ClickableWidget : public QWidget {
    Q_OBJECT
public:
    explicit ClickableWidget(const QString& path, QWidget* parent = nullptr);
    QString filePath;

signals:
    void clicked(const QString& path);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // CLICKABLEWIDGET_H
