#include "dialogoauthsetup.h"
#include "ui_dialogoauthsetup.h"

dialogOauthSetup::dialogOauthSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogOauthSetup)
{
    ui->setupUi(this);



    this->ui->lineEditOAuthToken->setText(genericHelper::getOAuthAccessToken());


    tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());


    QObject::connect(tw, SIGNAL(twitchReady(const QJsonDocument)), this, SLOT(successPopup(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(networkError(QString)), this, SLOT(errorPopup(QString)));
}

void dialogOauthSetup::errorPopup(QString message) {
    QMessageBox::warning(this, tr("twitcher"),
                                    message,
                                    QMessageBox::Ok
                                    );
}

void dialogOauthSetup::successPopup(const QJsonDocument message) {
    int ret = QMessageBox::information(this, tr("twitcher"),
                                    message.toJson(),
                                    QMessageBox::Ok
                                    );

    QJsonObject jsonObject = message.object();



    genericHelper::setUsername(jsonObject["name"].toString());

    emit valueChanged(  );


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

void dialogOauthSetup::on_pushButtonAuthorizeOnTwitch_clicked()
{

    QString link = "https://api.twitch.tv/kraken/oauth2/authorize?response_type=token&client_id=XXXXXXXXXXXXX&redirect_uri=http://oauth.abyle.org/&scope=channel_editor+user_read+chat_login+channel_read";
    QDesktopServices::openUrl(QUrl(link));
}

void dialogOauthSetup::on_pushButtonSave_clicked()
{
    genericHelper::setOAuthAccessToken(this->ui->lineEditOAuthToken->text());

    tw->setOAuthAccessToken(this->ui->lineEditOAuthToken->text());
    tw->getUser();

    QMessageBox::information(this, tr("twitcher"),
                                    tr("Saved."),
                                    QMessageBox::Ok
                                    );
    emit valueChanged(  );

}

void dialogOauthSetup::on_pushButtonTestOAuth_clicked()
{
    tw->setOAuthAccessToken(this->ui->lineEditOAuthToken->text());
    tw->getUser();



}

void dialogOauthSetup::on_pushButtonREvoke_clicked()
{
    QString link = "http://www.twitch.tv/settings/applications";
    QDesktopServices::openUrl(QUrl(link));
}
