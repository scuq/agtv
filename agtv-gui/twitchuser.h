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

        void followChannel(QString channelName);
        void unfollowChannel(QString channelName);



    private:
        bool currentlyUpdating;
        bool followedChannelsDataChanged;
        bool bookmarkedChannelsDataChanged;

        const QString userName;
        QMap<QString, TwitchChannel*> followedChannels;



    private slots:
        void updateFromJsonResponseUserFollowedChannels(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseUserFollowChannel(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseUserUnfollowChannel(const QJsonDocument &jsonResponseBuffer);

        void twitchNetworkError(const QString errorString);

    signals:
        void twitchFollowedChannelsDataChanged(const bool followedChannelsDataChanged);
        void twitchFollowChannelError(const QString msg);
        void twitchFollowChannelSuccess(const QString msg);
        void twitchUnfollowChannelError(const QString msg);
        void twitchUnfollowChannelSuccess(const QString msg);

};



#endif // TWITCHUSER_H
