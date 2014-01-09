#include "dialogobssettings.h"
#include "ui_dialogobssettings.h"

dialogOBSSettings::dialogOBSSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogOBSSettings)
{
    ui->setupUi(this);

    streamkey = "";

    ui->lineEditOBSBinary->setText(genericHelper::getOBSBin());

    // init twitch api object
    tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());




    QObject::connect(tw, SIGNAL(twitchStreamKeyReady(const QJsonObject)), this, SLOT(updateStreamKeyFromJsonResponse(const QJsonObject)));

}

dialogOBSSettings::~dialogOBSSettings()
{
    delete ui;
}

void dialogOBSSettings::on_pushButtonBrowseOBS_clicked()
{
    ui->lineEditOBSBinary->setText(QFileDialog::getOpenFileName(this,"Browse for OBS.exe",::getenv("PROGRAMFILES")));


}

void dialogOBSSettings::setDialogShown()
{
    dialogShown = true;
}

bool dialogOBSSettings::getDialogShown()
{
    return dialogShown;
}

void dialogOBSSettings::on_pushButtonApply_clicked()
{

    if (this->ui->checkBoxWriteTwitcherOBSConfig->checkState() == Qt::Checked) {

        genericHelper::writeTwitcherOBSConfig(streamkey);
    }


    if (this->ui->checkBoxWriteScenesConfig->checkState() == Qt::Checked) {
        genericHelper::writeTwitcherOBSScenesConfig();
    }

    genericHelper::setOBSBin( ui->lineEditOBSBinary->text());

     emit valueChanged(  );
}

void dialogOBSSettings::on_pushButtonClose_clicked()
{
    this->hide();
}

void dialogOBSSettings::on_checkBox_clicked()
{
    this->ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
    QMessageBox::information(this, tr("twitcher"),
                                    tr("If you want to start OBS in Admin-Mode, just launch twitcher in admin mode."),
                                    QMessageBox::Ok
                                    );
}

void dialogOBSSettings::updateStreamKeyFromJsonResponse(const QJsonObject &jsonObject) {

    tw->setOAuthAccessToken(genericHelper::getOAuthAccessToken());

    streamkey = jsonObject["stream_key"].toString();

    if (streamkey.length() > 5) {
        this->ui->pushButtonApply->setEnabled(true);
        int ret = QMessageBox::information(this, tr("twitcher"),
                                        "stream_key fetched: "+streamkey ,
                                        QMessageBox::Ok
                                        );

    } else {
        this->ui->checkBoxWriteTwitcherOBSConfig->setChecked(false);
        this->ui->pushButtonApply->setEnabled(true);
    }
}

void dialogOBSSettings::on_checkBoxWriteTwitcherOBSConfig_stateChanged(int arg1)
{
     if (this->ui->checkBoxWriteTwitcherOBSConfig->checkState() == Qt::Checked) {
         this->ui->pushButtonApply->setEnabled(false);
            tw->getKrakenChannel();
     } else {
         this->ui->pushButtonApply->setEnabled(true);
     }
}

void dialogOBSSettings::on_checkBoxWriteScenesConfig_stateChanged(int arg1)
{
     if (this->ui->checkBoxWriteScenesConfig->checkState() == Qt::Checked) {

     }
}
