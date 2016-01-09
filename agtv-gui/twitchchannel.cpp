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

    QObject::connect(this, SIGNAL(networkError(QString)), this, SLOT(twitchNetworkError(QString)));

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
    genericHelper::log( this->channelName + QString(": ") + QString("Updating Channel data"));
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

        if(this->isHosting && QString::compare(this->hostedChannel, this->channelStatus, Qt::CaseSensitive) != 0) {
            this->channelStatus = QString("Hosting ") + this->hostedChannel;
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

    if(updateChannelData(jsonObject))
        emit twitchChannelDataChanged(false);

    this->currentlyUpdatingChannel = false;
}
