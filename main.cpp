#include "tpmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tpMainWindow w;
    w.show();

    return a.exec();
}
