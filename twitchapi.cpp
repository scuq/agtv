#include "twitchapi.h"


TwitchApi::TwitchApi(QObject *parent, QString oauthtoken) :
    QObject(parent)
{
    QObject::connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));
    QObject::connect(&m_namM3u8, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseM3u8(QNetworkReply*)));
    QObject::connect(&m_follows, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseFollows(QNetworkReply*)));
    QObject::connect(&m_channelaccesstoken, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseChannelAccessToken(QNetworkReply*)));
    QObject::connect(&m_bookmark, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseBookmark(QNetworkReply*)));
    QObject::connect(&m_stream, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseStream(QNetworkReply*)));
    QObject::connect(&m_channel, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseChannel(QNetworkReply*)));
    QObject::connect(&m_host, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseHost(QNetworkReply*)));

    QObject::connect(&m_follow, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseFollow(QNetworkReply*)));
    QObject::connect(&m_unfollow, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponseUnfollow(QNetworkReply*)));

    this->oAuthAccessToken = oauthtoken;


}

void TwitchApi::setOAuthAccessToken(QString oauthtoken) {
    this->oAuthAccessToken = oauthtoken;
}

void TwitchApi::setStatusAndGameTitle(QString user, QHash<QString, QString> urlParams )
{

    genericHelper::log("twitch-api setStatusAndGameTitle - https://api.twitch.tv/kraken/channels/.");
    this->putRequest("https://api.twitch.tv/kraken/channels/"+user, urlParams);
}

void TwitchApi::followChannel(QString channel)
{
    QString user = genericHelper::getUsername();
    genericHelper::log("twitch-api followChannel - https://api.twitch.tv/kraken/users/"+user+"/follows/channels/"+channel);
    this->putRequestFollow("https://api.twitch.tv/kraken/users/"+user+"/follows/channels/"+channel);
}

void TwitchApi::unfollowChannel(QString channel)
{
    QString user = genericHelper::getUsername();
    genericHelper::log("twitch-api followChannel - https://api.twitch.tv/kraken/users/"+user+"/follows/channels/"+channel);
    this->deleteRequestUnfollow("https://api.twitch.tv/kraken/users/"+user+"/follows/channels/"+channel);
}

void TwitchApi::getUser()
{

    genericHelper::log("twitch-api getUser - https://api.twitch.tv/kraken/user.");
    this->getRequestAuthenticated("https://api.twitch.tv/kraken/user");
}

void TwitchApi::getFollows(QString user)
{

    genericHelper::log("twitch-api getFollows - https://api.twitch.tv/kraken/users/"+user+"/follows/channels");
    this->getRequestFollows("https://api.twitch.tv/kraken/users/"+user+"/follows/channels");
}



void TwitchApi::getStream(QString user)
{

    genericHelper::log("twitch-api getStream - https://api.twitch.tv/kraken/streams/"+user+"?stream_type=all");
    this->getRequestStream("https://api.twitch.tv/kraken/streams/"+user+"?stream_type=all");



}

void TwitchApi::getBookmarkStatus(QString channel)
{
    genericHelper::log("twitch-api getBookmarkStatus - https://api.twitch.tv/kraken/streams/"+channel);
    this->getRequestBookmark("https://api.twitch.tv/kraken/streams/"+channel);

}

/**
void TwitchApi::getPlaylist(QString user)
{

    genericHelper::log("twitch-api getPlaylist - https://api.twitch.tv/kraken/streams/"+user);
    //this->getRequest("http://usher.twitch.tv/api/channel/hls/"+user+".m3u8?player=twitchweb&&token="+this->oAuthAccessToken+"&sig={sig}&allow_audio_only=true&allow_source=true&type=any&p={random}");




}
**/

void TwitchApi::getGame()
{


    genericHelper::log("twitch-api getGame - https://api.twitch.tv/kraken/games/top?limit=100");
    this->getRequest("https://api.twitch.tv/kraken/games/top?limit=1001");



}

void TwitchApi::getKrakenChannel()
{


    genericHelper::log("twitch-api getKrakenChannel - https://api.twitch.tv/kraken/channel");
    this->getRequestAuthenticated("https://api.twitch.tv/kraken/channel");



}



void TwitchApi::getChannel(QString user)
{

    genericHelper::log("twitch-api getChannel - https://api.twitch.tv/kraken/channels/"+user);
    this->getRequestChannel("https://api.twitch.tv/kraken/channels/"+user);

}

void TwitchApi::getHost(QString channelid)
{
    genericHelper::log("twitch-api getHost - https://tmi.twitch.tv/hosts?include_logins=1&host="+channelid);
    this->getRequestHost("https://tmi.twitch.tv/hosts?include_logins=1&host="+channelid);
}

void TwitchApi::getChannelAccessToken(QString channel)
{
    genericHelper::log("twitch-api getChannelAccessToken - https://api.twitch.tv/kraken/channels/"+channel);
    this->getRequestChannelAccessToken("https://api.twitch.tv/api/channels/"+channel+"/access_token");

}

void TwitchApi::getPlayList(QString channel, QString token, QString sig)
{



    genericHelper::log("twitch-api getPlayList - http://usher.justin.tv/api/channel/hls/"+channel+".m3u8?token="+token+"&sig="+sig+"&allow_audio_only=true&allow_source=true&type=any&p="+QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->getRequestM3u8("http://usher.justin.tv/api/channel/hls/"+channel+".m3u8?token="+token+"&sig="+sig+"&allow_audio_only=true&allow_source=true&type=any&p="+QString::number(QDateTime::currentMSecsSinceEpoch()));
    // http://usher.twitch.tv/api/channel/hls/{channel}.m3u8?player=twitchweb&&token={token}&sig={sig}&allow_audio_only=true&allow_source=true&type=any&p={random}'
}

QString TwitchApi::getPlayListUrl(QString channel, QString token, QString sig)
{
    return "http://usher.justin.tv/api/channel/hls/"+channel+".m3u8?token="+token+"&sig="+sig+"&allow_audio_only=true&allow_source=true&type=any&p="+QString::number(QDateTime::currentMSecsSinceEpoch());
}


void TwitchApi::searchGames(QString searchStr)
{

    genericHelper::log("twitch-api searchGames - https://api.twitch.tv/kraken/search/games?q="+QUrl::toPercentEncoding(searchStr)+"&type=suggest");
    this->getRequest("https://api.twitch.tv/kraken/search/games?q="+QUrl::toPercentEncoding(searchStr)+"&type=suggest");

}

void TwitchApi::putRequest( const QString &urlString, QHash<QString, QString> urlParams)
{
    QUrl url ( urlString );
    QUrlQuery query(url);




    for (QHash<QString, QString>::iterator iter = urlParams.begin(); iter != urlParams.end(); ++iter) {

        query.addQueryItem(iter.key(),iter.value());
    }


    url.setQuery(query);

    QNetworkRequest req ( url );

    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader( "User-Agent" , "iOS / Safari 7: Mozilla/5.0 (iPad; CPU OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B554a Safari/9537.53" );
    req.setRawHeader("Authorization", "OAuth "+this->oAuthAccessToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );



    m_nam.put( req, url.toEncoded() );


}

void TwitchApi::putRequestFollow( const QString &urlString )
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader( "User-Agent" , "iOS / Safari 7: Mozilla/5.0 (iPad; CPU OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B554a Safari/9537.53" );
    req.setRawHeader("Authorization", "OAuth "+this->oAuthAccessToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_follow.put( req, url.toEncoded()  );


}

void TwitchApi::deleteRequestUnfollow( const QString &urlString )
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader( "User-Agent" , "iOS / Safari 7: Mozilla/5.0 (iPad; CPU OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B554a Safari/9537.53" );
    req.setRawHeader("Authorization", "OAuth "+this->oAuthAccessToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    req.setUrl(url);

    m_unfollow.deleteResource( req );
}

void TwitchApi::getRequestAuthenticated( const QString &urlString )
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader( "User-Agent" , "iOS / Safari 7: Mozilla/5.0 (iPad; CPU OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B554a Safari/9537.53" );
    req.setRawHeader("Authorization", "OAuth "+this->oAuthAccessToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_nam.get( req  );


}

void TwitchApi::getRequestM3u8( const QString &urlString )
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.apple.mpegurl");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_namM3u8.get( req  );


}

void TwitchApi::getRequestFollows(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_follows.get( req  );
}

void TwitchApi::getRequestBookmark(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_bookmark.get( req  );

}

void TwitchApi::getRequestStream(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_stream.get( req  );
}

void TwitchApi::getRequestChannel(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_channel.get( req  );
}

void TwitchApi::getRequestHost(const QString &urlString)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_host.get( req  );
}

void TwitchApi::getRequestChannelAccessToken(const QString &urlString)
{

    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_channelaccesstoken.get( req  );


}



void TwitchApi::getRequest( const QString &urlString )
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_nam.get( req  );


}


void TwitchApi::parseNetworkResponse( QNetworkReply *finished )
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        //qDebug()<< finished->errorString();
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();



   // qDebug() << data;

    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReady( json_buffer );


    // new parser

    QJsonObject jsonObject = json_buffer.object();

    if (!jsonObject["top"].isNull()){

        emit twitchGameReady ( jsonObject );
    } else if (!jsonObject["stream_key"].isNull()) {
        emit twitchStreamKeyReady ( jsonObject );
    }

}

void TwitchApi::parseNetworkResponseStream(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        //qDebug()<< finished->errorString();
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReadyStream( json_buffer );
}

void TwitchApi::parseNetworkResponseHost(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        //qDebug()<< finished->errorString();
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReadyHost( json_buffer );
}

void TwitchApi::parseNetworkResponseChannel(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        //qDebug()<< finished->errorString();
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReadyChannel( json_buffer );
}

void TwitchApi::parseNetworkResponseM3u8(QNetworkReply *finished)
{
    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
qDebug() << "parseNetworkResponseM3u8";
    emit twitchReadyM3u8( data );


//  qDebug() << data;
}

void TwitchApi::parseNetworkResponseFollows(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        //qDebug()<< finished->errorString();
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReadyFollows( json_buffer );
}

void TwitchApi::parseNetworkResponseBookmark(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        //qDebug()<< finished->errorString();
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReadyBookmark( json_buffer );
}

void TwitchApi::parseNetworkResponseChannelAccessToken(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        //qDebug()<< finished->errorString();
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();





    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReadyChannelAccessToken( json_buffer );


    // new parser



}


void TwitchApi::parseNetworkResponseFollow(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    json_buffer = QJsonDocument::fromJson(data);

    emit twitchReadyFollow( json_buffer );
}

void TwitchApi::parseNetworkResponseUnfollow(QNetworkReply *finished)
{
    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->errorString() );

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data = finished->readAll();
    json_buffer = QJsonDocument::fromJson(data);

    emit twitchReadyUnfollow( json_buffer );
}
