#ifndef TWITCHAPI_H
#define TWITCHAPI_H

#include <QObject>
#include <QHash>

class twitchApi : public QObject
{
    Q_OBJECT
public:
    explicit twitchApi(QObject *parent = 0, const QString apiVersion = "v5");

    QHash<QString, QString> getApiUrls_Users(QHash<QString, QString> user);
    QHash<QString, QString> getApiUrls_Search(QString querystr);

    QString getRequestHeader_Accept();

    QString getRequestHeader_ContentTypeHeader();

private:
    QString apiVersion;
    QHash<QString, QString> twitchUser;
    QString requestHeader_Accept;
    QString requestHeader_ContentTypeHeader;


    void setupHeaders();

signals:

public slots:
};

#endif // TWITCHAPI_H
