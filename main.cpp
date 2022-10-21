#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("QMainWindow {background: 'black';}");  //establecer hoja de estilo negra
    w.show();
    return a.exec();
}
