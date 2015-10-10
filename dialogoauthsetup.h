#ifndef DIALOGOAUTHSETUP_H
#define DIALOGOAUTHSETUP_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QString>
#include <QMessageBox>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include "generichelper.h"
#include "twitchapi.h"
#include "twitchclientid.h"
#include "imageloader.h"

namespace Ui {
class dialogOauthSetup;
}

class dialogOauthSetup : public QDialog
{
    Q_OBJECT

public:
    explicit dialogOauthSetup(QWidget *parent = 0);
    ~dialogOauthSetup();
    void setDialogShown();
    bool getDialogShown();
    void refreshUiData();

private slots:
    void on_pushButtonAuthorizeOnTwitch_clicked();

    void on_pushButtonTestOAuth_clicked();
    void on_AuthSuccess(const QJsonDocument message);
    void errorPopup(QString message);


    void on_pushButtonOk_clicked();

    void on_pushButtonRevoke_clicked();

    void on_pushButtonCancel_clicked();

    void loadProfileImage();

    void on_lineEditOAuthToken_textChanged(const QString &arg1);



signals:
void twitchAuthSetupChanged(bool);

private:
    Ui::dialogOauthSetup *ui;
    bool dialogShown;
    TwitchApi *tw;
    imageLoader *imgl;
    QPixmap profileimage;
    QString profileimageUrl;
    bool setupModeStarted = false;
};

#endif // DIALOGOAUTHSETUP_H
