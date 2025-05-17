#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("СПО мониторинга действий операторов");
    w.resize(1450, 600);
    w.show();
    return a.exec();
}
