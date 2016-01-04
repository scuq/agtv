#ifndef TWITCHCHANNEL_H
#define TWITCHCHANNEL_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include "twitchobject.h"

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

        TwitchChannel(QObject *parent, const QString oAuthToken = "", const QString channelName = "", const qint64 defaultTimerInterval = 1000);

        ChannelOnlineStatus getChannelOnlineStatus() const;
        QString getChannelName() const;
        QString getChannelTitle() const;
        QString getChannelGame() const;
        QString getChannelUrl() const;
        qint64 getChannelViewers() const;
        qint64 getChannelFollowers() const;

public:
        void on_timedUpdate();

        bool getIsPlaylist() const;
        bool getIsHosting() const;

        QString getHostedChannel() const;

private slots:
        void updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer);
        void twitchNetworkError(const QString errorString);
        void updateFromJsonResponseHost(const QJsonDocument &jsonResponseBuffer);
        void parseTwitchNetworkResponseHost();
        void updateFromJsonResponseChannel(const QJsonDocument &jsonResponseBuffer);

private:
        bool currentlyUpdating, currentlyUpdatingHost;

        QString channelName;
        QString channelStatus;
        QString channelGame;
        QString channelUrl;
        qint64 channelViewers;
        qint64 channelFollowers;
        qint64 channelId;
        ChannelOnlineStatus channelOnlineStatus;

        bool isHosting;
        bool isPlaylist;

        QString hostedChannel;

        bool parseStreamChannelObject(const QJsonObject channelObject, QString &name,
                                      QString &status, QString &game,
                                      QString &logoUrlString, QString &partner,
                                      QString &followers, QString &channelId);

        void getHost(QString channelid);
        void getRequestHost(const QString &urlString);

signals:
        void twitchReadyHost( const QJsonDocument &twitchAsJSON );
        void twitchChannelDataChanged(const bool onlineStatusChanged);
};

#endif // TWITCHCHANNEL_H
