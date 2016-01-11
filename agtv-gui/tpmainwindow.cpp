#include "tpmainwindow.h"
#include "ui_tpmainwindow.h"

#define DEFSTATUSTIMEOUT 5000

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

    AgtvDefItemDelegate =  new AgtvDefaultItemDelegate();

    // ignore update intervals <9 seconds
    if (genericHelper::getUpdateInterval() >= 5) {
        this->updateInterval = genericHelper::getUpdateInterval() * 1000;
    } else {
        this->updateInterval = 5000;
    }

    QStringList horzHeaders = { "Name", "Status", "#V",
                                "Game", "Status Message"};

    stmodel = new QStandardItemModel(0,5,this);
    stmodel->setHorizontalHeaderLabels(horzHeaders);
    stproxymodel = new AdvQSortFilterProxyModel(this);
    stproxymodel->setSourceModel(stmodel);
    stproxymodel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->ui->tableView->horizontalHeader()->show();
    this->ui->tableView->setItemDelegate(AgtvDefItemDelegate);
    this->ui->tableView->setModel(stproxymodel);

    stmodelbookmarks = new QStandardItemModel(0,5,this);
    stmodelbookmarks->setHorizontalHeaderLabels(horzHeaders);
    stproxymodelbookmarks = new AdvQSortFilterProxyModel(this);
    stproxymodelbookmarks->setSourceModel(stmodelbookmarks);
    stproxymodelbookmarks->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->ui->tableViewBookmarks->horizontalHeader()->show();
    this->ui->tableViewBookmarks->setItemDelegate(AgtvDefItemDelegate);
    this->ui->tableViewBookmarks->setModel(stproxymodelbookmarks);

    diaOauthSetup = new dialogOauthSetup(this);
    diaPositioner = new DialogPositioner(this);
    diaLaunch = new DialogLaunch(this);
    diaOptions = new DialogOptions(this);
    diaShowLogFile = new DialogShowLogFile(this);

    twitchUserLocal = new TwitchUserLocal(this,this->updateInterval);
    
    twitchUser = new TwitchUser(this,twitchUserLocal->getStoredOAuthAccessToken(),genericHelper::getUsername(),this->updateInterval);
    
    
        
    QObject::connect(twitchUser, SIGNAL(twitchNeedsOAuthSetup()), this, SLOT(on_actionSetup_Twitch_Auth_triggered()));
    
    twitchUser->checkAuthenticationSetup();
      
     QObject::connect(twitchUserLocal, SIGNAL(oAuthAccessTokenLoaded(QString)), diaOauthSetup, SLOT(setCurrentStoredAuthToken(QString)));
     QObject::connect(diaOauthSetup, SIGNAL(saveAuthTokenRequested(QString)), twitchUserLocal, SLOT(onSaveOAuthAccessToken(QString)));
     QObject::connect(twitchUser, SIGNAL(newUsernameDetected(QString)), twitchUserLocal, SLOT(onSaveUsername(QString)));    
   
    
    twitchUserLocal->getStoredOAuthAccessToken();

    QObject::connect(twitchUser, SIGNAL(twitchFollowedChannelsDataChanged(const bool)), this, SLOT(onTwitchFollowedChannelsDataChanged(const bool)));

    QObject::connect(twitchUser, SIGNAL(twitchFollowChannelError(const QString)), this, SLOT(showOnStatusBar(const QString)));

    QObject::connect(twitchUser, SIGNAL(twitchUnfollowChannelSuccess(const QString)), this, SLOT(updateOnUnfollow(const QString)));

    QObject::connect(twitchUser, SIGNAL(twitchUnfollowChannelError(const QString)), this, SLOT(showOnStatusBar(const QString)));



    QObject::connect(twitchUserLocal, SIGNAL(twitchBookmarkedChannelsDataChanged(const bool)), this, SLOT(onTwitchBookmarkedChannelsDataChanged(const bool)));
   // QObject::connect(twitchUserLocal, SIGNAL(twitchBookmarkedChannelsDataChanged(const bool)), this, SLOT(onTwitchBookmarkedChannelsDataChanged(const bool)));
 
    

    twitchUserLocal->loadBookmarks();

    QObject::connect(this, SIGNAL(setStreamTitle(QString,QString)), diaLaunch, SLOT(setStreamTitle(QString,QString)));
    QObject::connect(this, SIGNAL(setStreamUrl(QString)), diaLaunch, SLOT(setStreamUrl(QString)));
    QObject::connect(this, SIGNAL(setStreamLogoUrl(QString)), diaLaunch, SLOT(setStreamLogoUrl(QString)));
    QObject::connect(diaLaunch, SIGNAL(startStreamPlay(QString, QString, QString, int, int, int , int, bool, QString)), this, SLOT(executePlayer(QString, QString, QString, int, int, int , int, bool, QString)));

    QObject::connect(this->diaOptions, SIGNAL(settingsSaved()), this, SLOT(on_settingsSaved()));

    QObject::connect(this, SIGNAL(setStreamUrlWithQuality(QMap<QString, QString>)), diaLaunch, SLOT(setStreamUrlWithQuality(QMap<QString, QString>)));

    //QObject::connect(diaOauthSetup, SIGNAL(twitchAuthSetupChanged(bool)), this, SLOT(on_SwitchInputEnabled(bool)));
    QObject::connect(diaOauthSetup, SIGNAL(onAuthorizeRequested()), this, SLOT(onBrowserAuthorizeRequested()));
    QObject::connect(diaOauthSetup, SIGNAL(authTokenChanged(QString)), twitchUser, SLOT(validateNewAuthToken(QString)));
   
    
    QObject::connect(twitchUser, SIGNAL(authCheckSuccessfull()), diaOauthSetup, SLOT(onAuthOk()));
    QObject::connect(twitchUser, SIGNAL(authCheckFailed()), diaOauthSetup, SLOT(onAuthNok()));
    

    QObject::connect(this->ui->lineEditFilter, SIGNAL(textChanged(QString)), this->stproxymodel, SLOT(setFilterRegExp(QString)));
    QObject::connect(this->ui->lineEditFilterBookmark, SIGNAL(textChanged(QString)), this->stproxymodelbookmarks, SLOT(setFilterRegExp(QString)));

    QObject::connect(uc, SIGNAL(updateReady(const QString)), this, SLOT(on_updateNotify(const QString)));

    QObject::connect(this->ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabChanged(const int)));


    if (genericHelper::getCheckUpdate() == true) {
        uc->getCheck();
    }

    // init QPixmaps for ok, not ok icons
    //offline = QPixmap(":images/offline.png");
    //online = QPixmap(":images/online.png");

    desktop = QApplication::desktop();

    genericHelper::setPrimaryScreenWidth(desktop->screenGeometry(-1).width());

    // init qtimer, set update interval and connect the timeout singal of qtimer to the updateFormFieldsSlot of this form
    //refreshTimer = new QTimer(this);
    //refreshTimer->setInterval(updateInterval);
    //QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(on_loadData()));
    //refreshTimer->start(updateInterval);

    // some stuff needed for the tray icon
    createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();

    showOfflineStreamers = true;

    if (!twitchUserLocal->isUserSetupOk()) {
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

    this->stproxymodel->setShowApproximateViewerCount(genericHelper::getShowApproximateViewerCount());
    this->stproxymodelbookmarks->setShowApproximateViewerCount(genericHelper::getShowApproximateViewerCount());
    this->ui->actionShow_Approximate_Viewer_Count->setChecked(genericHelper::getShowApproximateViewerCount());

    ui->tableView->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());
    ui->tableViewBookmarks->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());

    if(! genericHelper::getFitAllContentToWindow()) {
        QTimer::singleShot(0, this, SLOT(restoreTableViewsManual()));
    }

    QTimer::singleShot(0, this, SLOT(restoreSortModes()));

#ifdef WINTERNALVLC
    //diaVideoPlayer = new DialogVideoPlayer;
    //diaVideoPlayer->initVLC();
#endif

    clipboard = QApplication::clipboard();

    diaTopGameBrowser = new DialogGameBrowser;

    QObject::connect(diaTopGameBrowser, SIGNAL(startStream(const QString)), this, SLOT(startFromGBrowser(const QString)));
}

void tpMainWindow::twitchChannelDataChanged(const bool onlineStatusChanged)
{
    TwitchChannel *channel = qobject_cast<TwitchChannel *>(QObject::sender());
    if(channel) {
        QString onlineStatus = "";
        switch (channel->getChannelOnlineStatus()) {
            case TwitchChannel::ChannelOnlineStatus::online:
                onlineStatus = "online";
                break;
            case TwitchChannel::ChannelOnlineStatus::offline:
                onlineStatus = "offline";
                break;
            case TwitchChannel::ChannelOnlineStatus::playlist:
                onlineStatus = "playlist";
                break;
            case TwitchChannel::ChannelOnlineStatus::hosting:
                onlineStatus = "hosting";
                break;
            default:
                onlineStatus = "unknown";
        }

        QString viewersString = QString::number(channel->getChannelViewers(), 'f', 0);

        if(! bunchUpdateStreamDataName(channel->getChannelName(), onlineStatus, viewersString,
                                       channel->getChannelGame(), channel->getChannelTitle()) ) {
            genericHelper::log(QString(Q_FUNC_INFO) + ": Error updating model");
        }

        if(onlineStatusChanged && genericHelper::getStreamOnlineNotify()) {
            emit( on_notifyByTray(channel->getChannelName() + " is now " + onlineStatus, channel->getChannelTitle()) );
        }
    }
}

bool tpMainWindow::bunchUpdateStreamDataName(const QString &name, const QString &onlineStatus,
                                             const QString &viewers, const QString &game,
                                             const QString &status)
{
    bool ret=true;
    if(! stproxymodel->updateCol(0, name.toLower(), 1, onlineStatus) &&
         ! stproxymodelbookmarks->updateCol(0, name.toLower(), 1, onlineStatus) )
        ret=false;
    if(! stproxymodel->updateCol(0, name.toLower(), 2, viewers) &&
         ! stproxymodelbookmarks->updateCol(0, name.toLower(), 2, viewers) )
        ret=false;
    if(! stproxymodel->updateCol(0, name.toLower(), 3, game) &&
         ! stproxymodelbookmarks->updateCol(0, name.toLower(), 3, game) )
        ret=false;
    if(! stproxymodel->updateCol(0, name.toLower(), 4, status) &&
         ! stproxymodelbookmarks->updateCol(0, name.toLower(), 4, status) )
        ret=false;

    for(int i = 0; i<this->stmodelbookmarks->rowCount(); ++i) {
        QModelIndex streamer_index = this->stmodelbookmarks->index(i,0);
        QModelIndex online_index = this->stmodelbookmarks->index(i,1);
        QModelIndex viewers_index = this->stmodelbookmarks->index(i,2);
        QModelIndex status_index = this->stmodelbookmarks->index(i,4);

        if ( this->stmodelbookmarks->itemData(streamer_index)[0].toString() == name )  {
            this->stmodelbookmarks->setData(online_index, onlineStatus);
            this->stmodelbookmarks->setData(status_index, status);
            this->stmodelbookmarks->setData(viewers_index, viewers);
        }
    }

    fitTableViewToContent(this->ui->tableView);
    fitTableViewToContent(this->ui->tableViewBookmarks);

    return ret;
}

void tpMainWindow::startFromGBrowser(const QString stream)
{
    TwitchChannel *channel = twitchChannels[stream];
    if(channel != 0) {
        channel->requestPlaylist();

        prepareDiaLauncher();

        emit setStreamTitle(stream, "" );
        emit setStreamLogoUrl(channel->getChannelLogoUrl());
    }
}

void tpMainWindow::saveSortModes()
{
    genericHelper::setFollowerSortCol(this->stproxymodel->sortColumn());
    genericHelper::setFollowerSortOrder(this->stproxymodel->sortOrder());
    genericHelper::setBookmarksSortCol(this->stproxymodelbookmarks->sortColumn());
    genericHelper::setBookmarksSortOrder(this->stproxymodelbookmarks->sortOrder());
}

void tpMainWindow::restoreSortModes()
{
    this->ui->tableView->sortByColumn(genericHelper::getFollowerSortCol(), genericHelper::getFollowerSortOrder());
    this->ui->tableViewBookmarks->sortByColumn(genericHelper::getBookmarksSortCol(), genericHelper::getBookmarksSortOrder());
}

void tpMainWindow::restoreTableViewsManual()
{
    QMap<int, int> tableViewColumnWidth = genericHelper::getFollowerTableViewColumnWidthManual();
    QMap<int, int> tableViewBookmarksColumnWidth = genericHelper::getBookmarksTableViewColumnWidthManual();

    QMapIterator<int, int> i(tableViewColumnWidth);
    while (i.hasNext()) {
        i.next();
        this->ui->tableView->setColumnWidth(i.key(), i.value());
    }

    i = tableViewBookmarksColumnWidth;
    while (i.hasNext()) {
        i.next();
        this->ui->tableViewBookmarks->setColumnWidth(i.key(), i.value());
    }
}

tpMainWindow::~tpMainWindow()
{
    delete ui;
}

void tpMainWindow::on_settingsSaved()
{
    if ( genericHelper::getFitAllContentToWindow() ) {
        saveTableViewStates();

        ui->tableView->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());
        ui->tableViewBookmarks->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());

        fitTableViewToContent(this->ui->tableView);
        fitTableViewToContent(this->ui->tableViewBookmarks);
    } else if (! genericHelper::getFitAllContentToWindow()) {
        ui->tableView->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());
        ui->tableViewBookmarks->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());

        restoreTableViewsManual();
    }
}

void tpMainWindow::fitTableViewToContent(QTableView *tableView)
{
    if ( genericHelper::getFitAllContentToWindow() ) {
        tableView->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());
        for (int c=0; c < tableView->horizontalHeader()->count()-1 ; ++c) {
            tableView->resizeColumnToContents(c);
        }
        tableView->resizeRowsToContents();
    }
}

void tpMainWindow::on_tabChanged(const int tabid)
{
    if ( genericHelper::getFitAllContentToWindow() ) {
        fitTableViewToContent(this->ui->tableView);
        fitTableViewToContent(this->ui->tableViewBookmarks);
    }
}

void tpMainWindow::disableInput()
{

    //refreshTimer->stop();

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
    /**

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
            if(!this->twitchChannels.contains(current)) {
                genericHelper::log(QString(__func__) + QString(" Adding TwitchChannel instance for channel ") + current);
                genericHelper::log(QString(__func__) + QString(" TODO: NEEDS DELETE HANDLING"));
                TwitchChannel *twitchChannel = new TwitchChannel(this, genericHelper::getOAuthAccessToken(), current, this->updateInterval);
                this->twitchChannels[current] = twitchChannel;
                QObject::connect(twitchChannel, SIGNAL(twitchChannelDataChanged(const bool)), this, SLOT(twitchChannelDataChanged(const bool)));
            }

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

        ++i;
    }

    fitTableViewToContent(this->ui->tableViewBookmarks);

    **/
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
    
    twitchUserLocal->setBookmarks(currentbookmarks);
    twitchUserLocal->loadBookmarks();
    
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

    add_follow = new QAction(tr("&Follow Channel..."), this);
    connect(add_follow, SIGNAL(triggered()), this, SLOT(addFollow()));

    add_follower_bookmark = new QAction(tr("Follow Channel"), this);
    connect(add_follower_bookmark, SIGNAL(triggered()), this, SLOT(addFollowerBookmark()));

    delete_follower = new QAction(tr("Unfollow Channel"), this);
    connect(delete_follower, SIGNAL(triggered()), this, SLOT(deleteFollower()));

    copy_streamurl = new QAction(tr("Copy URL"), this);
    copy_streamurl->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    connect(copy_streamurl, SIGNAL(triggered()), this, SLOT(copyUrl()));

}

void tpMainWindow::copyUrl() {
    QString link;
    switch(this->ui->tabWidget->currentIndex()) {
        case 0: // follower
            link = "http://www.twitch.tv/"+this->ui->tableView->selectionModel()->selectedRows(0).at(0).data().toString()+"/";
            break;
        case 1: // bookmarks
            link = "http://www.twitch.tv/"+this->ui->tableViewBookmarks->selectionModel()->selectedRows(0).at(0).data().toString()+"/";
            break;
        default:
            return;
    }
    if(! link.isEmpty()) {
        clipboard->setText(link);
        statusBar()->showMessage(tr("URL copied to clipboard") + ": " + link, DEFSTATUSTIMEOUT);
    }
}

void tpMainWindow::addFollowerBookmark()
{
    const QString _streamer = this->ui->tableViewBookmarks->selectionModel()->selectedRows(0).at(0).data().toString();
    const QString _status = this->ui->tableViewBookmarks->selectionModel()->selectedRows(1).at(0).data().toString();
    const QString _text = this->ui->tableViewBookmarks->selectionModel()->selectedRows(4).at(0).data().toString();

    twitchUser->followChannel(_streamer);
    
    this->ui->statusBar->showMessage(tr("Sent follow request for channel") + " " + _streamer, DEFSTATUSTIMEOUT);
}

void tpMainWindow::deleteFollower()
{
    const QString _streamer = this->ui->tableView->selectionModel()->selectedRows(0).at(0).data().toString();
    const QString _status = this->ui->tableView->selectionModel()->selectedRows(1).at(0).data().toString();
    const QString _text = this->ui->tableView->selectionModel()->selectedRows(4).at(0).data().toString();

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, genericHelper::getAppName(),
                 tr("Do you really want to unfollow ") + _streamer + "?",
                 QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        twitchUser->unfollowChannel(_streamer);


        followerToRemove.append(_streamer);
    }
}

void tpMainWindow::saveTableViewStates()
{
    QMap<int, int> tableViewColumnWidth;
    QMap<int, int> tableViewBookmarksColumnWidth;

    for(int c=0; c < ui->tableView->horizontalHeader()->count(); ++c)
    {
        tableViewColumnWidth[c] = ui->tableView->columnWidth(c);
    }

    for(int c=0; c < ui->tableViewBookmarks->horizontalHeader()->count(); ++c)
    {
        tableViewBookmarksColumnWidth[c] = ui->tableViewBookmarks->columnWidth(c);
    }

    genericHelper::setFollowerTableViewColumnWidthManual(tableViewColumnWidth);
    genericHelper::setBookmarksTableViewColumnWidthManual(tableViewBookmarksColumnWidth);
}

void tpMainWindow::myQuit()
{
    genericHelper::saveGeometry("main",saveGeometry());
    genericHelper::saveWindowstate("main",saveState());

    if (! genericHelper::getFitAllContentToWindow() ) {
        saveTableViewStates();
    }
    saveSortModes();

    for( int i=0; i<this->playerThreads.count(); ++i )
    {
       this->playerThreads.at(i)->terminate();
    }

#ifdef WINTERNALVLC
    for(const auto& player : diaVideoPlayers) {
        player->deleteLater();
    }
#endif

    qApp->quit();
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
     const QString _streamer = this->ui->tableView->selectionModel()->selectedRows(0).at(0).data().toString();
     const QString _status = this->ui->tableView->selectionModel()->selectedRows(1).at(0).data().toString();

     int ret = 0;

     if ( genericHelper::isHosting(_status) ) {
         QString _hostedfor = this->twitchChannels[_streamer]->getHostedChannel();
         ret = genericHelper::executeAddonHexchat( QStringList{_streamer, _hostedfor} );
     } else {
         ret = genericHelper::executeAddonHexchat( QStringList{_streamer} );
     }

     if (ret != 0) {
         QMessageBox::critical(this, genericHelper::getAppName(),
                                        tr("Can't find HexChat, please check options."),
                                        QMessageBox::Ok);
     }
}
void tpMainWindow::openChatHexChatBookmark()
{
    const QString _streamer = this->ui->tableViewBookmarks->selectionModel()->selectedRows(0).at(0).data().toString();
    const QString _status = this->ui->tableViewBookmarks->selectionModel()->selectedRows(1).at(0).data().toString();

    int ret = 0;

    if ( genericHelper::isHosting(_status) ) {
        QString _hostedfor = this->twitchChannels[_streamer]->getHostedChannel();
        ret = genericHelper::executeAddonHexchat( QStringList{_streamer, _hostedfor} );
    } else {
        ret = genericHelper::executeAddonHexchat( QStringList{_streamer} );
    }

    if (ret != 0) {
        QMessageBox::critical(this, genericHelper::getAppName(),
                                       tr("Can't find HexChat, please check options."),
                                       QMessageBox::Ok);
    }
}

void tpMainWindow::addBookmarkHosted()
{

       QString _channel;
       QString _hostedfor;
       _channel = this->ui->tableView->selectionModel()->selectedRows(0).at(0).data().toString();
       _hostedfor = this->twitchChannels[_channel]->getHostedChannel();

       if (twitchUserLocal->getBookmarks().count(_hostedfor) <= 0) {
           twitchUserLocal->addBookmark(_hostedfor);
       }

       this->ui->tabWidget->setCurrentIndex(1);

}

void tpMainWindow::deleteBookmark()
{
   const QString _streamer = this->ui->tableViewBookmarks->selectionModel()->selectedRows(0).at(0).data().toString();

   twitchUserLocal->deleteBookmark(_streamer);
}

void tpMainWindow::addBookmark()
{
    QString text = QInputDialog::getText(this, tr("Add Bookmark"), tr("Enter Channel URL or name"), QLineEdit::Normal,"");
    if (!text.isEmpty()) {
        twitchUserLocal->addBookmark(genericHelper::streamURLParser(text));
    }
}

void tpMainWindow::addFollow()
{
    QString _text = QInputDialog::getText(this, tr("Follow Channel"), tr("Enter Channel URL or name"), QLineEdit::Normal,"");
    QString _streamer = genericHelper::streamURLParser(_text);
    if (!_streamer.isEmpty()) {
        twitchUser->followChannel(_streamer);

        this->ui->statusBar->showMessage(tr("Sent follow request for channel") + " " + _streamer, DEFSTATUSTIMEOUT);
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

#ifdef WINTERNALVLC
void tpMainWindow::executeInternalPlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute, QString quality)
{
    diaVideoPlayers.push_back(new DialogVideoPlayer);
    diaVideoPlayers.last()->initVLC();

    if(diaVideoPlayers.last()->isInit()) {
        diaVideoPlayers.last()->setTitle(channel);
        diaVideoPlayers.last()->openUrl(url);
        diaVideoPlayers.last()->show();
    } else {
        QMessageBox::critical(this, genericHelper::getAppName(),
                                       tr("Error initializing internal VLC player! Maybe missing VLC plugins.\nTry switching to external VLC player in options."),
                                       QMessageBox::Ok);
    }
}
#endif

void tpMainWindow::executePlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute, QString quality)
{


//    playerq = new VideoPlayer();
//    playerq->resize(320, 240);
//    playerq->show();
//    playerq->setStreamUrl(url);
//    return;



#ifdef WINTERNALVLC
    if( genericHelper::getInternalVLC() ) {
        this->executeInternalPlayer(player, url, channel, streamWidth, streamHeight, xOffset, yOffset, mute, quality);
        return;
    }
#endif

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


    //url.append("'");
    //url.prepend("'");


    args << url;
    qresBin = QCoreApplication::applicationFilePath().replace(genericHelper::getAppName()+".exe","qres.exe");

    if (player == "vlc2") {
        playerBin = genericHelper::getVlcPath();
    }

    qresargs << "-e" << "\""+playerBin+"\"";
    qresargs << "-u" << args.join(" ");
    QFile playerBinFile( playerBin );
    QFile qresBinFile( qresBin );

    if(! playerBinFile.exists()) {
            QMessageBox::critical(this, genericHelper::getAppName(),
                                           tr("Can't find VLC, please check options."),
                                           QMessageBox::Ok);
        return ;
    }

    if(! qresBinFile.exists() && genericHelper::getStreamPositioning() ) {
            QMessageBox::critical(this, genericHelper::getAppName(),
                                           tr("Can't find qres, please check options."),
                                           QMessageBox::Ok);
        return ;
    }

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

    //genericHelper::log("player or qres binary not found, not starting: "+qresBin+" "+qresargs.join(" "));
    this->playerThreads.append(processLaunchThread);
}

void tpMainWindow::loadNew(const QString game, const QString url) {
    emit setStreamUrl(url);
}

void tpMainWindow::loadQualityNew(const QString game, const QMap<QString, QString> qualityUrls)
{
    emit setStreamUrlWithQuality(qualityUrls);
}

void tpMainWindow::updateFromJsonResponseFollows(const QJsonDocument &jsonResponseBuffer)
{
    /*
    QJsonObject jsonObject = jsonResponseBuffer.object();

    for(QJsonObject::const_iterator iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)  {
        if (iter.key() == "follows")
        {
            for (int i = 0; i <= iter.value().toArray().size(); i++)
            {
                QJsonValue _val = iter.value().toArray().at(i);

                QString channelName = _val.toObject()["channel"].toObject()["name"].toString();

                if(!this->twitchChannels.contains(channelName)) {
                    genericHelper::log(QString(__func__) + QString(" Adding TwitchChannel instance for channel ") + channelName);
                    genericHelper::log(QString(__func__) + QString(" TODO: NEEDS DELETE HANDLING"));
                    TwitchChannel *twitchChannel = new TwitchChannel(this, genericHelper::getOAuthAccessToken(), channelName, this->updateInterval);
                    this->twitchChannels[channelName] = twitchChannel;
                    QObject::connect(twitchChannel, SIGNAL(twitchChannelDataChanged(const bool)), this, SLOT(twitchChannelDataChanged(const bool)));
                }

                QString channelname = _val.toObject()["channel"].toObject()["name"].toString();

                if (this->stmodel->findItems(channelname, Qt::MatchExactly,0).length() <= 0) {

                    if (channelname.length() > 0) {

                        QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(channelname));
                        stmodel->setItem(i, 0, qsitem0);
                        QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg("offline"));
                        stmodel->setItem(i, 1, qsitem1);
                        QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(""));
                        stmodel->setItem(i, 2, qsitem2);
                        QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(""));
                        stmodel->setItem(i, 3, qsitem3);
                        QStandardItem *qsitem4 = new QStandardItem(QString("%0").arg(_val.toObject()["channel"].toObject()["status"].toString()));
                        stmodel->setItem(i, 4, qsitem4);

                        genericHelper::addFollow(channelname);
                    }
                }
            }
        }

    }

    fitTableViewToContent(this->ui->tableView);

    //this->statusBar()->showMessage("Following ("+QString::number(this->ui->treeWidget->topLevelItemCount())+")  Bookmarked ("+QString::number(this->ui->treeWidgetBookmarks->topLevelItemCount())+")");
    */
}

void tpMainWindow::onTwitchFollowedChannelsDataChanged(const bool &dataChanged)
{
    this->twitchChannels = twitchUser->getFollowedChannels();

    QMap<QString, TwitchChannel*>::iterator i = this->twitchChannels.begin();

    int y = 0;
    while (i != this->twitchChannels.end()) {
        TwitchChannel *twitchChannel = i.value();
        QObject::connect(twitchChannel, SIGNAL(twitchChannelDataChanged(const bool)), this, SLOT(twitchChannelDataChanged(const bool)));
        QObject::connect(twitchChannel, SIGNAL(TwitchChannelPlaylistUrlReady(const QString, const QString)),
                         this, SLOT(loadNew(const QString, const QString)));
        QObject::connect(twitchChannel, SIGNAL(twitchChannelQualityUrlsReady(const QString, const QMap<QString, QString>)),
                         this, SLOT(loadQualityNew(const QString, const QMap<QString, QString>)));

        if (this->stmodel->findItems(twitchChannel->getChannelName(), Qt::MatchExactly,0).length() <= 0) {

            if (twitchChannel->getChannelName().length() > 0) {

                QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelName()));
                stmodel->setItem(y, 0, qsitem0);
                QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg("offline"));
                stmodel->setItem(y, 1, qsitem1);
                QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(""));
                stmodel->setItem(y, 2, qsitem2);
                QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(""));
                stmodel->setItem(y, 3, qsitem3);
                QStandardItem *qsitem4 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelTitle()));
                stmodel->setItem(y, 4, qsitem4);


            }
        }

        ++y;
        ++i;
    }

}

void tpMainWindow::onTwitchBookmarkedChannelsDataChanged(const bool &dataChanged)
{
    QStringList _twitchChannelList;
   
    this->twitchChannelsBookmarks = twitchUserLocal->getBookmarkedChannels();

    QMap<QString, TwitchChannel*>::iterator i = this->twitchChannelsBookmarks.begin();

    int y = 0;
    while (i != this->twitchChannelsBookmarks.end()) {
        TwitchChannel *twitchChannel = i.value();
        QObject::connect(twitchChannel, SIGNAL(twitchChannelDataChanged(const bool)), this, SLOT(twitchChannelDataChanged(const bool)));
        QObject::connect(twitchChannel, SIGNAL(TwitchChannelPlaylistUrlReady(const QString, const QString)),
                         this, SLOT(loadNew(const QString, const QString)));
        QObject::connect(twitchChannel, SIGNAL(twitchChannelQualityUrlsReady(const QString, const QMap<QString, QString>)),
                         this, SLOT(loadQualityNew(const QString, const QMap<QString, QString>)));

            _twitchChannelList << twitchChannel->getChannelName();


            if (this->stmodelbookmarks->findItems(twitchChannel->getChannelName(),Qt::MatchExactly,0).length() <= 0) {
                QStandardItem *qsitem0 = new QStandardItem(QString("%0").arg(twitchChannel->getChannelName()));
                stmodelbookmarks->setItem(y, 0, qsitem0);
                QStandardItem *qsitem1 = new QStandardItem(QString("%0").arg("offline"));
                stmodelbookmarks->setItem(y, 1, qsitem1);
                QStandardItem *qsitem2 = new QStandardItem(QString("%0").arg(""));
                stmodelbookmarks->setItem(y, 2, qsitem2);
                QStandardItem *qsitem3 = new QStandardItem(QString("%0").arg(""));
                stmodelbookmarks->setItem(y, 3, qsitem3);
                QStandardItem *qsitem4 = new QStandardItem(QString("%0").arg(""));
                stmodelbookmarks->setItem(y, 4, qsitem4);
            } 
        

        ++y;
        ++i;
    }
    
    for(int y = 0; y<this->stmodelbookmarks->rowCount(); ++y) {
        QModelIndex streamer_index = this->stmodelbookmarks->index(y,0);


        if ( _twitchChannelList.count(this->stmodelbookmarks->itemData(streamer_index)[0].toString()) <= 0 )  {
            this->stmodelbookmarks->removeRow(streamer_index.row());
            break;
        }
    }
    


}

bool tpMainWindow::bunchUpdateStreamDataName(const QString &name, const QString &status,
                                             const QString &viewers)
{
    bool ret=true;
    if(! stproxymodel->updateCol(0, name.toLower(), 1, status) &&
         stproxymodelbookmarks->updateCol(0, name.toLower(), 1, status) )
        ret=false;
    if(! stproxymodel->updateCol(0, name.toLower(), 2, viewers) &&
         stproxymodelbookmarks->updateCol(0, name.toLower(), 2, viewers) )
        ret=false;
    return ret;
}

void tpMainWindow::updateFromJsonResponseFollow(const QJsonDocument &jsonResponseBuffer)
{
   // QJsonObject jsonObject = jsonResponseBuffer.object();

    // TODO: Test the response

    //this->loadData();
}

void tpMainWindow::updateFromJsonResponseUnfollow(const QJsonDocument &jsonResponseBuffer)
{
    /*
    QJsonObject jsonObject = jsonResponseBuffer.object();

    refreshTimer->stop();

    // TODO: Test the response
    QString _name;

    while(! followerToRemove.isEmpty() ) {
        _name = followerToRemove.takeLast();

        deleteFollowerFromList(_name);
    }

    refreshTimer->start(updateInterval);
    */
}

void tpMainWindow::updateOnUnfollow(QString msg)
{
    QString _name;

    while(! followerToRemove.isEmpty() ) {
        _name = followerToRemove.takeLast();

        deleteFollowerFromList(_name);
    }

    //refreshTimer->start(updateInterval);

}

void tpMainWindow::on_actionSetup_Twitch_Auth_triggered()
{
    qDebug() << "on_actionSetup_Twitch_Auth_triggered";
    if (diaOauthSetup->getDialogShown() == true)
    {
        diaOauthSetup->close();

        diaOauthSetup->show();

    } else {


        diaOauthSetup->show();
        diaOauthSetup->setDialogShown();
    }
}


void tpMainWindow::onBrowserAuthorizeRequested()
{
    
    QString link = "https://api.twitch.tv/kraken/oauth2/authorize?response_type=token&client_id="+this->twitchUser->getTwitchClientId()+"&redirect_uri=http://oauth.abyle.org/&scope=channel_editor+user_read+user_subscriptions+user_follows_edit+chat_login+channel_read";
    QDesktopServices::openUrl(QUrl(link));
}

//void tpMainWindow::onIsAuthOkCheck(QString newOAuthToken)
//{
//    twitchUser->setOAuthToken(newOAuthToken);  
//}

void tpMainWindow::showOnStatusBar(const QString errorMsg)
{
      this->ui->statusBar->showMessage(errorMsg, DEFSTATUSTIMEOUT);
}

void tpMainWindow::on_updateNotify(const QString &latestVersion)
{
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
    
#ifdef INTERNALIRC
    qDebug() << "internalirc";
    ircc = new IrcClient(0,"irc.twitch.tv",genericHelper::getUsername(),"oauth:"+genericHelper::getOAuthAccessToken());
    
    ircc->connectAndJoin(twitchUser->getFollowedChannelsList());
    ircc->resize(800, 480);
    ircc->show();

#else
    int ret = genericHelper::executeAddonHexchat(genericHelper::getFollows());
    if (ret != 0) {
        QMessageBox::critical(this, genericHelper::getAppName(),
                                       tr("Can't find HexChat, please check options."),
                                       QMessageBox::Ok);
    }
    
#endif
}

void tpMainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick) {
        this->show();
        this->raise();
        this->activateWindow();
        if( this->isMinimized() )
            this->showNormal();
    }
}

void tpMainWindow::on_notifyByTray(QString title, QString message)
{
    if( trayIcon->isVisible() ) {
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
    //this->loadData();
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
#ifdef WINTERNALVLC
                            "VLC-Qt made by <a href=http://vlc-qt.tano.si title=Tadej Novak>Tadej Novak</a></br>"
#endif
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

void tpMainWindow::on_actionShow_Approximate_Viewer_Count_toggled(bool arg1)
{
    genericHelper::setShowApproximateViewerCount(arg1);

    this->stproxymodel->setShowApproximateViewerCount(arg1);
    stproxymodel->setSourceModel(stmodel);

    this->stproxymodelbookmarks->setShowApproximateViewerCount(arg1);
    stproxymodelbookmarks->setSourceModel(stmodelbookmarks);
}

void tpMainWindow::on_Ready()
{
    /*
    refreshTimer->start(updateInterval);
    this->enableInput();
    */
}

void tpMainWindow::on_SwitchInputEnabled(bool enable)
{
    /*if (enable == true) {
        this->enableInput();
    } else {
        this->disableInput();
    }
    */
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

        tableviewContextMenu->addSeparator();
        tableviewContextMenu->addAction(copy_streamurl);
        tableviewContextMenu->addSeparator();
        tableviewContextMenu->addAction(add_follow);
        tableviewContextMenu->addAction(delete_follower);

    }
}

void tpMainWindow::prepareDiaLauncher()
{
    if (this->diaLaunch->getDialogShown() == true) {
        this->diaLaunch->close();
        this->diaLaunch->show();
    } else {
        this->diaLaunch->show();
        this->diaLaunch->setDialogShown();
    }
}

void tpMainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString _streamer;
    QString _status;

   _status = this->stproxymodel->data(index.sibling(index.row(),1),0).toString();
   _streamer = this->stproxymodel->data(index.sibling(index.row(),0),0).toString();

    if (genericHelper::isOnline(_status)) {
        if (launchBookmarkEnabled == true) {
            TwitchChannel *channel = twitchChannels[_streamer];
            if(channel != 0) {
                channel->requestPlaylist();

                prepareDiaLauncher();

                emit setStreamTitle( _streamer, "" );
                emit setStreamLogoUrl(channel->getChannelLogoUrl());
            }
        }
    }

    if (genericHelper::isHosting(_status)) {
        if (launchBookmarkEnabled == true) {
            QString _hostedStreamer = this->twitchChannels[_streamer]->getHostedChannel();
            TwitchChannel *channel = twitchChannels[_streamer];
            if(channel != 0) {
                channel->requestHostedPlaylist();

                prepareDiaLauncher();

                emit setStreamTitle( _streamer, "" );
                emit setStreamLogoUrl(channel->getChannelLogoUrl());
            }
        }
    }
}

void tpMainWindow::on_tableViewBookmarks_activated(const QModelIndex &index)
{
    QString _streamer;
    QString _status;

   _status = this->stproxymodelbookmarks->data(index.sibling(index.row(),1),0).toString();
   _streamer = this->stproxymodelbookmarks->data(index.sibling(index.row(),0),0).toString();

    if (genericHelper::isOnline(_status)) {
        if (launchBookmarkEnabled == true) {
            TwitchChannel *channel = twitchChannels[_streamer];
            if(channel != 0) {
                channel->requestPlaylist();

                prepareDiaLauncher();

                emit setStreamTitle( _streamer, "" );
                emit setStreamLogoUrl(channel->getChannelLogoUrl());
            }
        }
    }

    if (genericHelper::isHosting(_status)) {
        if (launchBookmarkEnabled == true) {
            QString _hostedStreamer = this->twitchChannels[_streamer]->getHostedChannel();
            TwitchChannel *channel = twitchChannels[_streamer];
            if(channel != 0) {
                channel->requestHostedPlaylist();

                prepareDiaLauncher();

                emit setStreamTitle( _streamer, "" );
                emit setStreamLogoUrl(channel->getChannelLogoUrl());
            }
        }
    }
}

void tpMainWindow::on_tableViewBookmarks_customContextMenuRequested(const QPoint &pos)
{
     QMenu *tableviewbookmarksContextMenu = new QMenu("Bookmarks", this);
     tableviewbookmarksContextMenu->addAction(add_bookmark);
     if ((this->ui->tableViewBookmarks->selectionModel()->selectedRows().count() > 0) || (this->stmodelbookmarks->rowCount() <= 0)) {
        tableviewbookmarksContextMenu->addAction(delete_bookmark);
        if ((this->stmodelbookmarks->rowCount() > 0)) {
            tableviewbookmarksContextMenu->addAction(open_in_hexchat_bookmark);
        }

        tableviewbookmarksContextMenu->addSeparator();
        tableviewbookmarksContextMenu->addAction(copy_streamurl);
        tableviewbookmarksContextMenu->addSeparator();
        tableviewbookmarksContextMenu->addAction(add_follower_bookmark);
    }
     tableviewbookmarksContextMenu->popup(this->ui->tableView->viewport()->mapToGlobal(pos));
}

void tpMainWindow::on_actionLogfile_triggered()
{
     //genericHelper::openLogWithNotepad();
    diaShowLogFile->loadFile(genericHelper::getLogFile());
    diaShowLogFile->show();
}

void tpMainWindow::on_tabWidget_currentChanged(int index)
{
    genericHelper::setSelectedTab(index);
}

void tpMainWindow::on_actionFit_Columns_to_Content_triggered()
{
    if(! genericHelper::getFitAllContentToWindow() ) {
        this->ui->tableView->resizeColumnsToContents();
        this->ui->tableViewBookmarks->resizeColumnsToContents();
    }
}

void tpMainWindow::twitchApiNetworkError(QString error)
{
    genericHelper::log("twitch-api " + error);
}


void tpMainWindow::deleteFollowerFromList(QString _name)
{
    //this->stproxymodel->deleteCol(0, _name );
    //this->loadData();
}

void tpMainWindow::on_actionShow_Game_Browser_triggered()
{
    diaTopGameBrowser->show();
}
