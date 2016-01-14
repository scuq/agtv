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
    QObject::connect(this->gameBrowser, SIGNAL(twitchGameBrowserStreamsForGameReady(const QList<TwitchGameBrowser::Stream>)),
                     this, SLOT(updateStreamsForGame(const QList<TwitchGameBrowser::Stream>)));

    this->gameBrowser->getGames();
}

void DialogGameBrowser::setupModels()
{
    QStringList horzHeaders = {"id", "Game", "Viewers"};

    stmodelTopGames = new QStandardItemModel(0,3,this);
    stmodelTopGames->setHorizontalHeaderLabels(horzHeaders);
    stproxymodelTopGames = new AdvQSortFilterProxyListModel(this);
    stproxymodelTopGames->setSourceModel(stmodelTopGames);
    stproxymodelTopGames->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->ui->tableViewTopGames->setModel(stproxymodelTopGames);
    // this->ui->tableViewTopGames->hideColumn(0);
    // this->ui->listViewTopGames->setModelColumn(1);

    this->ui->tableViewTopGames->setSortingEnabled(true);
    this->ui->tableViewTopGames->sortByColumn(2);

    horzHeaders = QStringList{ "Name", "Viewers", "Game", "Status Message"};

    stmodelGame = new QStandardItemModel(0,3,this);
    stmodelGame->setHorizontalHeaderLabels(horzHeaders);
    stproxymodelGame = new AdvQSortFilterProxyListModel(this);
    stproxymodelGame->setSourceModel(stmodelGame);
    stproxymodelGame->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->ui->tableViewGame->setModel(stproxymodelGame);
//    this->ui->tableViewGame->hideColumn(0);
//    this->ui->tableViewGame->hideColumn(3);
//    this->ui->tableViewGame->hideColumn(4);
//    this->ui->tableViewGame->hideColumn(5);

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

void DialogGameBrowser::updateTopGames()
{
    auto gameList = this->gameBrowser->getGameList();
    qDebug() << gameList.size();

    stmodelTopGames->setRowCount(gameList.size());

    qint64 row=0;
    for(auto iter = gameList.begin(); iter != gameList.end(); ++iter)  {
        QString gameid = iter.value()->gameid;
        QString gamename = iter.value()->gamename;
        qint64 viewers = iter.value()->viewers;

        QStandardItem *qsitem0 = new QStandardItem(gameid);
        stmodelTopGames->setItem(row, 0, qsitem0);
        QStandardItem *qsitem1 = new QStandardItem(gamename);
        qsitem1->setIcon(QIcon(iter.value()->logo));
        stmodelTopGames->setItem(row, 1, qsitem1);
        QStandardItem *qsitem2 = new QStandardItem(viewers);
        stmodelTopGames->setItem(row, 2, qsitem2);
        ++row;
    }

    this->ui->tableViewTopGames->resizeColumnsToContents();
    this->ui->tableViewTopGames->resizeRowsToContents();
    this->ui->tableViewTopGames->sortByColumn(2);
}

void DialogGameBrowser::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogGameBrowser::on_tableViewTopGames_activated(const QModelIndex &index)
{
    QString _game;

    _game = this->stproxymodelTopGames->data(index.sibling(index.row(),1),0).toString();

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

void DialogGameBrowser::on_tableViewGame_activated(const QModelIndex &index)
{
    QString _stream;

    _stream = this->stproxymodelGame->data(index.sibling(index.row(),0),0).toString();

    emit startStream( _stream );
}

void DialogGameBrowser::on_lineEditFilterGames_textChanged(const QString &arg1)
{
    stproxymodelTopGames->setFilterFixedString(arg1);
}
