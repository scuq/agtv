#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include "generichelper.h"


class imageLoader : public QObject
{
    Q_OBJECT
public:
    explicit imageLoader(QObject *parent = 0);
    QByteArray downloadedData() const;
    void download( QUrl imageUrl );


signals:
    void downloaded();

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;

private slots:

    void fileDownloaded(QNetworkReply* pReply);


public slots:
};

#endif // IMAGELOADER_H
