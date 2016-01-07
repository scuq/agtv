#ifndef TWITCHUSERLOCAL_H
#define TWITCHUSERLOCAL_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>


#include "twitchchannel.h"

#include "generichelper.h"

class TwitchUserLocal : public QObject
{
    Q_OBJECT
    public:

        TwitchUserLocal(QObject *parent, const qint64 defaultTimerInterval = 1000);
        
        QMap<QString, TwitchChannel*> getBookmarkedChannels();
 
        void loadBookmarks();
        
        QString getStoredOAuthAccessToken(QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        QString getStoredUsername(QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool saveOAuthAccessToken(QString oAuthAccessToken, QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);
        bool isUserSetupOk(QString company=SETTINGS_COMPANY, QString app=SETTINGS_PRODUCT);

    private:
  

        bool bookmarkedChannelsDataChanged;
        
        const QString userName;

        QMap<QString, TwitchChannel*> bookmarkedChannels;
        
        qint64 getRefreshTimerInterval();
        
        qint64 refreshTimerInterval;
        

    private slots:

    signals:
        void twitchBookmarkedChannelsDataChanged(const bool bookmarkedChannelsDataChanged);
};

#endif // TWITCHUSERLOCAL_H
