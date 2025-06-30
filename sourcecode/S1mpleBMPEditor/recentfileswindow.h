#ifndef RECENTFILESWINDOW_H
#define RECENTFILESWINDOW_H

#include <QDialog>
#include <QVector>
#include <QString>

class RecentFilesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RecentFilesWindow(const QVector<QString>& recentFiles, QWidget* parent = nullptr);

signals:
    void fileSelected(const QString& path);  // 当用户点击一个文件时发射此信号
    void clearRequested();
};

#endif // RECENTFILESWINDOW_H
