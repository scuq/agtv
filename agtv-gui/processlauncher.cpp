#include "processlauncher.h"

processLauncher::processLauncher(QObject *parent) :
    QObject(parent)
{



}

void processLauncher::launch() {
    process = new QProcess();

    QString program = this->programStr;

#if defined(Q_OS_WIN)


    connect(process,SIGNAL(readyRead()),this,SLOT(onProgramReadReady()));

    process->start(program, this->args, QIODevice::ReadOnly);



#else

    program.append(" ");
    program.append(this->args.join(" "));

    connect(process,SIGNAL(readyRead()),this,SLOT(onProgramReadReady()));

    process->start(program);

#endif





}

void processLauncher::onProgramReadReady()
{
    qDebug() << "Error: " << process->error();
    qDebug() << "Output: " << process->readAll();
    qDebug() << "Standart Error :"<< process->readAllStandardError();
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




