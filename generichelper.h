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



class genericHelper : public QObject
{
    Q_OBJECT
public:
    static genericHelper &GetInstance(){
        static genericHelper instance;
        return instance;
    }

    static QString getAppDir();
    static void log(QString logstring);
    static void deleteLog();
    static QString getOAuthAccessToken();
    static void setOAuthAccessToken(QString oauthtoken);
    static void setUsername(QString username);
    static QString getUsername();
    static void setFollows(QStringList follows);
    static QStringList getFollows();
    static void setOBSBin(QString obsbinpath);
    static QString getOBSBin();
    static void executeOBS();
    static void writeTwitcherOBSConfig(QString streamkey);
    static void writeTwitcherOBSScenesConfig();
    static void executeAddonHexchat(QStringList follows);
    static bool openLogWithNotepad();
    static void setErrorTimestamp();
    static bool getEnoughTimeSinceLastErrorElapsed();
    static int getUpdateInterval();
    static void setUpdateInterval(int interval);
    static bool getClearLogOnStartup();
    static void setClearLogOnStartup(bool clearlog);
    static bool getLoadGameImages();
    static void setLoadGameImages(bool gameimages);
    static bool getCheckUpdate();
    static void setCheckUpdate(bool checkupdate);

signals:

public slots:

};

#endif // GENERICHELPER_H
