#include "twitchchannelmodel.h"

TwitchChannelModel::TwitchChannelModel(QObject* parent, const qint64 defaultTimerInterval) :
    QStandardItemModel(parent)
{
    this->updateInterval = defaultTimerInterval;
}

void TwitchChannelModel::addChannel(const QString channel)
{
    if(channel.length() > 0) {
        if( ! this->twitchChannels.contains(channel) ) {
            TwitchChannel *twitchChannel = new TwitchChannel(this, QString(""), channel, this->updateInterval);
            this->twitchChannels[channel] = twitchChannel;
            QObject::connect(twitchChannel, SIGNAL(twitchChannelDataChanged(const bool)),
                             this, SLOT(twitchChannelDataChanged(const bool)));
            qint64 rows = this->rowCount();

            QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelName()));
            this->setItem(rows, 0, qsitem0);
            QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelOnlineStatusString()));
            this->setItem(rows, 1, qsitem1);
            QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelViewers()));
            this->setItem(rows, 2, qsitem2);
            QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelGame()));
            this->setItem(rows, 3, qsitem3);
            QStandardItem *qsitem4 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelTitle()));
            this->setItem(rows, 4, qsitem4);
        }
    }
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
            QModelIndex streamer_index = this->index(i,0);
            QModelIndex online_index = this->index(i,1);
            QModelIndex viewers_index = this->index(i,2);
            QModelIndex game_index = this->index(i,3);
            QModelIndex status_index = this->index(i,4);

            if ( this->itemData(streamer_index)[0].toString() == channel->getChannelName() )  {
                this->setData(online_index, channel->getChannelOnlineStatusString());
                this->setData(viewers_index, channel->getChannelViewers());
                this->setData(game_index, channel->getChannelGame());
                this->setData(status_index, channel->getChannelTitle());
            }
        }

        if(onlineStatusChanged) {
            emit( notifyByTray(channel->getChannelName() + " is now " + channel->getChannelOnlineStatusString(), channel->getChannelTitle()) );
        }
    }

}
