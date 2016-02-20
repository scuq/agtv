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
        foreach (const QJsonValue & value, topArray) {
            QJsonObject obj = value.toObject();
            TwitchGameBrowser::Game newGame;

            newGame.gameid = QString::number(obj["game"].toObject()["_id"].toInt(), 'f', 0);
            newGame.gamename = obj["game"].toObject()["name"].toString();
            newGame.viewers = QString::number(obj["viewers"].toInt(), 'f', 0);
            newGame.logoSmall = obj["game"].toObject()["logo"].toObject()["small"].toString();
            newGame.logoMedium = obj["game"].toObject()["logo"].toObject()["medium"].toString();
            newGame.logoLarge = obj["game"].toObject()["logo"].toObject()["large"].toString();

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
        foreach (const QJsonValue & value, streamsArray) {
            QJsonObject obj = value.toObject();

            TwitchGameBrowser::Stream newStream;
            newStream.streamer  = obj["channel"].toObject()["name"].toString();
            newStream.game  = obj["channel"].toObject()["game"].toString();
            newStream.status  = obj["channel"].toObject()["status"].toString();
            newStream.viewers = QString::number(obj["viewers"].toInt(), 'f', 0);

            streams.append(newStream);
        }
    }

    emit twitchGameBrowserStreamsForGameReady(streams);
}
