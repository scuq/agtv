#include "processlauncher.h"

processLauncher::processLauncher(QObject *parent) :
    QObject(parent)
{



}




void processLauncher::launch() {


    process = new QProcess();



    QString program = this->programStr;



    connect(process,SIGNAL(readyRead()),this,SLOT(onProgramReadReady()));

    process->start(program, this->args,QIODevice::ReadOnly);









}

void processLauncher::setProgramStr(QString programstr) {
    this->programStr = programstr;
}

void processLauncher::setArgs(QStringList args) {
    this->args = args;
}

void processLauncher::setPostProgramStr(QString programstr) {
    this->postProgramStr = programstr;
}

void processLauncher::setPostArgs(QStringList args) {
    this->postArgs = args;
}

void processLauncher::setPostProgramFindStr(QString findstr) {
    this->findstr = findstr;
}




