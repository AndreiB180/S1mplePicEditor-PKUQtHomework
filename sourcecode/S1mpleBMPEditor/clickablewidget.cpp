// ClickableWidget.cpp
#include "clickablewidget.h"
#include <QMouseEvent>

ClickableWidget::ClickableWidget(const QString& path, QWidget* parent)
    : QWidget(parent), filePath(path) {}

void ClickableWidget::mousePressEvent(QMouseEvent* event) {
    emit clicked(filePath);
    QWidget::mousePressEvent(event);
}
