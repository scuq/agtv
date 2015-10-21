#include "generichelper.h"
#include <QDebug>

void genericHelper::setOAuthAccessToken(QString oauthtoken){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("oauthAccessToken", oauthtoken);
         settings.sync();


}

void genericHelper::setUsername(QString username){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("username", username);
         settings.sync();


}

void genericHelper::setOBSBin(QString obsbinpath){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("obsbin", obsbinpath);
         settings.sync();


}



void genericHelper::setUpdateInterval(int interval){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("update_interval", QString::number(interval));
         settings.sync();


}

int genericHelper::getUpdateInterval(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _interval = "5";


    if (settings.value("update_interval", "12").toString().length() > 1) {
        _interval = settings.value("update_interval", "").toString();


    }

    return _interval.toInt();
}



void genericHelper::setViewerStreamsMute(QStringList mutes){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerstreamsmute", mutes.join(","));
         settings.sync();


}

QStringList genericHelper::getViewerStreamsMute(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QStringList _viewerstreamsmute;

    if (settings.value("viewerstreamsmute", "").toString().length() > 1) {
        _viewerstreamsmute = settings.value("viewerstreamsmute", "").toString().split(",");

    }

    return _viewerstreamsmute;
}


void genericHelper::setViewerStreams(QStringList streams){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerstreams", streams.join(","));
         settings.sync();


}

QStringList genericHelper::getViewerStreams(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QStringList _viewerstreams;

    if (settings.value("viewerstreams", "").toString().length() > 1) {
        _viewerstreams = settings.value("viewerstreams", "").toString().split(",");

    }

    return _viewerstreams;
}

void genericHelper::setFollows(QStringList follows){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("follows", follows.join(","));
         settings.sync();


}

void genericHelper::addFollow(QString follow)
{

    QStringList currentfollows = genericHelper::getFollows();
    if (currentfollows.count(follow) <= 0) {
        currentfollows << follow;
        genericHelper::setFollows(currentfollows);
    }

}

QStringList genericHelper::getFollows(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QStringList _follows;

    if (settings.value("follows", "").toString().length() > 1) {
        _follows = settings.value("follows", "").toString().split(",");
    }

    return _follows;
}

void genericHelper::setBookmarks(QStringList bookmarks){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("bookmarks", bookmarks.join(","));
         settings.sync();


}

void genericHelper::addBookmark(QString bookmark)
{
    QStringList currentbookmarks = genericHelper::getBookmarks();
    if (currentbookmarks.count(bookmark) <= 0) {
        currentbookmarks << bookmark;
        genericHelper::setBookmarks(currentbookmarks);
    }

}

void genericHelper::saveGeometry(QString window, QVariant geo)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("geo_"+window, geo);
    settings.sync();
}

void genericHelper::saveWindowstate(QString window, QVariant state)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("windowstate_"+window, state);
    settings.sync();
}

QVariant genericHelper::getGeometry(QString window)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QVariant _geo;


        _geo = settings.value("geo_"+window, "");




    return _geo;
}

QVariant genericHelper::getWindowstate(QString window)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QVariant _ws;


        _ws = settings.value("windowstate_"+window, "");



    return _ws;
}

QStringList genericHelper::getBookmarks(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QStringList _bookmarks;

    if (settings.value("bookmarks", "").toString().length() > 1) {
        _bookmarks = settings.value("bookmarks", "").toString().split(",");

    }

    return _bookmarks;
}

void genericHelper::setVlcArgs(QStringList args)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("vlc_args", args.join(","));
    settings.sync();
}

QStringList genericHelper::getVlcArgs()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QStringList _vlcargs;

    if (settings.value("vlc_args", "").toString().length() > 1) {
        _vlcargs = settings.value("vlc_args", "").toString().split(",");

    } else {
        _vlcargs << "--autoscale";
        _vlcargs << "--qt-minimal-view";
        _vlcargs << "--no-qt-system-tray";
        _vlcargs << "--network-caching=5000" ;
    }

    return _vlcargs;
}


void genericHelper::setCheckUpdate(bool checkupdate){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("check_update", checkupdate);
         settings.sync();


}

bool genericHelper::getJoinFollow()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _join_follow;

    if (settings.value("join_follow", "").toString().length() > 1) {
        _join_follow = settings.value("join_follow", "").toBool();

    }
    return _join_follow;
}

void genericHelper::setJoinFollow(bool join)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("join_follow", join);
    settings.sync();

}

bool genericHelper::getJoinBookmarks()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _join_bookmarks;

    if (settings.value("join_bookmarks", "").toString().length() > 1) {
        _join_bookmarks = settings.value("join_bookmarks", "").toBool();

    }
    return _join_bookmarks;
}

void genericHelper::setJoinBookmarks(bool join)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("join_bookmarks", join);
    settings.sync();
}

bool genericHelper::getStreamPositioning()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _positioning;

    if (settings.value("stream_positioning", "").toString().length() > 1) {
        _positioning = settings.value("stream_positioning", "").toBool();

    } else {
        _positioning = false;
    }

    return _positioning;
}

void genericHelper::setStreamPositioning(bool positioning)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("stream_positioning", positioning);
    settings.sync();
}

bool genericHelper::getShowOfflineStreamers()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _showoffline;

    if (settings.value("show_offline_streamers", "").toString().length() > 1) {
        _showoffline = settings.value("show_offline_streamers", "").toBool();

    } else {
        _showoffline = true;
    }

    return _showoffline;
}

void genericHelper::setShowOfflineStreamers(bool showoffline)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("show_offline_streamers", showoffline);
    settings.sync();
}

QString genericHelper::getUpdateCheckUrl()
{
    return "http://agtv.abyle.org/version/latest_"+genericHelper::getAppName();
}




bool genericHelper::getCheckUpdate(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _checkupdate;

    if (settings.value("check_update", "").toString().length() > 1) {
        _checkupdate = settings.value("check_update", "").toBool();

    } else {
        genericHelper::setCheckUpdate(true);
    }


    return _checkupdate;
}

void genericHelper::setLoadGameImages(bool gameimages){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("load_game_images", gameimages);
         settings.sync();


}

bool genericHelper::getLoadGameImages(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _gameimages;

    if (settings.value("load_game_images", "").toString().length() > 1) {
        _gameimages = settings.value("load_game_images", "").toBool();

    } else {
        genericHelper::setLoadGameImages(true);
    }


    return _gameimages;
}



void genericHelper::setClearLogOnStartup(bool clearlog){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("clear_log_on_start", clearlog);
         settings.sync();


}

bool genericHelper::getCloseToTray()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _closetotray;

    if (settings.value("close_to_tray", "").toString().length() > 1) {
        _closetotray = settings.value("close_to_tray", "").toBool();

    }

    return _closetotray;
}

void genericHelper::setCloseToTray(bool closetotray)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("close_to_tray", closetotray);
    settings.sync();
}

bool genericHelper::getStreamOnlineNotify()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _streamonlinenotify;

    if (settings.value("stream_online_notify", "").toString().length() > 1) {
        _streamonlinenotify = settings.value("stream_online_notify", "").toBool();

    }

    return _streamonlinenotify;
}

void genericHelper::setStreamOnlineNotify(bool streamonlinenotify)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("stream_online_notify", streamonlinenotify);
    settings.sync();
}

bool genericHelper::getClearLogOnStartup(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    bool _clearlog;

    if (settings.value("clear_log_on_start", "").toString().length() > 1) {
        _clearlog = settings.value("clear_log_on_start", "").toBool();

    }

    return _clearlog;
}



QString genericHelper::getUsername(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _username = "";

    if (settings.value("username", "").toString().length() > 1) {
        _username = settings.value("username", "").toString();

    }

    return _username;
}

QString genericHelper::getOBSBin(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _obsbin = "";

    if (settings.value("obsbin", "").toString().length() > 1) {
        _obsbin = settings.value("obsbin", "").toString();

    }

    return _obsbin;
}

bool genericHelper::isOnline(QString status)
{
        QRegExp regexEmail("^online" );

        if( regexEmail.indexIn( status ) != -1 ) {
            return true;
        } else {
            return false;
        }

}

bool genericHelper::isPlaylist(QString status)
{
        QRegExp regexEmail("^playlist" );

        if( regexEmail.indexIn( status ) != -1 ) {
            return true;
        } else {
            return false;
        }

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




    QFile profilefile(QString(::getenv("APPDATA")) + QDir::separator() + "OBS" + QDir::separator() + "profiles" + QDir::separator() + genericHelper::getAppName()+".ini");
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

    QString obsSceneImageFilePath = QCoreApplication::applicationFilePath().replace(genericHelper::getAppName()+".exe","") + QDir::separator() + "OBS-Images";

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




void genericHelper::executeAddonHexchat(QStringList channelsToJoin){

    QString addonHexchatBin = "";


    addonHexchatBin = QCoreApplication::applicationFilePath().replace(genericHelper::getAppName()+".exe","") + QDir::separator() +
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

        QString joins="";

        for (int i = 0; i < channelsToJoin.size(); ++i)
        {

            joins += "J=#" + channelsToJoin.at(i) + "\n";
        }

        QTextStream out(&file);
        out << "v=2.9.6\n\n"
            << "N=twitch.tv\n"
            << "I="+genericHelper::getUsername()+"\n"
            << "U="+genericHelper::getUsername()+"\n"
            << "P=oauth:"+genericHelper::getOAuthAccessToken()+"\n"
            << "L=7\n"
            << "J=#"+genericHelper::getUsername()+"\n"
            << joins
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
  notepad->start("notepad.exe", QStringList() << genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log");



  return true;
}

void genericHelper::setErrorTimestamp() {


    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("errorTimestamp",  QDateTime::currentMSecsSinceEpoch());
    settings.sync();
}

bool genericHelper::getEnoughTimeSinceLastErrorElapsed() {


    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

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




int genericHelper::getStreamHeight(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _viewerstreamheight = "";

    if (settings.value("viewerstreamheight", "").toString().length() > 1) {
        _viewerstreamheight = settings.value("viewerstreamheight", "").toString();

    }

    return _viewerstreamheight.toInt();


}

int genericHelper::getStreamWidth(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _viewerstreamwidth = "";

    if (settings.value("viewerstreamwidth", "").toString().length() > 1) {
        _viewerstreamwidth = settings.value("viewerstreamwidth", "").toString();

    }

    return _viewerstreamwidth.toInt();


}

int genericHelper::getSelectedScreen(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _viewerselectedscreen = "";

    if (settings.value("viewerselectedscreen", "").toString().length() > 0) {
        _viewerselectedscreen = settings.value("viewerselectedscreen", "").toString();

    }

    return _viewerselectedscreen.toInt();


}



int genericHelper::getConcurrentStreams(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _viewerconcurrentstreams = "";

    if (settings.value("viewerconcurrentstreams", "").toString().length() > 0) {
        _viewerconcurrentstreams = settings.value("viewerconcurrentstreams", "").toString();

    }

    return _viewerconcurrentstreams.toInt();


}

void genericHelper::setConcurrentStreams(int concurrentstreams){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerconcurrentstreams", QString::number(concurrentstreams));
         settings.sync();


}




void genericHelper::setSelectedScreen(int selectedScreen){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerselectedscreen", QString::number(selectedScreen));
         settings.sync();


}

int genericHelper::getSelectedTab()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _selectedtab = "";

    if (settings.value("selectedtab", "").toString().length() > 0) {
        _selectedtab = settings.value("selectedtab", "").toString();

    }

    return _selectedtab.toInt();
}

void genericHelper::setSelectedTab(int tab)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("selectedtab", QString::number(tab));
    settings.sync();

}

void genericHelper::setStreamWidth(int streamWidth){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerstreamwidth", QString::number(streamWidth));
         settings.sync();


}

void genericHelper::setPrimaryScreenWidth(int screenwidth){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerprimaryscreenwidth", QString::number(screenwidth));
         settings.sync();


}

bool genericHelper::setPosition(QString posname, QString geo)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("position_"+posname, geo);
    settings.sync();

    return true;
}

bool genericHelper::deletePosition(QString posname)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.remove("position_"+posname);
    settings.sync();

    return true;
}

QString genericHelper::getPosition(QString posname)
{

    if (posname == "<default>") {
        return "10,10,500,500";
    }

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _pos = "";

    if (settings.value("position_"+posname, "").toString().length() > 1) {
        _pos = settings.value("position_"+posname, "").toString();

    }

    return _pos;
}

QHash<QString, QString> genericHelper::getPositions()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QHash<QString, QString> hash;

    foreach (QString setting, settings.allKeys()) {

        if (setting.startsWith("position_")) {

            hash.insert(setting.replace("position_",""), settings.value(setting).toString());
        }
    }

    return hash;
}



int genericHelper::getPrimaryScreenWidth(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _viewerprimaryscreenwidth = "";

    if (settings.value("viewerprimaryscreenwidth", "").toString().length() > 1) {
        _viewerprimaryscreenwidth = settings.value("viewerprimaryscreenwidth", "").toString();

    }

    return _viewerprimaryscreenwidth.toInt();


}



void genericHelper::setPlayerQuality(QString quality){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerplayerquality", quality);
         settings.sync();


}

QString genericHelper::getPlayerQuality(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _viewerplayerquality = "";

    if (settings.value("viewerplayerquality", "").toString().length() > 1) {
        _viewerplayerquality = settings.value("viewerplayerquality", "").toString();

    }

    return _viewerplayerquality;


}


void genericHelper::setPlayerCacheSize(int cachesize){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerplayercachesize", QString::number(cachesize));
         settings.sync();


}

int genericHelper::getPlayerCacheSize(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _viewerplayercachesize = "";

    if (settings.value("viewerplayercachesize", "").toString().length() > 1) {
        _viewerplayercachesize = settings.value("viewerplayercachesize", "").toString();

    }

    return _viewerplayercachesize.toInt();


}



void genericHelper::setStreamHeight(int streamHeight){


         QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
         settings.setValue("viewerstreamheight", QString::number(streamHeight));
         settings.sync();


}


QString genericHelper::getOAuthAccessToken(){

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _oauthtoken = "";

    if (settings.value("oauthAccessToken", "").toString().length() > 1) {
        _oauthtoken = settings.value("oauthAccessToken", "").toString();

    }

    return _oauthtoken;


}

QString genericHelper::getAppDir(){

    QString _appDir = QDir::homePath() + QDir::separator () + genericHelper::getAppName();

    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    if (settings.value("appDir", "").toString().length() > 1) {


        _appDir = settings.value("appDir", "").toString();

    } else {


        QSettings settings(SETTINGS_COMPANY,genericHelper::getAppName());
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

QString genericHelper::getAppName()
{
     return SETTINGS_PRODUCT;
}

void genericHelper::log(QString logstring) {
    QFile file(genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log");
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    return;

    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss") << ": " << logstring << "\n";
    out.flush();

    file.close();
}

void genericHelper::deleteLog() {
    QFile file(genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log");
    file.remove();
    file.close();
}


