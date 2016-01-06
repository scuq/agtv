#ifndef TWITCHUSERLOCAL_H
#define TWITCHUSERLOCAL_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include "twitchobject.h"
#include "twitchchannel.h"

#include "generichelper.h"

class TwitchUserLocal : public TwitchObject
{
    Q_OBJECT
    public:

        TwitchUserLocal(QObject *parent, const QString oAuthToken, const QString username, const qint64 defaultTimerInterval = 1000);

        QMap<QString, TwitchChannel*> getBookmarkedChannels();

        void on_timedUpdate();
        void loadBookmarks();

    private:
        bool currentlyUpdating;
        bool followedChannelsDataChanged;
        bool bookmarkedChannelsDataChanged;

        const QString userName;

        QMap<QString, TwitchChannel*> bookmarkedChannels;



    private slots:

    signals:
        void twitchBookmarkedChannelsDataChanged(const bool bookmarkedChannelsDataChanged);
};

#endif // TWITCHUSERLOCAL_H
