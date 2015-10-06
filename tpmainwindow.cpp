#include "tpmainwindow.h"
#include "ui_tpmainwindow.h"

tpMainWindow::tpMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tpMainWindow)
{
    ui->setupUi(this);

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

    diaOauthSetup = new dialogOauthSetup(this);
    diaPositioner = new DialogPositioner(this);
    diaLaunch = new DialogLaunch(this);
    diaOptions = new DialogOptions(this);

    if (genericHelper::getUsername() == "") {
        this->disableInput();
    } else {


    // init twitch api object
    tw = new TwitchApi(this, genericHelper::getOAuthAccessToken());

    QObject::connect(tw, SIGNAL(twitchReadyChannelAccessToken(const QJsonDocument)), this, SLOT(onChannelAccessTokenReady(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyFollows(const QJsonDocument)), this, SLOT(updateFromJsonResponseFollows(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyBookmark(const QJsonDocument)), this, SLOT(updateFromJsonResponseBookmark(const QJsonDocument)));
    QObject::connect(tw, SIGNAL(twitchReadyStream(const QJsonDocument)), this, SLOT(updateFromJsonResponseStream(const QJsonDocument)));
    QObject::connect(this, SIGNAL(setStreamTitle(QString,QString)), diaLaunch, SLOT(setStreamTitle(QString,QString)));
    QObject::connect(this, SIGNAL(setStreamUrl(QString)), diaLaunch, SLOT(setStreamUrl(QString)));
    QObject::connect(diaLaunch, SIGNAL(startStreamPlay(QString, QString, QString, int, int, int , int, bool, QString)), this, SLOT(executePlayer(QString, QString, QString, int, int, int , int, bool, QString)));


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

    }

    // some stuff needed for the tray icon
    createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();




}

tpMainWindow::~tpMainWindow()
{
    delete ui;
}




void tpMainWindow::disableInput()
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

                    widget->setEnabled(false);
                    }
                }
}

void tpMainWindow::loadBookmarks()
{
    QStringList loadedbookmarks;

    loadedbookmarks = genericHelper::getBookmarks();


    QStringList currentbookmarks;

    QTreeWidgetItemIterator it(this->ui->treeWidgetBookmarks);
    while (*it) {
        if ((*it)->text(0) != "")
            currentbookmarks << (*it)->text(0);
        ++it;
    }



     QListIterator<QString> itr (loadedbookmarks);

     while (itr.hasNext()) {
           QString current = itr.next();

           if (currentbookmarks.count(current) <= 0) {

               QTreeWidgetItem * item = new QTreeWidgetItem();

               tw->getStream(current);

                if (this->ui->treeWidgetBookmarks->findItems(current,Qt::MatchExactly,0).length() <= 0) {

                   item->setText(0,current);
                   item->setText(1,"offline");

                   item->setTextColor(1,  QColor(255,2,29));


                   this->ui->treeWidgetBookmarks->addTopLevelItem(item);
                }
           }

           tw->getBookmarkStatus(current);
     }
}

void tpMainWindow::saveBookmarks()
{
    QStringList currentbookmarks;

    QTreeWidgetItemIterator it(this->ui->treeWidgetBookmarks);
    while (*it) {
        if ((*it)->text(0) != "")
            currentbookmarks << (*it)->text(0);
        ++it;
    }


    genericHelper::setBookmarks(currentbookmarks);
}

void tpMainWindow::enableDelete()
{
    this->ui->pushButtonDeleteBookmark->setEnabled(true);
    this->ui->pushButtonDeleteBookmarkCancel->setEnabled(true);
    this->ui->lineEditDeleteBookmark->setEnabled(true);
    this->ui->lineEditNewBookmark->setEnabled(false);
    this->ui->pushButtonAddBookmark->setEnabled(false);
    launchBookmarkEnabled = false;
}

void tpMainWindow::disableDelete()
{
    this->ui->pushButtonDeleteBookmark->setEnabled(false);
    this->ui->pushButtonDeleteBookmarkCancel->setEnabled(false);
    this->ui->lineEditDeleteBookmark->setEnabled(false);
    this->ui->lineEditNewBookmark->setEnabled(true);
    this->ui->pushButtonAddBookmark->setEnabled(true);
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
        trayIcon->showMessage(tr("Twitcher closed to tray."),
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
    open = new QAction(tr("&Open"), this);
    connect(open, SIGNAL(triggered()), this, SLOT(show()));


    close = new QAction(tr("&Quit"), this);
    connect(close, SIGNAL(triggered()), this, SLOT(myQuit()));
}


void tpMainWindow::myQuit()
{


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


    args << "--autoscale";
    args << "--qt-minimal-view";
    args << "--no-qt-system-tray";
    args << "--network-caching=5000" ;

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
    //qDebug() << "startM3u8Player";
    //qDebug() <<    m3u8playlist;
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

                   QTreeWidgetItem * item = new QTreeWidgetItem();


                   tw->getStream(_val.toObject()["channel"].toObject()["name"].toString());

                   if (this->ui->treeWidget->findItems(_val.toObject()["channel"].toObject()["name"].toString(),Qt::MatchExactly,0).length() <= 0) {
                        if (_val.toObject()["channel"].toObject()["name"].toString().length() > 0) {

                           item->setText(0,_val.toObject()["channel"].toObject()["name"].toString());
                           item->setText(1,"offline");
                           item->setText(2,_val.toObject()["channel"].toObject()["status"].toString());
                           item->setTextColor(1,  QColor(255,2,29));


                           this->ui->treeWidget->addTopLevelItem(item);
                        }
                   }

            }

       }

       }
       this->ui->treeWidget->resizeColumnToContents(0);
       this->ui->treeWidget->resizeColumnToContents(1);
       this->ui->treeWidget->resizeColumnToContents(2);
       this->statusBar()->showMessage("Following ("+QString::number(this->ui->treeWidget->topLevelItemCount())+")  Bookmarked ("+QString::number(this->ui->treeWidgetBookmarks->topLevelItemCount())+")");



}

void tpMainWindow::updateFromJsonResponseBookmark(const QJsonDocument &jsonResponseBuffer)
{

    QString onlinename;
    QString status;

    QJsonObject jsonObject = jsonResponseBuffer.object();


   for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
       if (iter.key() == "stream")
       {

          if (iter.value() != QJsonValue::Null)
          {
              onlinename = iter.value().toObject()["channel"].toObject()["name"].toString();
              status = iter.value().toObject()["channel"].toObject()["status"].toString();


              QTreeWidgetItemIterator it(this->ui->treeWidgetBookmarks);
                 while (*it) {
                     if ((*it)->text(0) == onlinename) {
                         if (((*it)->text(1) == "offline") && (genericHelper::getStreamOnlineNotify() == true)) {

                              if (trayIcon->isVisible()) {
                                    trayIcon->showMessage(onlinename+" online", iter.value().toObject()["channel"].toObject()["status"].toString());

                              }
                         }
                         (*it)->setText(1,"online");
                         (*it)->setTextColor(1,  QColor(85,85,255));
                         (*it)->setText(2,status);


                     }
                     ++it;
                 }

          }


       }
   }
   this->ui->treeWidgetBookmarks->resizeColumnToContents(0);
   this->ui->treeWidgetBookmarks->resizeColumnToContents(1);
   this->ui->treeWidgetBookmarks->resizeColumnToContents(2);
   this->statusBar()->showMessage("Following ("+QString::number(this->ui->treeWidget->topLevelItemCount())+")  Bookmarked ("+QString::number(this->ui->treeWidgetBookmarks->topLevelItemCount())+")");
}

void tpMainWindow::updateFromJsonResponseStream(const QJsonDocument &jsonResponseBuffer)
{

    QString onlinename;

    QJsonObject jsonObject = jsonResponseBuffer.object();



   for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
       if (iter.key() == "stream")
       {

          if (iter.value() != QJsonValue::Null)
          {
              onlinename = iter.value().toObject()["channel"].toObject()["name"].toString();

              QTreeWidgetItemIterator it(this->ui->treeWidget);
                 while (*it) {
                     if ((*it)->text(0).toLower() == onlinename.toLower()) {
                         if (((*it)->text(1) == "offline") && (genericHelper::getStreamOnlineNotify() == true)) {

                              if (trayIcon->isVisible()) {
                                  trayIcon->showMessage(onlinename+" online", iter.value().toObject()["channel"].toObject()["status"].toString());

                              }
                         }
                         (*it)->setText(1,"online");
                         (*it)->setTextColor(1,  QColor(85,85,255));
                     }
                     ++it;
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




void tpMainWindow::on_pushButtonAddBookmark_clicked()
{
    QTreeWidgetItem * item = new QTreeWidgetItem();


    item->setText(0,this->ui->lineEditNewBookmark->text().toLower());


    this->ui->treeWidgetBookmarks->addTopLevelItem(item);

    this->saveBookmarks();
    this->loadBookmarks();
}

void tpMainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{

    if (item->text(1) == "online") {

        if (launchBookmarkEnabled == true) {

            tw->getChannelAccessToken(item->text(0));

            if (diaLaunch->getDialogShown() == true)
            {
                diaLaunch->close();

                diaLaunch->show();

            } else {


                diaLaunch->show();
                diaLaunch->setDialogShown();
            }
            emit setStreamTitle( item->text(0), "" );
        }
    }
}

void tpMainWindow::on_treeWidgetBookmarks_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(1) == "online") {

        tw->getChannelAccessToken(item->text(0));

        if (diaLaunch->getDialogShown() == true)
        {
            diaLaunch->close();

            diaLaunch->show();

        } else {


            diaLaunch->show();
            diaLaunch->setDialogShown();
        }
        emit setStreamTitle( item->text(0), "" );

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

void tpMainWindow::on_treeWidgetBookmarks_itemClicked(QTreeWidgetItem *item, int column)
{

    this->ui->lineEditDeleteBookmark->setText(item->text(0));
    this->enableDelete();
}

void tpMainWindow::on_treeWidgetBookmarks_itemSelectionChanged()
{

}

void tpMainWindow::on_pushButtonDeleteBookmarkCancel_clicked()
{
    this->ui->lineEditDeleteBookmark->setText("");
    this->disableDelete();
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
    QString link = "https://github.com/scuq/twitcher/issues";
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

void tpMainWindow::on_pushButtonDeleteBookmark_clicked()
{
    QTreeWidgetItemIterator it(this->ui->treeWidgetBookmarks);
       while (*it) {
           if ((*it)->text(0) ==  this->ui->lineEditDeleteBookmark->text()) {
               delete (*it);




           }
           ++it;
       }

       this->disableDelete();
       this->saveBookmarks();
       this->loadBookmarks();
}

void tpMainWindow::on_actionCredits_triggered()
{
    QMessageBox::about(this, tr("Credits"), tr(""
                            "Icons made by <a href=http://www.freepik.com title=Freepik>Freepik</a> from <a href=http://www.flaticon.com title=Flaticon>www.flaticon.com</a></br>"

                                               ""));
}
