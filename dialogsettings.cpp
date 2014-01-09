#include "dialogsettings.h"
#include "ui_dialogsettings.h"

dialogSettings::dialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogSettings)
{
    ui->setupUi(this);

    this->ui->lineEditUpdateInterval->setText(QString::number(genericHelper::getUpdateInterval()));

    if (genericHelper::getClearLogOnStartup() == true)
    {
        this->ui->checkBoxClearLogOnStartup->setChecked(true);
    } else {
        this->ui->checkBoxClearLogOnStartup->setChecked(false);
    }

    if (genericHelper::getLoadGameImages() == true)
    {
        this->ui->checkBoxLoadGameImages->setChecked(true);
    } else {
        this->ui->checkBoxLoadGameImages->setChecked(false);
    }

    if (genericHelper::getCheckUpdate() == true)
    {
        this->ui->checkBoxCheckUpdate->setChecked(true);
    } else {
        this->ui->checkBoxCheckUpdate->setChecked(false);
    }

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



  if (this->ui->checkBoxClearLogOnStartup->checkState() == Qt::Checked) {
      genericHelper::setClearLogOnStartup(true);

  }  else {
      genericHelper::setClearLogOnStartup(false);
  }

  if (this->ui->checkBoxLoadGameImages->checkState() == Qt::Checked) {
      genericHelper::setLoadGameImages(true);

  }  else {
      genericHelper::setLoadGameImages(false);
  }


  if (this->ui->checkBoxCheckUpdate->checkState() == Qt::Checked) {
      genericHelper::setCheckUpdate(true);

  }  else {
      genericHelper::setCheckUpdate(false);
  }

  genericHelper::setUpdateInterval(this->ui->lineEditUpdateInterval->text().toInt());

   emit valueChanged(  );


}

void dialogSettings::on_pushButtonClose_clicked()
{
    this->hide();
}
