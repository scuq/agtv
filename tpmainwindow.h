#ifndef TPMAINWINDOW_H
#define TPMAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include "twitchapi.h"
#include "generichelper.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QComboBox>
#include <QTreeWidgetItem>
#include "dialogoauthsetup.h"
#include "dialogpositioner.h"
#include "dialoglaunch.h"
#include "processlauncher.h"
#include "dialogoptions.h"
#include <QSystemTrayIcon>
#include <QInputDialog>
#include <QCloseEvent>
#include <QThread>
//#include <streamtablemodel.h>
#include <QStandardItemModel>
#include "advqsortfilterproxymodel.h"
#include "imageloader.h"
#include "updatecheck.h"

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

    void on_actionSetup_Twitch_Auth_triggered();

    void on_updateNotify(const QString &latestVersion);

    void on_actionPositioner_triggered();

    void on_actionTwitch_Browser_triggered();

    void on_actionHexChat_triggered();

    void trayIconClicked(QSystemTrayIcon::ActivationReason);

    void on_notifyByTray(QString title, QString message);


    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionReport_Bug_triggered();

    void on_actionRefresh_triggered();

    void myQuit();

    void on_loadData();

    void openStreamBrowser();

    void openStreamBrowserBookmark();

    void addBookmarkHosted();

    void deleteBookmark();

    void addBookmark();

    void on_actionOptions_triggered();

    void on_actionQuit_triggered();



    void on_actionCredits_triggered();

    void on_actionShow_Offline_Streamers_toggled(bool arg1);

    void on_Ready();

    void on_SwitchInputEnabled(bool enable);

    void on_actionShow_Offline_Streamers_triggered();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableViewBookmarks_doubleClicked(const QModelIndex &index);

    void on_tableViewBookmarks_customContextMenuRequested(const QPoint &pos);

    void on_actionLogfile_triggered();

    void on_tabWidget_currentChanged(int index);

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

    QStandardItemModel *stmodel;
    AdvQSortFilterProxyModel *stproxymodel;

    QStandardItemModel *stmodelbookmarks;
    AdvQSortFilterProxyModel *stproxymodelbookmarks;



    QPixmap offline;
    QPixmap online;

    bool showOfflineStreamers;

    QTimer *refreshTimer;
    int updateInterval;


    dialogOauthSetup *diaOauthSetup;
    DialogPositioner *diaPositioner;
    DialogLaunch *diaLaunch;
    DialogOptions *diaOptions;

    QDesktopWidget *desktop;
    void updateCurrentScreenData();
    void disableInput();
    void enableInput();
    void loadBookmarks();
    void saveBookmarks();
    void enableDelete();
    void disableDelete();
    void loadData();


    QList<QThread*> playerThreads;



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
    QAction *delete_bookmark;
    QAction *add_bookmark;
    QAction *add_hosted_bookmark;


    updateCheck *uc;

    QString currArch;


    TwitchApi *tw;

public slots:
    void executePlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");

signals:

    void setStreamTitle(QString, QString);
    void setStreamUrl(QString);


};

#endif // TPMAINWINDOW_H
