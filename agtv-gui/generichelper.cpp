#include "generichelper.h"

#include <QDebug>
#include <QMessageBox>
#include <QByteArray>

void genericHelper::setOAuthAccessToken(QString oauthtoken)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("oauthAccessToken", oauthtoken);
    settings.sync();
}

void genericHelper::setUsername(QString username)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("username", username);
    settings.sync();
}

void genericHelper::setOBSBin(QString obsbinpath)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("obsbin", obsbinpath);
    settings.sync();
}

void genericHelper::setUpdateInterval(int interval)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("update_interval", QString::number(interval));
    settings.sync();
}

int genericHelper::getUpdateInterval()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _interval = "5";

    if (settings.value("update_interval", "12").toString().length() > 1) {
        _interval = settings.value("update_interval", "").toString();
    }

    return _interval.toInt();
}

int genericHelper::getUpdateIntervalMsec()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _interval = "5";

    if (settings.value("update_interval", "12").toString().length() > 1) {
        _interval = settings.value("update_interval", "").toString();
    }

    return _interval.toInt()*1000;
}

void genericHelper::setViewerStreamsMute(QStringList mutes)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerstreamsmute", mutes.join(","));
    settings.sync();
}

QStringList genericHelper::getViewerStreamsMute()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerstreamsmute", "").toString().split(",");
}

void genericHelper::setViewerStreams(QStringList streams)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerstreams", streams.join(","));
    settings.sync();
}

QStringList genericHelper::getViewerStreams()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerstreams", "").toString().split(",");
}

void genericHelper::setFollows(QStringList follows)
{
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

QStringList genericHelper::getFollows()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("follows", "").toString().split(",");
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
    return settings.value("geo_"+window, "");
}

QVariant genericHelper::getWindowstate(QString window)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("windowstate_"+window, "");
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

void genericHelper::setCheckUpdate(bool checkupdate)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("check_update", checkupdate);
    settings.sync();
}

bool genericHelper::getJoinFollow()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("join_follow", false).toBool();
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
    return settings.value("join_bookmarks", false).toBool();
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
    return settings.value("stream_positioning", false).toBool();
}

void genericHelper::setStreamPositioning(bool positioning)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("stream_positioning", positioning);
    settings.sync();
}

bool genericHelper::getStreamQuality()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("stream_quality_selection", false).toBool();
}

void genericHelper::setStreamQuality(bool quality)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("stream_quality_selection", quality);
    settings.sync();
}

bool genericHelper::getShowOfflineStreamers()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("show_offline_streamers", true).toBool();
}

void genericHelper::setShowOfflineStreamers(bool showoffline)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("show_offline_streamers", showoffline);
    settings.sync();
}

bool genericHelper::getShowApproximateViewerCount()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("show_approximate_viewer_count", false).toBool();
}

void genericHelper::setShowApproximateViewerCount(const bool approxviewercount)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("show_approximate_viewer_count", approxviewercount);
    settings.sync();
}

bool genericHelper::getShowFollowerCount()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("show_follower_count", false).toBool();
}

void genericHelper::setShowFollowerCount(const bool followercount)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("show_follower_count", followercount);
    settings.sync();
}

QString genericHelper::getUpdateCheckUrl()
{
    return "http://agtv.abyle.org/version/latest_"+genericHelper::getAppName();
}

bool genericHelper::getCheckUpdate()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("check_update", true).toBool();
}

void genericHelper::setLoadGameImages(bool gameimages)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("load_game_images", gameimages);
    settings.sync();
}

bool genericHelper::getLoadGameImages()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("load_game_images", true).toBool();
}

void genericHelper::setClearLogOnStartup(bool clearlog)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("clear_log_on_start", clearlog);
    settings.sync();
}

bool genericHelper::getCloseToTray()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("close_to_tray", false).toBool();
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
    return settings.value("stream_online_notify", false).toBool();
}

void genericHelper::setStreamOnlineNotify(bool streamonlinenotify)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("stream_online_notify", streamonlinenotify);
    settings.sync();
}

bool genericHelper::getClearLogOnStartup()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("clear_log_on_start", false).toBool();
}

QString genericHelper::getUsername()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("username", "").toString();
}

QString genericHelper::getOBSBin()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("obsbin", "").toString();
}

bool genericHelper::isOnline(const QString status)
{
    QRegExp regexEmail("^online" );

    if( regexEmail.indexIn( status ) != -1 ) {
        return true;
    } else {
        return false;
    }
}

bool genericHelper::isHosting(const QString status)
{
    QRegExp regexEmail("^hosting" );

    if( regexEmail.indexIn( status ) != -1 ) {
        return true;
    } else {
        return false;
    }
}

bool genericHelper::isPlaylist(const QString status)
{
    QRegExp regexEmail("^playlist" );

    if( regexEmail.indexIn( status ) != -1 ) {
        return true;
    } else {
        return false;
    }
}

void genericHelper::executeOBS()
{
    genericHelper::getOBSBin();

    QProcess *process = new QProcess(qApp);
    QString program = genericHelper::getOBSBin();

    QStringList args;

    args << "";

    process->start(program, args);

    genericHelper::log("launched: "+program);
}

void genericHelper::writeTwitcherOBSConfig(QString streamkey)
{
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

void genericHelper::writeTwitcherOBSScenesConfig()
{

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

int genericHelper::executeAddonHexchat(QStringList channelsToJoin)
{
    QString addonHexchatBin = "";

    addonHexchatBin = genericHelper::getHexChatPath();

    QFile addonHexchatBinFile( addonHexchatBin );

    if( addonHexchatBinFile.exists() ) {

        QDir addonHexchatDir(genericHelper::getAppDir() + QDir::separator() + "addon_hexchat_config");
        if (!addonHexchatDir.exists()) {
            addonHexchatDir.mkpath(".");
        }

        QFile file(genericHelper::getAppDir() + QDir::separator() + "addon_hexchat_config" + QDir::separator() + "servlist.conf");
        if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        return 1;

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
            << "C=CAP REQ :twitch.tv/membership\n"
            << "\n";
        out.flush();

        file.close();

        // Check for config file, if not exists create it and skip network list by default
        QFile conffile(genericHelper::getAppDir() + QDir::separator() + "addon_hexchat_config" + QDir::separator() + "hexchat.conf");
        if( ! conffile.exists() ) {
            if ( ! conffile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text) )
                return 1;

            QTextStream out(&conffile);
            out << "gui_slist_skip = 1";

            conffile.close();
        }

        qDebug() << addonHexchatBin;

        QStringList args;

        #ifdef Q_OS_WIN
        args << "-d" << genericHelper::getAppDir().replace("/", "\\") + QDir::separator() + "addon_hexchat_config";
        #else
        args << "-d" << genericHelper::getAppDir() + QDir::separator() + "addon_hexchat_config";
        #endif
        qDebug() << args;

        QProcess *process = new QProcess(qApp);

        QString program = addonHexchatBin;

        process->start(program, args);

        genericHelper::log("launched: "+program);

        return 0;
    } else {
        return 1;
    }
}


bool genericHelper::openLogWithNotepad()
{
    QProcess *notepad = new QProcess(qApp);
    notepad->start("notepad.exe", QStringList() << genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log");

    return true;
}

QString genericHelper::getLogFile()
{
    return genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log";
}

void genericHelper::setErrorTimestamp()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("errorTimestamp",  QDateTime::currentMSecsSinceEpoch());
    settings.sync();
}

bool genericHelper::getEnoughTimeSinceLastErrorElapsed()
{
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

int genericHelper::getStreamHeight()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerstreamheight", 10).toInt();
}

int genericHelper::getStreamWidth()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerstreamwidth", 10).toInt();
}

int genericHelper::getSelectedScreen()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerselectedscreen", 0).toInt();
}

int genericHelper::getConcurrentStreams()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerconcurrentstreams", 0).toInt();
}

void genericHelper::setConcurrentStreams(int concurrentstreams)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerconcurrentstreams", QString::number(concurrentstreams));
    settings.sync();
}

void genericHelper::setSelectedScreen(int selectedScreen)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerselectedscreen", QString::number(selectedScreen));
    settings.sync();
}

int genericHelper::getSelectedTab()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("selectedtab", 0).toInt();
}

void genericHelper::setSelectedTab(int tab)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("selectedtab", QString::number(tab));
    settings.sync();
}

void genericHelper::setStreamWidth(int streamWidth)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerstreamwidth", QString::number(streamWidth));
    settings.sync();
}

void genericHelper::setPrimaryScreenWidth(int screenwidth)
{
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

int genericHelper::getPrimaryScreenWidth()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerprimaryscreenwidth", 10).toInt();
}

void genericHelper::setPlayerQuality(QString quality)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerplayerquality", quality);
    settings.sync();
}

QString genericHelper::getPlayerQuality()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerplayerquality", "").toString();
}


void genericHelper::setPlayerCacheSize(int cachesize)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerplayercachesize", QString::number(cachesize));
    settings.sync();
}

int genericHelper::getPlayerCacheSize()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("viewerplayercachesize", 0).toInt();
}

void genericHelper::setStreamHeight(int streamHeight)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("viewerstreamheight", QString::number(streamHeight));
    settings.sync();
}


QString genericHelper::getOAuthAccessToken()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _oauthtoken = "";

    if (settings.value("oauthAccessToken", "").toString().length() > 1) {
        _oauthtoken = settings.value("oauthAccessToken", "").toString();

    }

    return _oauthtoken;
}

QString genericHelper::getAppDir()
{
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

void genericHelper::log(QString logstring)
{
    QFile file(genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log");
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    return;

    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss") << ": " << logstring << "\n";
    out.flush();

    file.close();
}

void genericHelper::deleteLog()
{
    QFile file(genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log");
    file.remove();
    file.close();
}

void genericHelper::setVlcPath(QString path)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("vlc_path", path);
    settings.sync();
}

QString genericHelper::getVlcPath()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _vlcpath;

    if (settings.value("vlc_path", "").toString().length() > 1) {
        _vlcpath = settings.value("vlc_path", "").toString();
    } else {

        #if defined(Q_OS_WIN)
             _vlcpath = QCoreApplication::applicationDirPath() + "/3rdparty-addons/vlc/VLCPortable.exe";
        #else
             _vlcpath = "vlc";
        #endif
            qDebug() << _vlcpath;
        _vlcpath = QCoreApplication::applicationDirPath();
    }

    return _vlcpath;
}

void genericHelper::setHexChatPath(QString path)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("hexchat_path", path);
    settings.sync();
}

QString genericHelper::getHexChatPath()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QString _hexchatpath;

    if (settings.value("hexchat_path", "").toString().length() > 1) {
        _hexchatpath = settings.value("hexchat_path", "").toString();
    } else {
    #if defined(Q_OS_WIN)
         _hexchatpath = QCoreApplication::applicationDirPath() + "/3rdparty-addons/hexchat/hexchat.exe";
    #else
         _hexchatpath = "hexchat";
    #endif
    }

    return _hexchatpath;
}

void genericHelper::setFitAllContentToWindow(const bool fit)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("fit_all_content_to_window", fit);
    settings.sync();
}

bool genericHelper::getFitAllContentToWindow()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("fit_all_content_to_window", false).toBool();
}

QByteArray genericHelper::getFollowerTableViewColumnWidth()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("follower_tableview_column_width").toByteArray();
}

void genericHelper::setFollowerTableViewColumnWidth(const QByteArray _columnwidth)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("follower_tableview_column_width", _columnwidth);
    settings.sync();
}

QByteArray genericHelper::getBookmarksTableViewColumnWidth()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("bookmarks_tableview_column_width").toByteArray();
}

void genericHelper::setBookmarksTableViewColumnWidth(const QByteArray _columnwidth)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("bookmarks_tableview_column_width", _columnwidth);
    settings.sync();
}

QMap<int, int> genericHelper::getFollowerTableViewColumnWidthManual()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QMap<int, int> _columnwidth;
    QStringList _columnwidthstringlist;

    _columnwidthstringlist = settings.value("follower_tableview_column_width").toStringList();
    int i=0;
    for(const QString& string : _columnwidthstringlist) {
        _columnwidth[i] = string.toInt();
        ++i;
    }

    return _columnwidth;
}

void genericHelper::setFollowerTableViewColumnWidthManual(const QMap<int, int> columnwidth)
{
    QStringList _columnwidthstringlist;
    for (const int& width : columnwidth) {
        _columnwidthstringlist.append(QString::number(width,'f',0));
    }
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("follower_tableview_column_width", _columnwidthstringlist);
    settings.sync();
}

QMap<int, int> genericHelper::getBookmarksTableViewColumnWidthManual()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    QMap<int, int> _columnwidth;
    QStringList _columnwidthstringlist;

    _columnwidthstringlist = settings.value("bookmarks_tableview_column_width").toStringList();
    int i=0;
    for(const QString& string : _columnwidthstringlist) {
        _columnwidth[i] = string.toInt();
        ++i;
    }

    return _columnwidth;
}

void genericHelper::setBookmarksTableViewColumnWidthManual(const QMap<int, int> columnwidth)
{
    QStringList _columnwidthstringlist;
    for (const int& width : columnwidth) {
        _columnwidthstringlist.append(QString::number(width,'f',0));
    }
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("bookmarks_tableview_column_width", _columnwidthstringlist);
    settings.sync();
}

int genericHelper::getFollowerSortCol()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("follower_sort_column", "0").toInt();
}

Qt::SortOrder genericHelper::getFollowerSortOrder()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    Qt::SortOrder _order;
    _order = Qt::SortOrder(settings.value("follower_sort_order", "0").toInt());
    return _order;
}

int genericHelper::getBookmarksSortCol()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("bookmarks_sort_column", "0").toInt();
}

Qt::SortOrder genericHelper::getBookmarksSortOrder()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());

    Qt::SortOrder _order;
    _order = Qt::SortOrder(settings.value("bookmarks_sort_order", "0").toInt());
    return _order;
}

void genericHelper::setFollowerSortCol(const int col)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("follower_sort_column",  col);
    settings.sync();
}

void genericHelper::setFollowerSortOrder(const Qt::SortOrder order)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("follower_sort_order",  order);
    settings.sync();
}

void genericHelper::setBookmarksSortCol(const int col)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("bookmarks_sort_column",  col);
    settings.sync();
}

void genericHelper::setBookmarksSortOrder(const Qt::SortOrder order)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("bookmarks_sort_order",  order);
    settings.sync();
}

bool genericHelper::getInternalVLC()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("internal_vlc", "0").toBool();
}

void genericHelper::setInternalVLC(const bool internalvlc)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("internal_vlc",  internalvlc);
    settings.sync();
}

bool genericHelper::getInternalChat()
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    return settings.value("internal_chat", "0").toBool();
}

void genericHelper::setInternalChat(const bool internalchat)
{
    QSettings settings(SETTINGS_COMPANY, genericHelper::getAppName());
    settings.setValue("internal_chat",  internalchat);
    settings.sync();
}

QString genericHelper::extractStreamNameFromURL(QString _url)
{
    QRegExp re("\\/([^\\/]*)$");
    _url.contains(re);
    return re.cap(1);
}

QString genericHelper::streamURLParser(QString _stream)
{
    QString streamname;

    // Detect if link
    bool isLink = false;
    if(_stream.contains("http://www.twitch.tv", Qt::CaseInsensitive) ||
       _stream.contains("https://www.twitch.tv", Qt::CaseInsensitive) ) {
        isLink = true;
    }

    if(isLink) {
        streamname = genericHelper::extractStreamNameFromURL(_stream);
    } else {
        // Assume it is the name already
        streamname = _stream;
    }

    streamname = streamname.toLower().trimmed();

    return streamname;
}
