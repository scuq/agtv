#ifndef TWITCHERMAINWINDOW_H
#define TWITCHERMAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include <QString>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QMessageBox>
#include <QCompleter>
#include <QTimer>
#include <QStringListModel>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QShortcut>
#include <QDateTime>
#include "generichelper.h"
#include "dialogoauthsetup.h"
#include "twitchapi.h"
#include "dialogabout.h"
#include "dialogobssettings.h"
#include "dialoggamestats.h"
#include "dialogsettings.h"
#include "twitchernativeeventfilter.h"

namespace Ui {
class twitcherMainWindow;
}

class twitcherMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit twitcherMainWindow(QWidget *parent = 0);
    ~twitcherMainWindow();
    void updateFormFields();

private slots:

    void toForground();

    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void on_actionClose_to_Tray_triggered();


    void updateFromJsonResponse(const QJsonDocument &jsonResponseBuffer);
    void updateFormFieldsSlot();
    void errorPopup(QString message);
    void on_actionSetup_Twitch_Auth_triggered();


    void on_pushButtonApplyTitles_clicked();

    void on_lineEditGameTitle_textChanged(const QString &arg1);

    void on_actionRefresh_triggered();

    void on_actionAbout_triggered();

    void on_pushButtonClearTitlesOnTwtich_clicked();



    void on_actionOBS_Settings_triggered();

    void on_pushButtonLaunchOBS_clicked();

    void on_pushButtonLaunchHexchat_clicked();

    void on_actionApplications_twitch_tv_triggered();

    void on_actionDashboard_twitch_tv_triggered();

    void on_pushButtonCancel_clicked();

    void on_lineEditBroadcastTitle_textChanged(const QString &arg1);

    void on_actionGame_stats_triggered();

    void on_actionLog_triggered();

    void on_lineEditBroadcastTitle_textEdited(const QString &arg1);

    void on_lineEditGameTitle_textEdited(const QString &arg1);

    void on_actionQuit_triggered();

    void on_actionSettings_triggered();

private:
    Ui::twitcherMainWindow *ui;

    dialogOauthSetup *diaOauthSetup;
    dialogAbout *diaAbout;
    dialogOBSSettings *diaOBSSettings;
    dialogGameStats *diaGameStats;
    dialogSettings *diaSettings;

    TwitchApi *tw;
    QHash<QString, QString> apiPutParams;
    QCompleter *completer;
    QStringList completerList;
    QStringListModel *completerModel;
    QTimer *refreshTimer;
    QStringList following;
    bool found;
    QString currentGame;
    QString topGame;
    double currentGameCount;
    double topGameCount;
    int errorCount;
    int updateInterval;

    TwitcherNativeEventFilter *twitcherKeyEvent;

    QPixmap nok;
    QPixmap ok;

    void registerHotkey();

    // tray icon
    void closeEvent(QCloseEvent *); // Overriding the window's close event
    QAction *open;
    QAction *close;
    void createActions();
    void createTrayIcon();
    void setIcon(QString iconname = "bird");
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    bool localTitleEdit;

};

#endif // TWITCHERMAINWINDOW_H
