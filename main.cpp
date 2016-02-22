#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFont font;
    font.setPointSize(160);
    font.setFamily("wenquanyi");
    font.setBold(true);
    a.setFont(font);

    w.show();


    return a.exec();
}
