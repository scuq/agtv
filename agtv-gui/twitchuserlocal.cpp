#include "twitchuserlocal.h"

TwitchUserLocal::TwitchUserLocal(QObject *parent, const qint64 defaultTimerInterval) :
    QObject(parent)
{
    this->refreshTimerInterval = defaultTimerInterval;
    
    this->bookmarkedChannelsDataChanged = false;
    

}




QMap<QString, TwitchChannel *> TwitchUserLocal::getBookmarkedChannels()
{
    return this->bookmarkedChannels;
}


void TwitchUserLocal::loadBookmarks()
{
    QStringList loadedbookmarks;
    loadedbookmarks = genericHelper::getBookmarks();


    QListIterator<QString> itr (loadedbookmarks);

    while (itr.hasNext()) {
        QString current = itr.next();
        TwitchChannel *twitchChannel = new TwitchChannel(this, "this->oAuthToken", current, this->getRefreshTimerInterval());
        this->bookmarkedChannels[current] = twitchChannel;

        this->bookmarkedChannelsDataChanged = true;
    }

     qDebug() << "emit bookmarkedChannels";
     emit twitchBookmarkedChannelsDataChanged(this->bookmarkedChannelsDataChanged);
     
}

QString TwitchUserLocal::getStoredOAuthAccessToken(QString company, QString app)
{
    QSettings settings(company, app);
    
    QString _oauthtoken = "<NONE>";
    
    if (settings.value("oauthAccessToken", "").toString().length() > 1) {
        _oauthtoken = settings.value("oauthAccessToken", "").toString();
    
    }
    
    return _oauthtoken;
}

QString TwitchUserLocal::getStoredUsername(QString company, QString app)
{
    QSettings settings(company, app);
    return settings.value("username", "").toString();
}

bool TwitchUserLocal::saveOAuthAccessToken(QString oAuthAccessToken, QString company, QString app)
{
    bool ok = false;
    
    if (oAuthAccessToken.length() > 0) {
        QSettings settings(company, app);
        settings.setValue("oauthAccessToken", oAuthAccessToken);
        settings.sync();    
        ok = true;
    }
    
    return ok;
}

bool TwitchUserLocal::isUserSetupOk(QString company, QString app)
{
    if (this->getStoredUsername() != "") {
        return true;
    } else {
        return false;
    }
}

qint64 TwitchUserLocal::getRefreshTimerInterval()
{
    return this->refreshTimerInterval;
}
