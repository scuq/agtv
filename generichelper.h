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
#include <QByteArray>



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
    static bool isOnline(const QString status);
    static bool isHosting(const QString status);
    static bool isPlaylist(const QString status);
    static void executeOBS();
    static void writeTwitcherOBSConfig(QString streamkey);
    static void writeTwitcherOBSScenesConfig();
    static int executeAddonHexchat(QStringList channelsToJoin);
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
    static bool getShowApproximateViewerCount();
    static void setShowApproximateViewerCount(bool approxviewercount);

    static bool getStreamQuality();
    static void setStreamQuality(bool quality);

    static bool getInternalVLC();
    static void setInternalVLC(const bool internalvlc);

    static QString getLogFile();

    static QString getUpdateCheckUrl();

    static void setVlcPath(QString path);
    static QString getVlcPath();
    static void setHexChatPath(QString path);
    static QString getHexChatPath();

    static void setFitAllContentToWindow(const bool fit);
    static bool getFitAllContentToWindow();

    static QByteArray getFollowerTableViewColumnWidth();
    static void setFollowerTableViewColumnWidth(const QByteArray _columnwidth);
    static QByteArray getBookmarksTableViewColumnWidth();
    static void setBookmarksTableViewColumnWidth(const QByteArray _columnwidth);

    static QMap<int, int> getFollowerTableViewColumnWidthManual();
    static void setFollowerTableViewColumnWidthManual(const QMap<int, int> columnwidth);

    static QMap<int, int> getBookmarksTableViewColumnWidthManual();
    static void setBookmarksTableViewColumnWidthManual(const QMap<int, int> columnwidth);

    static int getFollowerSortCol();
    static Qt::SortOrder getFollowerSortOrder();
    static int getBookmarksSortCol();
    static Qt::SortOrder getBookmarksSortOrder();

    static void setFollowerSortCol(const int col);
    static void setFollowerSortOrder(const Qt::SortOrder order);
    static void setBookmarksSortCol(const int col);
    static void setBookmarksSortOrder(const Qt::SortOrder order);

signals:

public slots:

};




#endif // GENERICHELPER_H
