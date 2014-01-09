#include "dialoggamestats.h"
#include "ui_dialoggamestats.h"
#include <QtDebug>

dialogGameStats::dialogGameStats(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogGameStats)
{
    ui->setupUi(this);
    gamebrowserurl = QUrl("http://www.twitch.tv/directory/game/");
}

dialogGameStats::~dialogGameStats()
{
    delete ui;
}

void dialogGameStats::setDialogShown()
{
    dialogShown = true;
}

bool dialogGameStats::getDialogShown()
{
    return dialogShown;
}

void dialogGameStats::updateGameStatsFromJsonResponse(const QJsonObject &jsonObject) {

    if (!jsonObject["top"].isNull()) {



            for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter) {
                if (iter.key() == "top")
                {

                   // while (this->ui->tableWidgetGameStats->rowCount() > 0)
                   // {
                   //     this->ui->tableWidgetGameStats->removeRow(0);
                   // }

                    this->ui->treeWidgetGameStats->clear();

                    //this->ui->tableWidgetGameStats->setColumnCount(2);
                   // this->ui->tableWidgetGameStats->horizontalHeaderItem(0)->setText("Viewers");
                   // this->ui->tableWidgetGameStats->horizontalHeaderItem(1)->setText("Game");


                    for (int i = 0; i <= iter.value().toArray().size(); i++)
                    {
                   // qDebug() << iter.value().toArray().at(i).toObject()["game"].toObject()["name"].toString();
                   // qDebug() <<  iter.value().toArray().at(i).toObject()["viewers"].toDouble();


                    if (iter.value().toArray().at(i).toObject()["game"].toObject()["name"].toString() != "") {
                        QTreeWidgetItem * item = new QTreeWidgetItem();
                        item->setText(0,QString::number(i+1));
                        item->setText(1,QString::number(iter.value().toArray().at(i).toObject()["viewers"].toDouble()));
                        item->setText(2,iter.value().toArray().at(i).toObject()["game"].toObject()["name"].toString());
                        item->setTextColor(1,  QColor(85,85,255));


                        this->ui->treeWidgetGameStats->addTopLevelItem(item);

                    }





                    }







                }
            }



        }

}

void dialogGameStats::on_pushButtonClose_clicked()
{
    this->hide();
}

void dialogGameStats::on_treeWidgetGameStats_itemClicked(QTreeWidgetItem *item, int column)
{




}

void dialogGameStats::on_treeWidgetGameStats_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    gamebrowserurl = QUrl("http://www.twitch.tv/directory/game/" + QString(QUrl::toPercentEncoding(item->text(2))));

    QDesktopServices::openUrl(gamebrowserurl);
}
