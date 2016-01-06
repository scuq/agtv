#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QWindow>
#include <QThread>
#include <QUrl>


class processLauncher : public QObject
{
    Q_OBJECT
public:
    explicit processLauncher(QObject *parent = 0);
    void setProgramStr(QString programstr);
    void setArgs(QStringList args);
    void setPostProgramStr(QString programstr);
    void setPostArgs(QStringList args);
    void setPostProgramFindStr(QString findstr);


signals:

public slots:
    void launch();

private slots:
    void onProgramReadReady();


private:
    QString programStr;
    QStringList args;
    QString postProgramStr;
    QStringList postArgs;
    QProcess *process;
    QProcess *postProcess;
    QString findstr;
    bool embed;
    QRect geo;




};

#endif // PROCESSLAUNCHER_H
