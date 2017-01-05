#include "twitchobject.h"

#include "generichelper.h"

TwitchObject::TwitchObject(QObject *parent, QString token, const qint64 defaultTimerInterval)
    : oAuthToken(token)
{
    nwManager = new QNetworkAccessManager(this);

    api = new twitchApi(this,"v5");

    this->setupSignalMappers();

    this->setTwitchClientId();

    this->refreshTimerInterval = defaultTimerInterval;
    this->refreshTimer = new QTimer(this);
}

void TwitchObject::setupSignalMappers()
{
    channelAccessTokenSignalMapper = new QSignalMapper(this);
    connect(channelAccessTokenSignalMapper, SIGNAL(mapped(QString)),
            this, SLOT(parseNetworkResponseChannelAccessToken(QString)));
}

void TwitchObject::setupTimer()
{
    QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(on_timedUpdate()));
    this->refreshTimer->start(this->refreshTimerInterval);
}

void TwitchObject::setInterval(qint64 msec)
{
    this->refreshTimerInterval = msec;

    if(this->refreshTimer->timerId() != -1) {
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
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
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
            genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyStream( json_buffer );

        reply->deleteLater();
    }
}

void TwitchObject::getChannel(QString user)
{
    this->getRequestChannel("https://api.twitch.tv/kraken/channels/"+user+"&client_id="+this->getTwitchClientId());
}


void TwitchObject::getRequestChannel(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
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
            genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
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

void TwitchObject::setOAuthToken(QString oauthtoken)
{
    this->oAuthToken = oauthtoken;

}

QString TwitchObject::getTwitchClientId()
{
    return "pb2gmicczhmqn5sb9vtfltph95zs4ji";
    return this->twitchClientId;
}

void TwitchObject::setTwitchClientId()
{
    //this->getRequestClientId("http://agtv.abyle.org/clid/twitchClientId");
}

void TwitchObject::setUserAgentStr(QString useragent)
{
    this->userAgentStr = useragent;
}

QString TwitchObject::getUserAgentStr()
{
    return this->userAgentStr;
}

void TwitchObject::getRequestHost(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", this->api->getRequestHeader_Accept().toStdString().c_str());
    req.setHeader(QNetworkRequest::ContentTypeHeader, this->api->getRequestHeader_ContentTypeHeader().toStdString().c_str());

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseHost()));

    // TODO: implement timeout handling
}

void TwitchObject::getRequest(const QString &urlString, QString callingFuncName, QHash<QString, QString> getParams)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", this->api->getRequestHeader_Accept().toStdString().c_str());
    req.setRawHeader("Client-ID", this->getTwitchClientId().toStdString().c_str());
    req.setRawHeader("Authorization", "OAuth "+this->oAuthToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, this->api->getRequestHeader_ContentTypeHeader().toStdString().c_str());

    QNetworkReply *reply = nwManager->get(req);

    netReplies[reply] = callingFuncName;
    netParams[reply] = getParams;

    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseNetworkResponse()));

}

void TwitchObject::getRequestUser(const QString &urlString, QString callingFuncName)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", "application/vnd.twitchtv.v5+json");
    req.setRawHeader("Client-ID", this->getTwitchClientId().toStdString().c_str());

    req.setRawHeader("Authorization", "OAuth "+this->oAuthToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);

    netReplies[reply] = callingFuncName;
    
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseUser()));

}

void TwitchObject::putRequestUser(const QString &urlString, QString callingFuncName)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader("Authorization", "OAuth "+this->oAuthToken.toLatin1());
    
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->put(req, url.toEncoded());
    
    netReplies[reply] = callingFuncName;
    
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseUser()));
    
}

void TwitchObject::putRequestUser(const QString &urlString, QHash<QString, QString> setParams, QString callingFuncName)
{
    QUrl url ( urlString );
    QUrlQuery query(url);
    
    for (QHash<QString, QString>::iterator iter = setParams.begin(); iter != setParams.end(); ++iter) {

        query.addQueryItem(iter.key(),iter.value());
    }
    
    url.setQuery(query);

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader("Authorization", "OAuth "+this->oAuthToken.toLatin1());
    
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->put(req, url.toEncoded());
    
    netReplies[reply] = callingFuncName;
    
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseUser()));  
    

}

void TwitchObject::putRequest(const QString &urlString, QString callingFuncName, QHash<QString, QString> setParams)
{
    QUrl url ( urlString );
    QUrlQuery query(url);

    for (QHash<QString, QString>::iterator iter = setParams.begin(); iter != setParams.end(); ++iter) {

        query.addQueryItem(iter.key(),iter.value());
    }

    url.setQuery(query);

    QNetworkRequest req ( url );

    req.setRawHeader("User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", this->api->getRequestHeader_Accept().toStdString().c_str());
    req.setRawHeader("Client-ID", this->getTwitchClientId().toStdString().c_str());
    req.setRawHeader("Authorization", "OAuth "+this->oAuthToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, this->api->getRequestHeader_ContentTypeHeader().toStdString().c_str());

    QNetworkReply *reply = nwManager->put(req, url.toEncoded());

    netReplies[reply] = callingFuncName;

    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseNetworkResponse()));

}

void TwitchObject::delRequestUser(const QString &urlString, QString callingFuncName)
{

    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader("Authorization", "OAuth "+this->oAuthToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    req.setUrl(url);

    QNetworkReply *reply = nwManager->deleteResource(req);
    
    netReplies[reply] = callingFuncName;
    
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseUser()));

}

void TwitchObject::getRequestClientId(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);

    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseClientId()));
}

qint64 TwitchObject::getPendingReplyCount()
{
    return this->netReplies.count();
}

void TwitchObject::parseTwitchNetworkResponseHost()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyHost( json_buffer );

        reply->deleteLater();
    }
}



void TwitchObject::parseTwitchNetworkResponseClientId()
{
    
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        this->twitchClientId = reply->readAll().replace("\n",""); 
        
        reply->deleteLater();
    }    
}

void TwitchObject::getTopGames(int offset=0, int limit=10)
{
    // https://github.com/justintv/Twitch-API/blob/master/v3_resources/games.md
    this->getRequestTopGames("https://api.twitch.tv/kraken/games/top?limit="+QString::number(limit,'f',0)+"&offset="+QString::number(offset,'f',0));
}

void TwitchObject::getRequestTopGames(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseNetworkResponseTopGames()));
}

void TwitchObject::parseNetworkResponseTopGames()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyTopGames( json_buffer );

        reply->deleteLater();
    }
}

void TwitchObject::getStreamsForGame(const QString game)
{
    // https://github.com/justintv/Twitch-API/blob/master/v3_resources/games.md
    this->getRequestStreamsForGame("https://api.twitch.tv/kraken/streams/?game=" + game);
}

void TwitchObject::getRequestStreamsForGame(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseNetworkResponseStreamsForGame()));
}

void TwitchObject::parseNetworkResponseStreamsForGame()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyStreamsForGame( json_buffer );

        reply->deleteLater();
    }
}

void TwitchObject::getChannelAccessToken(QString channel)
{
    if(! tokenReplies.contains(channel)) {
        this->getRequestChannelAccessToken("https://api.twitch.tv/api/channels/"+channel+"/access_token", channel);
    } else {
        genericHelper::log( QString("ERROR: ") + QString(Q_FUNC_INFO) + QString(": getChannelAccessToken already running for channel ") + channel);
    }
}

void TwitchObject::setUserStatusAndGameTitle(QString user, QHash<QString, QString> setParams)
{
  
      this->putRequestUser("https://api.twitch.tv/kraken/channels/"+user, setParams, "TwitchObject::setUserStatusAndGameTitle");
  
}

void TwitchObject::getRequestChannelAccessToken(const QString &urlString, const QString channel)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , this->userAgentStr.toStdString().c_str());
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->get(req);
    tokenReplies[channel] = reply;

    channelAccessTokenSignalMapper->setMapping(reply, channel);
    connect(reply, SIGNAL(finished()),
            channelAccessTokenSignalMapper, SLOT(map()));
}

void TwitchObject::parseNetworkResponseChannelAccessToken(const QString channel)
{
    QNetworkReply *reply = tokenReplies.take(channel);
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString("ERROR: ") + QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());
        emit twitchReadyChannelAccessToken( channel, json_buffer );

        reply->deleteLater();
    } else {
        genericHelper::log( QString(Q_FUNC_INFO) + QString(": Cannot retrieve reply for channel ") + channel);
    }
}
