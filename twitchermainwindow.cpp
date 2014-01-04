#include "twitchermainwindow.h"
#include "ui_twitchermainwindow.h"


twitcherMainWindow::twitcherMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::twitcherMainWindow)
{
    ui->setupUi(this);

    genericHelper::log("twitcher app started.");

    errorCount = 0;

    updateInterval = 7000;

    if (genericHelper::getUpdateInterval() >= 7) {
        updateInterval = genericHelper::getUpdateInterval() * 1000;
    }


   // QShortcut *shortcut_tofg = new QShortcut(QKeySequence("Ctrl+Alt+F12"), this);
   // shortcut_tofg->setContext(Qt::ApplicationShortcut);
   // QObject::connect(shortcut_tofg, SIGNAL(activated()), this, SLOT(toForground()));

    // create nativeEventFilter, register Hotkey and install eventFilter on QApp instance
    twitcherKeyEvent = new TwitcherNativeEventFilter();



    this->registerHotkey();
    QObject::connect(twitcherKeyEvent, SIGNAL(hotkeyPressed()), this, SLOT(toForground()));


    diaOauthSetup = new dialogOauthSetup(this);
    diaAbout = new dialogAbout(this);
    diaOBSSettings = new dialogOBSSettings(this);
    diaGameStats = new dialogGameStats(this);
    diaSettings = new dialogSettings(this);

    nok = QPixmap(":images/nok.png");
    ok = QPixmap(":images/ok.png");

    // some stuff needed for the tray icon
    createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();


    tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());

    currentGame = "";
    topGame = "";
    currentGameCount = 0;
    topGameCount = 0;

     this->updateFormFields();

    QObject::connect(tw, SIGNAL(twitchReady(const QJsonDocument)), this, SLOT(updateFromJsonResponse(const QJsonDocument)));

    QObject::connect(tw, SIGNAL(twitchGameReady(const QJsonObject)), diaGameStats, SLOT(updateGameStatsFromJsonResponse(const QJsonObject)));

    QObject::connect(tw, SIGNAL(networkError(QString)), this, SLOT(errorPopup(QString)));

    QObject::connect(diaOauthSetup, SIGNAL(valueChanged()), this, SLOT(updateFormFieldsSlot()));

    //this->ui->tableWidgetJsonResponse->setVisible(false);



    completerModel = new QStringListModel();

    completer = new QCompleter(completerModel, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    this->ui->lineEditGameTitle->setCompleter(completer);


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
    this->ui->lineEditUsername->setText(genericHelper::getUsername());



    this->ui->labelBroadcastTitleStatus->setPixmap(ok);
    this->ui->labelGameTitleStatus->setPixmap(ok);

    if ((genericHelper::getUsername().length() > 1) && errorCount <= 5){





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

        if (this->ui->lineEditBroadcastTitle->text() != jsonObject["status"].toString()) {
            this->ui->lineEditBroadcastTitle->setText(jsonObject["status"].toString());
        }

        if (this->ui->lineEditGameTitle->text() != jsonObject["game"].toString()) {
            this->ui->lineEditGameTitle->setText(jsonObject["game"].toString());
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

    this->ui->lineEditBroadcastTitle->setText("");
    this->ui->lineEditGameTitle->setText("");


    tw->setStatusAndGameTitle("https://api.twitch.tv/kraken/channels/"+genericHelper::getUsername(), apiPutParams);
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

    apiPutParams.insert("channel[status]", this->ui->lineEditBroadcastTitle->text());
    apiPutParams.insert("channel[game]", this->ui->lineEditGameTitle->text());

    tw->setStatusAndGameTitle("https://api.twitch.tv/kraken/channels/"+genericHelper::getUsername(), apiPutParams);

    //this->updateFormFields();
    refreshTimer->start(updateInterval);

    this->ui->lineEditBroadcastTitle->setStyleSheet("QLineEdit{background-color: white; alternate-background-color: white; selection-background-color: white;}");
    this->ui->lineEditGameTitle->setStyleSheet("QLineEdit{background-color: white; alternate-background-color: white; selection-background-color: white;}");

}

void twitcherMainWindow::on_pushButtonCancel_clicked()
{


    this->ui->lineEditBroadcastTitle->setStyleSheet("QLineEdit{background-color: white; alternate-background-color: white; selection-background-color: white;}");
    this->ui->lineEditGameTitle->setStyleSheet("QLineEdit{background-color: white; alternate-background-color: white; selection-background-color: white;}");

    this->updateFormFields();


    refreshTimer->start(updateInterval);

}

void twitcherMainWindow::on_lineEditBroadcastTitle_textChanged(const QString &arg1)
{


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

void twitcherMainWindow::on_lineEditBroadcastTitle_textEdited(const QString &arg1)
{
    refreshTimer->stop();




    this->ui->labelBroadcastTitleStatus->setPixmap(nok);

}

void twitcherMainWindow::on_lineEditGameTitle_textEdited(const QString &arg1)
{
    refreshTimer->stop();
    if (arg1.length() > 1)
    {
     tw->searchGames(arg1);
    }

    this->ui->labelBroadcastTitleStatus->setPixmap(nok);

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
