#ifndef TWITCHAPI_H
#define TWITCHAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QHash>
#include <QtCore/QJsonObject>

#include <QtCore/QJsonDocument>
#include "generichelper.h"


class TwitchApi : public QObject
{
    Q_OBJECT
public:
    explicit TwitchApi(QObject *parent = 0, QString oauthtoken = "NONE");
    void setStatusAndGameTitle(const QString &url, QHash<QString, QString> urlParams);
    void getUser();
    void getStream(QString user);
    void getChannel(QString user);
    void getGame();
    void searchGames(QString searchStr);
    void setOAuthAccessToken(QString oauthtoken);
    void getFollows(QString user);

private:
    void getRequest( const QString &url );
    void getRequestAuthenticated( const QString &url );
    void putRequest( const QString &url, QHash<QString, QString> urlParams);
    QNetworkAccessManager m_nam;
    QJsonDocument json_buffer;
    QString oAuthAccessToken;


signals:
    void twitchReady( const QJsonDocument &twitchAsJSON );
    void twitchGameReady( const QJsonObject &twitchAsJSONObj );
    void networkError( QString errmessage );

public slots:
    void parseNetworkResponse( QNetworkReply *finished );
};

#endif // TWITCHAPI_H
