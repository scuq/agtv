#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include "generichelper.h"

class updateCheck : public QObject
{
    Q_OBJECT
public:
    explicit updateCheck(QObject *parent = 0);
    void getCheck();

private:
     QNetworkAccessManager m_namu;
     void getRequest( const QString &url );
     QString version;

signals:
    void updateReady( const QString &latestVersion );
    void networkError( QString errmessage );



public slots:
    void parseNetworkResponse( QNetworkReply *finished );

};

#endif // UPDATECHECK_H
