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
#include <QCloseEvent>
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

    void on_actionSetup_Twitch_Auth_triggered();

    void on_pushButtonAddBookmark_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidgetBookmarks_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_actionPositioner_triggered();

    void on_actionTwitch_Browser_triggered();

    void on_actionHexChat_triggered();

    void on_treeWidgetBookmarks_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidgetBookmarks_itemSelectionChanged();

    void on_pushButtonDeleteBookmarkCancel_clicked();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionReport_Bug_triggered();

    void on_actionRefresh_triggered();

    void myQuit();

    void on_loadData();


    void on_actionOptions_triggered();

    void on_actionQuit_triggered();

    void on_pushButtonDeleteBookmark_clicked();

    void on_actionCredits_triggered();

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

    QPixmap offline;
    QPixmap online;

    QTimer *refreshTimer;
    int updateInterval;


    dialogOauthSetup *diaOauthSetup;
    DialogPositioner *diaPositioner;
    DialogLaunch *diaLaunch;
    DialogOptions *diaOptions;

    QDesktopWidget *desktop;
    void updateCurrentScreenData();
    void disableInput();
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



    TwitchApi *tw;
    TwitchApi *tw2;

public slots:
    void executePlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute=false, QString quality="best");

signals:

    void setStreamTitle(QString, QString);
    void setStreamUrl(QString);


};

#endif // TPMAINWINDOW_H
