#ifndef TWITCHOBJECT_H
#define TWITCHOBJECT_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QString>
#include <QTimer>
#include <QSignalMapper>
#include <QUrlQuery>
#include "twitchapi.h"

//!  Base class for the Twitch API interface
/*!
  Provides basic get functions and corresponding signals to interface with the Twitch
  Api.
*/

class TwitchObject : public QObject
{
    Q_OBJECT

    public:
        QNetworkAccessManager *nwManager;

        TwitchObject(QObject *parent = 0, QString token = QString(), const qint64 defaultTimerInterval = 5000);

        enum class SearchEndpoint {
            Channels,
            Streams,
            Games
        };

        //! Set the time interval
        /*!
          \param msec time in milliseconds
        */
        void setInterval(qint64 msec);
        //! Start the update timer
        void startUpdateTimer()  const;
        //! Stop the update timer
        void stopUpdateTimer()  const;

        //! Fires a 'stream' request to the Twitch API
        /*!
          \param channelName the channel name to which the request should be sent
        */
        void getStream(QString channelName);
        //! Fires a 'channel' request to the Twitch API
        /*!
          \param user the channel/user name to which the request should be sent
        */
        void getChannel(QString user);
        //! Fires a host request to the Twitch API
        /*!
          \param channelId the channel id to which the request should be sent
        */


        


        void getHost(QString channelId);

        qint64 getRefreshTimerInterval();

        QString getOAuthToken();
        
        void setOAuthToken(QString oauthtoken);
        
        QString getTwitchClientId();
        
        void setTwitchClientId();
        
        void setUserAgentStr(QString useragent);
        
        QString getUserAgentStr();
       
        void getTopGames(int offset, int limit);
        void getStreamsForGame(const QString game);

        void getChannelAccessToken(QString channel);
        
        void setUserStatusAndGameTitle(QString user, QHash<QString, QString> setParams);

public slots:
        virtual void on_timedUpdate() = 0;

    protected:
        twitchApi *api;

        QMap<QNetworkReply*, QString> netReplies;
        QMap<QNetworkReply*, QHash<QString, QString> > parseParams;

        void setupTimer();

        void getId(QString querystr, TwitchObject::SearchEndpoint searchEndpoint, QString callingFuncName);


        qint64 getPendingReplyCount();

        void getRequestStream(const QString &urlString);
        void getRequestChannel(const QString &urlString);
        void getRequestHost(const QString &urlString);
        void getRequestUser(const QString &urlString, QString callingFuncName);
        void getRequest(const QString &urlString, QString callingFuncName, QHash<QString, QString> parseParams = QHash<QString, QString>());
        void getRequestSearch(const QString &urlString, QString callingFuncName, QHash<QString, QString> parseParams = QHash<QString, QString>());

        void putRequestUser(const QString &urlString, QString callingFuncName);
        void putRequestUser(const QString &urlString, QHash<QString, QString> setParams, QString callingFuncName);
        void putRequest(const QString &urlString, QString callingFuncName, QHash<QString, QString> setParams = QHash<QString, QString>() );
        void delRequestUser(const QString &urlString, QString callingFuncName);
        void getRequestClientId(const QString &urlString);
        void getRequestTopGames(const QString &urlString);
        void getRequestStreamsForGame(const QString &urlString);
        void getRequestChannelAccessToken(const QString &urlString, const QString channel);

    private:
        QTimer *refreshTimer;
        QString oAuthToken;
        QString twitchClientId;
        QString userAgentStr;


        qint64 refreshTimerInterval;
        
        QHash<QString, QString> apiUrls_Search;

        QMap<QString, QNetworkReply*> tokenReplies;

        QSignalMapper *channelAccessTokenSignalMapper;

        void setupSignalMappers();



protected slots:
        virtual void parseNetworkResponse() = 0;

private slots:
        void parseNetworkResponseSearch();
        void parseTwitchNetworkResponseStream();
        void parseTwitchNetworkResponseChannel();
        void parseTwitchNetworkResponseHost();
        void parseTwitchNetworkResponseClientId();
        void parseNetworkResponseTopGames();
        void parseNetworkResponseStreamsForGame();
        void parseNetworkResponseChannelAccessToken(const QString channel);
        void updateFromJsonResponseGetSearch(const QJsonDocument &jsonResponseBuffer, TwitchObject::SearchEndpoint SearchEndpoint, QHash<QString,QString> params);

signals:
        void networkError( QString errmessage );
        void twitchNetworkErrorUserFollowedChannels( QString errmessage );
        void twitchNetworkErrorSearch( QString errmessage );
        void twitchNetworkErrorUserFollowChannel( QString errmessage );
        void twitchNetworkErrorUserUnfollowChannel( QString errmessage );
        void twitchNetworkErrorUserAuthenticationStatus( QString errmessage );
        
        void twitchReadyStream( const QJsonDocument &twitchAsJSON );
        void twitchReadyChannel( const QJsonDocument &twitchAsJSON );
        void twitchReadyHost( const QJsonDocument &twitchAsJSON );
        void twitchReadyUserFollowedChannels( const QJsonDocument &twitchAsJSON );
        void twitchReadyUserFollowChannel( const QJsonDocument &twitchAsJSON );
        void twitchReadySearch( const QJsonDocument &twitchAsJSON, TwitchObject::SearchEndpoint SearchEndpoint, QHash<QString,QString> params);
        void twitchReadyUserUnfollowChannel( const QJsonDocument &twitchAsJSON );
        void twitchReadyGetStreamer( const QJsonDocument &twitchAsJSON, QString lookedupstreamer);
        void twitchReadyUserSetStatusAndGameTitle ( const QJsonDocument &twitchAsJSON );
        void twitchReadyUserAuthenticationStatus( const QJsonDocument &twitchAsJSON );
        void twitchReadyTopGames( const QJsonDocument &twitchAsJSON );
        void twitchReadyStreamsForGame( const QJsonDocument &twitchAsJSON );
        void twitchReadyChannelAccessToken( const QString channel, const QJsonDocument &twitchAsJSON );

        void twitchFollowChannelByIdReady(const QString, QHash<QString,QString> result);
        void twitchFollowChannelByIdError( QString errmessage );

        void twitchDataChanged();
};

#endif // TWITCHOBJECT_H
