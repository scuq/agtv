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


    if (settings.value("update_interval", "12").toString().length() > 1) {
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



void genericHelper::setCheckUpdate(bool checkupdate){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("check_update", checkupdate);
         settings.sync();


}

bool genericHelper::getCheckUpdate(){

    QSettings settings("Abyle", "twitcher");

    bool _checkupdate;

    if (settings.value("check_update", "").toString().length() > 1) {
        _checkupdate = settings.value("check_update", "").toBool();

    } else {
        genericHelper::setCheckUpdate(true);
    }


    return _checkupdate;
}

void genericHelper::setLoadGameImages(bool gameimages){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("load_game_images", gameimages);
         settings.sync();


}

bool genericHelper::getLoadGameImages(){

    QSettings settings("Abyle", "twitcher");

    bool _gameimages;

    if (settings.value("load_game_images", "").toString().length() > 1) {
        _gameimages = settings.value("load_game_images", "").toBool();

    } else {
        genericHelper::setLoadGameImages(true);
    }


    return _gameimages;
}



void genericHelper::setClearLogOnStartup(bool clearlog){


         QSettings settings("Abyle", "twitcher");
         settings.setValue("clear_log_on_start", clearlog);
         settings.sync();


}

bool genericHelper::getClearLogOnStartup(){

    QSettings settings("Abyle", "twitcher");

    bool _clearlog;

    if (settings.value("clear_log_on_start", "").toString().length() > 1) {
        _clearlog = settings.value("clear_log_on_start", "").toBool();

    }

    return _clearlog;
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

void genericHelper::writeTwitcherOBSConfig(QString streamkey){




    QFile profilefile(QString(::getenv("APPDATA")) + QDir::separator() + "OBS" + QDir::separator() + "profiles" + QDir::separator() + "twitcher.ini");
    if (!profilefile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    return;

    QTextStream out(&profilefile);
    out << "[Publish]\n"
        << "LowLatencyMethod=0\n"
        << "LatencyFactor=20\n"
        << "Dashboard=\n"
        << "StartStreamHotkey=1143\n"
        << "StopStreamHotkey=1143\n"
        << "SavePath=\n"
        << "SaveToFile=0\n"
        << "Delay=0\n"
        << "AutoReconnectTimeout=10\n"
        << "AutoReconnect=1\n"
        << "LowLatencyMode=0\n"
        << "URL=EU: Amsterdam, NL\n"
        << "PlayPath="+streamkey+"\n"
        << "BindToIP=Default\n"
        << "Mode=0\n"
        << "Service=1\n"
        << "\n";
    out.flush();

    profilefile.close();


}

void genericHelper::writeTwitcherOBSScenesConfig(){

    qDebug() << "writeTwitcherOBSScenesConfig";

    QFile scenefilecp(QString(::getenv("APPDATA")) + QDir::separator() + "OBS" + QDir::separator() + "scenes.xconfig");

    if (scenefilecp.exists()) {
        scenefilecp.copy(QString(::getenv("APPDATA")) + QDir::separator() + "OBS" + QDir::separator() + "scenes.xconfig_" + QDateTime::currentDateTime().toString("yyyyMMddTHHmmss"));
        genericHelper::log("your scenes.xconfig was copied to " + QDateTime::currentDateTime().toString("yyyyMMddTHHmmss") );

    }


    scenefilecp.close();

    QFile scenefile(QString(::getenv("APPDATA")) + QDir::separator() + "OBS" + QDir::separator() + "scenes.xconfig");


    scenefile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    QString obsSceneImageFilePath = QCoreApplication::applicationFilePath().replace("/twitcher.exe","") + QDir::separator() + "OBS-Images";

    QString img_blackout = obsSceneImageFilePath + QDir::separator() + "blackout.png";
    img_blackout = img_blackout.replace("/", "\\");
    img_blackout = img_blackout.replace("\\", "\\\\");

    QString img_hiddenarea = obsSceneImageFilePath + QDir::separator() + "hiddenarea.png";
    img_hiddenarea = img_hiddenarea.replace("/", "\\");
    img_hiddenarea = img_hiddenarea.replace("\\", "\\\\");

    QString img_coffee = obsSceneImageFilePath + QDir::separator() + "coffee.png";
    img_coffee = img_coffee.replace("/", "\\");
    img_coffee = img_coffee.replace("\\", "\\\\");

    QString img_afk =  obsSceneImageFilePath + QDir::separator() + "afk.png";
    img_afk = img_afk.replace("/", "\\");
    img_afk = img_afk.replace("\\", "\\\\");


    qDebug() << obsSceneImageFilePath;



    QTextStream outsc(&scenefile);
    outsc << "scenes : {\n"
        << "  Twitcher : {\n"
        << "    class : Scene\n"
        << "    sources : {\n"
        << "      Image_Blackout : {\n"
        << "        render : 0\n"
        << "        class : BitmapImageSource\n"
        << "        data : {\n"
        << "          path : \"" + img_blackout + "\"\n"
        << "          opacity : 100\n"
        << "          color : -1\n"
        << "          useColorKey : 0\n"
        << "          keyColor : -1\n"
        << "          keySimilarity : 10\n"
        << "          keyBlend : 0\n"
        << "          monitor : 0\n"
        << "        }\n"
        << "        cx : 1920\n"
        << "        cy : 1080\n"
        << "      }\n"
        << "      Image_HiddenArea : {\n"
        << "        render : 0\n"
        << "        class : BitmapImageSource\n"
        << "        data : {\n"
        << "          path : \"" + img_hiddenarea + "\"\n"
        << "          opacity : 100\n"
        << "          color : -1\n"
        << "          useColorKey : 0\n"
        << "          keyColor : -1\n"
        << "          keySimilarity : 10\n"
        << "          keyBlend : 0\n"
        << "          monitor : 0\n"
        << "        }\n"
        << "        cx : 400\n"
        << "        cy : 400\n"
        << "        y : 680\n"
        << "      }\n"
        << "      Image_Coffee : {\n"
        << "        render : 0\n"
        << "        class : BitmapImageSource\n"
        << "        data : {\n"
        << "          path : \"" + img_coffee + "\"\n"
        << "          opacity : 100\n"
        << "          color : -1\n"
        << "          useColorKey : 0\n"
        << "          keyColor : -1\n"
        << "          keySimilarity : 10\n"
        << "          keyBlend : 0\n"
        << "          monitor : 0\n"
        << "        }\n"
        << "        cx : 198\n"
        << "        cy : 198\n"
        << "      }\n"
        << "      Image_Afk : {\n"
        << "        render : 0\n"
        << "        class : BitmapImageSource\n"
        << "        data : {\n"
        << "          path : \"" + img_afk + "\"\n"
        << "          opacity : 100\n"
        << "          color : -1\n"
        << "          useColorKey : 0\n"
        << "          keyColor : -1\n"
        << "         keySimilarity : 10\n"
        << "          keyBlend : 0\n"
        << "          monitor : 0\n"
        << "        }\n"
        << "        cx : 800\n"
        << "        cy : 200\n"
        << "        x : 560\n"
        << "      }\n"
        << "      Capture_CTRL+F12 : {\n"
        << "        render : 1\n"
        << "        class : GlobalSource\n"
        << "        data : {\n"
        << "          name : f12-game\n"
        << "        }\n"
        << "        cx : 1920\n"
        << "        cy : 1080\n"
        << "      }\n"
        << "    }\n"
        << "  }\n"
        << "}\n"
        << "global sources : {\n"
        << "  f12-game : {\n"
        << "    class : GraphicsCapture\n"
        << "    data : {\n"
        << "      window : ""\n"
        << "      windowClass : TWNClientFramework\n"
        << "      stretchImage : 0\n"
        << "      ignoreAspect : 0\n"
        << "      captureMouse : 1\n"
        << "      invertMouse : 0\n"
        << "      useHotkey : 1\n"
        << "      hotkey : 635\n"
        << "      gamma : 100\n"
        << "    }\n"
        << "    cx : 1920\n"
        << "    cy : 1080\n"
        << "  }\n"
        << "}\n"
        << "\n";



    scenefile.close();



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

  QProcess *notepad = new QProcess(qApp);
  notepad->start("notepad.exe", QStringList() << genericHelper::getAppDir() + QDir::separator() + "twitcher.log");



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

void genericHelper::deleteLog() {
    QFile file(genericHelper::getAppDir() + QDir::separator() + "twitcher.log");
    file.remove();
    file.close();
}


