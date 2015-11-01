#ifndef TPMAINWINDOW_H
#define TPMAINWINDOW_H

#include "generichelper.h"

#include "dialogoauthsetup.h"
#include "dialogoptions.h"
#include "dialogpositioner.h"
#include "dialoglaunch.h"
#include "dialogshowlogfile.h"
#ifdef WINTERNALVLC
#include "dialogvideoplayer.h"
#endif
#include "dialoggamebrowser.h"

#include "agtvdefaultitemdelegate.h"
#include "advqsortfilterproxymodel.h"
#include "filedownloader.h"
#include "imageloader.h"
#include "processlauncher.h"
#include "videoplayer.h"
#include "twitchapi.h"
#include "updatecheck.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSystemTrayIcon>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QTableView>
#include <QTreeWidgetItem>
#include <QThread>

namespace Ui {
class tpMainWindow;
}

class tpMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit tpMainWindow(QWidget *parent = 0);
    ~tpMainWindow();

private slots:

    void onChannelAccessTokenReady(const QJsonDocument &jsonResponseBuffer);

    void startM3u8Player(QString m3u8playlist);

    void updateFromJsonResponseFollows(const QJsonDocument &jsonResponseBuffer);
    void updateFromJsonResponseBookmark(const QJsonDocument &jsonResponseBuffer);
    void updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer);
    void updateFromJsonResponseChannel(const QJsonDocument &jsonResponseBuffer);
    void updateFromJsonResponseHost(const QJsonDocument &jsonResponseBuffer);

    void updateFromJsonResponseFollow(const QJsonDocument &jsonResponseBuffer);
    void updateFromJsonResponseUnfollow(const QJsonDocument &jsonResponseBuffer);

    void on_actionSetup_Twitch_Auth_triggered();

    void twitchApiNetworkError(QString error);

    void on_updateNotify(const QString &latestVersion);

    void on_actionPositioner_triggered();

    void on_actionTwitch_Browser_triggered();

    void on_actionHexChat_triggered();

    void trayIconClicked(QSystemTrayIcon::ActivationReason);

    void on_notifyByTray(QString title, QString message);

    void restoreTableViewsManual();
    void restoreSortModes();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionReport_Bug_triggered();

    void on_actionRefresh_triggered();

    void myQuit();

    void on_loadData();

    void openStreamBrowser();

    void openStreamBrowserBookmark();

    void openChatHexChat();
    void openChatHexChatBookmark();

    void addFollowerBookmark();
    void deleteFollower();

    void addBookmarkHosted();

    void deleteBookmark();

    void addBookmark();

    void addFollow();

    void on_actionOptions_triggered();

    void on_actionQuit_triggered();

    void on_tabChanged(const int tabid);

    void on_actionCredits_triggered();

    void on_actionShow_Offline_Streamers_toggled(bool arg1);

    void on_Ready();

    void on_SwitchInputEnabled(bool enable);

    void on_actionShow_Offline_Streamers_triggered();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_tableView_activated(const QModelIndex &index);

    void on_tableViewBookmarks_activated(const QModelIndex &index);

    void on_tableViewBookmarks_customContextMenuRequested(const QPoint &pos);

    void on_actionLogfile_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_actionFit_Columns_to_Content_triggered();

    void on_actionShow_Approximate_Viewer_Count_toggled(bool arg1);

    void on_settingsSaved();

    void loadQuality();

    void copyUrl();

    void on_actionShow_Game_Browser_triggered();

private:
    Ui::tpMainWindow *ui;

    QString currentScreenResolution;
    QString streamResolution;
    QStringList viewerStreams;
    QString version;
    int streamWidth;
    int streamHeight;
    int xOffset;
    int yOffset;
    bool launchBookmarkEnabled;

    VideoPlayer *playerq;

    AgtvDefaultItemDelegate *AgtvDefItemDelegate;

    QStandardItemModel *stmodel;
    AdvQSortFilterProxyModel *stproxymodel;

    QStandardItemModel *stmodelbookmarks;
    AdvQSortFilterProxyModel *stproxymodelbookmarks;

    FileDownloader *m_m3u8playlist;

    QPixmap offline;
    QPixmap online;

    bool showOfflineStreamers;

    QTimer *refreshTimer;
    int updateInterval;

    QMap<QString, QString> parseM3U8Playlist(QString m3u8playlist);

    dialogOauthSetup *diaOauthSetup;
    DialogPositioner *diaPositioner;
    DialogLaunch *diaLaunch;
    DialogOptions *diaOptions;
    DialogShowLogFile *diaShowLogFile;
    DialogGameBrowser *diaTopGameBrowser;

    QClipboard *clipboard;

#ifdef WINTERNALVLC
    // DialogVideoPlayer *diaVideoPlayer;
    QVector<DialogVideoPlayer *> diaVideoPlayers;
#endif

    QVector<QString> followerToRemove;

    QDesktopWidget *desktop;
    void updateCurrentScreenData();
    void disableInput();
    void enableInput();
    void loadBookmarks();
    void saveBookmarks();
    void enableDelete();
    void disableDelete();
    void loadData();

    void prepareDiaLauncher();

    bool bunchUpdateStreamDataName(const QString &name, const QString &status,
                                   const QString &viewers);

    static void fitTableViewToContent(QTableView *tableView);

    void saveSortModes();

    void deleteFollowerFromList(QString _name);

    QList<QThread*> playerThreads;

    void saveTableViewStates();

    // tray icon
    void closeEvent(QCloseEvent *); // Overriding the window's close event
    QAction *open;
    QAction *close;
    void createActions();
    void createTrayIcon();
    void setIcon(QString iconname = "agtv_32");
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    bool localTitleEdit;

    //tableview context meu
    QAction *open_in_browser;
    QAction *open_in_browser_bookmark;
    QAction *open_in_hexchat;
    QAction *open_in_hexchat_bookmark;
    QAction *delete_bookmark;
    QAction *add_bookmark;
    QAction *add_hosted_bookmark;
    QAction *add_follower_bookmark;
    QAction *delete_follower;
    QAction *copy_streamurl;
    QAction *add_follow;

    updateCheck *uc;

    QString currArch;


    TwitchApi *tw;

    QMap<QString, QString> channelLogoUrl;

public slots:
    void executePlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");

#ifdef WINTERNALVLC
    void executeInternalPlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");
#endif

signals:

    void setStreamTitle(QString, QString);
    void setStreamUrl(QString);
    void setStreamLogoUrl(QString);
    void setStreamUrlWithQuality(QMap<QString, QString>);


};

#endif // TPMAINWINDOW_H
