#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("jsengine-test");
    QApplication::setOrganizationName("volker.wick");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
