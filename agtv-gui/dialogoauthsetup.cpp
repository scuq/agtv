#include "dialogoauthsetup.h"
#include "ui_dialogoauthsetup.h"

dialogOauthSetup::dialogOauthSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogOauthSetup)
{
    ui->setupUi(this);
     
    this->ui->pushButtonSave->setEnabled(false);
    this->ui->pushButtonRestore->setHidden(true);
    
    
}


void dialogOauthSetup::onAuthNok()
{
    emit authTokenSetupSuccessful(false);
    this->ui->pushButtonSave->setEnabled(false);        
}

void dialogOauthSetup::onAuthOk()
{  
    this->ui->pushButtonSave->setEnabled(true);
    this->ui->pushButtonRevoke->setEnabled(true);
    emit authTokenSetupSuccessful(true);
}

dialogOauthSetup::~dialogOauthSetup()
{
    delete ui;
}

void dialogOauthSetup::setDialogShown()
{
    dialogShown = true;
    
}

bool dialogOauthSetup::getDialogShown()
{
    return dialogShown;
    
}

void dialogOauthSetup::setCurrentStoredAuthToken(QString currentStoredAuthToken, QHash<QString,QString> user)
{

   if ((currentStoredAuthToken == "<NONE>") || (currentStoredAuthToken == "") ) {
       this->ui->lineEditOAuthToken->setPlaceholderText("<< Paste token HERE >>");
       this->ui->pushButtonRestore->setHidden(false);
       this->ui->pushButtonRestore->setEnabled(true);
   } else {   
    this->ui->lineEditOAuthToken->setText(currentStoredAuthToken);
   }

   this->ui->labelUsername->setText(user["name"]);
   this->ui->labelUserid->setText(user["id"]);
   this->ui->labelEmail->setText(user["email"]);
   this->ui->labelBio->setText(user["bio"]);
   this->ui->labelCreatedat->setText(user["created_at"]);
}

void dialogOauthSetup::on_pushButtonAuthorizeOnTwitch_clicked()
{

    emit onAuthorizeRequested();
    
}


void dialogOauthSetup::on_pushButtonRevoke_clicked()
{
    QString link = "http://www.twitch.tv/settings/applications";
    QDesktopServices::openUrl(QUrl(link));
}

void dialogOauthSetup::on_pushButtonCancel_clicked()
{
    this->hide();
}

void dialogOauthSetup::on_pushButtonVerify_clicked()
{
    emit authTokenChanged(this->ui->lineEditOAuthToken->text());
}

void dialogOauthSetup::on_pushButtonSave_clicked()
{
    emit saveAuthTokenRequested(this->ui->lineEditOAuthToken->text());  
    this->hide();    
}

void dialogOauthSetup::on_lineEditOAuthToken_textChanged(const QString &arg1)
{
    if (arg1.length() > 8) {
        this->ui->pushButtonVerify->setEnabled(true);
    } else {
        this->ui->pushButtonVerify->setEnabled(false);
    }
}

void dialogOauthSetup::on_pushButtonRestore_clicked()
{
    emit restoreSettings();
}
