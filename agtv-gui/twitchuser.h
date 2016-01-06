#ifndef TWITCHUSER_H
#define TWITCHUSER_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include "twitchobject.h"
#include "twitchchannel.h"

#include "generichelper.h"

class TwitchUser : public TwitchObject
{
    Q_OBJECT
    public:

        TwitchUser(QObject *parent, const QString oAuthToken, const QString username, const qint64 defaultTimerInterval = 1000);


        QMap<QString, TwitchChannel*> getFollowedChannels();
        QMap<QString, TwitchChannel*> getBookmarkedChannels();
        void on_timedUpdate();


    private:
        bool currentlyUpdating;
        bool followedChannelsDataChanged;
        bool bookmarkedChannelsDataChanged;
        void loadBookmarks();


        const QString userName;
        QMap<QString, TwitchChannel*> followedChannels;
        QMap<QString, TwitchChannel*> bookmarkedChannels;



    private slots:
        void updateFromJsonResponseUserFollowedChannels(const QJsonDocument &jsonResponseBuffer);
        void twitchNetworkError(const QString errorString);

    signals:
        void twitchFollowedChannelsDataChanged(const bool followedChannelsDataChanged);
        void twitchBookmarkedChannelsDataChanged(const bool bookmarkedChannelsDataChanged);

};



#endif // TWITCHUSER_H
