#include "tpmainwindow.h"
#include <QApplication>
#include "generichelper.h"
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory mem("CvZcgbqvJ9XIKqx1bZE6");
     if(!mem.create(1))
     {
        QMessageBox::critical(0,"Instance detected!",genericHelper::getAppName()+" is already running!\nApplication terminating...","Ok");
        exit(0);
     }

        tpMainWindow w;
        w.show();



        return a.exec();


}
