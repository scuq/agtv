#include "dialogobssettings.h"
#include "ui_dialogobssettings.h"

dialogOBSSettings::dialogOBSSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogOBSSettings)
{
    ui->setupUi(this);

    ui->lineEditOBSBinary->setText(genericHelper::getOBSBin());
}

dialogOBSSettings::~dialogOBSSettings()
{
    delete ui;
}

void dialogOBSSettings::on_pushButtonBrowseOBS_clicked()
{
    ui->lineEditOBSBinary->setText(QFileDialog::getOpenFileName(this,"Browse for OBS.exe",getenv("PROGRAMFILES")));


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
    genericHelper::setOBSBin( ui->lineEditOBSBinary->text());
}

void dialogOBSSettings::on_pushButtonClose_clicked()
{
    this->hide();
}

void dialogOBSSettings::on_checkBox_clicked()
{
    this->ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
    int ret = QMessageBox::information(this, tr("twitcher"),
                                    tr("If you want to start OBS in Admin-Mode, just launch twitcher in admin mode."),
                                    QMessageBox::Ok
                                    );
}
