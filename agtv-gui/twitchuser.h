#ifndef TWITCHUSER_H
#define TWITCHUSER_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QElapsedTimer>

#include "twitchobject.h"
#include "twitchchannel.h"

class TwitchUser : public TwitchObject
{
    Q_OBJECT
    public:

        TwitchUser(QObject *parent, const QString oAuthToken, const QString username, const qint64 defaultTimerInterval = 1000, QString useragent = "-");
        
        enum class AuthenticationStatus {
            ok,
            nok,
            needssetup,
            unknown
        };

        QMap<QString, TwitchChannel*> getFollowedChannels();
        QStringList getFollowedChannelsList();
        void on_timedUpdate();
        
        void followChannel(QString channelName);
        void unfollowChannel(QString channelName);
        void checkAuthenticationSetup();
        void setStatusAndGameTitle(QHash<QString, QString> setParams );
     
    private:
        bool currentlyUpdating;
        bool followedChannelsDataChanged;
        bool bookmarkedChannelsDataChanged;
        AuthenticationStatus authStatus;
        QElapsedTimer timerLastStatusChange;
       
        QString userName;
        QMap<QString, TwitchChannel*> followedChannels;
        QStringList followedChannelsList;
        
        void setAuthenticationStatus(AuthenticationStatus newStatus);


    private slots:
        void updateFromJsonResponseUserFollowedChannels(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseUserFollowChannel(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseUserUnfollowChannel(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseUserSetStatusAndGameTitle(const QJsonDocument &jsonResponseBuffer);
        void updateFromJsonResponseUserAuthenticationStatus(const QJsonDocument &jsonResponseBuffer);
        void onAuthCheckSuccessfull();
        void validateNewAuthToken(QString newOAuthToken);
        void onAuthTokenSetupSuccessful(bool);
        
        void onTwitchNetworkErrorUserFollowedChannels(const QString errorString);
        void onTwitchNetworkErrorUserFollowChannel(const QString errorString);
        void onTwitchNetworkErrorUserUnfollowChannel(const QString errorString);
        void onTwitchNetworkErrorUserAuthenticationStatus(const QString errorString);

    signals:
        void twitchFollowedChannelsDataChanged(const bool followedChannelsDataChanged);
        void twitchFollowChannelError(const QString msg);
        void twitchFollowChannelSuccess(const QString msg);
        void twitchUnfollowChannelError(const QString msg);
        void twitchUnfollowChannelSuccess(const QString msg);
        void twitchNeedsOAuthSetup();
        void authCheckSuccessfull();
        void authCheckFailed();
        void newUsernameDetected(QString username);

};



#endif // TWITCHUSER_H
