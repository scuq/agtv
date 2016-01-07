#include "twitchuser.h"

TwitchUser::TwitchUser(QObject *parent, const QString oAuthToken, const QString username, const qint64 defaultTimerInterval) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval), userName( username )
{

    



     this->currentlyUpdating = false;

     this->followedChannelsDataChanged = false;
    
    
    
    
     QObject::connect(this, SIGNAL(twitchReadyUserAuthenticationStatus(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserAuthenticationStatus(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserFollowedChannels(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserFollowedChannels(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserFollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserFollowChannel(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserUnfollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserUnfollowChannel(const QJsonDocument)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserFollowedChannels(QString)), this, SLOT(twitchNetworkErrorUserFollowedChannels(QString)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserFollowChannel(QString)), this, SLOT(twitchNetworkErrorUserFollowChannel(QString)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserUnfollowChannel(QString)), this, SLOT(twitchNetworkErrorUserUnfollowChannel(QString)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserAuthenticationStatus(QString)), this, SLOT(twitchNetworkErrorUserAuthenticationStatus(QString)));

     this->setTwitchClientId();
     
     this->getUserAuthenticationStatus(this->userName);

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

void TwitchUser::followChannel(QString channelName)
{
    this->followChannelUser(channelName,this->userName);
}

void TwitchUser::unfollowChannel(QString channelName)
{
    this->unfollowChannelUser(channelName,this->userName);

    this->followedChannels.remove(channelName);

    this->followedChannelsDataChanged = true;

    emit twitchFollowedChannelsDataChanged(this->followedChannelsDataChanged);
}

void TwitchUser::checkAuthenticationSetup()
{
    
    
    
    if ((this->getOAuthToken() == "<NONE>") ||(this->getOAuthToken() == "")) {
        this->authStatus = AuthenticationStatus::needssetup;
        emit twitchNeedsOAuthSetup();
    } else {
        this->authStatus = AuthenticationStatus::unknown;
    }
    
    
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

void TwitchUser::updateFromJsonResponseUserFollowChannel(const QJsonDocument &jsonResponseBuffer)
{

    QJsonObject jsonObject = jsonResponseBuffer.object();

    if (!jsonObject["created_at"].isNull()) {

        QString channelName = jsonObject["channel"].toObject()["name"].toString();

        TwitchChannel *twitchChannel = new TwitchChannel(this, this->getOAuthToken(), channelName, this->getRefreshTimerInterval());
        this->followedChannels[channelName] = twitchChannel;

        this->followedChannelsDataChanged = true;

        emit twitchFollowedChannelsDataChanged(this->followedChannelsDataChanged);
        emit twitchFollowChannelSuccess("follow ok.");

    } else {
        emit twitchFollowChannelError("channel doesn't exist.");
    }

}

void TwitchUser::updateFromJsonResponseUserUnfollowChannel(const QJsonDocument &jsonResponseBuffer)
{

    if (jsonResponseBuffer.isEmpty()) {
        emit twitchUnfollowChannelSuccess("unfollow ok.");
    } else {

        emit twitchUnfollowChannelError("unfollow failed.");
    }
}

void TwitchUser::updateFromJsonResponseUserAuthenticationStatus(const QJsonDocument &jsonResponseBuffer)
{
    
    QJsonObject jsonObject = jsonResponseBuffer.object();
    
    if (!jsonObject["email"].isNull()) {
         this->authStatus = AuthenticationStatus::ok;
         emit onAuthCheckSuccessfull();
         
    }
    
   
}

void TwitchUser::onAuthCheckSuccessfull()
{
    this->getUserFollowedChannels(this->userName);
}


void TwitchUser::twitchNetworkErrorUserFollowedChannels(const QString errorString)
{
    qDebug() << errorString;
}

void TwitchUser::twitchNetworkErrorUserFollowChannel(const QString errorString)
{
    qDebug() << errorString;
}

void TwitchUser::twitchNetworkErrorUserUnfollowChannel(const QString errorString)
{
    qDebug() << errorString;
}

void TwitchUser::twitchNetworkErrorUserAuthenticationStatus(const QString errorString)
{
    
    if (errorString.contains("requires authentication")) {
        this->authStatus = AuthenticationStatus::nok;
    }
    
}
