#ifndef DIALOGGAMEBROWSER_H
#define DIALOGGAMEBROWSER_H

#include <QDialog>
#include <QStandardItemModel>

#include "advqsortfilterproxymodel.h"
#include "twitchgamebrowser.h"

namespace Ui {
class DialogGameBrowser;
}

class DialogGameBrowser : public QDialog
{
    Q_OBJECT

    public:
        explicit DialogGameBrowser(QWidget *parent = 0);
        ~DialogGameBrowser();

        void showEvent(QShowEvent *e);

    private slots:
        void updateStreamsForGame(const QList<TwitchGameBrowser::Stream> streams);
        void on_pushButtonClose_clicked();
        void on_tableViewTopGames_activated(const QModelIndex &index);
        void onTableViewGamesScrolled(int);
        void on_tableViewGame_activated(const QModelIndex &index);
        void updateTopGames();

        void updateLogoForGame(const QString gamename);
        void updateLogoForStream(const TwitchGameBrowser::Stream stream);
signals:
        void startStream( const QString streamer );

    private:
        Ui::DialogGameBrowser *ui;

        TwitchGameBrowser *gameBrowser;

        QStandardItemModel *stmodelTopGames;
        AdvQSortFilterProxyModel *stmodelTopGamesproxymodel;
        QStandardItemModel *stmodelGame;

        void setupModels();
};

#endif // DIALOGGAMEBROWSER_H
