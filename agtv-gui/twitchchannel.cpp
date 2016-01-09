#include <QRegularExpression>

#include "twitchchannel.h"

#include "generichelper.h"

TwitchChannel::TwitchChannel(QObject *parent, const QString oAuthToken, const QString channel, const qint64 defaultTimerInterval) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval), channelName( channel )
{
    this->channelViewers = 0;
    this->channelFollowers = 0;
    this->channelId = -1;
    this->channelOnlineStatus = ChannelOnlineStatus::offline;

    this->currentlyUpdating = false;
    this->currentlyUpdatingHost = false;
    this->currentlyUpdatingChannel = false;

    this->isHosting = false;

    QObject::connect(this, SIGNAL(twitchReadyStream(const QJsonDocument)), this, SLOT(updateFromJsonResponseStream(const QJsonDocument)));
    QObject::connect(this, SIGNAL(twitchReadyHost(const QJsonDocument)), this, SLOT(updateFromJsonResponseHost(const QJsonDocument)));
    QObject::connect(this, SIGNAL(twitchReadyChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseChannel(const QJsonDocument)));
    QObject::connect(this, SIGNAL(twitchReadyChannelAccessToken(const QJsonDocument)), this, SLOT(updateFromJsonResponseAccessTokenReady(const QJsonDocument)));

    QObject::connect(this, SIGNAL(networkError(QString)), this, SLOT(twitchNetworkError(QString)));

    QObject::connect(this, SIGNAL(downloadedPlaylistReady(const QByteArray)), this, SLOT(on_downloadedPlaylistReady(const QByteArray)));

    this->getChannel(this->channelName);
    this->currentlyUpdatingChannel = true;

    this->setupTimer();

    genericHelper::log( this->channelName + QString(": ") + QString("TwitchChannel setup and ready"));
}

void TwitchChannel::twitchNetworkError(const QString errorString)
{
    genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + errorString);
    this->currentlyUpdating = false;
    this->currentlyUpdatingChannel = false;
    this->currentlyUpdatingHost = false;
}

void TwitchChannel::on_timedUpdate() {
    this->doStreamUpdate();
    this->doHostUpdate();
}

QString TwitchChannel::getAccessToken() const
{
    return accessToken;
}

void TwitchChannel::doStreamUpdate() {
    if(! this->currentlyUpdating) {
        this->currentlyUpdating = true;
        this->getStream(this->channelName);
    } else {
        genericHelper::log( this->channelName + QString(": ") + QString("Not starting Stream update"));
    }
}

void TwitchChannel::doChannelUpdate() {
    if(!this->currentlyUpdatingChannel) {
        this->getChannel(this->channelName);
        this->currentlyUpdatingChannel = true;
    } else {
        genericHelper::log( this->channelName + QString(": ") + QString("Not starting Channel update"));
    }
}

void TwitchChannel::doHostUpdate() {
    if((! this->currentlyUpdatingHost) && (this->channelId != -1) ) {
        this->currentlyUpdatingHost = true;
        this->getHost(QString::number(this->channelId,'f',0));
    } else {
        genericHelper::log( this->channelName + QString(": ") + QString("Not starting Host update"));
    }
}

bool TwitchChannel::getIsPartner() const
{
    return isPartner;
}

QString TwitchChannel::getChannelLogoUrl() const
{
    return channelLogoUrl;
}

TwitchChannel::ChannelOnlineStatus TwitchChannel::getChannelOnlineStatus() const
{
    return channelOnlineStatus;
}

QString TwitchChannel::getChannelName() const
{
    return channelName;
}

QString TwitchChannel::getChannelTitle() const
{
    return channelStatus;
}

QString TwitchChannel::getChannelGame() const
{
    return channelGame;
}

QString TwitchChannel::getChannelUrl() const
{
    return channelUrl;
}

qint64 TwitchChannel::getChannelViewers() const
{
    return channelViewers;
}

qint64 TwitchChannel::getChannelFollowers() const
{
    return channelFollowers;
}

QString TwitchChannel::getHostedChannel() const
{
    return hostedChannel;
}

bool TwitchChannel::getIsHosting() const
{
    return isHosting;
}

bool TwitchChannel::getIsPlaylist() const
{
    return isPlaylist;
}

bool TwitchChannel::updateChannelData(const QJsonObject channelObject) {
    // genericHelper::log( this->channelName + QString(": ") + QString("Updating Channel data"));
    bool dataChanged = false;

    QString name = channelObject["name"].toString();
    if(QString::compare(this->channelName, name, Qt::CaseSensitive) != 0) {
        genericHelper::log( this->channelName + QString(": ") + QString("ERROR: ChannelName wrong in JSON: ") + name);
    }

    QString status = channelObject["status"].toString();
    if(QString::compare(this->channelStatus, status, Qt::CaseSensitive) != 0 && ! this->isHosting) {
        this->channelStatus = status;
        dataChanged = true;
        genericHelper::log( this->channelName + QString(": ") + QString("Status changed"));
    }

    QString game = channelObject["game"].toString();
    if(QString::compare(this->channelGame, game, Qt::CaseSensitive) != 0) {
        this->channelGame = game;
        dataChanged = true;
        genericHelper::log( this->channelName + QString(": ") + QString("Game changed"));
    }

    QString logoUrlString = channelObject["logo"].toString();
    if(QString::compare(this->channelLogoUrl, logoUrlString, Qt::CaseSensitive) != 0) {
        this->channelLogoUrl = logoUrlString;
        dataChanged = true;
        genericHelper::log( this->channelName + QString(": ") + QString("Channel Logo Url changed"));
    }

    bool partner = channelObject["partner"].toBool();
    if(this->isPartner != partner) {
        this->isPartner = partner;
        dataChanged = true;
        genericHelper::log( this->channelName + QString(": ") + QString("Partner status changed"));
    }

    QString followers = QString::number(channelObject["followers"].toInt(),'f',0);
    if(this->channelFollowers != followers.toInt()) {
        this->channelFollowers = followers.toInt();
        dataChanged = true;
        genericHelper::log( this->channelName + QString(": ") + QString("Followers changed"));
    }

    QString id = QString::number(channelObject["_id"].toInt(),'f',0);
    if(this->channelId != id.toInt()) {
        this->channelId = id.toInt();
        dataChanged = true;
        genericHelper::log( this->channelName + QString(": ") + QString("Channel ID changed"));
    }

    return dataChanged;
}

void TwitchChannel::updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer)
{
    bool dataChanged = false;
    bool onlineStatusChanged = false;

    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto streamValue = jsonObject["stream"];

    if (streamValue != QJsonValue::Null) {
        // Object is live
        dataChanged = updateChannelData(streamValue.toObject()["channel"].toObject());

        QString viewersString = QString::number(streamValue.toObject()["viewers"].toInt(),'f',0);
        if(this->channelViewers != viewersString.toInt()) {
            this->channelViewers = viewersString.toInt();
            dataChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Viewers changed"));
        }

        this->isPlaylist = streamValue.toObject()["is_playlist"].toBool();
        ChannelOnlineStatus currentChannelOnlineStatus = ( !this->isPlaylist ? ChannelOnlineStatus::online : ChannelOnlineStatus::playlist );
        if(this->channelOnlineStatus != currentChannelOnlineStatus) {
            this->channelOnlineStatus = currentChannelOnlineStatus;
            dataChanged = true;
            onlineStatusChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Online Status changed to online"));
        }
    } else {
        // "stream": null -> object is offline
        // We fire up a channel update in that case
        this->doChannelUpdate();
        // and try to figure out if it is really offline or hosting
        ChannelOnlineStatus currentChannelOnlineStatus = ChannelOnlineStatus::offline;
        if(this->channelOnlineStatus != currentChannelOnlineStatus && !this->isHosting) {
            this->channelOnlineStatus = currentChannelOnlineStatus;
            dataChanged = true;
            onlineStatusChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Online Status changed to offline"));
        } else if(this->isHosting &&this->channelOnlineStatus != ChannelOnlineStatus::hosting ) {
            this->channelOnlineStatus = ChannelOnlineStatus::hosting;
            dataChanged = true;
            onlineStatusChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Online Status changed to hosting"));
        }

        if(this->isHosting) {
            QString hostedStatus = QString("Hosting ") + this->hostedChannel;
            if (QString::compare(hostedStatus, this->channelStatus, Qt::CaseSensitive) != 0) {
                this->channelStatus = hostedStatus;
                dataChanged = true;
                onlineStatusChanged = true;
                genericHelper::log( this->channelName + QString(": ") + QString("Hosted channel changed"));
            }
        }
    }

    if(dataChanged)
        emit twitchChannelDataChanged(onlineStatusChanged);

    this->currentlyUpdating = false;
}

void TwitchChannel::updateFromJsonResponseHost(const QJsonDocument &jsonResponseBuffer)
{
    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto hostsValue = jsonObject["hosts"];

    if ( hostsValue != QJsonValue::Null &&
         hostsValue.toArray().at(0).toObject().keys().count("target_login") > 0 )
    {
        QString hostlogin = hostsValue.toArray().at(0).toObject()["host_login"].toString();
        hostedChannel = hostsValue.toArray().at(0).toObject()["target_login"].toString();

        this->isHosting = true;
    } else {
        this->isHosting = false;
    }

    this->currentlyUpdatingHost = false;
}

void TwitchChannel::updateFromJsonResponseChannel(const QJsonDocument &jsonResponseBuffer)
{
    QJsonObject jsonObject = jsonResponseBuffer.object();

    if(updateChannelData(jsonObject))
        emit twitchChannelDataChanged(false);

    this->currentlyUpdatingChannel = false;
}

QString TwitchChannel::buildPlaylistUrlFromJson(const QJsonDocument &jsonResponseBuffer)
{
    QString _pl = "";

    QJsonObject jsonObject = jsonResponseBuffer.object();

    if ( (!jsonObject["token"].isNull()) &&
          (!jsonObject["sig"].isNull()) ) {
        QString channel = "";

        QListIterator<QString> itr (jsonObject["token"].toString().split(","));

        while (itr.hasNext()) {
            QString current = itr.next();

            if ( current.contains("channel") ) {
                if ( current.count(":") > 0) {
                    channel = QString(current.split(":")[1]).replace("\"","");
                }
            }
        }

        if (channel != "") {
            _pl = this->getPlayListUrl(channel,QUrl::toPercentEncoding(jsonObject["token"].toString()).replace("%7B","{").replace("%7D","}").replace("%3A",":").replace("%2C",",").replace("%5B","[").replace("%5D","]"),jsonObject["sig"].toString());
            return _pl;
        } else {
            return QString();
        }
    }
    return QString();
}

void TwitchChannel::updateFromJsonResponseAccessTokenReady(const QJsonDocument &jsonResponseBuffer)
{
    QString _pl = this->buildPlaylistUrlFromJson(jsonResponseBuffer);
    if (! genericHelper::getStreamQuality()) {
        TwitchChannelPlaylistUrlReady(this->channelGame, _pl);
    } else {
        QUrl streamUrl( _pl );
        this->downloadUrl(streamUrl);
    }
}

QString TwitchChannel::getPlayListUrl(QString channel, QString token, QString sig)
{
    return "http://usher.justin.tv/api/channel/hls/"+channel+".m3u8?token="+token+"&sig="+sig+"&allow_audio_only=true&allow_source=true&type=any&p="+QString::number(QDateTime::currentMSecsSinceEpoch());
}

void TwitchChannel::requestPlaylist()
{
    this->getChannelAccessToken(this->getChannelName());
}

void TwitchChannel::requestHostedPlaylist()
{
    this->getChannelAccessToken(this->getHostedChannel());
}

void TwitchChannel::downloadUrl(QUrl url)
{
    QNetworkRequest req (url);

    req.setRawHeader("Accept", "application/vnd.apple.mpegurl");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(fileDownloaded()));
}

void TwitchChannel::fileDownloaded()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QByteArray m_DownloadedData = reply->readAll();

        emit downloadedPlaylistReady( m_DownloadedData );

        reply->deleteLater();
    }
}

void TwitchChannel::on_downloadedPlaylistReady(const QByteArray playlist)
{
    QMap<QString, QString> videofiles = this->parseM3U8Playlist(playlist);

    emit twitchChannelQualityUrlsReady(this->channelGame, videofiles);
}

QMap<QString, QString> TwitchChannel::parseM3U8Playlist(QString m3u8playlist)
{
    QStringList m3u8playlistlines = m3u8playlist.split("\n");
    QMap<QString, QString> map;
    QRegularExpression requality("VIDEO=([^s]+)");

    QString quality;

    QStringListIterator it(m3u8playlistlines);

    QString string;

    for(int l=0; l<m3u8playlistlines.size(); ++l) {
        string = m3u8playlistlines.at(l);
        if ( string.startsWith("#EXT-X-STREAM-INF:PROGRAM-ID") ) {
            QRegularExpressionMatch match = requality.match(string);
            if (match.hasMatch()) {
                quality = match.captured(1).replace("\"","");
                if(QString::compare(quality, "chunked") == 0)
                    quality = "source";
                map[quality] = m3u8playlistlines.at(l+1);
            } else {
                qDebug() << "Could not extract quality from string:\n" << string;
            }
        }
    }

    return map;
}
