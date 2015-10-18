#include "dialogoauthsetup.h"
#include "ui_dialogoauthsetup.h"

dialogOauthSetup::dialogOauthSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogOauthSetup)
{
    ui->setupUi(this);

    setupModeStarted = false;

    profileimageUrl = "";

    if (genericHelper::getOAuthAccessToken().length() > 3) {
        tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());
    } else {
        tw = new TwitchApi(this, "");
    }

    // init the image loader
    imgl = new imageLoader(this);

    QObject::connect(tw, SIGNAL(twitchReady(const QJsonDocument)), this, SLOT(on_AuthSuccess(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(networkError(QString)), this, SLOT(errorPopup(QString)));
    QObject::connect(imgl, SIGNAL(downloaded()), this, SLOT(loadProfileImage()));
}

void dialogOauthSetup::errorPopup(QString message) {
    QMessageBox::warning(this, tr("twitcher"),
                                    message,
                                    QMessageBox::Ok
                                    );
}

void dialogOauthSetup::on_AuthSuccess(const QJsonDocument message) {

    //int ret = QMessageBox::information(this, genericHelper::getAppName(),
      //                              message.toJson(),
        //                            QMessageBox::Ok
          //                          );



    QJsonObject jsonObject = message.object();



    //
    this->ui->labelUsername->setText(jsonObject["name"].toString());
    this->ui->labelEmail->setText(jsonObject["email"].toString());
    this->ui->labelAuthStatus->setText("<font color=green>OK</font>");
    this->profileimageUrl = jsonObject["logo"].toString();
    imgl->download(QUrl(this->profileimageUrl));





}

dialogOauthSetup::~dialogOauthSetup()
{
    delete ui;
}

void dialogOauthSetup::setDialogShown()
{
    dialogShown = true;
    this->refreshUiData();
}

bool dialogOauthSetup::getDialogShown()
{
    return dialogShown;
    this->refreshUiData();
}

void dialogOauthSetup::refreshUiData()
{
    if (this->setupModeStarted == false) {
        this->ui->lineEditOAuthToken->setText(genericHelper::getOAuthAccessToken());
    }
    this->ui->labelAuthStatus->setEnabled(true);
    this->ui->pushButtonOk->setEnabled(true);
    this->ui->pushButtonCancel->setEnabled(true);

    if (this->ui->lineEditOAuthToken->text() == "") {

        this->ui->pushButtonAuthorizeOnTwitch->setEnabled(true);

        this->ui->labelAuthStatus->setText("<font color=red>Failed</font>");
        this->ui->pushButtonOk->setEnabled(false);
        this->ui->pushButtonTestOAuth->setEnabled(false);

        //this->ui->pushButtonRevoke->setEnabled(false);
    } else {
        this->ui->pushButtonAuthorizeOnTwitch->setEnabled(false);
        this->ui->pushButtonRevoke->setEnabled(true);
        if (this->setupModeStarted == false) {
            this->ui->labelAuthStatus->setText("<font color=green>OK</font>");
        }
        this->ui->pushButtonOk->setEnabled(true);
        this->ui->pushButtonTestOAuth->setEnabled(true);
        this->ui->labelUsername->setEnabled(true);
        this->ui->labelEmail->setEnabled(true);
        this->ui->labelProfileImage->setEnabled(true);
        if (genericHelper::getOAuthAccessToken().length() > 3) {
            tw->setOAuthAccessToken(genericHelper::getOAuthAccessToken());
            tw->getUser();
        }

    }


}

void dialogOauthSetup::on_pushButtonAuthorizeOnTwitch_clicked()
{

    QString link = "https://api.twitch.tv/kraken/oauth2/authorize?response_type=token&client_id="+twitchclientid+"&redirect_uri=http://oauth.abyle.org/&scope=channel_editor+user_read+chat_login+channel_read";
    QDesktopServices::openUrl(QUrl(link));
    this->ui->pushButtonAuthorizeOnTwitch->setEnabled(false);
    this->setupModeStarted = true;
}


void dialogOauthSetup::on_pushButtonTestOAuth_clicked()
{
    tw->setOAuthAccessToken(this->ui->lineEditOAuthToken->text());
    tw->getUser();




}

void dialogOauthSetup::on_pushButtonOk_clicked()
{
    if ( (this->ui->labelUsername->text().length() > 0) && (this->ui->lineEditOAuthToken->text().length() > 0) ) {

        genericHelper::setUsername(this->ui->labelUsername->text());
        genericHelper::setOAuthAccessToken(this->ui->lineEditOAuthToken->text());

        tw->setOAuthAccessToken(this->ui->lineEditOAuthToken->text());
        tw->getUser();

        this->setupModeStarted = false;


        emit twitchAuthSetupChanged(true);
        this->hide();

    }
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

void dialogOauthSetup::loadProfileImage()
{
    profileimage.loadFromData(imgl->downloadedData());

    genericHelper::log("downloaded image, dimension:" + QString::number(profileimage.size().height()) + "x" +QString::number(profileimage.size().width()));

    if (profileimage.size().height() > 0) {
      this->ui->labelProfileImage->setPixmap(profileimage.scaled(128,128));
    } else {
        genericHelper::log("height is 0, image not set.");
    }
}

void dialogOauthSetup::on_lineEditOAuthToken_textChanged(const QString &arg1)
{

    this->refreshUiData();
}


