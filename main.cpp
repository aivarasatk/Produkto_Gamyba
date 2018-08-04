#include "mainwindow.h"
#include <QApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.showFileSelection();

    return a.exec();
}
