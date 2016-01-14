#include "twitchchannelmodel.h"

TwitchChannelModel::TwitchChannelModel(QObject* parent, const qint64 defaultTimerInterval) :
    QStandardItemModel(parent)
{
    this->updateInterval = defaultTimerInterval;
}

QList<QString> TwitchChannelModel::getChannelList()
{
    QList<QString> channels;
    for(auto channel : twitchChannels) {
        channels.append(channel->getChannelName());
    }
    return channels;
}

bool TwitchChannelModel::addChannel(const QString channel)
{
    bool success = false;
    if(channel.length() > 0) {
        if( ! this->twitchChannels.contains(channel) ) {
            TwitchChannel *twitchChannel = new TwitchChannel(this, QString(""), channel, this->updateInterval);
            this->twitchChannels[channel] = twitchChannel;
            QObject::connect(twitchChannel, SIGNAL(twitchChannelDataChanged(const bool)),
                             this, SLOT(twitchChannelDataChanged(const bool)));
            qint64 rows = this->rowCount();

            QStandardItem *qsitem0 = new QStandardItem(twitchChannel->getChannelName());
            this->setItem(rows, (qint64)TwitchChannelModel::ColumnIndex::channelName, qsitem0);
            QStandardItem *qsitem1 = new QStandardItem(twitchChannel->getChannelOnlineStatusString());
            this->setItem(rows, (qint64)TwitchChannelModel::ColumnIndex::onlineStatus, qsitem1);
            QStandardItem *qsitem2 = new QStandardItem(twitchChannel->getChannelViewers());
            this->setItem(rows, (qint64)TwitchChannelModel::ColumnIndex::viewers, qsitem2);
            QStandardItem *qsitem3 = new QStandardItem(twitchChannel->getChannelFollowers());
            this->setItem(rows, (qint64)TwitchChannelModel::ColumnIndex::followers, qsitem3);
            QStandardItem *qsitem4 = new QStandardItem(twitchChannel->getChannelGame());
            this->setItem(rows, (qint64)TwitchChannelModel::ColumnIndex::game, qsitem4);
            QStandardItem *qsitem5 = new QStandardItem(twitchChannel->getChannelTitle());
            this->setItem(rows, (qint64)TwitchChannelModel::ColumnIndex::status, qsitem5);

            success = true;
        }
    }

    return success;
}

TwitchChannel* TwitchChannelModel::getChannel(const QString channel)
{
    if(this->twitchChannels.contains(channel)) {
        return this->twitchChannels[channel];
    } else {
        return 0;
    }
}

void TwitchChannelModel::removeChannel(const QString channel)
{
    if(this->twitchChannels.contains(channel)) {
        TwitchChannel *twitchChannel = this->twitchChannels.take(channel);
        twitchChannel->deleteLater();

        for(int i = 0; i<this->rowCount(); ++i) {
            QModelIndex streamer_index = this->index(i,0);
            if ( this->itemData(streamer_index)[0].toString() == channel )  {
                this->removeRow(i);
            }
        }
    }
}

void TwitchChannelModel::twitchChannelDataChanged(const bool onlineStatusChanged) {
    TwitchChannel *channel = qobject_cast<TwitchChannel *>(QObject::sender());
    if(channel) {
        for(int i = 0; i<this->rowCount(); ++i) {
            QModelIndex streamer_index = this->index(i, (qint64)TwitchChannelModel::ColumnIndex::channelName);
            QModelIndex online_index = this->index(i, (qint64)TwitchChannelModel::ColumnIndex::onlineStatus);
            QModelIndex viewers_index = this->index(i, (qint64)TwitchChannelModel::ColumnIndex::viewers);
            QModelIndex followers_index = this->index(i, (qint64)TwitchChannelModel::ColumnIndex::followers);
            QModelIndex game_index = this->index(i, (qint64)TwitchChannelModel::ColumnIndex::game);
            QModelIndex status_index = this->index(i, (qint64)TwitchChannelModel::ColumnIndex::status);

            if ( this->itemData(streamer_index)[0].toString() == channel->getChannelName() )  {
                this->setData(online_index, channel->getChannelOnlineStatusString());
                this->setData(viewers_index, channel->getChannelViewers());
                this->setData(followers_index, channel->getChannelFollowers());
                this->setData(game_index, channel->getChannelGame());
                this->setData(status_index, channel->getChannelTitle());
            }
        }

        if(onlineStatusChanged) {
            emit( notifyByTray(channel->getChannelName() + " is now " + channel->getChannelOnlineStatusString(), channel->getChannelTitle()) );
        }
    }
}

qint64 TwitchChannelModel::getUpdateInterval() const
{
    return updateInterval;
}

void TwitchChannelModel::setUpdateInterval(const qint64 &value)
{
    updateInterval = value;
    for(auto channel : twitchChannels) {
        channel->setInterval(updateInterval);
    }
}
