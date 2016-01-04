#ifndef TWITCHOBJECT_H
#define TWITCHOBJECT_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QString>
#include <QTimer>

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

        TwitchObject(QObject *parent, QString oAuthToken, const qint64 defaultTimerInterval);

        //! Set the time interval
        /*!
          \param msec time in milliseconds
        */
        void setInterval(qint64 msec);
        //! Start the update timer
        void startUpdateTimer();
        //! Stop the update timer
        void stopUpdateTimer();

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

    public slots:
        virtual void on_timedUpdate() = 0;

    protected:
        void setupTimer();

    private:
        QTimer *refreshTimer;
        QString oAuthToken;

        qint64 refreshTimerInterval;

        void getRequestStream(const QString &urlString);
        void getRequestChannel(const QString &urlString);
        void getRequestHost(const QString &urlString);

    private slots:
        void parseTwitchNetworkResponseStream();
        void parseTwitchNetworkResponseChannel();
        void parseTwitchNetworkResponseHost();

    signals:
        void networkError( QString errmessage );

        void twitchReadyStream( const QJsonDocument &twitchAsJSON );
        void twitchReadyChannel( const QJsonDocument &twitchAsJSON );
        void twitchReadyHost( const QJsonDocument &twitchAsJSON );

        void twitchDataChanged();
};

#endif // TWITCHOBJECT_H
