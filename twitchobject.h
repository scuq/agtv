#ifndef TWITCHOBJECT_H
#define TWITCHOBJECT_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QString>
#include <QTimer>

class TwitchObject : public QObject
{
    Q_OBJECT

    public:
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
        QNetworkAccessManager m_stream, m_channel;

        qint64 refreshTimerInterval;

        void getRequestStream(const QString &urlString);
        void getRequestChannel(const QString &urlString);

signals:
        void networkError( QString errmessage );

        void twitchReadyStream( const QJsonDocument &twitchAsJSON );
        void twitchReadyChannel( const QJsonDocument &twitchAsJSON );
        void twitchDataChanged();

private slots:
        void parseNetworkResponseStream(QNetworkReply *finished);
        void parseNetworkResponseChannel(QNetworkReply *finished);

};

#endif // TWITCHOBJECT_H
