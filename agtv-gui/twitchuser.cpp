#include "twitchuser.h"

TwitchUser::TwitchUser(QObject *parent, const QString oAuthToken, const QString username, const qint64 defaultTimerInterval) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval), userName( username )
{


     this->currentlyUpdating = false;

     this->followedChannelsDataChanged = false;

     QObject::connect(this, SIGNAL(twitchReadyUserFollowedChannels(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserFollowedChannels(const QJsonDocument)));

     QObject::connect(this, SIGNAL(networkError(QString)), this, SLOT(twitchNetworkError(QString)));

     this->getUserFollowedChannels(this->userName);

     this->setupTimer();

}

QMap<QString, TwitchChannel *> TwitchUser::getFollowedChannels()
{
    return this->followedChannels;
}


void TwitchUser::on_timedUpdate()
{
    return;
}


void TwitchUser::updateFromJsonResponseUserFollowedChannels(const QJsonDocument &jsonResponseBuffer)
{

    QJsonObject jsonObject = jsonResponseBuffer.object();

    for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
        if (iter.key() == "follows")
        {
            for (int i = 0; i <= iter.value().toArray().size(); i++)
            {
                QJsonValue _val = iter.value().toArray().at(i);

                QString channelName = _val.toObject()["channel"].toObject()["name"].toString();


                TwitchChannel *twitchChannel = new TwitchChannel(this, this->getOAuthToken(), channelName, this->getRefreshTimerInterval());
                this->followedChannels[channelName] = twitchChannel;

                this->followedChannelsDataChanged = true;

            }
        }

    }


    emit twitchFollowedChannelsDataChanged(this->followedChannelsDataChanged);



}

void TwitchUser::twitchNetworkError(const QString errorString)
{
    qDebug() << errorString;
}
