#ifndef TWITCHCHANNELMODEL_H
#define TWITCHCHANNELMODEL_H

#include <QObject>
#include <QStandardItemModel>

#include "twitchchannel.h"

class TwitchChannelModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum class ColumnIndex {
        channelName = 0,
        onlineStatus = 1,
        viewers = 2,
        followers = 3,
        game = 4,
        status = 5
    };

    TwitchChannelModel(QObject* parent = 0, const qint64 defaultTimerInterval = 1000);

    void addChannel(const QString channel);
    void removeChannel(const QString channel);

    TwitchChannel* getChannel(const QString channel);

private slots:
    void twitchChannelDataChanged(const bool onlineStatusChanged);

private:
    QMap<QString, TwitchChannel*> twitchChannels;
    qint64 updateInterval;

signals:
    void notifyByTray(QString title, QString message);

};

#endif // TWITCHCHANNELMODEL_H
