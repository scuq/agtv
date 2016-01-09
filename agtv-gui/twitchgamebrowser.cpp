#include "twitchgamebrowser.h"

#include "generichelper.h"

TwitchGameBrowser::TwitchGameBrowser(QObject *parent, const QString oAuthToken, const qint64 defaultTimerInterval) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval)
{
    this->curroffset = 0;
    this->limit = 10;

    QObject::connect(this, SIGNAL(twitchReadyTopGames(const QJsonDocument)),
                     this, SLOT(updateFromJsonResponseTopGames(const QJsonDocument)));
    QObject::connect(this, SIGNAL(twitchReadyStreamsForGame(const QJsonDocument)),
                     this, SLOT(updateFromJsonResponseStreamsForGame(const QJsonDocument)));
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

QList<TwitchGameBrowser::Game> TwitchGameBrowser::getGameList() const
{
    return gameList;
}

void TwitchGameBrowser::updateFromJsonResponseTopGames(const QJsonDocument &jsonResponseBuffer)
{
    QJsonObject jsonObject = jsonResponseBuffer.object();

    auto topValue = jsonObject["top"];
    if (topValue != QJsonValue::Null && topValue.isArray()) {
        auto topArray = topValue.toArray();
        for (auto iter = topArray.begin(); iter != topArray.end(); ++iter)
        {
            TwitchGameBrowser::Game newGame;

            newGame.gameid = QString::number(iter->toObject()["game"].toObject()["_id"].toInt(), 'f', 0);
            newGame.gamename = iter->toObject()["game"].toObject()["name"].toString();
            newGame.viewers = QString::number(iter->toObject()["viewers"].toInt(), 'f', 0);
            newGame.logoSmall = iter->toObject()["game"].toObject()["logo"].toObject()["small"].toString();
            newGame.logoMedium = iter->toObject()["game"].toObject()["logo"].toObject()["medium"].toString();
            newGame.logoLarge = iter->toObject()["game"].toObject()["logo"].toObject()["large"].toString();

            gameList.append(newGame);
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

    emit twitchGameBrowserStreamsForGameReady(streams);
}
