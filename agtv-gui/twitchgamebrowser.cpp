#include "twitchgamebrowser.h"

#include "generichelper.h"

TwitchGameBrowser::TwitchGameBrowser(QObject *parent, const QString oAuthToken, const qint64 defaultTimerInterval) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval)
{
    this->curroffset = 0;
    this->limit = 10;

    this->setupLogoSignalMappers();

    QObject::connect(this, SIGNAL(twitchReadyTopGames(const QJsonDocument)),
                     this, SLOT(updateFromJsonResponseTopGames(const QJsonDocument)));
    QObject::connect(this, SIGNAL(twitchReadyStreamsForGame(const QJsonDocument)),
                     this, SLOT(updateFromJsonResponseStreamsForGame(const QJsonDocument)));
}

void TwitchGameBrowser::setupLogoSignalMappers()
{
    logoSignalMapper = new QSignalMapper(this);
    connect(logoSignalMapper, SIGNAL(mapped(QString)),
            this, SLOT(parseNetworkResponseGameLogo(QString)));

    streamLogoSignalMapper = new QSignalMapper(this);
    connect(streamLogoSignalMapper, SIGNAL(mapped(QString)),
            this, SLOT(parseNetworkResponseStreamLogo(QString)));
}

void TwitchGameBrowser::on_timedUpdate()
{
    return;
}

void TwitchGameBrowser::getGame(QString game)
{
    this->getStreamsForGame(game.replace(" ","+"));
}

void TwitchGameBrowser::getGames()
{
    this->getTopGames(this->curroffset, this->limit);
}

void TwitchGameBrowser::getMoreGames()
{
    this->curroffset += this->limit;
    this->getGames();
}

qint64 TwitchGameBrowser::getCurroffset() const
{
    return curroffset;
}

qint64 TwitchGameBrowser::getLimit() const
{
    return limit;
}

QMap<QString, TwitchGameBrowser::Game*> TwitchGameBrowser::getGameList() const
{
    return games;
}

void TwitchGameBrowser::parseTopObject(const QJsonObject &topObject, TwitchGameBrowser::Game *game)
{
    game->gameid = topObject["game"].toObject()["_id"].toInt();
    game->gamename = topObject["game"].toObject()["name"].toString();
    game->viewers = topObject["viewers"].toInt();
    game->channels = topObject["channels"].toInt();
    game->box_large_url = topObject["game"].toObject()["box"].toObject()["large"].toString();
    game->box_medium_url = topObject["game"].toObject()["box"].toObject()["medium"].toString();
    game->box_small_url = topObject["game"].toObject()["box"].toObject()["small"].toString();
    game->logo_large_url = topObject["game"].toObject()["logo"].toObject()["large"].toString();
    game->logo_medium_url = topObject["game"].toObject()["logo"].toObject()["medium"].toString();
    game->logo_small_url = topObject["game"].toObject()["logo"].toObject()["small"].toString();
}

void TwitchGameBrowser::updateFromJsonResponseTopGames(const QJsonDocument &jsonResponseBuffer)
{
    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto topValue = jsonObject["top"];
    if (topValue != QJsonValue::Null && topValue.isArray()) {
        auto topArray = topValue.toArray();
        for (auto iter = topArray.begin(); iter != topArray.end(); ++iter)
        {
            QJsonObject topObject = iter->toObject();
            QString gamename = topObject["game"].toObject()["name"].toString();
            if(games.contains(gamename)) {
                this->parseTopObject(topObject, games[gamename]);
            } else {
                TwitchGameBrowser::Game *newGame = new TwitchGameBrowser::Game;
                this->parseTopObject(topObject, newGame);

                games[gamename] = newGame;
                this->getGameLogo(gamename);
            }
        }
    }
    emit twitchGameBrowserReadyTopGames();
}

void TwitchGameBrowser::updateFromJsonResponseStreamsForGame(const QJsonDocument &jsonResponseBuffer)
{
    QList<TwitchGameBrowser::Stream> streams;

    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto streamsValue = jsonObject["streams"];
    if (streamsValue != QJsonValue::Null && streamsValue.isArray()) {
        auto streamsArray = streamsValue.toArray();
        foreach (const QJsonValue & value, streamsArray) {
            QJsonObject obj = value.toObject();
            TwitchGameBrowser::Stream newStream;

            newStream.streamer  = obj["channel"].toObject()["name"].toString();
            newStream.game  = obj["channel"].toObject()["game"].toString();
            newStream.status  = obj["channel"].toObject()["status"].toString();
            newStream.viewers = QString::number(obj["viewers"].toInt(), 'f', 0);
            newStream.logo_url  = obj["preview"].toObject()["medium"].toString();

            streams.append(newStream);

            this->getStreamLogo(newStream);
        }
    }

    emit twitchGameBrowserStreamsForGameReady(streams);
}

void TwitchGameBrowser::getGameLogo(QString gamename)
{
    if(! logoReplies.contains(gamename)) {
        QString logoUrl = "";
        for(auto game : games) {
            if(QString::compare(game->gamename, gamename, Qt::CaseSensitive) == 0) {
                logoUrl = game->box_large_url;
                break;
            }
        }
        if(logoUrl.length() > 0) {
            this->getRequestGameLogo(logoUrl, gamename);
        }
    } else {
        genericHelper::log( QString("ERROR: ") + QString(Q_FUNC_INFO) + QString(": getGameLogo already running for game ") + gamename);
    }
}

void TwitchGameBrowser::getRequestGameLogo(const QString &urlString, const QString game)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );

    QNetworkReply *reply = nwManager->get(req);
    logoReplies[game] = reply;
    // qDebug() << urlString;

    logoSignalMapper->setMapping(reply, game);
    connect(reply, SIGNAL(finished()),
            logoSignalMapper, SLOT(map()));
}

void TwitchGameBrowser::parseNetworkResponseGameLogo(const QString gamename)
{
    QNetworkReply *reply = logoReplies.take(gamename);
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString("ERROR: ") + QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QPixmap logoImage;
        logoImage.loadFromData(reply->readAll());

        genericHelper::log("downloaded logo image, dimension:" + QString::number(logoImage.size().height()) + "x" +QString::number(logoImage.size().width()));

        // qDebug() << "downloaded logo image, dimension:" + QString::number(logoImage.size().height()) + "x" +QString::number(logoImage.size().width());

        if (logoImage.size().height() > 0) {
            for(auto game : games) {
                if(QString::compare(game->gamename, gamename, Qt::CaseSensitive) == 0) {
                    game->logo = logoImage;
                    break;
                }
            }
        } else {
           //  qDebug() << "height is 0, stream logo image not set.";
        }

        reply->deleteLater();
    } else {
        genericHelper::log( QString(Q_FUNC_INFO) + QString(": Cannot retrieve reply for game ") + gamename);
    }
    emit twitchGameBrowserLogoForGameReady(gamename);
}

void TwitchGameBrowser::getStreamLogo(const TwitchGameBrowser::Stream stream)
{
    if(! streamLogoReplies.contains(stream.streamer)) {
        QString logoUrl = "";
        logoUrl = stream.logo_url;
        if(logoUrl.length() > 0) {
            this->getRequestStreamLogo(logoUrl, stream.streamer);
        }
    } else {
        genericHelper::log( QString("ERROR: ") + QString(Q_FUNC_INFO) + QString(": getStreamLogo already running for streamer ") + stream.streamer);
    }
}

void TwitchGameBrowser::getRequestStreamLogo(const QString &urlString, const QString game)
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );

    QNetworkReply *reply = nwManager->get(req);
    streamLogoReplies[game] = reply;
    // qDebug() << urlString;

    streamLogoSignalMapper->setMapping(reply, game);
    connect(reply, SIGNAL(finished()),
            streamLogoSignalMapper, SLOT(map()));
}

void TwitchGameBrowser::parseNetworkResponse()
{

}

void TwitchGameBrowser::parseNetworkResponseStreamLogo(const QString gamename)
{
    QNetworkReply *reply = streamLogoReplies.take(gamename);
    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            emit networkError( reply->errorString() );
            genericHelper::log( QString("ERROR: ") + QString(Q_FUNC_INFO) + QString(": ") + reply->errorString());
            return;
        }

        QPixmap logoImage;
        logoImage.loadFromData(reply->readAll());

        genericHelper::log("downloaded logo image, dimension:" + QString::number(logoImage.size().height()) + "x" +QString::number(logoImage.size().width()));

        // qDebug() << "downloaded logo image, dimension:" + QString::number(logoImage.size().height()) + "x" +QString::number(logoImage.size().width());

        TwitchGameBrowser::Stream newStream;

        if (logoImage.size().height() > 0) {
            newStream.streamer  = gamename;
            newStream.logo = logoImage;
        } else {
           //  qDebug() << "height is 0, stream logo image not set.";
        }

        reply->deleteLater();

        emit twitchGameBrowserLogoForStreamReady(newStream);
    } else {
        genericHelper::log( QString(Q_FUNC_INFO) + QString(": Cannot retrieve reply for game ") + gamename);
    }
}

