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
    loadedbookmarks = this->getBookmarks();

    QListIterator<QString> itr (loadedbookmarks);

    while (itr.hasNext()) {
        QString current = itr.next();
        if(!this->bookmarkedChannels.contains(current)) {
            TwitchChannel *twitchChannel = new TwitchChannel(this, "this->oAuthToken", current, this->getRefreshTimerInterval());
            // STOPPING TIMER FOR NOW
            twitchChannel->stopUpdateTimer();
            // STOPPING TIMER FOR NOW
            this->bookmarkedChannels[current] = twitchChannel;
            
            this->bookmarkedChannelsDataChanged = true;
            
        }
    }
    
    emit twitchBookmarkedChannelsDataChanged(this->bookmarkedChannelsDataChanged);
    
    this->bookmarkedChannelsDataChanged = false;
}

QString TwitchUserLocal::getStoredOAuthAccessToken(QString company, QString app)
{
    QSettings settings(company, app);
    
    QString _oauthtoken = "<NONE>";
    
    if (settings.value("oauthAccessToken", "").toString().length() > 1) {
        _oauthtoken = settings.value("oauthAccessToken", "").toString();
    
    }
    
    emit oAuthAccessTokenLoaded(_oauthtoken, this->getStoredUser());
    
    return _oauthtoken;
}

QHash<QString, QString> TwitchUserLocal::getStoredUser(QString company, QString app)
{
    QHash<QString,QString> storeduser;
    QSettings settings(company, app);
    storeduser["id"] = settings.value("user_id", "").toString();
    storeduser["name"] = settings.value("user_name", "").toString();
    storeduser["email"] = settings.value("user_email", "").toString();
    storeduser["bio"] = settings.value("user_bio", "").toString();
    storeduser["created_at"] = settings.value("user_created_at", "").toString();
    return storeduser;
}

QString TwitchUserLocal::getStoredUsername(QString company, QString app)
{
    QSettings settings(company, app);
    return settings.value("user_name", "").toString();
}

QString TwitchUserLocal::getStoredUserid(QString company, QString app)
{
    QSettings settings(company, app);
    return settings.value("user_id", "").toString();
}

QStringList TwitchUserLocal::getBookmarks(QString company, QString app)
{
    QSettings settings(company, app);
      
    return settings.value("bookmarks", QStringList()).toStringList();    
}

bool TwitchUserLocal::setBookmarks(QStringList bookmarks, QString company, QString app)
{
    QSettings settings(company, app);
    settings.setValue("bookmarks", bookmarks);
    settings.sync();    
    
    return true;
}

bool TwitchUserLocal::addBookmark(QString bookmark, QString company, QString app)
{

    QStringList currentbookmarks = this->getBookmarks();

    if (currentbookmarks.count(bookmark) <= 0) {
     
        currentbookmarks << bookmark;
        this->setBookmarks(currentbookmarks);
        this->loadBookmarks();
    }    
    

    
    return true;
}

bool TwitchUserLocal::deleteBookmark(QString bookmark, QString company, QString app)
{
    int bookmarkIndex;
    
    QStringList currentbookmarks = this->getBookmarks();
    
    bookmarkIndex = currentbookmarks.indexOf(bookmark);
    
    if (bookmarkIndex >= 0) {
            currentbookmarks.removeAt(bookmarkIndex);
            this->setBookmarks(currentbookmarks);
            this->bookmarkedChannels.remove(bookmark);
            
            emit twitchBookmarkedChannelsDataChanged(true);
            return true;
    }
    
    return false;
    
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

bool TwitchUserLocal::saveUser(QHash<QString, QString> user, QString company, QString app)
{
    bool ok = false;
    QSettings settings(company, app);

    QHashIterator<QString, QString> i(user);
    while (i.hasNext()) {
        i.next();
        settings.setValue("user_"+i.key(), i.value());
        settings.sync();
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

bool TwitchUserLocal::backupSettings(QString filename, QString company, QString app)
{

        QSettings* settings = new QSettings(filename, QSettings::IniFormat);
        settings->setValue("oauthAccessToken", this->getStoredOAuthAccessToken());
        settings->setValue("user_name", this->getStoredUsername());
        settings->setValue("user_id", this->getStoredUserid());
        settings->setValue("bookmarks", this->getBookmarks(company, app));
        settings->sync();
        
        return true;
        
}

bool TwitchUserLocal::restoreSettings(QString filename, QString company, QString app)
{
    QHash<QString,QString> restoreuser;

    QSettings* settingsFile = new QSettings(filename, QSettings::IniFormat);

    restoreuser["user_name"] = settingsFile->value("user_name", "").toString();
    restoreuser["user_id"] = settingsFile->value("user_name", "").toString();
    this->saveUser(restoreuser);
    this->saveOAuthAccessToken(settingsFile->value("oauthAccessToken", "").toString());
    this->setBookmarks(settingsFile->value("bookmarks", QStringList()).toStringList());
    this->loadBookmarks();
    
    emit oAuthAccessTokenLoaded(this->getStoredOAuthAccessToken(),this->getStoredUser());
    emit backupRestoredSuccessful(true);
    
    return true;    
}

void TwitchUserLocal::onSaveOAuthAccessToken(QString oAuthAccessToken)
{
    this->saveOAuthAccessToken(oAuthAccessToken);
}

void TwitchUserLocal::onSaveUser(QHash<QString, QString> user)
{
    this->saveUser(user);
}



qint64 TwitchUserLocal::getRefreshTimerInterval()
{
    return this->refreshTimerInterval;
}
