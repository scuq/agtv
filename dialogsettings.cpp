#include "dialogsettings.h"
#include "ui_dialogsettings.h"

dialogSettings::dialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogSettings)
{
    ui->setupUi(this);

    this->ui->lineEditUpdateInterval->setText(QString::number(genericHelper::getUpdateInterval()));

}

dialogSettings::~dialogSettings()
{
    delete ui;
}

void dialogSettings::setDialogShown()
{
    dialogShown = true;
}

bool dialogSettings::getDialogShown()
{
    return dialogShown;
}

void dialogSettings::on_pushButtonApply_clicked()
{

  genericHelper::setUpdateInterval(this->ui->lineEditUpdateInterval->text().toInt());
}

void dialogSettings::on_pushButtonClose_clicked()
{
    this->hide();
}
