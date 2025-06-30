#ifndef IMPORTWINDOW_H
#define IMPORTWINDOW_H

#include <QDialog>

namespace Ui {
class ImportWindow;
}

class ImportWindow : public QDialog
{
    Q_OBJECT

public slots:
    void paintEvent(QPaintEvent *Event);

public:
    explicit ImportWindow(QWidget *parent = nullptr);
    ~ImportWindow();

private slots:
    void on_exit_clicked();

    void on_newpic_clicked();

    void on_openpic_clicked();

private:
    Ui::ImportWindow *ui;

    void setButtonStyle(QPushButton* button,const QString& filename);
    void setButtonTip(QPushButton* button,const QString& buttonword);
    void initButtons();

};



#endif // IMPORTWINDOW_H
