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
    this->isHosting = false;

    QObject::connect(this, SIGNAL(twitchReadyStream(const QJsonDocument)), this, SLOT(updateFromJsonResponseStream(const QJsonDocument)));
    QObject::connect(this, SIGNAL(twitchReadyHost(const QJsonDocument)), this, SLOT(updateFromJsonResponseHost(const QJsonDocument)));
    QObject::connect(this, SIGNAL(twitchReadyChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseChannel(const QJsonDocument)));

    QObject::connect(this, SIGNAL(networkError(QString)), this, SLOT(twitchNetworkError(QString)));

    this->getChannel(this->channelName);

    this->setupTimer();
}

void TwitchChannel::twitchNetworkError(const QString errorString)
{
    genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + errorString);
    this->currentlyUpdating = false;
}

void TwitchChannel::on_timedUpdate() {
    if(! this->currentlyUpdating) {
        this->currentlyUpdating = true;
        this->getStream(this->channelName);
    }
    if((! this->currentlyUpdatingHost) && (this->channelId != -1) ) {
        this->currentlyUpdatingHost = true;
        this->getHost(QString::number(this->channelId,'f',0));
    }
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

bool TwitchChannel::parseStreamChannelObject(const QJsonObject channelObject, QString &name,
                                             QString &status, QString &game,
                                             QString &logoUrlString, QString &partner,
                                             QString &followers, QString &channelId)
{
    name = channelObject["name"].toString();
    status = channelObject["status"].toString();
    game = channelObject["game"].toString();
    logoUrlString = channelObject["logo"].toString();
    partner = channelObject["partner"].toBool();
    followers = QString::number(channelObject["followers"].toInt(),'f',0);
    channelId = QString::number(channelObject["_id"].toInt(),'f',0);

    return true;
}

void TwitchChannel::updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer)
{
    bool dataChanged = false;
    bool onlineStatusChanged = false;

    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto streamValue = jsonObject["stream"];

    if (streamValue != QJsonValue::Null) {
        // Object is live
        QString onlinename, status, game, viewersString, logoUrlString, partner, followersString, channelIdString;
        this->parseStreamChannelObject(streamValue.toObject()["channel"].toObject(),
                onlinename, status, game, logoUrlString, partner, followersString, channelIdString);

        viewersString = QString::number(streamValue.toObject()["viewers"].toInt(),'f',0);
        this->isPlaylist = streamValue.toObject()["is_playlist"].toBool();

        if(QString::compare(this->channelGame, game, Qt::CaseSensitive) != 0) {
            this->channelGame = game;
            dataChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Game changed"));
        }
        if(QString::compare(this->channelStatus, status, Qt::CaseSensitive) != 0) {
            this->channelStatus = status;
            dataChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Status changed"));
        }
        if(this->channelViewers != viewersString.toInt()) {
            this->channelViewers = viewersString.toInt();
            dataChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Viewers changed"));
        }
        if(this->channelFollowers != followersString.toInt()) {
            this->channelFollowers = followersString.toInt();
            dataChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Followers changed"));
        }

        this->channelId = channelIdString.toInt();

        ChannelOnlineStatus currentChannelOnlineStatus = ( !this->isPlaylist ? ChannelOnlineStatus::online : ChannelOnlineStatus::playlist );
        if(this->channelOnlineStatus != currentChannelOnlineStatus) {
            this->channelOnlineStatus = currentChannelOnlineStatus;
            dataChanged = true;
            onlineStatusChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Online Status changed to online"));
        }
    } else {
        // "stream": null -> object is offline
        QString streamurl = jsonObject.value("_links").toObject()["self"].toString();
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

        if(this->isHosting && QString::compare(this->hostedChannel, this->channelStatus, Qt::CaseSensitive) != 0) {
            this->channelStatus = this->hostedChannel;
            dataChanged = true;
            onlineStatusChanged = true;
            genericHelper::log( this->channelName + QString(": ") + QString("Hosted channel changed"));
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

    QString onlinename, status, game, viewersString, logoUrlString,
            partner, followersString, channelIdString;
    this->parseStreamChannelObject(jsonObject,
                                   onlinename, status, game,
                                   logoUrlString, partner, followersString,
                                   channelIdString);

    this->channelId = channelIdString.toInt();
    this->channelLogoUrl = logoUrlString;
}
