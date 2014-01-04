#include "generichelper.h"
#include <QDebug>

void genericHelper::setOAuthAccessToken(QString oauthtoken){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("oauthAccessToken", oauthtoken);
         settings.sync();


}

void genericHelper::setUsername(QString username){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("username", username);
         settings.sync();


}

void genericHelper::setOBSBin(QString obsbinpath){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("obsbin", obsbinpath);
         settings.sync();


}



void genericHelper::setUpdateInterval(int interval){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("update_interval", QString::number(interval));
         settings.sync();


}

int genericHelper::getUpdateInterval(){

    QSettings settings("Abyle", "twitcher");

    QString _interval = "5";


    if (settings.value("update_interval", "5").toString().length() > 1) {
        _interval = settings.value("update_interval", "").toString();


    }

    return _interval.toInt();
}



void genericHelper::setFollows(QStringList follows){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("follows", follows.join(","));
         settings.sync();


}

QStringList genericHelper::getFollows(){

    QSettings settings("Abyle", "twitcher");

    QStringList _follows;

    if (settings.value("follows", "").toString().length() > 1) {
        _follows = settings.value("follows", "").toString().split(",");

    }

    return _follows;
}

QString genericHelper::getUsername(){

    QSettings settings("Abyle", "twitcher");

    QString _username = "";

    if (settings.value("username", "").toString().length() > 1) {
        _username = settings.value("username", "").toString();

    }

    return _username;
}

QString genericHelper::getOBSBin(){

    QSettings settings("Abyle", "twitcher");

    QString _obsbin = "";

    if (settings.value("obsbin", "").toString().length() > 1) {
        _obsbin = settings.value("obsbin", "").toString();

    }

    return _obsbin;
}

void genericHelper::executeOBS(){

    genericHelper::getOBSBin();


    QProcess *process = new QProcess(qApp);
    QString program = genericHelper::getOBSBin();

    QStringList args;

    args << "";



    process->start(program, args);

    genericHelper::log("launched: "+program);


    //qDebug() << args;
    //qDebug() << program;

}

void genericHelper::executeAddonHexchat(QStringList follows){


    QString addonHexchatBin = "";





    addonHexchatBin = QCoreApplication::applicationFilePath().replace("/twitcher.exe","") + QDir::separator() +
            "3rdparty-addons" + QDir::separator() +
            "hexchat" + QDir::separator() + "hexchat.exe";

    QFile addonHexchatBinFile( addonHexchatBin );

    if( addonHexchatBinFile.exists() )

    {

        QDir addonHexchatDir(genericHelper::getAppDir() + QDir::separator() + "addon_hexchat_config");
        if (!addonHexchatDir.exists()) {
            addonHexchatDir.mkpath(".");
        }

        QFile file(genericHelper::getAppDir() + QDir::separator() + "addon_hexchat_config" + QDir::separator() + "servlist.conf");
        if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        return;

        // build follow channel join list
        QString followjoins="";

        for (int i = 0; i < follows.size(); ++i)
        {

            followjoins += "J=#" + follows.at(i) + "\n";
        }


        QTextStream out(&file);
        out << "v=2.9.6\n\n"
            << "N=twitch.tv\n"
            << "I="+genericHelper::getUsername()+"\n"
            << "U="+genericHelper::getUsername()+"\n"
            << "P=oauth:"+genericHelper::getOAuthAccessToken()+"\n"
            << "L=7\n"
            << "J=#"+genericHelper::getUsername()+"\n"
            << followjoins
            << "F=24\n"
            << "D=0\n"
            << "S=irc.twitch.tv/6667\n"
            << "\n";
        out.flush();

        file.close();

        qDebug() << addonHexchatBin;

        QStringList args;

        args << "-d" << genericHelper::getAppDir().replace("/", "\\") + QDir::separator() + "addon_hexchat_config";

        qDebug() << args;

        QProcess *process = new QProcess(qApp);

        QString program = addonHexchatBin;

        process->start(program, args);

        genericHelper::log("launched: "+program);

        //qDebug() << process->pid();
    }



    //




    //qDebug() << args;
    //qDebug() << program;

}

bool genericHelper::openLogWithNotepad() {
  QProcess notepad;
  notepad.start(
        "notepad.exe",
        QStringList() << genericHelper::getAppDir() + QDir::separator() + "twitcher.log");

  notepad.waitForFinished();

  return true;
}

void genericHelper::setErrorTimestamp() {


    QSettings settings("Abyle", "twitcher");
    settings.setValue("errorTimestamp",  QDateTime::currentMSecsSinceEpoch());
    settings.sync();
}

bool genericHelper::getEnoughTimeSinceLastErrorElapsed() {


    QSettings settings("Abyle", "twitcher");

    QString _errortime = "";

    if (settings.value("errorTimestamp", "").toString().length() > 1) {
        _errortime = settings.value("errorTimestamp", "").toString();

    }

   if (QDateTime::currentMSecsSinceEpoch() - _errortime.toDouble() >= 30000)
   {

       genericHelper::log("enough time (30s) elapsed since last errors, reseted error count.");
       settings.setValue("errorTimestamp",  0);
       settings.sync();
       return true;
   } else {
       return false;
   }




}


QString genericHelper::getOAuthAccessToken(){

    QSettings settings("Abyle", "twitcher");

    QString _oauthtoken = "";

    if (settings.value("oauthAccessToken", "").toString().length() > 1) {
        _oauthtoken = settings.value("oauthAccessToken", "").toString();

    }

    return _oauthtoken;


}

QString genericHelper::getAppDir(){

    QString _appDir = QDir::homePath() + QDir::separator () + "twitcher";

    QSettings settings("Abyle", "twitcher");

    if (settings.value("appDir", "").toString().length() > 1) {


        _appDir = settings.value("appDir", "").toString();

    } else {


        QSettings settings("Abyle","twitcher");
        settings.setValue("appDir", _appDir);
        settings.sync();
    }



    if (QDir(_appDir).exists() == true) {

    } else {

        QDir().mkdir(_appDir);

    }

    if (QDir(_appDir).exists() == true) {
       return _appDir;
    } else {

        return QDir::tempPath();
    }

}

void genericHelper::log(QString logstring) {
    QFile file(genericHelper::getAppDir() + QDir::separator() + "twitcher.log");
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    return;

    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss") << ": " << logstring << "\n";
    out.flush();

    file.close();
}
