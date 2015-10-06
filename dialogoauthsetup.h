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

private slots:
    void on_pushButtonAuthorizeOnTwitch_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonTestOAuth_clicked();
    void successPopup(const QJsonDocument message);
    void errorPopup(QString message);

    void on_pushButtonREvoke_clicked();

signals:
void valueChanged();

private:
    Ui::dialogOauthSetup *ui;
    bool dialogShown;
    TwitchApi *tw;
};

#endif // DIALOGOAUTHSETUP_H
