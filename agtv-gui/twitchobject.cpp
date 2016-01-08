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
    this->getRequestUser("https://api.twitch.tv/kraken/users/"+user+"/follows/channels", __func__);
}

void TwitchObject::followChannelUser(QString channelName, QString user)
{
    this->putRequestUser("https://api.twitch.tv/kraken/users/"+user+"/follows/channels/"+channelName, __func__);
}

void TwitchObject::unfollowChannelUser(QString channelName, QString user)
{
    this->delRequestUser("https://api.twitch.tv/kraken/users/"+user+"/follows/channels/"+channelName, __func__);
}

void TwitchObject::getUserAuthenticationStatus()
{  
    this->getRequestUser("https://api.twitch.tv/kraken/user", __func__);
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

void TwitchObject::setOAuthToken(QString oauthtoken)
{
    this->oAuthToken = oauthtoken;

}

QString TwitchObject::getTwitchClientId()
{
    return this->twitchClientId;
}

void TwitchObject::setTwitchClientId()
{
    this->getRequestClientId("http://agtv.abyle.org/clid/twitchClientId");
}

void TwitchObject::setUserAgentStr(QString useragent)
{
    this->userAgentStr = useragent;
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

void TwitchObject::getRequestUser(const QString &urlString, QString callingFuncName)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
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
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader("Authorization", "OAuth "+this->oAuthToken.toLatin1());
    
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    QNetworkReply *reply = nwManager->put(req, url.toEncoded());
    
    netReplies[reply] = callingFuncName;
    
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parseTwitchNetworkResponseUser()));

}

void TwitchObject::delRequestUser(const QString &urlString, QString callingFuncName)
{

    QUrl url ( urlString );

    QNetworkRequest req ( url );
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
    
    QString callingFuncName = "";
    
    callingFuncName = netReplies[reply];
    
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            //emit networkError( reply->errorString() );
            
            if (callingFuncName == "TwitchObject::getUserFollowedChannels") {
                emit twitchNetworkErrorUserFollowedChannels( reply->errorString() );
            } else if (callingFuncName == "TwitchObject::followChannelUser") {
                emit twitchNetworkErrorUserFollowChannel( reply->errorString() );
            } else if (callingFuncName == "TwitchObject::unfollowChannelUser") {
                emit twitchNetworkErrorUserUnfollowChannel( reply->errorString() );
            } else if (callingFuncName == "TwitchObject::getUserAuthenticationStatus") {
                emit twitchNetworkErrorUserAuthenticationStatus( reply->errorString() );
            }
                       
            genericHelper::log( QString(__func__) + "(" + callingFuncName + ")" + QString(": ") + reply->errorString());
            
            reply->abort();
            reply->deleteLater();
            netReplies.remove(reply);

            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());


        if (callingFuncName == "TwitchObject::getUserFollowedChannels") {
            emit twitchReadyUserFollowedChannels( json_buffer );
        } else if (callingFuncName == "TwitchObject::followChannelUser") {
            emit twitchReadyUserFollowChannel( json_buffer );
        } else if (callingFuncName == "TwitchObject::unfollowChannelUser") {
            emit twitchReadyUserUnfollowChannel( json_buffer );
        } else if (callingFuncName == "TwitchObject::getUserAuthenticationStatus") {
            emit twitchReadyUserAuthenticationStatus( json_buffer );
        }
        

        reply->deleteLater();
        netReplies.remove(reply);
        
    } else {
        genericHelper::log( QString(__func__) + QString(": ") + "reply is NULL");
    }
    qDebug() << "Pending Replys: "  << this->getPendingReplyCount();
    
    
}

void TwitchObject::parseTwitchNetworkResponseClientId()
{
    
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString(__func__) + QString(": ") + reply->errorString());
            return;
        }

        this->twitchClientId = reply->readAll().replace("\n",""); 
        
        reply->deleteLater();
    }    
}
