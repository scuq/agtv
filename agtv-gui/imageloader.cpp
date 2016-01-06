#include "imageloader.h"

imageLoader::imageLoader(QObject *parent) : QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                      SLOT(fileDownloaded(QNetworkReply*)));

}

void imageLoader::download(QUrl imageUrl)
{
    QNetworkRequest request(imageUrl);

    genericHelper::log("imageLoader - download of requested: "+imageUrl.toString());

    m_WebCtrl.get(request);
}

void imageLoader::fileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray imageLoader::downloadedData() const
{
    return m_DownloadedData;
}
