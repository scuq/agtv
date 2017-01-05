#ifndef TWITCHGAMEBROWSER_H
#define TWITCHGAMEBROWSER_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QPixmap>

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
            qint64 gameid;
            QString gamename;
            qint64 viewers;
            qint64 channels;
            QString box_large_url;
            QString box_medium_url;
            QString box_small_url;
            QString logo_large_url;
            QString logo_medium_url;
            QString logo_small_url;
            QPixmap logo;
        };

        struct Stream {
            QString streamer;
            QString game;
            QString status;
            QString viewers;
            QString logo_url;
            QPixmap logo;
        };

        void on_timedUpdate();

        void getGames();
        void getMoreGames();

        qint64 getCurroffset() const;

        qint64 getLimit() const;

        QMap<QString, Game*> getGameList() const;

        void getGame(QString game);
private:
        qint64 curroffset;
        qint64 limit;
        QMap<QString, Game*> games;

        QMap<QString, QNetworkReply*> logoReplies;
        QSignalMapper *logoSignalMapper;

        QMap<QString, QNetworkReply*> streamLogoReplies;
        QSignalMapper *streamLogoSignalMapper;

        void setupLogoSignalMappers();
        void getGameLogo(QString gamename);
        void getRequestGameLogo(const QString &urlString, const QString game);
        void parseTopObject(const QJsonObject &topObject, Game *game);
        void getStreamLogo(const Stream stream);
        void getRequestStreamLogo(const QString &urlString, const QString game);
        void parseNetworkResponse();
private slots:
        void updateFromJsonResponseTopGames(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseStreamsForGame(const QJsonDocument &jsonResponseBuffer);

        void parseNetworkResponseGameLogo(const QString gamename);
        void parseNetworkResponseStreamLogo(const QString gamename);
signals:
        void twitchGameBrowserReadyTopGames();
        void twitchGameBrowserStreamsForGameReady(const QList<TwitchGameBrowser::Stream> streams);
        void twitchGameBrowserLogoForGameReady(const QString gamename);
        void twitchGameBrowserLogoForStreamReady(const TwitchGameBrowser::Stream stream);
};

#endif // TWITCHGAMEBROWSER_H
