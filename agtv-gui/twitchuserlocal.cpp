#include "twitchuserlocal.h"

TwitchUserLocal::TwitchUserLocal(QObject *parent, const QString oAuthToken, const QString username, const qint64 defaultTimerInterval) :
    TwitchObject(parent, oAuthToken, defaultTimerInterval), userName( username )
{
     this->bookmarkedChannelsDataChanged = false;

}


QMap<QString, TwitchChannel *> TwitchUserLocal::getBookmarkedChannels()
{
    return this->bookmarkedChannels;
}

void TwitchUserLocal::on_timedUpdate()
{
    return;
}


void TwitchUserLocal::loadBookmarks()
{
    QStringList loadedbookmarks;
    loadedbookmarks = genericHelper::getBookmarks();


    QListIterator<QString> itr (loadedbookmarks);

    while (itr.hasNext()) {
        QString current = itr.next();
        TwitchChannel *twitchChannel = new TwitchChannel(this, this->getOAuthToken(), current, this->getRefreshTimerInterval());
        this->bookmarkedChannels[current] = twitchChannel;

        this->bookmarkedChannelsDataChanged = true;
    }

     qDebug() << "emit bookmarkedChannels";
     emit twitchBookmarkedChannelsDataChanged(this->bookmarkedChannelsDataChanged);

}
