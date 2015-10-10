#include "dialogoptions.h"
#include "ui_dialogoptions.h"

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);


}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::setDialogShown()
{
    dialogShown = true;
    this->refreshUiData();

}

bool DialogOptions::getDialogShown()
{
    this->refreshUiData();
    return dialogShown;
}

void DialogOptions::refreshUiData()
{
    this->ui->plainTextEdit->clear();
    this->ui->checkBoxCloseToTray->setChecked(genericHelper::getCloseToTray());
    this->ui->checkBoxClearLogOnStart->setChecked(genericHelper::getClearLogOnStartup());
    this->ui->checkBoxStreamOnlineNotify->setChecked(genericHelper::getStreamOnlineNotify());
    this->ui->spinBoxPollInterval->setValue(genericHelper::getUpdateInterval());
    this->ui->checkBoxJoinBookmarked->setChecked(genericHelper::getJoinBookmarks());
    this->ui->checkBoxJoinFollowed->setChecked(genericHelper::getJoinFollow());
    this->ui->plainTextEdit->appendPlainText(genericHelper::getVlcArgs().join(" "));
    this->ui->checkBoxStreamPositioning->setChecked(genericHelper::getStreamPositioning());
    restoreGeometry(genericHelper::getGeometry("options").toByteArray());

}

void DialogOptions::on_pushButtonOk_clicked()
{

    if (this->ui->checkBoxStreamPositioning->checkState() == Qt::Checked) {

        genericHelper::setStreamPositioning(true);
    } else {
        genericHelper::setStreamPositioning(false);
    }
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
    if (this->ui->checkBoxClearLogOnStart->checkState() == Qt::Checked) {

        genericHelper::setClearLogOnStartup(true);
    } else {
        genericHelper::setClearLogOnStartup(false);
    }
    if (this->ui->checkBoxJoinBookmarked->checkState() == Qt::Checked) {

        genericHelper::setJoinBookmarks(true);
    } else {
        genericHelper::setJoinBookmarks(false);
    }
    if (this->ui->checkBoxJoinFollowed->checkState() == Qt::Checked) {

        genericHelper::setJoinFollow(true);
    } else {
        genericHelper::setJoinFollow(false);
    }
    if (this->ui->plainTextEdit->toPlainText().length() > 0) {
        genericHelper::setVlcArgs(this->ui->plainTextEdit->toPlainText().split(" "));
    }

    genericHelper::setUpdateInterval( this->ui->spinBoxPollInterval->value());

    this->hide();
    genericHelper::saveGeometry("options",saveGeometry());
}

void DialogOptions::on_pushButtonCancel_clicked()
{
    this->hide();
    genericHelper::saveGeometry("options",saveGeometry());

}
