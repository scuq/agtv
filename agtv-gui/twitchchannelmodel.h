#ifndef TWITCHCHANNELMODEL_H
#define TWITCHCHANNELMODEL_H

#include <QObject>
#include <QStandardItemModel>

#include "twitchchannel.h"

class TwitchChannelModel : public QStandardItemModel
{
    Q_OBJECT

public:
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
