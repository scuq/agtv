#include "dialogoptions.h"
#include "ui_dialogoptions.h"

#include <QFileDialog>

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
    this->ui->checkBoxUpdateCheck->setChecked(genericHelper::getCheckUpdate());

    this->ui->vlcPathEdit->setText(genericHelper::getVlcPath());
    this->ui->hexchatPathEdit->setText(genericHelper::getHexChatPath());

    restoreGeometry(genericHelper::getGeometry("options").toByteArray());

}

void DialogOptions::on_pushButtonOk_clicked()
{
    if (this->ui->checkBoxUpdateCheck->checkState() == Qt::Checked) {

        genericHelper::setCheckUpdate(true);
    } else {
        genericHelper::setCheckUpdate(false);
    }
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
    genericHelper::setVlcPath(this->ui->vlcPathEdit->text());
    genericHelper::setHexChatPath(this->ui->hexchatPathEdit->text());

    genericHelper::setUpdateInterval( this->ui->spinBoxPollInterval->value());

    this->hide();
    genericHelper::saveGeometry("options",saveGeometry());
}

void DialogOptions::on_pushButtonCancel_clicked()
{
    this->hide();
    genericHelper::saveGeometry("options",saveGeometry());

}

void DialogOptions::on_vlcPathSelectButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Select VLC Executable"), QCoreApplication::applicationDirPath(), tr("VLC Execitable (*)"));
    if(! fileName.isEmpty())
        this->ui->vlcPathEdit->setText(fileName);
}

void DialogOptions::on_hexchatPathSelectButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Select HexChat Executable"), QCoreApplication::applicationDirPath(), tr("HexChat Executable (*)"));
    if(! fileName.isEmpty())
        this->ui->hexchatPathEdit->setText(fileName);
}
