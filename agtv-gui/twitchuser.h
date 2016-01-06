#ifndef TWITCHUSER_H
#define TWITCHUSER_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include "twitchobject.h"
#include "twitchchannel.h"

class TwitchUser : public TwitchObject
{
    Q_OBJECT
    public:

        TwitchUser(QObject *parent, const QString oAuthToken, const QString username, const qint64 defaultTimerInterval = 1000);

        QMap<QString, TwitchChannel*> getFollowedChannels();

        void on_timedUpdate();
        void loadBookmarks();

    private:
        bool currentlyUpdating;
        bool followedChannelsDataChanged;
        bool bookmarkedChannelsDataChanged;

        const QString userName;
        QMap<QString, TwitchChannel*> followedChannels;



    private slots:
        void updateFromJsonResponseUserFollowedChannels(const QJsonDocument &jsonResponseBuffer);
        void twitchNetworkError(const QString errorString);

    signals:
        void twitchFollowedChannelsDataChanged(const bool followedChannelsDataChanged);

};



#endif // TWITCHUSER_H
