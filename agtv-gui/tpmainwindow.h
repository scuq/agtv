#ifndef TPMAINWINDOW_H
#define TPMAINWINDOW_H

#include <QClipboard>
#include <QInputDialog>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTableView>

#include "generichelper.h"

#include "dialogoauthsetup.h"
#include "dialogoptions.h"
#include "dialogpositioner.h"
#include "dialoglaunch.h"
#include "dialogshowlogfile.h"
#ifdef WINTERNALVLC
#include "dialogvideoplayer.h"
#endif
#ifdef INTERNALIRC
#include "ircclient.h"
#endif
#include "dialoggamebrowser.h"

#include "agtvdefaultitemdelegate.h"
#include "advqsortfilterproxymodel.h"
#include "updatecheck.h"
#include "twitchchannel.h"
#include "twitchuser.h"
#include "twitchuserlocal.h"

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
    void onTwitchFollowedChannelsDataChanged(const bool &dataChanged);
    void onTwitchBookmarkedChannelsDataChanged(const bool &dataChanged);

    void updateFromJsonResponseFollow(const QJsonDocument &jsonResponseBuffer);
    void updateFromJsonResponseUnfollow(const QJsonDocument &jsonResponseBuffer);

    void updateOnUnfollow(QString msg);

    void on_actionSetup_Twitch_Auth_triggered();
    
    void onBrowserAuthorizeRequested();
    
    //void onIsAuthOkCheck(QString newOAuthToken);

    void showOnStatusBar(const QString errorMsg);

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

    void copyUrl();

    void on_actionShow_Game_Browser_triggered();

    void startFromGBrowser(const QString stream);

private:
    Ui::tpMainWindow *ui;

    QString version;

    int xOffset;
    int yOffset;
    bool launchBookmarkEnabled;

    AgtvDefaultItemDelegate *AgtvDefItemDelegate;

    QStandardItemModel *stmodel;
    AdvQSortFilterProxyModel *stproxymodel;

    QStandardItemModel *stmodelbookmarks;
    AdvQSortFilterProxyModel *stproxymodelbookmarks;

    dialogOauthSetup *diaOauthSetup;
    DialogPositioner *diaPositioner;
    DialogLaunch *diaLaunch;
    DialogOptions *diaOptions;
    DialogShowLogFile *diaShowLogFile;
    DialogGameBrowser *diaTopGameBrowser;

    QClipboard *clipboard;

#ifdef WINTERNALVLC
    QVector<DialogVideoPlayer *> diaVideoPlayers;
#endif
    
#ifdef INTERNALIRC
    IrcClient *ircc;
#endif

    QVector<QString> followerToRemove;

    void updateCurrentScreenData();
    void loadBookmarks();
    void saveBookmarks();
    void setEnableInput(bool enabled);
    void enableDelete();
    void disableDelete();
    void loadData();

    void prepareDiaLauncher(QString _streamer, QString _logoUrl);

    bool bunchUpdateStreamDataName(const QString &name, const QString &status,
                                   const QString &viewers);


    bool bunchUpdateStreamDataName(const QString &name, const QString &onlineStatus,
                                                 const QString &viewers, const QString &game,
                                                 const QString &status);

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

    TwitchUser *twitchUser;
    TwitchUserLocal *twitchUserLocal;
    QMap<QString, TwitchChannel*> twitchChannels;
    QMap<QString, TwitchChannel*> twitchChannelsBookmarks;

    void openHexchat();

    void setupModelsViews();
    void setupTwitchApi();
    void setupSignalsMain();
    void setupTrayIcon();
    void setupUpdateCheck();
    void setupDialogs();
    void setupSignalsTwitchApi();

public slots:
    void twitchChannelDataChanged(const bool onlineStatusChanged);

    void executePlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");
#ifdef WINTERNALVLC
    void executeInternalPlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");
#endif
    void executeExternalPlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");

signals:
    void authOk(bool);
};

#endif // TPMAINWINDOW_H
