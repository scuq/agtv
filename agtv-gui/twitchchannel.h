#ifndef TWITCHCHANNEL_H
#define TWITCHCHANNEL_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include "twitchobject.h"

//!  Channel class for the Twitch API interface
/*!
  Provides channel related get functions and corresponding signals to interface with the Twitch
  Api.
*/

class TwitchChannel : public TwitchObject
{
    Q_OBJECT
    public:
        enum class ChannelOnlineStatus {
            offline,
            online,
            playlist,
            hosting
        };

        TwitchChannel(QObject *parent, const QString oAuthToken, const QString channel, const qint64 defaultTimerInterval = 1000);

        ChannelOnlineStatus getChannelOnlineStatus() const;
        QString getChannelName() const;
        QString getChannelTitle() const;
        QString getChannelGame() const;
        QString getChannelUrl() const;
        QString getChannelLogoUrl() const;
        qint64 getChannelViewers() const;
        qint64 getChannelFollowers() const;
        bool getIsPartner() const;
        bool getIsPlaylist() const;
        bool getIsHosting() const;
        QString getHostedChannel() const;

        void on_timedUpdate();

    private:
        bool currentlyUpdating, currentlyUpdatingHost, currentlyUpdatingChannel;

        const QString channelName;
        QString channelStatus;
        QString channelGame;
        QString channelUrl;
        QString channelLogoUrl;
        qint64 channelViewers;
        qint64 channelFollowers;
        qint64 channelId;
        bool isPartner;
        ChannelOnlineStatus channelOnlineStatus;

        bool isHosting;
        bool isPlaylist;

        QString hostedChannel;

        bool updateChannelData(const QJsonObject channelObject);

        void doStreamUpdate();
        void doChannelUpdate();
        void doHostUpdate();

    private slots:
        void updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer);
        void twitchNetworkError(const QString errorString);
        void updateFromJsonResponseHost(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseChannel(const QJsonDocument &jsonResponseBuffer);

    signals:
        void twitchChannelDataChanged(const bool onlineStatusChanged);
};

#endif // TWITCHCHANNEL_H
