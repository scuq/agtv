#include "dialoglaunch.h"
#include "ui_dialoglaunch.h"

DialogLaunch::DialogLaunch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLaunch)
{
    ui->setupUi(this);
    this->loadPositions();
    this->ui->pushButtonStart->setEnabled(false);

}

DialogLaunch::~DialogLaunch()
{
    delete ui;
}


void DialogLaunch::setDialogShown()
{
    dialogShown = true;
    this->refreshUiData();


}

bool DialogLaunch::getDialogShown()
{
     this->refreshUiData();
    return dialogShown;
}

void DialogLaunch::refreshUiData()
{
    restoreGeometry(genericHelper::getGeometry("launch").toByteArray());
    qDebug() << genericHelper::getStreamPositioning();
    if (genericHelper::getStreamPositioning() == false) {
        this->ui->comboBoxPosSelect->setEnabled(false);
        this->ui->comboBoxPosSelect->setHidden(true);
        this->ui->labelPos->setHidden(true);
    } else {
        this->ui->comboBoxPosSelect->setEnabled(true);
        this->ui->comboBoxPosSelect->setHidden(false);
        this->ui->labelPos->setHidden(false);
    }
}

void DialogLaunch::setStreamTitle(QString streamtitle, QString position)
{

    this->ui->pushButtonStart->setEnabled(false);
    this->streamurl = "";
    this->loadPositions();
    this->ui->labelStream->setText(streamtitle);

}

void DialogLaunch::setStreamUrl(QString streamurl)
{


    this->streamurl = streamurl;

    this->ui->pushButtonStart->setEnabled(true);


}

void DialogLaunch::loadPositions()
{

    this->ui->comboBoxPosSelect->clear();
    QHash<QString, QString> poss;
    poss = genericHelper::getPositions();
    if (poss.keys().length() == 0) {
           this->ui->comboBoxPosSelect->addItem("<default>");
    } else {
        this->ui->comboBoxPosSelect->addItems(poss.keys());
    }


}

void DialogLaunch::on_pushButtonStart_clicked()
{

    QString geo;
    geo = genericHelper::getPosition(this->ui->comboBoxPosSelect->currentText());

    if (geo.count(",") == 3) {

        QStringList _geo;
        _geo = geo.split(",");


        x = QString(_geo[0]).toInt();
        y = QString(_geo[1]).toInt();
        w = QString(_geo[2]).toInt();
        h = QString(_geo[3]).toInt();
    }


    emit startStreamPlay ( "vlc2", streamurl, this->ui->labelStream->text(), w, h, x, y, false, "" );

    this->streamurl = "";
    this->ui->pushButtonStart->setEnabled(false);
    this->hide();
    genericHelper::saveGeometry("launch",saveGeometry());

}
