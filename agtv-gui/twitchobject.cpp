#include "twitchobject.h"

#include "generichelper.h"

TwitchObject::TwitchObject(QObject *parent, QString token, const qint64 defaultTimerInterval)
    : oAuthToken(token)
{
    nwManager = new QNetworkAccessManager();

    this->refreshTimerInterval = defaultTimerInterval;
    this->refreshTimer = new QTimer(this);
}

void TwitchObject::setupTimer()
{
    QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(on_timedUpdate()));
    this->refreshTimer->start(this->refreshTimerInterval);
}

void TwitchObject::setInterval(qint64 msec)
{
    if(this->refreshTimer->timerId() != -1) {
        this->refreshTimerInterval = msec;
        this->refreshTimer->start(msec);
    }
}

void TwitchObject::startUpdateTimer() const
{
    this->refreshTimer->start(this->refreshTimerInterval);
}

void TwitchObject::stopUpdateTimer() const
{
    this->refreshTimer->stop();
}
void TwitchObject::getStream(QString channelName)
{
    this->getRequestStream("https://api.twitch.tv/kraken/streams/"+channelName+"?stream_type=all");
}

void TwitchObject::getRequestStream(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseStream()));

    // TODO: implement timeout handling
}

void TwitchObject::parseTwitchNetworkResponseStream()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(__func__) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyStream( json_buffer );

        reply->deleteLater();
    }
}

void TwitchObject::getChannel(QString user)
{
    this->getRequestChannel("https://api.twitch.tv/kraken/channels/"+user);
}

void TwitchObject::getUserFollowedChannels(QString user)
{
    this->getRequestUser("https://api.twitch.tv/kraken/users/"+user+"/follows/channels");
}

void TwitchObject::getRequestChannel(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseChannel()));

    // TODO: implement timeout handling
}

void TwitchObject::parseTwitchNetworkResponseChannel()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(__func__) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyChannel( json_buffer );

        reply->deleteLater();
    }
}

void TwitchObject::getHost(QString channelId)
{
    this->getRequestHost("https://tmi.twitch.tv/hosts?include_logins=1&host="+channelId);
}

qint64 TwitchObject::getRefreshTimerInterval()
{
    return this->refreshTimerInterval;
}

QString TwitchObject::getOAuthToken()
{
    return this->oAuthToken;
}

void TwitchObject::getRequestHost(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseHost()));

    // TODO: implement timeout handling
}

void TwitchObject::getRequestUser(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseUser()));
}

void TwitchObject::parseTwitchNetworkResponseHost()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(__func__) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyHost( json_buffer );

        reply->deleteLater();
    }
}

void TwitchObject::parseTwitchNetworkResponseUser()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(__func__) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyUserFollowedChannels( json_buffer );

        reply->deleteLater();
    }
}
