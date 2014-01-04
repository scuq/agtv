#include "twitchapi.h"


TwitchApi::TwitchApi(QObject *parent, QString oauthtoken) :
    QObject(parent)
{
    QObject::connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));
    this->oAuthAccessToken = oauthtoken;


}

void TwitchApi::setOAuthAccessToken(QString oauthtoken) {
    this->oAuthAccessToken = oauthtoken;
}

void TwitchApi::setStatusAndGameTitle( const QString &urlString, QHash<QString, QString> urlParams )
{
    this->putRequest(urlString, urlParams);
}

void TwitchApi::getUser()
{

    genericHelper::log("twitch-api getUser - https://api.twitch.tv/kraken/user.");
    this->getRequestAuthenticated("https://api.twitch.tv/kraken/user");
}

void TwitchApi::getFollows(QString user)
{

    genericHelper::log("twitch-api getFollows - https://api.twitch.tv/kraken/users/"+user+"/follows/channels");
    this->getRequest("https://api.twitch.tv/kraken/users/"+user+"/follows/channels");
}



void TwitchApi::getStream(QString user)
{

    genericHelper::log("twitch-api getStream - https://api.twitch.tv/kraken/streams/"+user);
    this->getRequest("https://api.twitch.tv/kraken/streams/"+user);



}

void TwitchApi::getGame()
{


    genericHelper::log("twitch-api getGame - https://api.twitch.tv/kraken/games/top?limit=100");
    this->getRequest("https://api.twitch.tv/kraken/games/top?limit=1001");



}

void TwitchApi::getChannel(QString user)
{

    genericHelper::log("twitch-api getChannel - https://api.twitch.tv/kraken/channels/"+user);
    this->getRequest("https://api.twitch.tv/kraken/channels/"+user);

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
    req.setRawHeader("Authorization", "OAuth "+this->oAuthAccessToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );



    m_nam.put( req, url.toEncoded() );


}

void TwitchApi::getRequestAuthenticated( const QString &urlString )
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    req.setRawHeader("Authorization", "OAuth "+this->oAuthAccessToken.toLatin1());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_nam.get( req  );


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

  //  qDebug() << data;

    json_buffer = QJsonDocument::fromJson(data);



   // qDebug()<< json_buffer;
    emit twitchReady( json_buffer );


    // new parser

    QJsonObject jsonObject = json_buffer.object();

    if (!jsonObject["top"].isNull()){

        emit twitchGameReady ( jsonObject );
    }

}
