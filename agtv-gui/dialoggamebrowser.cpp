#include "dialoggamebrowser.h"
#include "ui_dialoggamebrowser.h"

#include <QScrollBar>

DialogGameBrowser::DialogGameBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGameBrowser)
{
    ui->setupUi(this);

    // agtvDefItemDelegate =  new AgtvDefaultItemDelegate();
    htmlDelegate = new HTMLDelegate;

    QStringList horzHeaders = { "id", "Game", "Viewers", "Logo Small", "Logo Medium", "Logo Large" };

    stmodelTopGames = new QStandardItemModel(0,5,this);
    stmodelTopGames->setHorizontalHeaderLabels(horzHeaders);
    stproxymodelTopGames = new AdvQSortFilterProxyListModel(this);
    stproxymodelTopGames->setSourceModel(stmodelTopGames);
    stproxymodelTopGames->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // this->ui->listViewTopGames->setItemDelegate(agtvDefItemDelegate);
    // this->ui->listViewTopGames->setItemDelegate(htmlDelegate);
    this->ui->tableViewTopGames->setModel(stproxymodelTopGames);
    this->ui->tableViewTopGames->hideColumn(0);
    this->ui->tableViewTopGames->hideColumn(3);
    this->ui->tableViewTopGames->hideColumn(4);
    this->ui->tableViewTopGames->hideColumn(5);
    // this->ui->listViewTopGames->setModelColumn(1);


    horzHeaders = QStringList{ "Name", "Viewers", "Game", "Status Message"};

    stmodelGame = new QStandardItemModel(0,3,this);
    stmodelGame->setHorizontalHeaderLabels(horzHeaders);
    stproxymodelGame = new AdvQSortFilterProxyListModel(this);
    stproxymodelGame->setSourceModel(stmodelGame);
    stproxymodelGame->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // this->ui->listViewTopGames->setItemDelegate(agtvDefItemDelegate);
    // this->ui->listViewTopGames->setItemDelegate(htmlDelegate);

    this->ui->tableViewGame->setModel(stproxymodelGame);
//    this->ui->tableViewGame->hideColumn(0);
//    this->ui->tableViewGame->hideColumn(3);
//    this->ui->tableViewGame->hideColumn(4);
//    this->ui->tableViewGame->hideColumn(5);

    tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());

    QObject::connect(this->ui->tableViewTopGames->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onTableViewGamesScrolled(int)));

    QObject::connect(tw, SIGNAL(twitchReadyTopGames(const QJsonDocument)), this, SLOT(updateFromJsonResponseTopGames(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchStreamsForGameReady(const QJsonDocument)), this, SLOT(updateFromJsonResponseStreamsForGame(const QJsonDocument)));

    this->offset = 0;
    this->tw->getTopGames(offset, 10);
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
        this-> offset += 10;
        this->tw->getTopGames(offset, 10);
    }
}

DialogGameBrowser::~DialogGameBrowser()
{
    delete ui;
}

void DialogGameBrowser::updateFromJsonResponseTopGames(const QJsonDocument &jsonResponseBuffer)
{
    QJsonObject jsonObject = jsonResponseBuffer.object();

    for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
        if (iter.key() == "top")
        {
            for (int i = 0; i < iter.value().toArray().size(); i++)
            {
                QJsonValue _val = iter.value().toArray().at(i);

                QString gameid = QString::number(_val.toObject()["game"].toObject()["_id"].toInt(), 'f', 0);
                QString gamename = _val.toObject()["game"].toObject()["name"].toString();
                QString viewers = QString::number(_val.toObject()["viewers"].toInt(), 'f', 0);
                QString logosmall = _val.toObject()["game"].toObject()["logo"].toObject()["small"].toString();
                QString logomedium = _val.toObject()["game"].toObject()["logo"].toObject()["medium"].toString();
                QString logolarge = _val.toObject()["game"].toObject()["logo"].toObject()["large"].toString();

                if( this->stproxymodelTopGames->getColData(0,gameid,0).toString().isEmpty() ) {
                    QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(gameid));
                    QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg(gamename));
                    QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(viewers));
                    QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(logosmall));
                    QStandardItem *qsitem4 = new QStandardItem(QString("%0").arg(logomedium));
                    QStandardItem *qsitem5 = new QStandardItem(QString("%0").arg(logolarge));
                    QList<QStandardItem*> items = { qsitem0, qsitem1, qsitem2, qsitem3, qsitem4, qsitem5};
                    stmodelTopGames->appendRow(items);
                } else {
                    // Update entry
                    stproxymodelTopGames->updateCol(0, gameid, 1, gamename);
                    stproxymodelTopGames->updateCol(0, gameid, 2, viewers);
                    stproxymodelTopGames->updateCol(0, gameid, 3, logosmall);
                    stproxymodelTopGames->updateCol(0, gameid, 4, logomedium);
                    stproxymodelTopGames->updateCol(0, gameid, 5, logolarge);
                }
            }
        }
    }

    if(this->offset < 100) {
        this-> offset += 10;
        this->tw->getTopGames(offset, 10);
    } else {
        this->ui->tableViewTopGames->resizeColumnsToContents();
    }
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
    this->tw->getStreamsForGame(_game.replace(" ","+"));
}

void DialogGameBrowser::updateFromJsonResponseStreamsForGame(const QJsonDocument &jsonResponseBuffer)
{
    QJsonObject jsonObject = jsonResponseBuffer.object();

    stmodelGame->removeRows(0, stmodelGame->rowCount());

    for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
        if (iter.key() == "streams")
        {
            for (int i = 0; i < iter.value().toArray().size(); i++)
            {
                QJsonValue _val = iter.value().toArray().at(i);

                QString streamer  = _val.toObject()["channel"].toObject()["name"].toString();
                QString game  = _val.toObject()["channel"].toObject()["game"].toString();
                QString status  = _val.toObject()["channel"].toObject()["status"].toString();
                QString viewers = QString::number(_val.toObject()["viewers"].toInt(), 'f', 0);

                QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(streamer));
                QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg(viewers));
                QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(game));
                QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(status));
                QList<QStandardItem*> items = { qsitem0, qsitem1, qsitem2, qsitem3};
                stmodelGame->appendRow(items);
            }
        }
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
