#include "mainwindow.h"
#include "importwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include<QStandardPaths>



int main(int argc, char *argv[])
{



    QApplication a(argc, argv);




    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "S1mpleBMPEditor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    ImportWindow *imp=new ImportWindow();
    imp->show();
    return a.exec();
}
