#ifndef TWITCHOBJECT_H
#define TWITCHOBJECT_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QString>
#include <QTimer>

class TwitchObject : public QObject
{
    Q_OBJECT

    public:
        QNetworkAccessManager *nwManager;

        TwitchObject(QObject *parent, QString oAuthToken, const qint64 defaultTimerInterval);

        void setInterval(qint64 msec);
        void startUpdateTimer();
        void stopUpdateTimer();

    public slots:
        virtual void on_timedUpdate(){}

    protected:
        QTimer *refreshTimer;
        void getStream(QString channelName);
        void getChannel(QString user);

        void setupTimer();

private:
        QString oAuthToken;

        qint64 refreshTimerInterval;

        void getRequestStream(const QString &urlString);
        void getRequestChannel(const QString &urlString);

signals:
        void networkError( QString errmessage );

        void twitchReadyStream( const QJsonDocument &twitchAsJSON );
        void twitchReadyChannel( const QJsonDocument &twitchAsJSON );
        void twitchDataChanged();

private slots:
        void parseTwitchNetworkResponseStream();
        void parseTwitchNetworkResponseChannel();

};

#endif // TWITCHOBJECT_H
