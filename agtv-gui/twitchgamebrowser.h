#ifndef TWITCHGAMEBROWSER_H
#define TWITCHGAMEBROWSER_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>

#include "twitchobject.h"

//!  GameBrowser class for the Twitch API interface
/*!
  Provides functions to build a Game Browser from interfacing with the Twitch API.
*/
class TwitchGameBrowser : public TwitchObject
{
    Q_OBJECT

    public:
        TwitchGameBrowser(QObject *parent, const QString oAuthToken, const qint64 defaultTimerInterval = 1000);

        struct Game {
            QString gameid;
            QString gamename;
            QString viewers;
            QString logoSmall;
            QString logoMedium;
            QString logoLarge;
        };

        struct Stream {
            QString streamer;
            QString game;
            QString status;
            QString viewers;
        };

        void on_timedUpdate();

        void getGames();
        void getMoreGames();

        qint64 getCurroffset() const;

        qint64 getLimit() const;

        QList<Game> getGameList() const;

        void getGame(QString game);
private:
        qint64 curroffset;
        qint64 limit;
        QList<Game> gameList;

    private slots:
        void updateFromJsonResponseTopGames(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseStreamsForGame(const QJsonDocument &jsonResponseBuffer);

    signals:
        void twitchGameBrowserReadyTopGames();
        void twitchGameBrowserStreamsForGameReady(const QList<TwitchGameBrowser::Stream> streams);
};

#endif // TWITCHGAMEBROWSER_H
