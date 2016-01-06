#include "updatecheck.h"

updateCheck::updateCheck(QObject *parent) :
    QObject(parent)
{

    version=VERSION;
    QObject::connect(&m_namu, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));


}

void updateCheck::getRequest( const QString &urlString )
{
    QUrl url ( urlString );

    QNetworkRequest req ( url );
    req.setRawHeader( "User-Agent" , QString(genericHelper::getAppName()+"/"+version).toStdString().c_str());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

    m_namu.get( req  );


}

void updateCheck::getCheck()
{

    genericHelper::log("update-check getCheck - "+genericHelper::getUpdateCheckUrl());
    this->getRequest(genericHelper::getUpdateCheckUrl());
}

void updateCheck::parseNetworkResponse( QNetworkReply *finished )
{
    if ( finished->error() != QNetworkReply::NoError )
    {


        genericHelper::log("update-check failed:"+finished->errorString());
        //emit networkError( finished->errorString() );

        return;
    }


    QByteArray data = finished->readAll();


    QString latestVersion = QString(data).trimmed();
    QString currentVersion = version;

    genericHelper::log("update-check latest version:"+latestVersion.replace(".",""));
    genericHelper::log("update-check current version:"+currentVersion.replace(".",""));


    if (latestVersion.replace(".","").toInt() > currentVersion.replace(".","").toInt()) {

        emit updateReady( QString(data).trimmed() );
    }











}
