#ifndef TWITCHUSERLOCAL_H
#define TWITCHUSERLOCAL_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>

#include "twitchchannel.h"



class TwitchUserLocal : public QObject
{
    Q_OBJECT
    public:

        TwitchUserLocal(QObject *parent, const qint64 defaultTimerInterval = 1000);
        
        QMap<QString, TwitchChannel*> getBookmarkedChannels();
 
        void loadBookmarks();
        
        QString getStoredOAuthAccessToken(QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        QString getStoredUsername(QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        QStringList getBookmarks(QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool setBookmarks(QStringList bookmarks, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool addBookmark(QString bookmark, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool deleteBookmark(QString bookmark, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool saveOAuthAccessToken(QString oAuthAccessToken, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool saveUsername(QString userName, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool isUserSetupOk(QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool backupSettings(QString filename, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool restoreSettings(QString filename, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        
    public slots:
        void onSaveOAuthAccessToken(QString oAuthAccessToken);
        void onSaveUsername(QString userName);
        

    private:
  

        bool bookmarkedChannelsDataChanged;
        
        const QString userName;

        QMap<QString, TwitchChannel*> bookmarkedChannels;
        
        qint64 getRefreshTimerInterval();
        
        qint64 refreshTimerInterval;
        

    private slots:


        
    signals:
        void twitchBookmarkedChannelsDataChanged(const bool bookmarkedChannelsDataChanged);
        void oAuthAccessTokenLoaded(QString oAuthAccessToken);
        void backupRestoredSuccessful(bool);
        
        
};
#endif // TWITCHUSERLOCAL_H
