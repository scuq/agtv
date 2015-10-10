#ifndef GENERICHELPER_H
#define GENERICHELPER_H

#include "QString"
#include <QFileInfo>
#include <QDateTime>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDateTime>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QTextStream>
#include <QTimer>
#include "processlauncher.h"
#include <QThread>
#include <QHash>



class genericHelper : public QObject
{
    Q_OBJECT
public:
    static genericHelper &GetInstance(){
        static genericHelper instance;
        return instance;
    }


    static QString getAppDir();
    static QString getAppName();
    static void log(QString logstring);
    static void deleteLog();
    static QString getOAuthAccessToken();
    static void setOAuthAccessToken(QString oauthtoken);
    static int getConcurrentStreams();
    static void setConcurrentStreams(int concurrentstreams);
    static int getPrimaryScreenWidth();
    static void setPrimaryScreenWidth(int screenwidth);
    static bool setPosition(QString posname, QString geo);
    static bool deletePosition(QString posname);
    static QString getPosition(QString posname);
    static QHash<QString, QString> getPositions();
    static int getStreamWidth();
    static void setStreamWidth(int streamWidth);
    static int getPlayerCacheSize();
    static void setPlayerCacheSize(int cachesize);
    static QString getPlayerQuality();
    static void setPlayerQuality(QString quality);
    static int getSelectedScreen();
    static void setSelectedScreen(int selectedScreen);
    static int getSelectedTab();
    static void setSelectedTab(int tab);
    static int getStreamHeight();
    static void setStreamHeight(int streamHeight);
    static void setUsername(QString username);
    static QString getUsername();
    static void setFollows(QStringList follows);
    static void addFollow(QString follow);
    static QStringList getFollows();
    static void setBookmarks(QStringList follows);
    static void addBookmark(QString bookmark);
    static void saveGeometry(QString window, QVariant geo);
    static void saveWindowstate(QString window, QVariant state);
    static QVariant getGeometry(QString window);
    static QVariant getWindowstate(QString window);
    static QStringList getBookmarks();
    static void setVlcArgs(QStringList args);
    static QStringList getVlcArgs();
    static void setViewerStreams(QStringList streams);
    static QStringList getViewerStreams();
    static void setViewerStreamsMute(QStringList mutes);
    static QStringList getViewerStreamsMute();
    static void setOBSBin(QString obsbinpath);
    static QString getOBSBin();
    static void executeOBS();
    static void writeTwitcherOBSConfig(QString streamkey);
    static void writeTwitcherOBSScenesConfig();
    static void executeAddonHexchat(QStringList follows);
    static void executeAddonLivestreamer(QString player, QString url, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");

    static bool openLogWithNotepad();
    static void setErrorTimestamp();
    static bool getEnoughTimeSinceLastErrorElapsed();
    static int getUpdateInterval();
    static void setUpdateInterval(int interval);
    static bool getClearLogOnStartup();
    static void setClearLogOnStartup(bool clearlog);
    static bool getCloseToTray();
    static void setCloseToTray(bool closetotray);
    static bool getStreamOnlineNotify();
    static void setStreamOnlineNotify(bool streamonlinenotify);
    static bool getLoadGameImages();
    static void setLoadGameImages(bool gameimages);
    static bool getCheckUpdate();
    static void setCheckUpdate(bool checkupdate);
    static bool getJoinFollow();
    static void setJoinFollow(bool join);
    static bool getJoinBookmarks();
    static void setJoinBookmarks(bool join);
    static bool getStreamPositioning();
    static void setStreamPositioning(bool positioning);
    static bool getShowOfflineStreamers();
    static void setShowOfflineStreamers(bool showoffline);


signals:

public slots:

};




#endif // GENERICHELPER_H
