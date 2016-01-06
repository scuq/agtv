#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include <QTextStream>
#include <QSysInfo>
#include <QTimer>

QString getPlatform() {
#ifdef Q_OS_WIN32
    return QString("win");
#endif
#ifdef Q_OS_LINUX
    return "lin";
#endif
#ifdef Q_OS_MAC
    return "osx";
#endif
}

QString getArch() {
    return QSysInfo::currentCpuArchitecture();
}

QString getVersion(QString level) {

    bool lvlfound = false;
    QString versionstr = "";


    if (level == "major") {
       versionstr = QDateTime::currentDateTime().toString("yy");
       lvlfound=true;
    }

    if ((level == "minor") && (lvlfound == false)) {
       versionstr = QDateTime::currentDateTime().toString("MM");
       lvlfound=true;
    }

    if ((level == "patch") && (lvlfound == false)) {
       versionstr = QDateTime::currentDateTime().toString("dd");
       lvlfound=true;
    }

    if ((level == "build") && (lvlfound == false)) {
       versionstr = QDateTime::currentDateTime().toString("HHmm");
       lvlfound=true;
    }




    return versionstr;
}






int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QCommandLineParser parser;
        parser.setApplicationDescription("deployer");
        parser.addHelpOption();
        parser.addVersionOption();


    QCommandLineOption versionMajor(QStringList() << "m" << "major",
               QCoreApplication::translate("main", "print major version"));

    QCommandLineOption versionMinor(QStringList() << "i" << "minor",
               QCoreApplication::translate("main", "print minor version"));

    QCommandLineOption versionPatch(QStringList() << "p" << "patch",
               QCoreApplication::translate("main", "print patch version"));

    QCommandLineOption versionBuild(QStringList() << "b" << "build",
               QCoreApplication::translate("main", "print build version"));

    QCommandLineOption platform(QStringList() << "o" << "platform",
               QCoreApplication::translate("main", "print platform"));

    QCommandLineOption arch(QStringList() << "a" << "arch",
               QCoreApplication::translate("main", "print architecture"));

    QCommandLineOption copy(QStringList() << "c" << "copy",
               QCoreApplication::translate("main", "copy"));

    QCommandLineOption copyrecursive(QStringList() << "r" << "recursive",
               QCoreApplication::translate("main", "copy recursive"));

    QCommandLineOption exclude(QStringList() << "e" << "exclude",
            QCoreApplication::translate("main", "exclude"),
            QCoreApplication::translate("main", "exclude from copy regex"));


    QCommandLineOption source(QStringList() << "s" << "source",
            QCoreApplication::translate("main", "source"),
            QCoreApplication::translate("main", "source"));

    QCommandLineOption destination(QStringList() << "d" << "destination",
            QCoreApplication::translate("main", "destination"),
            QCoreApplication::translate("main", "destination"));


    parser.addOption(versionMajor);
    parser.addOption(versionMinor);
    parser.addOption(versionPatch);
    parser.addOption(versionBuild);
    parser.addOption(platform);
    parser.addOption(arch);

    parser.addOption(copy);
    parser.addOption(source);
    parser.addOption(destination);
    parser.addOption(copyrecursive);
    parser.addOption(exclude);

    parser.process(a);

    bool skip = false;

    if ((parser.isSet(copy) == true ) && (skip == false) && (parser.isSet(source) == true) && (parser.isSet(destination) == true)) {
         QTextStream(stdout) <<  "copy src to dst\n";
         skip = true;
    }


    if ((parser.isSet(versionMajor) == true ) && (skip == false)) {
         QTextStream(stdout) << getVersion("major") << "\n";
         skip = true;
    }

    if ((parser.isSet(versionMinor) == true ) && (skip == false)) {
         QTextStream(stdout) << getVersion("minor") << "\n";
         skip = true;
    }

    if ((parser.isSet(versionPatch) == true ) && (skip == false)) {
         QTextStream(stdout) << getVersion("patch") << "\n";
         skip = true;
    }

    if ((parser.isSet(versionBuild) == true ) && (skip == false)) {
         QTextStream(stdout) << getVersion("build") << "\n";
         skip = true;
    }

    if ((parser.isSet(platform) == true ) && (skip == false)) {
         QTextStream(stdout) << getPlatform() << "\n";
         skip = true;
    }

    if ((parser.isSet(arch) == true ) && (skip == false)) {
         QTextStream(stdout) << getArch() << "\n";
         skip = true;
    }



    if (skip == false) {
        QTextStream(stdout) << getVersion("major")+"."+getVersion("minor")+"."+getVersion("patch")+"."+getVersion("build");
    }


    QTimer::singleShot(200, &a, SLOT(quit()));
    return a.exec();
}

