#include "dialogoptions.h"
#include "ui_dialogoptions.h"

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

    this->ui->checkBoxCloseToTray->setChecked(genericHelper::getCloseToTray());
    this->ui->checkBoxStreamOnlineNotify->setChecked(genericHelper::getStreamOnlineNotify());
    this->ui->spinBoxPollInterval->setValue(genericHelper::getUpdateInterval());

}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::setDialogShown()
{
    dialogShown = true;
}

bool DialogOptions::getDialogShown()
{
    return dialogShown;
}

void DialogOptions::on_pushButtonOk_clicked()
{

    if (this->ui->checkBoxCloseToTray->checkState() == Qt::Checked) {

        genericHelper::setCloseToTray(true);
    } else {
        genericHelper::setCloseToTray(false);
    }
    if (this->ui->checkBoxStreamOnlineNotify->checkState() == Qt::Checked) {

        genericHelper::setStreamOnlineNotify(true);
    } else {
        genericHelper::setStreamOnlineNotify(false);
    }


    genericHelper::setUpdateInterval( this->ui->spinBoxPollInterval->value());

    this->hide();
}

void DialogOptions::on_pushButtonCancel_clicked()
{
    this->hide();
}
