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

void TwitchGameBrowser::updateFromJsonResponseTopGames(const QJsonDocument &jsonResponseBuffer)
{
    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto topValue = jsonObject["top"];
    if (topValue != QJsonValue::Null && topValue.isArray()) {
        auto topArray = topValue.toArray();
        for (auto iter = topArray.begin(); iter != topArray.end(); ++iter)
        {
            QString gameid = QString::number(iter->toObject()["game"].toObject()["_id"].toInt(), 'f', 0);
            QString gamename = iter->toObject()["game"].toObject()["name"].toString();
            qint64 viewers = iter->toObject()["viewers"].toInt();
            QString logoSmall = iter->toObject()["game"].toObject()["box"].toObject()["small"].toString();
            QString logoMedium = iter->toObject()["game"].toObject()["box"].toObject()["medium"].toString();
            QString logoLarge = iter->toObject()["game"].toObject()["box"].toObject()["large"].toString();

            if(games.contains(gamename)) {
                games[gamename]->gameid = gameid;
                games[gamename]->gamename = gamename;
                games[gamename]->viewers = viewers;
                games[gamename]->logoSmall = logoSmall;
                games[gamename]->logoMedium = logoMedium;
                games[gamename]->logoLarge = logoLarge;
            } else {
                TwitchGameBrowser::Game *newGame = new TwitchGameBrowser::Game;
                newGame->gameid = gameid;
                newGame->gamename = gamename;
                newGame->viewers = viewers;
                newGame->logoSmall = logoSmall;
                newGame->logoMedium = logoMedium;
                newGame->logoLarge = logoLarge;

                games[gamename] = newGame;
                this->getGameLogo(gamename);
            }
        }
    }
}

void TwitchGameBrowser::updateFromJsonResponseStreamsForGame(const QJsonDocument &jsonResponseBuffer)
{
    QList<TwitchGameBrowser::Stream> streams;

    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto streamsValue = jsonObject["streams"];
    if (streamsValue != QJsonValue::Null && streamsValue.isArray()) {
        auto streamsArray = streamsValue.toArray();
        for (auto iter = streamsArray.begin(); iter != streamsArray.end(); ++iter)
        {
            TwitchGameBrowser::Stream newStream;

            newStream.streamer  = iter->toObject()["channel"].toObject()["name"].toString();
            newStream.game  = iter->toObject()["channel"].toObject()["game"].toString();
            newStream.status  = iter->toObject()["channel"].toObject()["status"].toString();
            newStream.viewers = QString::number(iter->toObject()["viewers"].toInt(), 'f', 0);

            streams.append(newStream);
        }
    }

    // emit twitchGameBrowserStreamsForGameReady(streams);
}

void TwitchGameBrowser::getGameLogo(QString gamename)
{
    if(! logoReplies.contains(gamename)) {
        QString logoUrl = "";
        for(auto game : games) {
            if(QString::compare(game->gamename, gamename, Qt::CaseSensitive) == 0) {
                logoUrl = game->logoLarge;
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

    emit twitchGameBrowserReadyTopGames();
}

