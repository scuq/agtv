#ifndef TWITCHCHANNEL_H
#define TWITCHCHANNEL_H

#include <QByteArray>
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
        ~TwitchChannel();

        ChannelOnlineStatus getChannelOnlineStatus() const;
        QString getChannelOnlineStatusString() const;
        QString getChannelName() const;
        QString getChannelTitle() const;
        QString getChannelGame() const;
        QString getChannelUrl() const;
        QString getChannelLogoUrl() const;
        qint64 getChannelViewers() const;
        QString getChannelViewersString() const;
        qint64 getChannelFollowers() const;
        bool getIsPartner() const;
        bool getIsPlaylist() const;
        bool getIsHosting() const;
        QString getHostedChannel() const;
        QString getAccessToken() const;

        void on_timedUpdate();

        void requestPlaylist();
        void requestHostedPlaylist();
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

        QString accessToken;

        bool isHosting;
        bool isPlaylist;

        QString hostedChannel;

        bool updateChannelData(const QJsonObject channelObject);

        void doStreamUpdate();
        void doChannelUpdate();
        void doHostUpdate();

        QString getPlayListUrl(QString channel, QString token, QString sig);
        void setStreamUrl(QString url);
        void downloadUrl(QUrl url);
        QString buildPlaylistUrlFromJson(const QJsonDocument &jsonResponseBuffer);
        QMap<QString, QString> parseM3U8Playlist(QString m3u8playlist);
        bool updateOnlineStatus(const ChannelOnlineStatus currentChannelOnlineStatus);

private slots:
        void updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer);
        void twitchNetworkError(const QString errorString);
        void updateFromJsonResponseHost(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseChannel(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseAccessTokenReady(const QString &_channel, const QJsonDocument &jsonResponseBuffer);

        void fileDownloaded();
        void on_downloadedPlaylistReady(const QByteArray playlist);
signals:
        void twitchChannelDataChanged(const bool onlineStatusChanged);
        void TwitchChannelPlaylistUrlReady(const QString game, const QString url);
        void twitchChannelQualityUrlsReady(const QString game, const QMap<QString, QString> qualityUrls);
        void downloadedPlaylistReady(const QByteArray playlist);
};

#endif // TWITCHCHANNEL_H
