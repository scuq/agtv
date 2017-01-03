#include "twitchuser.h"

#include "generichelper.h"

TwitchUser::TwitchUser(QObject *parent, const QString oAuthToken, const QString username, const QString userid, const qint64 defaultTimerInterval, QString useragent) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval), userName( username ), userId ( userid )
{

     this->setUserAgentStr(useragent);    
    
     this->currentlyUpdating = false;

     this->followedChannelsDataChanged = false;
        
     QObject::connect(this, SIGNAL(twitchReadyUserAuthenticationStatus(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserAuthenticationStatus(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserFollowedChannels(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserFollowedChannels(const QJsonDocument)));
     
     QObject::connect(this, SIGNAL(twitchReadyUserUnfollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserUnfollowedChannels(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserFollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserFollowChannel(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserUnfollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserUnfollowChannel(const QJsonDocument)));
     
     QObject::connect(this, SIGNAL(twitchReadyUserSetStatusAndGameTitle (const QJsonDocument)), this, SLOT(updateFromJsonResponseUserSetStatusAndGameTitle(const QJsonDocument)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserFollowedChannels(QString)), this, SLOT(onTwitchNetworkErrorUserFollowedChannels(QString)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserFollowChannel(QString)), this, SLOT(onTwitchNetworkErrorUserFollowChannel(QString)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserUnfollowChannel(QString)), this, SLOT(onTwitchNetworkErrorUserUnfollowChannel(QString)));
     
     QObject::connect(this, SIGNAL(twitchNetworkErrorUserAuthenticationStatus(QString)), this, SLOT(onTwitchNetworkErrorUserAuthenticationStatus(QString)));
     
     QObject::connect(this, SIGNAL(authCheckSuccessfull()), this, SLOT(onAuthCheckSuccessfull()));
         
     // fetch and set twitch client id
     this->setTwitchClientId();
     
     // trigger auth status update
     this->getUserAuthenticationStatus();

     this->setupTimer();

     this->getUserFollowedChannels(this->userId);

}

QMap<QString, TwitchChannel *> TwitchUser::getFollowedChannels()
{
    return this->followedChannels;
}

QStringList TwitchUser::getFollowedChannelsList()
{
    return this->followedChannelsList;
}

void TwitchUser::on_timedUpdate()
{
    this->getUserAuthenticationStatus();
    
    this->getUserFollowedChannels(this->userId);
}

void TwitchUser::followChannel(QString channelName)
{
    this->followChannelUser(channelName,this->userId);
}

void TwitchUser::unfollowChannel(QString channelName)
{
    this->unfollowChannelUser(channelName,this->userId);

    this->followedChannels.remove(channelName);

    this->followedChannelsDataChanged = true;

    emit twitchFollowedChannelsDataChanged(this->followedChannelsDataChanged);
}

void TwitchUser::checkAuthenticationSetup()
{   
  
    if ((this->getOAuthToken() == "<NONE>") ||(this->getOAuthToken() == "")) {
        this->setAuthenticationStatus(AuthenticationStatus::needssetup);  
    } else {
        this->setAuthenticationStatus(AuthenticationStatus::unknown);  
    }  
    
}

void TwitchUser::setStatusAndGameTitle(QHash<QString, QString> setParams)
{
    
    this->setUserStatusAndGameTitle(this->userId, setParams);
}

void TwitchUser::setAuthenticationStatus(AuthenticationStatus newStatus)
{
    
    
    if (this->authStatus != newStatus) {
        
        if (timerLastStatusChange.elapsed() > 100) {
        
            this->authStatus = newStatus;
            timerLastStatusChange.restart();
            
            switch( newStatus )
            {
            case AuthenticationStatus::ok:
                emit authCheckSuccessfull();
                break ;
            case AuthenticationStatus::needssetup:
                emit twitchNeedsOAuthSetup();
                break ;
            default:
                emit authCheckFailed();
                break ;
                
            }
        
        } else  {
            qDebug() << "Last status change occured < 100ms dampening emits.";
        }
    } else {
         qDebug() << "Last status is current status dampening emits.";
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
                if(channelName.length()>0) {
                    if(!this->followedChannels.contains(channelName)) {
                        TwitchChannel *twitchChannel = new TwitchChannel(this, this->getOAuthToken(), channelName, this->getRefreshTimerInterval());
                        twitchChannel->setUserAgentStr(this->getUserAgentStr());
                        // STOPPING TIMER FOR NOW
                        twitchChannel->stopUpdateTimer();
                        // STOPPING TIMER FOR NOW
                        this->followedChannels[channelName] = twitchChannel;
                        this->followedChannelsList.append(channelName);
                        this->followedChannelsDataChanged = true;
                    }
                }
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
        // STOPPING TIMER FOR NOW
        twitchChannel->stopUpdateTimer();
        // STOPPING TIMER FOR NOW
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
        emit twitchFollowedChannelsDataChanged(this->followedChannelsDataChanged);
    } else {
        emit twitchUnfollowChannelError("unfollow failed.");
    }
}

void TwitchUser::updateFromJsonResponseUserSetStatusAndGameTitle(const QJsonDocument &jsonResponseBuffer)
{
    qDebug() << jsonResponseBuffer;
}



void TwitchUser::updateFromJsonResponseUserAuthenticationStatus(const QJsonDocument &jsonResponseBuffer)
{
    
    QJsonObject jsonObject = jsonResponseBuffer.object();


    if (!jsonObject["email"].isNull()) {
        
         
         this->setAuthenticationStatus(AuthenticationStatus::ok);
       
         // set username if <NONE> or empty - needed on first setup
         if ((this->userName == "<NONE>") || (this->userName == "")) {
             if (!jsonObject["name"].isNull()) {
                 this->userName = jsonObject["name"].toString();
                 emit newUsernameDetected(this->userName);
             }
         }
         if ((this->userId == "<NONE>") || (this->userId == "")) {
             if (!jsonObject["_id"].isNull()) {
                 this->userId = QString::number(jsonObject["_id"].toDouble(),'g',10);
                 emit newUseridDetected(this->userId);
             }
         }
         if ((this->userBio == "<NONE>") || (this->userBio == "")) {
             if (!jsonObject["bio"].isNull()) {
                 this->userBio = jsonObject["bio"].toString();
                 emit newUserbioDetected(this->userBio);
             }
         }
         if ((this->userEmail == "<NONE>") || (this->userEmail == "")) {
             if (!jsonObject["email"].isNull()) {
                 this->userEmail = jsonObject["email"].toString();
                 emit newUseremailDetected(this->userEmail);
             }
         }
         if ((this->userCreatedAt == "<NONE>") || (this->userCreatedAt == "")) {
             if (!jsonObject["created_at"].isNull()) {
                 this->userCreatedAt = jsonObject["created_at"].toString();
                 emit newUsercreatedatDetected(this->userCreatedAt);
             }
         }

         
        
         
    } else {
       
        this->setAuthenticationStatus(AuthenticationStatus::nok);
    }
    
   
}

void TwitchUser::onAuthCheckSuccessfull()
{
    // this->getUserFollowedChannels(this->userName);
}

void TwitchUser::validateNewAuthToken(QString newOAuthToken)
{
    this->setOAuthToken(newOAuthToken);
    this->getUserAuthenticationStatus();
    
}

void TwitchUser::onAuthTokenSetupSuccessful(bool)
{
    qDebug() << "onAuthTokenSetupSuccessful";
    this->stopUpdateTimer();
    
    // trigger auth status update
    this->getUserAuthenticationStatus();

    // this->setupTimer();

    this->getUserFollowedChannels(this->userId);
    
    this->startUpdateTimer();    
}


void TwitchUser::onTwitchNetworkErrorUserFollowedChannels(const QString errorString)
{
    qDebug() << errorString;
}

void TwitchUser::onTwitchNetworkErrorUserFollowChannel(const QString errorString)
{
    qDebug() << errorString;
}

void TwitchUser::onTwitchNetworkErrorUserUnfollowChannel(const QString errorString)
{
    qDebug() << errorString;
}

void TwitchUser::onTwitchNetworkErrorUserAuthenticationStatus(const QString errorString)
{

    
    if (errorString.contains("requires authentication")) {
        this->setAuthenticationStatus(AuthenticationStatus::nok);
    }
    
}
