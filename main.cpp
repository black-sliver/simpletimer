#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("SimpleTimer");
    QCoreApplication::setApplicationName("SimpleTimer");
    MainWindow w;
    w.show();
    return a.exec();
}
