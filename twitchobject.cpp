#include "twitchobject.h"

TwitchObject::TwitchObject(QObject *parent, QString oAuthToken, const qint64 defaultTimerInterval)
{
    this->oAuthToken = oAuthToken;

    QObject::connect(&m_stream, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseStream(QNetworkReply*)));
    QObject::connect(&m_channel, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseChannel(QNetworkReply*)));

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

void TwitchObject::startUpdateTimer()
{
    this->refreshTimer->start(this->refreshTimerInterval);
}

void TwitchObject::stopUpdateTimer()
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

    m_stream.get( req  );
    // TODO: implement timeout handling
}

void TwitchObject::parseNetworkResponseStream(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        emit networkError( finished->errorString() );
        qDebug() << finished->errorString();
        return;
    }

    QJsonDocument json_buffer = QJsonDocument::fromJson(finished->readAll());

    emit twitchReadyStream( json_buffer );
}

void TwitchObject::getChannel(QString user)
{
    this->getRequestChannel("https://api.twitch.tv/kraken/channels/"+user);
}

void TwitchObject::getRequestChannel(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_channel.get( req  );
}

void TwitchObject::parseNetworkResponseChannel(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        emit networkError( finished->errorString() );
        qDebug() << finished->errorString();
        return;
    }

    auto json_buffer = QJsonDocument::fromJson(finished->readAll());

    emit twitchReadyChannel( json_buffer );
}
