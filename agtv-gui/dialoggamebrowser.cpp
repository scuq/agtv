#include "dialoggamebrowser.h"
#include "ui_dialoggamebrowser.h"

#include <QScrollBar>

DialogGameBrowser::DialogGameBrowser(QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogGameBrowser)
{
    ui->setupUi(this);

    this->setupModels();

    this->gameBrowser = new TwitchGameBrowser(this, QString(""));
    QObject::connect(this->gameBrowser, SIGNAL(twitchGameBrowserReadyTopGames()), this, SLOT(updateTopGames()));
    QObject::connect(this->gameBrowser, SIGNAL(twitchGameBrowserLogoForGameReady(const QString)), this, SLOT(updateLogoForGame(const QString)));
    QObject::connect(this->gameBrowser, SIGNAL(twitchGameBrowserLogoForStreamReady(const TwitchGameBrowser::Stream)), this, SLOT(updateLogoForStream(const TwitchGameBrowser::Stream)));
    QObject::connect(this->gameBrowser, SIGNAL(twitchGameBrowserStreamsForGameReady(const QList<TwitchGameBrowser::Stream>)),
                     this, SLOT(updateStreamsForGame(const QList<TwitchGameBrowser::Stream>)));

    this->gameBrowser->getGames();
}

void DialogGameBrowser::setupModels()
{
    QStringList horzHeaders = {"id", "Game", "Channels", "Viewers"};

    stmodelTopGames = new QStandardItemModel(0, 4, this);
    stmodelTopGames->setHorizontalHeaderLabels(horzHeaders);
    stmodelTopGamesproxymodel = new AdvQSortFilterProxyModel(this);
    stmodelTopGamesproxymodel->setSourceModel(stmodelTopGames);
    stmodelTopGamesproxymodel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->ui->tableViewTopGames->setModel(stmodelTopGamesproxymodel);

    this->ui->tableViewTopGames->setSortingEnabled(true);
    this->ui->tableViewTopGames->sortByColumn(2);
    this->ui->tableViewTopGames->hideColumn(0);

    horzHeaders = QStringList{ "Name", "Viewers", "Game", "Status Message"};

    stmodelGame = new QStandardItemModel(0,3,this);
    stmodelGame->setHorizontalHeaderLabels(horzHeaders);

    this->ui->tableViewGame->setModel(stmodelGame);

    QObject::connect(this->ui->tableViewTopGames->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     this, SLOT(onTableViewGamesScrolled(int)));
}

void DialogGameBrowser::showEvent(QShowEvent *e)
{
    this->ui->toolBox->setCurrentIndex(0);
    this->ui->toolBox->setItemText(1, "");
    this->ui->toolBox->setItemEnabled(1, false);
    QDialog::showEvent(e);
}

void DialogGameBrowser::onTableViewGamesScrolled(int value)
{
    if(value > 0.8*this->ui->tableViewTopGames->verticalScrollBar()->maximum() ) {
        this->gameBrowser->getMoreGames();
    }
}

DialogGameBrowser::~DialogGameBrowser()
{
    delete ui;
}

void DialogGameBrowser::updateLogoForGame(const QString gamename)
{
    for(int row=0; row<stmodelTopGames->rowCount(); row++) {
        QString modelgamename = stmodelTopGames->data(stmodelTopGames->index(row,1)).toString();
        if(QString::compare(gamename, modelgamename) == 0) {
            QStandardItem *qsitem1 = new QStandardItem(gamename);
            qsitem1->setIcon(QIcon(this->gameBrowser->getGameList()[gamename]->logo));
            this->stmodelTopGames->setItem(row,1,qsitem1);
        }
    }
    this->ui->tableViewTopGames->resizeColumnsToContents();
    this->ui->tableViewTopGames->resizeRowsToContents();
    this->ui->tableViewTopGames->sortByColumn(2);
}

void DialogGameBrowser::updateTopGames()
{
    auto gameList = this->gameBrowser->getGameList();
    qDebug() << gameList.size();

    stmodelTopGames->setRowCount(gameList.size());

    qint64 row=0;
    for(auto iter = gameList.begin(); iter != gameList.end(); ++iter)  {
        qint64 gameid = iter.value()->gameid;
        QString gamename = iter.value()->gamename;
        qint64 viewers = iter.value()->viewers;
        qint64 channels = iter.value()->channels;

        QStandardItem *qsitem0 = new QStandardItem(gameid);
        stmodelTopGames->setItem(row, 0, qsitem0);
        QStandardItem *qsitem1 = new QStandardItem(gamename);
        qsitem1->setIcon(QIcon(iter.value()->logo));
        stmodelTopGames->setItem(row, 1, qsitem1);
        QStandardItem *qsitem2 = new QStandardItem(channels);
        qsitem2->setData(channels, Qt::DisplayRole);
        stmodelTopGames->setItem(row, 2, qsitem2);
        QStandardItem *qsitem3 = new QStandardItem(viewers);
        qsitem3->setData(viewers, Qt::DisplayRole);
        stmodelTopGames->setItem(row, 3, qsitem3);
        ++row;
    }

    this->ui->tableViewTopGames->resizeColumnsToContents();
    this->ui->tableViewTopGames->resizeRowsToContents();
    // this->ui->tableViewTopGames->sortByColumn(3);
}

void DialogGameBrowser::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogGameBrowser::on_tableViewTopGames_activated(const QModelIndex &index)
{
    QString _game;

    _game = this->stmodelTopGamesproxymodel->data(index.sibling(index.row(),1),0).toString();
    qDebug() << _game;

    stmodelGame->removeRows(0, stmodelGame->rowCount());

    this->ui->toolBox->setItemEnabled(1, true);
    this->ui->toolBox->setItemText(1,_game);
    this->ui->toolBox->setCurrentIndex(1);
    this->gameBrowser->getGame(_game);
}

void DialogGameBrowser::updateStreamsForGame(const QList<TwitchGameBrowser::Stream> streams)
{
    stmodelGame->removeRows(0, stmodelGame->rowCount());

    for(auto iter = streams.begin(); iter != streams.end(); ++iter)  {
        QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(iter->streamer));
        QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg(iter->viewers));
        QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(iter->game));
        QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(iter->status));
        QList<QStandardItem*> items = { qsitem0, qsitem1, qsitem2, qsitem3};
        stmodelGame->appendRow(items);
    }
}

void DialogGameBrowser::updateLogoForStream(const TwitchGameBrowser::Stream stream)
{
    for(int row=0; row<stmodelGame->rowCount(); row++) {
        QString modelstreamname = stmodelGame->data(stmodelGame->index(row,0)).toString();
        if(QString::compare(stream.streamer, modelstreamname) == 0) {
            QStandardItem *qsitem1 = new QStandardItem(stream.streamer);
            qsitem1->setIcon(QIcon(stream.logo));
            this->stmodelGame->setItem(row,0,qsitem1);
        }
    }
    this->ui->tableViewGame->resizeColumnsToContents();
    this->ui->tableViewGame->resizeRowsToContents();
}

void DialogGameBrowser::on_tableViewGame_activated(const QModelIndex &index)
{
    QString _stream;

    _stream = this->stmodelGame->data(index.sibling(index.row(),0),0).toString();

    emit startStream( _stream );
}
