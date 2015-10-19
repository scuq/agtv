#include "tpmainwindow.h"
#include "ui_tpmainwindow.h"


tpMainWindow::tpMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tpMainWindow)
{
    ui->setupUi(this);

    currArch = CURRARCH;


    // delete logfile if clear log on startup setting is true
    if (genericHelper::getClearLogOnStartup() == true) {
        genericHelper::deleteLog();
    }


    restoreGeometry(genericHelper::getGeometry("main").toByteArray());
    restoreState(genericHelper::getWindowstate("main").toByteArray());

    this->ui->tabWidget->setCurrentIndex(genericHelper::getSelectedTab());






    uc = new updateCheck(this);




    xOffset = 0;
    yOffset = 0;

    launchBookmarkEnabled = true;

    version = VERSION;


    updateInterval = 12000;



    // ignore update intervals <9 seconds
    if (genericHelper::getUpdateInterval() >= 9) {
        updateInterval = genericHelper::getUpdateInterval() * 1000;
    }

    this->ui->mainToolBar->hide();


    stmodel = new QStandardItemModel(0,5,this);
    stproxymodel = new AdvQSortFilterProxyModel(this);

    stmodelbookmarks = new QStandardItemModel(0,5,this);
    stproxymodelbookmarks = new AdvQSortFilterProxyModel(this);




    stproxymodel->setSourceModel(stmodel);

    QStringList horzHeaders;

    horzHeaders << "Name" << "Status" << "#V" << "Game" << "Status Message";

    stmodel->setHorizontalHeaderLabels(horzHeaders);




    stproxymodelbookmarks->setSourceModel(stmodelbookmarks);






    stmodelbookmarks->setHorizontalHeaderLabels(horzHeaders);

    this->ui->tableView->horizontalHeader()->show();
    this->ui->tableViewBookmarks->horizontalHeader()->show();



    this->ui->tableView->setModel(stproxymodel);
    this->ui->tableViewBookmarks->setModel(stproxymodelbookmarks);


    diaOauthSetup = new dialogOauthSetup(this);
    diaPositioner = new DialogPositioner(this);
    diaLaunch = new DialogLaunch(this);
    diaOptions = new DialogOptions(this);




    // init twitch api object
    tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());

    QObject::connect(tw, SIGNAL(twitchReadyChannelAccessToken(const QJsonDocument)), this, SLOT(onChannelAccessTokenReady(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyFollows(const QJsonDocument)), this, SLOT(updateFromJsonResponseFollows(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyBookmark(const QJsonDocument)), this, SLOT(updateFromJsonResponseBookmark(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyStream(const QJsonDocument)), this, SLOT(updateFromJsonResponseStream(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyChannel(const QJsonDocument)), this, SLOT(updateFromJsonResponseChannel(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyHost(const QJsonDocument)), this, SLOT(updateFromJsonResponseHost(const QJsonDocument)));
    QObject::connect(this, SIGNAL(setStreamTitle(QString,QString)), diaLaunch, SLOT(setStreamTitle(QString,QString)));
    QObject::connect(this, SIGNAL(setStreamUrl(QString)), diaLaunch, SLOT(setStreamUrl(QString)));
    QObject::connect(diaLaunch, SIGNAL(startStreamPlay(QString, QString, QString, int, int, int , int, bool, QString)), this, SLOT(executePlayer(QString, QString, QString, int, int, int , int, bool, QString)));

    QObject::connect(diaOauthSetup, SIGNAL(twitchAuthSetupChanged(bool)), this, SLOT(on_SwitchInputEnabled(bool)));


    QObject::connect(this->ui->lineEditFilter, SIGNAL(textChanged(QString)), this->stproxymodel, SLOT(setFilterRegExp(QString)));
    QObject::connect(this->ui->lineEditFilterBookmark, SIGNAL(textChanged(QString)), this->stproxymodelbookmarks, SLOT(setFilterRegExp(QString)));

    QObject::connect(uc, SIGNAL(updateReady(const QString)), this, SLOT(on_updateNotify(const QString)));

    if (genericHelper::getCheckUpdate() == true) {
        uc->getCheck();
    }

    this->loadData();


    // init QPixmaps for ok, not ok icons
    //offline = QPixmap(":images/offline.png");
    //online = QPixmap(":images/online.png");

    desktop = QApplication::desktop();

    genericHelper::setPrimaryScreenWidth(desktop->screenGeometry(-1).width());

    // init qtimer, set update interval and connect the timeout singal of qtimer to the updateFormFieldsSlot of this form
    refreshTimer = new QTimer(this);
    refreshTimer->setInterval(updateInterval);
    QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(on_loadData()));
    refreshTimer->start(updateInterval);



    // some stuff needed for the tray icon
    createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();

    showOfflineStreamers = true;

    if (genericHelper::getUsername() == "") {
        this->disableInput();
    }


    if (genericHelper::getShowOfflineStreamers() == true) {
        this->stproxymodel->setShowOffline(true);
        this->stproxymodelbookmarks->setShowOffline(true);
        this->ui->actionShow_Offline_Streamers->setChecked(true);


    } else {
        this->stproxymodel->setShowOffline(false);
        this->stproxymodelbookmarks->setShowOffline(false);
        this->ui->actionShow_Offline_Streamers->setChecked(false);

    }


}

tpMainWindow::~tpMainWindow()
{
    delete ui;
}




void tpMainWindow::disableInput()
{

    refreshTimer->stop();

    genericHelper::setClearLogOnStartup(true);
    genericHelper::setCheckUpdate(true);



    QList<QWidget*> widgets = this->findChildren<QWidget*>();


                foreach (QWidget *widget, widgets)
                {

                    if (
                    (widget->objectName() != "mainToolBar") &&
                    (widget->objectName() != "menuBar") &&
                    (widget->objectName() != "menuFile") &&
                    (widget->objectName() != "pushButtonAuthorizeOnTwitch") &&
                    (widget->objectName() != "lineEditOAuthToken") &&
                    (widget->objectName() != "pushButtonTestOAuth") &&
                    (widget->objectName() != "pushButtonREvoke") &&
                    (widget->objectName() != "pushButtonSave") &&
                    (widget->objectName() != "dialogOauthSetup")
                    )
                    {

                    widget->setEnabled(false);
                    }
                }
}

void tpMainWindow::enableInput()
{
    QList<QWidget*> widgets = this->findChildren<QWidget*>();


                foreach (QWidget *widget, widgets)
                {

                    if (
                    (widget->objectName() != "mainToolBar") &&
                    (widget->objectName() != "menuBar") &&
                    (widget->objectName() != "menuFile") &&
                    (widget->objectName() != "pushButtonAuthorizeOnTwitch") &&
                    (widget->objectName() != "lineEditOAuthToken") &&
                    (widget->objectName() != "pushButtonTestOAuth") &&
                    (widget->objectName() != "pushButtonREvoke") &&
                    (widget->objectName() != "pushButtonSave") &&
                    (widget->objectName() != "dialogOauthSetup")
                    )
                    {

                    widget->setEnabled(true);
                    }
                }
}

void tpMainWindow::loadBookmarks()
{
    QStringList loadedbookmarks;

    loadedbookmarks = genericHelper::getBookmarks();


    QStringList currentbookmarks;




      for( int row = 0; row < this->stmodelbookmarks->rowCount(); ++row )
      {
        currentbookmarks << this->stmodelbookmarks->item( row, 0 )->text();

      }




     QListIterator<QString> itr (loadedbookmarks);
    int i = 0;
     while (itr.hasNext()) {
           QString current = itr.next();

           if (currentbookmarks.count(current) <= 0) {



               tw->getStream(current);
               tw->getChannel(current);

                if (this->stmodelbookmarks->findItems(current,Qt::MatchExactly,0).length() <= 0) {


                   QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(current));
                   stmodelbookmarks->setItem(i, 0, qsitem0);
                   QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg("offline"));
                   stmodelbookmarks->setItem(i, 1, qsitem1);
                   QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(""));
                   stmodelbookmarks->setItem(i, 2, qsitem2);
                   QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(""));
                   stmodelbookmarks->setItem(i, 3, qsitem3);
                   QStandardItem *qsitem4 = new QStandardItem(QString("%0").arg(""));
                   stmodelbookmarks->setItem(i, 4, qsitem4);



                }
           }

           tw->getBookmarkStatus(current);
           ++i;
     }
}

void tpMainWindow::saveBookmarks()
{
    QStringList currentbookmarks;

    //QTreeWidgetItemIterator it(this->ui->treeWidgetBookmarks);
    //while (*it) {
     //   if ((*it)->text(0) != "")
     //       currentbookmarks << (*it)->text(0);
     //   ++it;
    //}

    for( int row = 0; row < this->stmodelbookmarks->rowCount(); ++row )
    {
      currentbookmarks << this->stmodelbookmarks->item( row, 0 )->text();

    }

    genericHelper::setBookmarks(currentbookmarks);
}

void tpMainWindow::enableDelete()
{
    //this->ui->pushButtonDeleteBookmark->setEnabled(true);
    //this->ui->pushButtonDeleteBookmarkCancel->setEnabled(true);
    //this->ui->lineEditDeleteBookmark->setEnabled(true);
    //this->ui->lineEditNewBookmark->setEnabled(false);
    //this->ui->pushButtonAddBookmark->setEnabled(false);
    launchBookmarkEnabled = false;
}

void tpMainWindow::disableDelete()
{
    //this->ui->pushButtonDeleteBookmark->setEnabled(false);
    //this->ui->pushButtonDeleteBookmarkCancel->setEnabled(false);
    //this->ui->lineEditDeleteBookmark->setEnabled(false);
    //this->ui->lineEditNewBookmark->setEnabled(true);
    //this->ui->pushButtonAddBookmark->setEnabled(true);
    launchBookmarkEnabled = true;
}

void tpMainWindow::loadData()
{
    //this->ui->treeWidget->clear();
    //this->ui->treeWidgetBookmarks->clear();
    tw->getFollows(genericHelper::getUsername());
    this->loadBookmarks();
}

void tpMainWindow::closeEvent(QCloseEvent *event)
{


    if (genericHelper::getCloseToTray() == true) {

    if (trayIcon->isVisible()) {
        trayIcon->showMessage(genericHelper::getAppName()+" closed to tray.",
        tr(""));
        hide();


        event->ignore(); // Don't let the event propagate to the base class
    }
    } else {
        this->myQuit();
    }
}



void tpMainWindow::createActions()
{

    //tray
    open = new QAction(tr("&Open"), this);
    connect(open, SIGNAL(triggered()), this, SLOT(show()));


    close = new QAction(tr("&Quit"), this);
    connect(close, SIGNAL(triggered()), this, SLOT(myQuit()));

    open_in_browser = new QAction(tr("&Open in Browser"), this);
    connect(open_in_browser, SIGNAL(triggered()), this, SLOT(openStreamBrowser()));

    open_in_hexchat = new QAction(tr("Open in &HexChat"), this);
    connect(open_in_hexchat, SIGNAL(triggered()), this, SLOT(openChatHexChat()));

    open_in_hexchat_bookmark = new QAction(tr("Open in &HexChat"), this);
    connect(open_in_hexchat_bookmark, SIGNAL(triggered()), this, SLOT(openChatHexChatBookmark()));

    open_in_browser_bookmark = new QAction(tr("&Open in Browser"), this);
    connect(open_in_browser_bookmark, SIGNAL(triggered()), this, SLOT(openStreamBrowserBookmark()));

    add_hosted_bookmark = new QAction(tr("&Add Hosted to Bookmarks"), this);
    connect(add_hosted_bookmark, SIGNAL(triggered()), this, SLOT(addBookmarkHosted()));

    delete_bookmark = new QAction(tr("&Remove"), this);
    connect(delete_bookmark, SIGNAL(triggered()), this, SLOT(deleteBookmark()));

    add_bookmark = new QAction(tr("&Add"), this);
    connect(add_bookmark, SIGNAL(triggered()), this, SLOT(addBookmark()));


}


void tpMainWindow::myQuit()
{

    genericHelper::saveGeometry("main",saveGeometry());
    genericHelper::saveWindowstate("main",saveState());

    for( int i=0; i<this->playerThreads.count(); ++i )
    {
       this->playerThreads.at(i)->terminate();
    }
    //while (i.hasNext()) {
      //  i.next()->terminate();
   // }





    qApp->quit();

}

void tpMainWindow::on_loadData()
{
    this->loadData();
}

void tpMainWindow::openStreamBrowser()
{

    QString link = "http://www.twitch.tv/"+this->ui->tableView->selectionModel()->selectedRows(0).at(0).data().toString()+"/";
    QDesktopServices::openUrl(QUrl(link));
}

void tpMainWindow::openStreamBrowserBookmark()
{



    QString link = "http://www.twitch.tv/"+this->ui->tableViewBookmarks->selectionModel()->selectedRows(0).at(0).data().toString()+"/";
    QDesktopServices::openUrl(QUrl(link));
}

void tpMainWindow::openChatHexChat()
{
     genericHelper::executeAddonHexchat(this->ui->tableView->selectionModel()->selectedRows(0).at(0).data().toStringList());
}
void tpMainWindow::openChatHexChatBookmark()
{
    genericHelper::executeAddonHexchat(this->ui->tableViewBookmarks->selectionModel()->selectedRows(0).at(0).data().toStringList());

}

void tpMainWindow::addBookmarkHosted()
{

       QString hostedfor;
       hostedfor = this->ui->tableView->selectionModel()->selectedRows(4).at(0).data().toString();

       if (genericHelper::getBookmarks().count(hostedfor) <= 0) {
           genericHelper::addBookmark(hostedfor);
           this->loadBookmarks();
       }




       this->ui->tabWidget->setCurrentIndex(1);
       //this->ui->lineEditFilterBookmark->setText(hostedfor);

}

void tpMainWindow::deleteBookmark()
{

    refreshTimer->stop();

    //get selections
    QItemSelection selection = this->ui->tableViewBookmarks->selectionModel()->selection();

    //find out selected rows
    QList<int> removeRows;
    foreach(QModelIndex index, selection.indexes()) {
        if(!removeRows.contains(index.row())) {
            removeRows.append(index.row());
        }
    }



    //loop through all selected rows
    for(int i=0;i<removeRows.count();++i)
    {
        //decrement all rows after the current - as the row-number will change if we remove the current
        for(int j=i;j<removeRows.count();++j) {
            if(removeRows.at(j) > removeRows.at(i)) {
                removeRows[j]--;
            }
        }
        //remove the selected row
        this->stproxymodelbookmarks->removeRows(removeRows.at(i), 1);
    }

   //this->disableDelete();

   this->saveBookmarks();
   this->loadBookmarks();


   refreshTimer->start(updateInterval);

}

void tpMainWindow::addBookmark()
{
    QString text = QInputDialog::getText(this, tr("Add Bookmark"), tr("Channel/Streamer name"), QLineEdit::Normal,"");
    if (!text.isEmpty()) {
        genericHelper::addBookmark(text);
        this->loadBookmarks();
    }
}

void tpMainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(open);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(close);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void tpMainWindow::setIcon(QString iconname)
{
        trayIcon->setIcon(QIcon(":/images/"+iconname+".png"));
}

void tpMainWindow::executePlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute, QString quality)
{
    QString qresBin = "";
    QString playerBin = "";
    QString playerarg = "";
    QStringList args;
    QStringList qresargs;
    QThread* processLaunchThread = new QThread;

    qresargs << "-f" << "^http://.+" << channel << ".+";
    qresargs << "-x" << QString::number(xOffset);
    qresargs << "-y" << QString::number(yOffset);
    qresargs << "-w" << QString::number(streamWidth);
    qresargs << "-t" << QString::number(streamHeight);
    qresargs << "-i" << "30";
    qresargs << "-d" << "20";
    args = genericHelper::getVlcArgs();

    if (mute == true) {
        args << "--volume=0";
    }

    args << url;
    qresBin = QCoreApplication::applicationFilePath().replace(genericHelper::getAppName()+".exe","qres.exe");

    if (player == "vlc2") {
        playerBin = QCoreApplication::applicationFilePath().replace(genericHelper::getAppName()+".exe","") + QDir::separator() +
                "3rdparty-addons" + QDir::separator() +
                "vlc" + QDir::separator() + "VLCPortable.exe";
    }

    qresargs << "-e" << "\""+playerBin+"\"";
    qresargs << "-u" << args.join(" ");
    QFile playerBinFile( playerBin );
    QFile qresBinFile( qresBin );

    if (genericHelper::getStreamPositioning() == true) {
        if( (qresBinFile.exists()) && (playerBinFile.exists()) )

        {
            processLauncher *processL = new processLauncher();
            processL->setProgramStr("\""+qresBin+"\"");
            processL->setArgs(qresargs);
            genericHelper::log("starting player: "+qresBin+" "+qresargs.join(" "));
            processL->moveToThread(processLaunchThread);
            // connect the thread start started signal to the process slot of the riftLogWatcher class
            QObject::connect(processLaunchThread, SIGNAL(started()), processL, SLOT(launch()));
            processLaunchThread->start();
        }
    } else {
        if( playerBinFile.exists() )

        {

            processLauncher *processL = new processLauncher();

            processL->setProgramStr("\""+playerBin+"\"");
            processL->setArgs(args);

            genericHelper::log("starting player: "+playerBin+" "+args.join(" "));

            processL->moveToThread(processLaunchThread);



            // connect the thread start started signal to the process slot of the riftLogWatcher class
            QObject::connect(processLaunchThread, SIGNAL(started()), processL, SLOT(launch()));

            processLaunchThread->start();



        }
    }

    genericHelper::log("player or qres binary not found, not starting: "+qresBin+" "+qresargs.join(" "));
    this->playerThreads.append(processLaunchThread);

}







void tpMainWindow::onChannelAccessTokenReady(const QJsonDocument &jsonResponseBuffer)
{




    QString _pl = "";

    QJsonObject jsonObject = jsonResponseBuffer.object();


    if (
            (!jsonObject["token"].isNull())
            &&
            (!jsonObject["sig"].isNull())

    )
    {

        QString channel = "";


        QListIterator<QString> itr (jsonObject["token"].toString().split(","));

         while (itr.hasNext()) {
               QString current = itr.next();

               if ( current.contains("channel") ) {
                        if ( current.count(":") > 0) {
                            channel = QString(current.split(":")[1]).replace("\"","");
                        }

                }
        }


        if (channel != "") {

            _pl = tw->getPlayListUrl(channel,QUrl::toPercentEncoding(jsonObject["token"].toString()).replace("%7B","{").replace("%7D","}").replace("%3A",":").replace("%2C",",").replace("%5B","[").replace("%5D","]"),jsonObject["sig"].toString());

            emit setStreamUrl( _pl );
        }

    }

}

void tpMainWindow::startM3u8Player(QString m3u8playlist)
{

}

void tpMainWindow::updateFromJsonResponseFollows(const QJsonDocument &jsonResponseBuffer)
{



       QJsonObject jsonObject = jsonResponseBuffer.object();


       for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
            if (iter.key() == "follows")
            {

               for (int i = 0; i <= iter.value().toArray().size(); i++)
               {



                   QJsonValue _val = iter.value().toArray().at(i);





                   tw->getStream(_val.toObject()["channel"].toObject()["name"].toString());
                   tw->getChannel(_val.toObject()["channel"].toObject()["name"].toString());


                   if (this->stmodel->findItems(_val.toObject()["channel"].toObject()["name"].toString(),Qt::MatchExactly,0).length() <= 0) {

                        if (_val.toObject()["channel"].toObject()["name"].toString().length() > 0) {




                           QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(_val.toObject()["channel"].toObject()["name"].toString()));
                           stmodel->setItem(i, 0, qsitem0);
                           QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg("offline"));
                           stmodel->setItem(i, 1, qsitem1);
                           QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(""));
                           stmodel->setItem(i, 2, qsitem2);
                           QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(""));
                           stmodel->setItem(i, 3, qsitem3);
                           QStandardItem *qsitem4 = new QStandardItem(QString("%0").arg(_val.toObject()["channel"].toObject()["status"].toString()));
                           stmodel->setItem(i, 4, qsitem4);


                           genericHelper::addFollow(_val.toObject()["channel"].toObject()["name"].toString());
                        }


                   }

            }

       }

       }

       this->ui->tableView->resizeColumnsToContents();
       //this->statusBar()->showMessage("Following ("+QString::number(this->ui->treeWidget->topLevelItemCount())+")  Bookmarked ("+QString::number(this->ui->treeWidgetBookmarks->topLevelItemCount())+")");



}

void tpMainWindow::updateFromJsonResponseBookmark(const QJsonDocument &jsonResponseBuffer)
{

    QString onlinename;
    QString status;
    QString viewers;

    QJsonObject jsonObject = jsonResponseBuffer.object();



    for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
       onlinename = "";
       status = "";
       if (iter.key() == "stream")
       {


          if (iter.value() != QJsonValue::Null)
          {
              onlinename = iter.value().toObject()["channel"].toObject()["name"].toString();
              status = iter.value().toObject()["channel"].toObject()["status"].toString();
              viewers = QString::number(iter.value().toObject()["viewers"].toInt(),'f',0);





              for(int i = 0; i<this->stmodelbookmarks->rowCount(); ++i)
              {

                  QModelIndex streamer_index = this->stmodelbookmarks->index(i,0);
                  QModelIndex online_index = this->stmodelbookmarks->index(i,1);
                  QModelIndex viewers_index = this->stmodelbookmarks->index(i,2);
                  QModelIndex status_index = this->stmodelbookmarks->index(i,4);

                  if ( this->stmodelbookmarks->itemData(streamer_index)[0].toString() == onlinename )  {
                      this->stmodelbookmarks->setData(online_index,"online");
                      this->stmodelbookmarks->setData(status_index,status);
                      this->stmodelbookmarks->setData(viewers_index,viewers);



                  }




            }


          }


       }

   }

    this->ui->tableViewBookmarks->resizeColumnsToContents();
   //this->statusBar()->showMessage("Following ("+QString::number(this->ui->treeWidget->topLevelItemCount())+")  Bookmarked ("+QString::number(this->ui->treeWidgetBookmarks->topLevelItemCount())+")");
}

void tpMainWindow::updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer)
{

    QString onlinename;

    QJsonObject jsonObject = jsonResponseBuffer.object();

    int stateTrans = false;


   for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
       onlinename = "";
       if (iter.key() == "stream")
       {

          if (iter.value() != QJsonValue::Null)
          {
              onlinename = iter.value().toObject()["channel"].toObject()["name"].toString();

              QString viewers = QString::number(iter.value().toObject()["viewers"].toInt(),'f',0);



              if (genericHelper::isOnline(this->stproxymodel->getColData(0,onlinename.toLower(),1).toString()) == false) {
                stateTrans = true;
              }


              bool updateok = stproxymodel->updateCol(0,onlinename.toLower(),1,"online");
              updateok = stproxymodel->updateCol(0,onlinename.toLower(),2,viewers);


              if ((stateTrans == true) && (updateok == true) && (genericHelper::getStreamOnlineNotify() == true)) {
                  emit (on_notifyByTray(onlinename+" is now online.",iter.value().toObject()["channel"].toObject()["status"].toString()));
              }





          }


       }
   }
}

void tpMainWindow::updateFromJsonResponseChannel(const QJsonDocument &jsonResponseBuffer)
{
    QString channelid;

    QJsonObject jsonObject = jsonResponseBuffer.object();



    bool updateok = stproxymodel->updateCol(0,jsonObject["display_name"].toString().toLower(),3,jsonObject["game"].toString());
    updateok = stproxymodelbookmarks->updateCol(0,jsonObject["display_name"].toString().toLower(),3,jsonObject["game"].toString());

    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableViewBookmarks->resizeColumnsToContents();

    channelid = QString::number(jsonObject["_id"].toDouble(),'f',0);


    this->ui->tableView->resizeColumnsToContents();

    tw->getHost(channelid);






}

void tpMainWindow::updateFromJsonResponseHost(const QJsonDocument &jsonResponseBuffer)
{
    //qDebug() << jsonResponseBuffer;
    QJsonObject jsonObject = jsonResponseBuffer.object();

    QString hostlogin;
    QString targetlogin;

    for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
        hostlogin = "";
        targetlogin = "";
        if (iter.key() == "hosts")
        {

           if (iter.value() != QJsonValue::Null)
           {

                if (iter.value().toArray().at(0).toObject().keys().count("target_login") > 0) {

                    hostlogin = iter.value().toArray().at(0).toObject()["host_login"].toString();
                    targetlogin = iter.value().toArray().at(0).toObject()["target_login"].toString();



                    if (genericHelper::isOnline(this->stproxymodel->getColData(0,hostlogin.toLower(),1).toString()) == false) {


                    bool updateok = stproxymodel->updateCol(0,hostlogin.toLower(),1,"hosting");
                    updateok = stproxymodel->updateCol(0,hostlogin.toLower(),4,targetlogin);



                    }
                }

            }

        }

    }
}



void tpMainWindow::on_actionSetup_Twitch_Auth_triggered()
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

void tpMainWindow::on_updateNotify(const QString &latestVersion)
{
    qDebug() << "update";
    QString latestVersionNumber = latestVersion;



    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, tr("twitcher"),
                                    "New version ("+latestVersionNumber+") available, do you want to update?",
                                    QMessageBox::Yes|QMessageBox::No);


    if (reply == QMessageBox::Yes) {

       QDesktopServices::openUrl(QUrl("http://agtv.abyle.org/downloads/agtv-"+latestVersionNumber+"-core-"+this->currArch+"-installer.exe"));

      } else {
        genericHelper::log("update dismissed.");
      }

}








void tpMainWindow::on_actionPositioner_triggered()
{
    if (diaPositioner->getDialogShown() == true)
    {
        diaPositioner->close();

        diaPositioner->show();

    } else {


        diaPositioner->show();
        diaPositioner->setDialogShown();
    }
}

void tpMainWindow::on_actionTwitch_Browser_triggered()
{
    QString link = "http://www.twitch.tv/"+genericHelper::getUsername()+"/profile";
    QDesktopServices::openUrl(QUrl(link));
}

void tpMainWindow::on_actionHexChat_triggered()
{
    genericHelper::executeAddonHexchat(genericHelper::getFollows());
}

void tpMainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{

        if(reason == QSystemTrayIcon::DoubleClick) {
            this->show();
            this->raise();
        }

}

void tpMainWindow::on_notifyByTray(QString title, QString message)
{
    if (trayIcon->isVisible()) {
        trayIcon->showMessage(title, message);

    }
}






void tpMainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About agtv version:"+version,
                tr("<b>agtv (Abyle gaming tv player)</b> allows to view twitch streams "
                   "without using a webbrowser, flash or livestreamer "
                   ""));
}

void tpMainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void tpMainWindow::on_actionReport_Bug_triggered()
{
    QString link = "https://github.com/scuq/agtv/issues";
    QDesktopServices::openUrl(QUrl(link));
}

void tpMainWindow::on_actionRefresh_triggered()
{
    this->loadData();
}


void tpMainWindow::on_actionOptions_triggered()
{
    if (diaOptions->getDialogShown() == true)
    {
        diaOptions->close();

        diaOptions->show();

    } else {


        diaOptions->show();
        diaOptions->setDialogShown();
    }
}

void tpMainWindow::on_actionQuit_triggered()
{
    this->myQuit();
}



void tpMainWindow::on_actionCredits_triggered()
{
    QMessageBox::about(this, tr("Credits"), tr(""
                            "Icons made by <a href=http://www.freepik.com title=Freepik>Freepik</a> from <a href=http://www.flaticon.com title=Flaticon>www.flaticon.com</a></br>"

                                               ""));
}

void tpMainWindow::on_actionShow_Offline_Streamers_toggled(bool arg1)
{

    genericHelper::setShowOfflineStreamers(arg1);

    this->stproxymodel->setShowOffline(arg1);
    stproxymodel->setSourceModel(stmodel);

    this->stproxymodelbookmarks->setShowOffline(arg1);
    stproxymodelbookmarks->setSourceModel(stmodelbookmarks);


}

void tpMainWindow::on_Ready()
{
    refreshTimer->start(updateInterval);
    this->enableInput();
}

void tpMainWindow::on_SwitchInputEnabled(bool enable)
{
    if (enable == true) {
        this->enableInput();
    } else {
        this->disableInput();
    }
}



void tpMainWindow::on_actionShow_Offline_Streamers_triggered()
{

}

void tpMainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{

    if (this->ui->tableView->selectionModel()->selectedRows().count() > 0) {
        QMenu *tableviewContextMenu = new QMenu("Following", this);


        tableviewContextMenu->addAction(open_in_browser);
        tableviewContextMenu->addAction(open_in_hexchat);

        if (this->stproxymodel->getColData(0,this->ui->tableView->selectionModel()->selectedRows(0).at(0).data().toString(),1) == "hosting") {
            tableviewContextMenu->addAction(add_hosted_bookmark);
        }

        tableviewContextMenu->popup(this->ui->tableView->viewport()->mapToGlobal(pos));


        //add_hosted_bookmark




    }



}

void tpMainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{

    QString _streamer;
    QString _status;



   _status = this->stproxymodel->data(index.sibling(index.row(),1),0).toString();
   _streamer = this->stproxymodel->data(index.sibling(index.row(),0),0).toString();





    if (genericHelper::isOnline(_status) == true) {

        if (launchBookmarkEnabled == true) {

            tw->getChannelAccessToken(_streamer);

            if (diaLaunch->getDialogShown() == true)
            {
                diaLaunch->close();

                diaLaunch->show();

            } else {


                diaLaunch->show();
                diaLaunch->setDialogShown();
            }
            emit setStreamTitle( _streamer, "" );
        }
    }
}

void tpMainWindow::on_tableViewBookmarks_doubleClicked(const QModelIndex &index)
{
    QString _streamer;
    QString _status;



   _status = this->stproxymodelbookmarks->data(index.sibling(index.row(),1),0).toString();
   _streamer = this->stproxymodelbookmarks->data(index.sibling(index.row(),0),0).toString();




    if (genericHelper::isOnline(_status) == true) {

        if (launchBookmarkEnabled == true) {

            tw->getChannelAccessToken(_streamer);

            if (diaLaunch->getDialogShown() == true)
            {
                diaLaunch->close();

                diaLaunch->show();

            } else {


                diaLaunch->show();
                diaLaunch->setDialogShown();
            }
            emit setStreamTitle( _streamer, "" );
        }
    }

}

void tpMainWindow::on_tableViewBookmarks_customContextMenuRequested(const QPoint &pos)
{

     if ((this->ui->tableViewBookmarks->selectionModel()->selectedRows().count() > 0) || (this->stmodelbookmarks->rowCount() <= 0)) {




        QMenu *tableviewbookmarksContextMenu = new QMenu("Bookmarks", this);


        tableviewbookmarksContextMenu->addAction(add_bookmark);
        tableviewbookmarksContextMenu->addAction(delete_bookmark);

        if ((this->stmodelbookmarks->rowCount() > 0)) {
            tableviewbookmarksContextMenu->addAction(open_in_hexchat_bookmark);
        }

        tableviewbookmarksContextMenu->popup(this->ui->tableView->viewport()->mapToGlobal(pos));

    }
}

void tpMainWindow::on_actionLogfile_triggered()
{
     genericHelper::openLogWithNotepad();
}

void tpMainWindow::on_tabWidget_currentChanged(int index)
{
    genericHelper::setSelectedTab(index);
}
