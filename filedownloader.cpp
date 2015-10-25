#include "filedownloader.h"

FileDownloader::FileDownloader(QObject *parent) :
    QObject(parent)
{
    connect( &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
             this, SLOT (fileDownloaded(QNetworkReply*))
             );
}

FileDownloader::~FileDownloader()
{
}

void FileDownloader::downloadUrl(QUrl imageUrl)
{
    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();

    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
