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
  
public slots:
    void setCurrentStoredAuthToken(QString currentStoredAuthToken, QHash <QString, QString> user);
    

private slots:
    void on_pushButtonAuthorizeOnTwitch_clicked();
    
    void onAuthOk();
    
    void onAuthNok();

    void on_pushButtonRevoke_clicked();

    void on_pushButtonCancel_clicked();
    
    void on_pushButtonVerify_clicked();
    
    void on_pushButtonSave_clicked();
    
    void on_lineEditOAuthToken_textChanged(const QString &arg1);
    
    void on_pushButtonRestore_clicked();
    
signals:


    void onAuthorizeRequested();
    void authTokenChanged(QString);
    void saveAuthTokenRequested(QString);
    void authTokenSetupSuccessful(bool);
    void restoreSettings();

private:
    Ui::dialogOauthSetup *ui;
    bool dialogShown;


};

#endif // DIALOGOAUTHSETUP_H
