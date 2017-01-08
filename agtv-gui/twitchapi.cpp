#include "twitchapi.h"

twitchApi::twitchApi(QObject *parent, const QString apiVersion) : QObject(parent)
{
    this->apiVersion = apiVersion;

    this->setupHeaders();
}

QHash<QString, QString> twitchApi::getApiUrls_Users(QHash<QString, QString> user)
{
    QHash<QString, QString> urls_Users;

    urls_Users["GetUserFollows"] = QString("https://api.twitch.tv/kraken/users/%1/follows/channels").arg(user["id"]);
    urls_Users["GetUser"] = QString("https://api.twitch.tv/kraken/user");
    urls_Users["FollowChannel"] = QString("https://api.twitch.tv/kraken/users/%1/follows/channels/%CHANNELID%").arg(user["id"]);


    return urls_Users;
}

QHash<QString, QString> twitchApi::getApiUrls_Search(QString querystr)
{
    QHash<QString, QString> urls_Search;

    urls_Search["Channels"] = QString("https://api.twitch.tv/kraken/search/channels?query=%1").arg(querystr);

    return urls_Search;
}



QString twitchApi::getRequestHeader_Accept()
{
    return this->requestHeader_Accept;
}

QString twitchApi::getRequestHeader_ContentTypeHeader()
{
    return this->requestHeader_ContentTypeHeader;
}

void twitchApi::setupHeaders()
{
    if (this->apiVersion == "v5") {
        this->requestHeader_Accept="application/vnd.twitchtv.v5+json";
        this->requestHeader_ContentTypeHeader="application/x-www-form-urlencoded";
    }
}

