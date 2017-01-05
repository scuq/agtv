#include "twitchuser.h"

#include "generichelper.h"

TwitchUser::TwitchUser(QObject *parent, const QString oAuthToken, const QString username, const QString userid, const qint64 defaultTimerInterval, QString useragent) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval)
{

     this->user["name"] = username;
     this->user["id"] = userid;

     apiUrls_Users = this->api->getApiUrls_Users(user);

     this->setUserAgentStr(useragent);    
    
     this->currentlyUpdating = false;

     this->followedChannelsDataChanged = false;
        
     QObject::connect(this, SIGNAL(twitchReadyUserAuthenticationStatus(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserAuthenticationStatus(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserFollowedChannels(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserFollowedChannels(const QJsonDocument)));
     
     QObject::connect(this, SIGNAL(twitchReadyUserUnfollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserUnfollowedChannels(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserFollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserFollowChannel(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyUserUnfollowChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseUserUnfollowChannel(const QJsonDocument)));

     QObject::connect(this, SIGNAL(twitchReadyGetStreamer(const QJsonDocument, QString)), this, SLOT(updateFromJsonResponseGetStreamer(const QJsonDocument, QString)));
     
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

     this->getUserFollowedChannels(this->user);

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
    
    this->getUserFollowedChannels(user);
}

void TwitchUser::followChannel(QString channelName)
{
    this->followChannelUser(channelName,user);
}

void TwitchUser::unfollowChannel(QString channelName)
{
    this->unfollowChannelUser(channelName,user);

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

void TwitchUser::getUserFollowedChannels(QHash<QString, QString> user)
{
    this->getRequest(apiUrls_Users["GetUserFollows"], "TwitchObject::getUserFollowedChannels");
}

void TwitchUser::getStreamerId(QString streamer)
{
    QHash<QString,QString> _streamer;
    _streamer["streamer"] = streamer;
    this->getRequest(apiUrls_Users["SearchChannels"].replace("%STREAMER%",streamer), "TwitchObject::getStreamer", _streamer);
}

void TwitchUser::getUserAuthenticationStatus()
{
    this->getRequest(apiUrls_Users["GetUser"], "TwitchObject::getUserAuthenticationStatus");
}

void TwitchUser::followChannelUser(QString channelName, QHash<QString, QString> user)
{
    this->getRequest(apiUrls_Users["FollowChannel"].replace("%CHANNEL%",channelName), "TwitchObject::followChannelUser");
}

void TwitchUser::unfollowChannelUser(QString channelName, QHash<QString, QString> user)
{
    this->delRequestUser("https://api.twitch.tv/kraken/users/"+user["id"]+"/follows/channels/"+channelName+"&client_id="+this->getTwitchClientId(), "TwitchObject::unfollowChannelUser");
}


void TwitchUser::setStatusAndGameTitle(QHash<QString, QString> setParams)
{
    
    this->setUserStatusAndGameTitle(this->user["id"], setParams);
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
         if ((this->user["name"] == "<NONE>") || (this->user["name"] == "")) {
             if (!jsonObject["name"].isNull()) {
                 this->user["name"] = jsonObject["name"].toString();

             }
         }
         if ((this->user["id"] == "<NONE>") || (this->user["id"] == "")) {
             if (!jsonObject["_id"].isNull()) {
                 this->user["id"] = QString::number(jsonObject["_id"].toDouble(),'g',10);

             }
         }
         if ((this->user["bio"] == "<NONE>") || (this->user["bio"] == "")) {
             if (!jsonObject["bio"].isNull()) {
                 this->user["bio"] = jsonObject["bio"].toString();

             }
         }
         if ((this->user["email"] == "<NONE>") || (this->user["email"] == "")) {
             if (!jsonObject["email"].isNull()) {
                 this->user["email"] = jsonObject["email"].toString();

             }
         }
         if ((this->user["created_at"] == "<NONE>") || (this->user["created_at"] == "")) {
             if (!jsonObject["created_at"].isNull()) {
                 this->user["created_at"] = jsonObject["created_at"].toString();

             }
         }
         emit newUserDetected(this->user);

         
        
         
    } else {
       
        this->setAuthenticationStatus(AuthenticationStatus::nok);
    }
    

}

void TwitchUser::updateFromJsonResponseGetStreamer(const QJsonDocument &jsonResponseBuffer, QString lookedupstreamer)
{

    QHash<QString,QString> streamer;
    bool found = false;

    if (jsonResponseBuffer.isEmpty()) {

        emit twitchStreamerIdLookupError("streamer "+lookedupstreamer+" not found.",streamer);
    } else {
        QJsonObject jsonObject = jsonResponseBuffer.object();


        if (!jsonObject["channels"].isNull()) {



            for(int i=0;i<jsonObject["channels"].toArray().count();i++)
            {

                if (jsonObject["channels"].toArray().at(i).toObject()["name"].toString() == lookedupstreamer) {
                    if (!jsonObject["channels"].toArray().at(i).toObject()["_id"].isNull()) {
                        streamer["id"] = QString::number(jsonObject["channels"].toArray().at(i).toObject()["_id"].toDouble(),'g',10);
                    }
                    if (!jsonObject["channels"].toArray().at(i).toObject()["name"].isNull()) {
                        streamer["name"] = jsonObject["channels"].toArray().at(i).toObject()["name"].toString();
                    }
                    found = true;
                    emit twitchStreamerIdLookupSuccess("streamer "+lookedupstreamer+" found.",streamer);
                }

            }

        }
        if (found == false) {
            emit twitchStreamerIdLookupError("streamer "+lookedupstreamer+" not found.",streamer);
        }



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

    this->getUserFollowedChannels(this->user);
    
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

void TwitchUser::twitchNetworkErrorGetStreamer(const QString errorString)
{
     qDebug() << errorString;
}

void TwitchUser::parseNetworkResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    QString callingFuncName = "";

    callingFuncName = netReplies[reply];
    QHash<QString,QString> params = netParams[reply];

    if(reply) {
        if ( reply->error() != QNetworkReply::NoError ) {
            //emit networkError( reply->errorString() );

            if (callingFuncName == "TwitchObject::getUserFollowedChannels") {
                emit twitchNetworkErrorUserFollowedChannels( reply->errorString() );
            } else if (callingFuncName == "TwitchObject::followChannelUser") {
                emit twitchNetworkErrorUserFollowChannel( reply->errorString() );
            } else if (callingFuncName == "TwitchObject::unfollowChannelUser") {
                emit twitchNetworkErrorUserUnfollowChannel( reply->errorString() );
            } else if (callingFuncName == "TwitchObject::getUserAuthenticationStatus") {
                emit twitchNetworkErrorUserAuthenticationStatus( reply->errorString() );
            } else if (callingFuncName == "TwitchObject::getStreamer") {
                emit twitchNetworkErrorGetStreamer( reply->errorString() );
            }

            genericHelper::log( QString(Q_FUNC_INFO) + "(" + callingFuncName + ")" + QString(": ") + reply->errorString());

            reply->abort();
            reply->deleteLater();
            netReplies.remove(reply);

            return;
        }

        QJsonDocument json_buffer = QJsonDocument::fromJson(reply->readAll());

        if (callingFuncName == "TwitchObject::getUserFollowedChannels") {
            emit twitchReadyUserFollowedChannels( json_buffer );
        } else if (callingFuncName == "TwitchObject::followChannelUser") {
            emit twitchReadyUserFollowChannel( json_buffer );
        } else if (callingFuncName == "TwitchObject::unfollowChannelUser") {
            emit twitchReadyUserUnfollowChannel( json_buffer );
        } else if (callingFuncName == "TwitchObject::getUserAuthenticationStatus") {
            emit twitchReadyUserAuthenticationStatus( json_buffer );
        } else if (callingFuncName == "TwitchObject::setUserStatusAndGameTitle") {
            emit twitchReadyUserSetStatusAndGameTitle ( json_buffer );
        } else if (callingFuncName == "TwitchObject::getStreamer") {
            emit twitchReadyGetStreamer ( json_buffer, params["streamer"] );
        }


        genericHelper::log( QString(Q_FUNC_INFO) + QString(": Success"));

        reply->deleteLater();
        netReplies.remove(reply);

    } else {
        genericHelper::log( QString(Q_FUNC_INFO) + QString(": ") + "reply is NULL");
    }
    qDebug() << "Pending Replys: "  << this->getPendingReplyCount();
}
