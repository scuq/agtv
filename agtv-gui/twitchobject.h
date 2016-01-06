#ifndef TWITCHOBJECT_H
#define TWITCHOBJECT_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QString>
#include <QTimer>
#include <QSignalMapper>

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

        TwitchObject(QObject *parent, QString token, const qint64 defaultTimerInterval);

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
        void getUserFollowedChannels(QString user);
        //! Fires a user request to the Twitch API
        /*!

        */
        void followChannelUser(QString channelName, QString user);

        void unfollowChannelUser(QString channelName, QString user);

        void getHost(QString channelId);

        qint64 getRefreshTimerInterval();

        QString getOAuthToken();



    public slots:
        virtual void on_timedUpdate() = 0;

    protected:
        void setupTimer();

    private:
        QTimer *refreshTimer;
        const QString oAuthToken;

        qint64 refreshTimerInterval;

        void getRequestStream(const QString &urlString);
        void getRequestChannel(const QString &urlString);
        void getRequestHost(const QString &urlString);
        void getRequestUser(const QString &urlString, QString callingFuncName);
        void putRequestUser(const QString &urlString, QString callingFuncName);
        void delRequestUser(const QString &urlString, QString callingFuncName);

        QSignalMapper *smUserNetworkRequest;

    private slots:
        void parseTwitchNetworkResponseStream();
        void parseTwitchNetworkResponseChannel();
        void parseTwitchNetworkResponseHost();
        void parseTwitchNetworkResponseUser(QString callingFuncName);

    signals:
        void networkError( QString errmessage );

        void twitchReadyStream( const QJsonDocument &twitchAsJSON );
        void twitchReadyChannel( const QJsonDocument &twitchAsJSON );
        void twitchReadyHost( const QJsonDocument &twitchAsJSON );
        void twitchReadyUserFollowedChannels( const QJsonDocument &twitchAsJSON );
        void twitchReadyUserFollowChannel( const QJsonDocument &twitchAsJSON );
        void twitchReadyUserUnfollowChannel( const QJsonDocument &twitchAsJSON );

        void twitchDataChanged();
};

#endif // TWITCHOBJECT_H
