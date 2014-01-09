#include "twitchermainwindow.h"
#include "ui_twitchermainwindow.h"


twitcherMainWindow::twitcherMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::twitcherMainWindow)
{

    ui->setupUi(this);

    startup = true;

    // delete logfile if clear log on startup setting is true
    if (genericHelper::getClearLogOnStartup() == true) {
        genericHelper::deleteLog();
    }

    // hide (empty) toolbar
    this->ui->mainToolBar->hide();

    genericHelper::log("twitcher app started.");

    errorCount = 0;

    updateInterval = 12000;

    // ignore update intervals <9 seconds
    if (genericHelper::getUpdateInterval() >= 9) {
        updateInterval = genericHelper::getUpdateInterval() * 1000;
    }



    // create nativeEventFilter, register Hotkey and install eventFilter on QApp instance
    twitcherKeyEvent = new TwitcherNativeEventFilter();
    this->registerHotkey();
    QObject::connect(twitcherKeyEvent, SIGNAL(hotkeyPressed()), this, SLOT(toForground()));


    // init sub dialogs
    diaOauthSetup = new dialogOauthSetup(this);
    diaAbout = new dialogAbout(this);
    diaOBSSettings = new dialogOBSSettings(this);
    diaGameStats = new dialogGameStats(this);
    diaSettings = new dialogSettings(this);


    // init QPixmaps for ok, not ok icons
    nok = QPixmap(":images/nok.png");
    ok = QPixmap(":images/ok.png");
    game = QPixmap(":images/noimg.png");


    // some stuff needed for the tray icon
    createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();


    uc = new updateCheck(this);

    if (genericHelper::getCheckUpdate() == true) {
        uc->getCheck();
    }



    QObject::connect(uc, SIGNAL(updateReady(const QString)), this, SLOT(updateNotify(const QString)));

    // init twitch api object
    tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());

    currentGame = "";
    topGame = "";
    currentGameCount = 0;
    topGameCount = 0;

    // init the image loader
    imgl = new imageLoader(this);

    currentGameImage = QUrl();

    // update all form fields and labels
    this->updateFormFields();


    // connect the twitchReady signal of the twitchapi object to the updateFromJsonResponse slot of this form
    QObject::connect(tw, SIGNAL(twitchReady(const QJsonDocument)), this, SLOT(updateFromJsonResponse(const QJsonDocument)));

    // connect the twitchGameReady signal of the twitchapi object to the updateGameStatsFromJsonResponse slot from the game stats form
    QObject::connect(tw, SIGNAL(twitchGameReady(const QJsonObject)), diaGameStats, SLOT(updateGameStatsFromJsonResponse(const QJsonObject)));

    // connect the networkError signal of the twitchapi to the errorPopup slot of this form
    QObject::connect(tw, SIGNAL(networkError(QString)), this, SLOT(errorPopup(QString)));

    // connect the valueChanged signal of the oauth-setup subform to the updateFormFieldsSlot of this form
    QObject::connect(diaOauthSetup, SIGNAL(valueChanged()), this, SLOT(updateFormFieldsSlot()));

    // connect the valueChanged signal of the settings  subform to the updateFormFieldsSlot of this form
    QObject::connect(diaSettings, SIGNAL(valueChanged()), this, SLOT(updateFormFieldsSlot()));

    // connect the valueChanged signal of the obs settings subform to the updateFormFieldsSlot of this form
    QObject::connect(diaOBSSettings, SIGNAL(valueChanged()), this, SLOT(updateFormFieldsSlot()));

    // connect the image downloader downloaded signal to the loadGameImage slot of this form
    QObject::connect(imgl, SIGNAL(downloaded()), this, SLOT(loadGameImage()));

    // init game title completer, and enable it on the game title lineEdit
    completerModel = new QStringListModel();

    completer = new QCompleter(completerModel, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    this->ui->lineEditGameTitle->setCompleter(completer);


    // init qtimer, set update interval and connect the timeout singal of qtimer to the updateFormFieldsSlot of this form
    refreshTimer = new QTimer(this);
    refreshTimer->setInterval(updateInterval);
    QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(updateFormFieldsSlot()));
    refreshTimer->start(updateInterval);

    found = false;



}

twitcherMainWindow::~twitcherMainWindow()
{

    delete ui;
    delete trayIcon;
    delete trayIconMenu;
    delete open;
    delete close;
}

void twitcherMainWindow::createActions()
{
    open = new QAction(tr("&Open"), this);
    connect(open, SIGNAL(triggered()), this, SLOT(show()));


    close = new QAction(tr("&Quit"), this);
    connect(close, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void twitcherMainWindow::loadGameImage() {




      game.loadFromData(imgl->downloadedData());

      genericHelper::log("downloaded image, dimension:" + QString::number(game.size().height()) + "x" +QString::number(game.size().width()));

      if (game.size().height() > 0) {
        this->ui->labelGameImage->setPixmap(game.scaled(32,32));
      } else {
          genericHelper::log("height is 0, image not set.");
      }



}

void twitcherMainWindow::registerHotkey()
{



    bool hotKeyRegistered = false;

    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
    if(QSysInfo::windowsVersion() > QSysInfo::WV_VISTA)
    {
        hotKeyRegistered = ::RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, VK_F12);
    }
    else
    {
        // No repeat is only supported in 7 and later
        hotKeyRegistered = ::RegisterHotKey(NULL, 1, MOD_ALT, VK_F12);
    }

    if (hotKeyRegistered == false) {
        genericHelper::log("failed to register global hotkey Alt+F12");
    } else {
        genericHelper::log("successfully registered global hotkey Alt+F12");
    }

    qApp->instance()->installNativeEventFilter(twitcherKeyEvent);


}

void twitcherMainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);


    trayIconMenu->addAction(open);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(close);


    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);


    connect(
            trayIcon,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
           );
}

void twitcherMainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        this->show();
}

void twitcherMainWindow::setIcon(QString iconname)
{
    trayIcon->setIcon(QIcon(":/images/"+iconname+".png"));
}

void twitcherMainWindow::on_actionClose_to_Tray_triggered()
{
    this->hide();
}

void twitcherMainWindow::closeEvent(QCloseEvent *event)
{

    // save geometry
    //settings->setValue("geometry", saveGeometry());
    //settings->setValue("windowstate", saveState());

    if (trayIcon->isVisible()) {
        trayIcon->showMessage(tr("Twitcher closed to tray."),
        tr(""));
        hide();


        event->ignore(); // Don't let the event propagate to the base class
    }


}

void twitcherMainWindow::toForground() {

    this->showMinimized();
    this->setWindowState(Qt::WindowActive);
    this->showNormal();
}

void twitcherMainWindow::updateFormFields() {

    if (genericHelper::getLoadGameImages() == false) {
        this->ui->labelGameImage->hide();
    } else {
        this->ui->labelGameImage->show();
    }


    this->ui->lineEditUsername->setText(genericHelper::getUsername());


    // if username/twitch setup is done and error count < 5, continue
    if ((genericHelper::getUsername().length() > 1) && errorCount <= 5){


        // set current game while typing
        if (this->ui->lineEditGameTitle->text().length() > 2)
        {
            currentGame = this->ui->lineEditGameTitle->text();
        }

        currentGameCount = 0;
        topGameCount = 0;

        tw->getStream(genericHelper::getUsername());

        tw->getChannel(genericHelper::getUsername());

        tw->getGame();


        following.clear();

        tw->getFollows(genericHelper::getUsername());

        if (this->ui->lineEditUsername->text() == "")
        {
            this->ui->pushButtonApplyTitles->setEnabled(false);
            this->ui->pushButtonClearTitlesOnTwtich->setEnabled(false);

        } else {
            this->ui->pushButtonApplyTitles->setEnabled(true);
            this->ui->pushButtonClearTitlesOnTwtich->setEnabled(true);

        }

        this->ui->labelBroadcastTitleStatus->setPixmap(ok);
        this->ui->labelGameTitleStatus->setPixmap(ok);




    } else {
        if (genericHelper::getEnoughTimeSinceLastErrorElapsed() == true)
        {
            errorCount = 0;
        }
    }
}

void twitcherMainWindow::updateFormFieldsSlot() {


    this->updateFormFields();
}

void twitcherMainWindow::errorPopup(QString message) {
    genericHelper::log("error: - "+message);
    errorCount += 1;
    if (errorCount <= 5) {
         genericHelper::setErrorTimestamp();
    }
    trayIcon->showMessage(tr("twitcher error"),
    message);

    //int ret = QMessageBox::warning(this, tr("twitcher"),
      //                              message,
        //                            QMessageBox::Ok
          //                          );
}



void twitcherMainWindow::updateNotify(const QString &latestVersion) {

    QString latestVersionNumber = latestVersion;

    latestVersionNumber.insert(2,".");

    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, tr("twitcher"),
                                    "New version ("+latestVersionNumber+") available, do you want to update?",
                                    QMessageBox::Yes|QMessageBox::No);


    if (reply == QMessageBox::Yes) {

       QDesktopServices::openUrl(QUrl("http://twitcher.abyle.org/downloads/twitcher-"+latestVersionNumber+"-x64-install.exe"));

      } else {
        genericHelper::log("update dismissed.");
      }

}

// this is dirty, I know...
void twitcherMainWindow::updateFromJsonResponse(const QJsonDocument &jsonResponseBuffer) {



    if (errorCount <= 5) {


    this->ui->lineEditLastRefresh->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    found = false;


    //this->ui->tableWidgetJsonResponse->clear();

    QJsonObject jsonObject = jsonResponseBuffer.object();



    // if answer for game search lookup
    if ((!jsonObject["games"].isNull()) && (found == false)) {



            found = true;

            completerList.clear();

            for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter) {
                 if (iter.key() == "games")
                 {

                    for (int i = 0; i <= iter.value().toArray().size(); i++)
                    {

                        QJsonValue _val = iter.value().toArray().at(i);
                        completerList.append(_val.toObject()["name"].toString());
                        //qDebug() << _val.toObject()["name"].toString();

//                         QUrl imageUrl("http://static-cdn.jtvnw.net/ttv-logoart/Diablo%20III-60x36.jpg");

                           if (!_val.toObject()["box"].toObject()["small"].isNull()) {
//                                qDebug() << _val.toObject()["box"].toObject()["small"];
                                //this->currentGameImage = ;


                                if (genericHelper::getLoadGameImages() == true) {
                                    imgl->download(QUrl(_val.toObject()["box"].toObject()["small"].toString()));
                                }
                            }
                    }

                 }

             }

             completerModel->setStringList(completerList);

    // if answer for (get) stream
    } else if ((!jsonObject["stream"].isNull()) && (found == false)){

                found = true;

                for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter) {
                    if (iter.key() == "stream")
                    {



                        this->ui->lineEditChannelViews->setText(QString::number(iter.value().toObject()["channel"].toObject()["views"].toDouble()));
                        this->ui->lineEditViewers->setText(QString::number(iter.value().toObject()["viewers"].toDouble()));


                    }
                }


     } else if ((!jsonObject["follows"].isNull())  && (found == false)){

        found = true;

        for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter) {
            if (iter.key() == "follows")
            {


                for (int i = 0; i <= iter.value().toArray().size(); i++)
                {

                       QJsonValue _val = iter.value().toArray().at(i);
                       if (_val.toObject()["channel"].toObject()["name"].toString() != "")
                       {
                           following.append(_val.toObject()["channel"].toObject()["name"].toString());
                       }

                }

            }

        }

        genericHelper::setFollows(following);

     } else if ((!jsonObject["top"].isNull()) && (found == false)){

        found = true;

        for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter) {
            if (iter.key() == "top")
            {




                for (int i = 0; i <= iter.value().toArray().size(); i++)
                {
                    if (i == 0) {
                        //qDebug() << iter.value().toArray().at(i).toObject()["game"].toObject()["name"];
                        topGame = iter.value().toArray().at(i).toObject()["game"].toObject()["name"].toString();
                        topGameCount = iter.value().toArray().at(i).toObject()["viewers"].toDouble();
                    }

                    if (iter.value().toArray().at(i).toObject()["game"].toObject()["name"] == currentGame) {
                        currentGameCount = iter.value().toArray().at(i).toObject()["viewers"].toDouble();
                    }

                    //qDebug() << iter.value().toArray().at(i).toObject()["game"].toObject()["name"];

                }
                //this->ui->lineEditChannelViews->setText(QString::number(iter.value().toObject()["channel"].toObject()["views"].toDouble()));
                //this->ui->lineEditViewers->setText(QString::number(iter.value().toObject()["viewers"].toDouble()));


            }
        }


        this->ui->lineEditViewersTop->setText(QString::number(topGameCount));

        if (this->ui->lineEditGameTitle->text() != "") {
            this->ui->lineEditViewersGame->setText(QString::number(currentGameCount));
        } else {
            this->ui->lineEditViewersGame->setText(QString::number(0));
        }
        this->ui->lineEditViewersTop->setToolTip(topGame);

    } else if ((!jsonObject["status"].isNull()) && (found == false)){

        found = true;

        if (this->ui->plainTextEditBroadcastTitle->toPlainText() != jsonObject["status"].toString()) {
            this->ui->plainTextEditBroadcastTitle->blockSignals(true);
            this->ui->plainTextEditBroadcastTitle->setPlainText(jsonObject["status"].toString());
            this->ui->plainTextEditBroadcastTitle->blockSignals(false);
        }




        if (this->ui->lineEditGameTitle->text() != jsonObject["game"].toString()) {
           this->ui->lineEditGameTitle->setText(jsonObject["game"].toString());
        }

        if ((startup == true) && (this->ui->lineEditGameTitle->text().length() > 2)) {
                startup = false;
              tw->searchGames(this->ui->lineEditGameTitle->text());
        }





    }


    //int i=0;
    //this->ui->tableWidgetJsonResponse->setColumnCount(2);
    //foreach (const QVariant & key, jsonObject.keys())
    //{
    //        i++;

            //this->ui->tableWidgetJsonResponse->setRowCount(this->ui->tableWidgetJsonResponse->rowCount()+1);
            //this->ui->tableWidgetJsonResponse->setItem( this->ui->tableWidgetJsonResponse->rowCount()-1, 0, new QTableWidgetItem( key.toString() ) );
            //this->ui->tableWidgetJsonResponse->setItem( this->ui->tableWidgetJsonResponse->rowCount()-1, 1, new QTableWidgetItem( jsonObject[key.toString()].toString() ) );

            //qDebug() << i << " " << key.toString() << " " << jsonObject[key.toString()].toString();
    //}

  } else {

        genericHelper::log("5 consecutively errors occurred, stopping updates.");

        trayIcon->showMessage(tr("twitcher error"),
        tr("5 consecutively errors occurred, stopping updates."));



    }
}

void twitcherMainWindow::on_actionSetup_Twitch_Auth_triggered()
{
    if (diaOauthSetup->getDialogShown() == true)
    {
        diaOauthSetup->close();

        diaOauthSetup->show();

    } else {


        diaOauthSetup->show();
        diaOauthSetup->setDialogShown();
    }
}



void twitcherMainWindow::on_lineEditGameTitle_textChanged(const QString &arg1)
{


}

void twitcherMainWindow::on_actionRefresh_triggered()
{
     this->updateFormFields();
}

void twitcherMainWindow::on_actionAbout_triggered()
{
    if (diaAbout->getDialogShown() == true)
    {
        diaAbout->close();

        diaAbout->show();

    } else {


        diaAbout->show();
        diaAbout->setDialogShown();
    }
}

void twitcherMainWindow::on_pushButtonClearTitlesOnTwtich_clicked()
{



    tw->setOAuthAccessToken(genericHelper::getOAuthAccessToken());

    apiPutParams.insert("channel[status]", "");
    apiPutParams.insert("channel[game]", "");

    this->ui->plainTextEditBroadcastTitle->blockSignals(true);
    this->ui->plainTextEditBroadcastTitle->setPlainText("");
    this->ui->plainTextEditBroadcastTitle->blockSignals(false);
    this->ui->lineEditGameTitle->setText("");


    tw->setStatusAndGameTitle(genericHelper::getUsername(), apiPutParams);


    game = QPixmap(":images/noimg.png");

    this->ui->labelGameImage->setPixmap( game );
}



void twitcherMainWindow::on_actionOBS_Settings_triggered()
{
    if (diaOBSSettings->getDialogShown() == true)
    {
        diaOBSSettings->close();

        diaOBSSettings->show();

    } else {


        diaOBSSettings->show();
        diaOBSSettings->setDialogShown();
    }
}

void twitcherMainWindow::on_pushButtonLaunchOBS_clicked()
{
    if (genericHelper::getOBSBin().length() <= 1)
    {
        int ret = QMessageBox::information(this, tr("twitcher"),
                                        tr("Configure your OBS settings first."),
                                        QMessageBox::Ok
                                        );
    }
    genericHelper::executeOBS();
}

void twitcherMainWindow::on_pushButtonLaunchHexchat_clicked()
{

    genericHelper::executeAddonHexchat(genericHelper::getFollows());

}

void twitcherMainWindow::on_actionApplications_twitch_tv_triggered()
{
    QString link = "http://www.twitch.tv/settings/applications";
    QDesktopServices::openUrl(QUrl(link));
}

void twitcherMainWindow::on_actionDashboard_twitch_tv_triggered()
{
    if (genericHelper::getUsername().length() <= 1)
    {
        int ret = QMessageBox::information(this, tr("twitcher"),
                                        tr("Configure twitch auth settings first."),
                                        QMessageBox::Ok
                                        );
    } else {

        QString link = "http://www.twitch.tv/"+genericHelper::getUsername()+"/dashboard";
        QDesktopServices::openUrl(QUrl(link));
    }
}

void twitcherMainWindow::on_pushButtonApplyTitles_clicked()
{




    tw->setOAuthAccessToken(genericHelper::getOAuthAccessToken());

    apiPutParams.insert("channel[status]", this->ui->plainTextEditBroadcastTitle->toPlainText());
    apiPutParams.insert("channel[game]", this->ui->lineEditGameTitle->text());

    tw->setStatusAndGameTitle(genericHelper::getUsername(), apiPutParams);

    this->updateFormFields();




    refreshTimer->start(updateInterval);


    

}

void twitcherMainWindow::on_pushButtonCancel_clicked()
{


   

    this->updateFormFields();


    refreshTimer->start(updateInterval);

}



void twitcherMainWindow::on_actionGame_stats_triggered()
{
    if (diaGameStats->getDialogShown() == true)
    {
        diaGameStats->close();

        diaGameStats->show();

    } else {


        diaGameStats->show();
        diaGameStats->setDialogShown();
    }
}

void twitcherMainWindow::on_actionLog_triggered()
{
    genericHelper::openLogWithNotepad();
}



void twitcherMainWindow::on_lineEditGameTitle_textEdited(const QString &arg1)
{
    refreshTimer->stop();
    if (arg1.length() > 1)
    {
     tw->searchGames(arg1);
    }

    this->ui->labelGameTitleStatus->setPixmap(nok);

}

void twitcherMainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void twitcherMainWindow::on_actionSettings_triggered()
{
    if (diaSettings->getDialogShown() == true)
    {
        diaSettings->close();

        diaSettings->show();

    } else {


        diaSettings->show();
        diaSettings->setDialogShown();
    }
}

void twitcherMainWindow::on_plainTextEditBroadcastTitle_textChanged()
{
    refreshTimer->stop();




    this->ui->labelBroadcastTitleStatus->setPixmap(nok);
}

void twitcherMainWindow::on_lineEditGameTitle_editingFinished()
{

}

void twitcherMainWindow::on_actionReport_Issue_google_triggered()
{


    QString link = "http://code.google.com/p/twitcher/issues/entry";
    QDesktopServices::openUrl(QUrl(link));

}

void twitcherMainWindow::on_actionReport_Issue_email_triggered()
{
   QDesktopServices::openUrl(QUrl("mailto:sq+twitcher@abyle.org?subject=twitcher-issue"));
}
