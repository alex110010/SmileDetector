#include "mainwidj.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidj w;
    w.show();

    return a.exec();
}
