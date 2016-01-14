#include "tpmainwindow.h"
#include "ui_tpmainwindow.h"

#define DEFSTATUSTIMEOUT 5000

tpMainWindow::tpMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::tpMainWindow)
{
    ui->setupUi(this);

    genericHelper::log(QString("Starting AGTV..."));

    currArch = CURRARCH;
    version = VERSION;

    // delete logfile if clear log on startup setting is true
    if (genericHelper::getClearLogOnStartup() == true) {
        genericHelper::deleteLog();
    }

    restoreGeometry(genericHelper::getGeometry("main").toByteArray());
    restoreState(genericHelper::getWindowstate("main").toByteArray());

    this->ui->tabWidget->setCurrentIndex(genericHelper::getSelectedTab());

    xOffset = 0; yOffset = 0;

    launchBookmarkEnabled = true;

    // Set a minimum of 5 seconds for the update interval
    if (genericHelper::getUpdateInterval() < 5) {
        genericHelper::setUpdateInterval(5);
    }

    this->setupModelsViews();
    this->setupTwitchApi();
    this->setupDialogs();
    this->setupSignalsMain();
    this->setupUpdateCheck();
    this->setupTrayIcon();
    this->setupSignalsTwitchApi();

    twitchUser->checkAuthenticationSetup();

    twitchUserLocal->getStoredOAuthAccessToken();
    twitchUserLocal->loadBookmarks();
    if (!twitchUserLocal->isUserSetupOk()) {
        this->setEnableInput(false);
    }

    QDesktopWidget *desktop = QApplication::desktop();
    genericHelper::setPrimaryScreenWidth(desktop->screenGeometry(-1).width());

#ifdef INTERNALIRC
    if(genericHelper::getInternalChat()) {
        ircc = new IrcClient(0,"irc.twitch.tv",genericHelper::getUsername(),"oauth:"+genericHelper::getOAuthAccessToken());
    } else {
        ircc = 0;
    }
#endif
    
    clipboard = QApplication::clipboard();
}

void tpMainWindow::setupSignalsTwitchApi()
{
    QObject::connect(twitchUserLocal, SIGNAL(oAuthAccessTokenLoaded(QString)), diaOauthSetup, SLOT(setCurrentStoredAuthToken(QString)));
    QObject::connect(diaOauthSetup, SIGNAL(saveAuthTokenRequested(QString)), twitchUserLocal, SLOT(onSaveOAuthAccessToken(QString)));
    QObject::connect(twitchUser, SIGNAL(newUsernameDetected(QString)), twitchUserLocal, SLOT(onSaveUsername(QString)));

    // QObject::connect(twitchUserLocal, SIGNAL(twitchBookmarkedChannelsDataChanged(const bool)), this, SLOT(onTwitchBookmarkedChannelsDataChanged(const bool)));
    QObject::connect(twitchUser, SIGNAL(authCheckSuccessfull()), diaOauthSetup, SLOT(onAuthOk()));
    QObject::connect(twitchUser, SIGNAL(authCheckFailed()), diaOauthSetup, SLOT(onAuthNok()));
}

void tpMainWindow::setupUpdateCheck()
{
    uc = new updateCheck(this);
    QObject::connect(uc, SIGNAL(updateReady(const QString)), this, SLOT(on_updateNotify(const QString)));
    if (genericHelper::getCheckUpdate() == true) {
        uc->getCheck();
    }
}

void tpMainWindow::setupTrayIcon()
{
    createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();
}

void tpMainWindow::setupDialogs()
{
    diaOauthSetup = new dialogOauthSetup(this);
    QObject::connect(diaOauthSetup, SIGNAL(onAuthorizeRequested()), this, SLOT(onBrowserAuthorizeRequested()));
    QObject::connect(diaOauthSetup, SIGNAL(authTokenChanged(QString)), twitchUser, SLOT(validateNewAuthToken(QString)));
    //QObject::connect(diaOauthSetup, SIGNAL(twitchAuthSetupChanged(bool)), this, SLOT(on_SwitchInputEnabled(bool)));

    diaPositioner = new DialogPositioner(this);

    diaLaunch = new DialogLaunch(this);
    QObject::connect(diaLaunch, SIGNAL(startStreamPlay(QString, QString, QString, int, int, int , int, bool, QString)), this, SLOT(executePlayer(QString, QString, QString, int, int, int , int, bool, QString)));

    diaOptions = new DialogOptions(this);
    QObject::connect(this->diaOptions, SIGNAL(settingsSaved()), this, SLOT(on_settingsSaved()));

    diaShowLogFile = new DialogShowLogFile(this);

    diaTopGameBrowser = new DialogGameBrowser;
    QObject::connect(diaTopGameBrowser, SIGNAL(startStream(const QString)), this, SLOT(startFromGBrowser(const QString)));
}

void tpMainWindow::setupSignalsMain()
{
    QObject::connect(this->ui->lineEditFilter, SIGNAL(textChanged(QString)), this->stproxymodel, SLOT(setFilterRegExp(QString)));
    QObject::connect(this->ui->lineEditFilterBookmark, SIGNAL(textChanged(QString)), this->stproxymodelbookmarks, SLOT(setFilterRegExp(QString)));
    QObject::connect(this->ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabChanged(const int)));
}

void tpMainWindow::setupTwitchApi()
{
    twitchUserLocal = new TwitchUserLocal(this, genericHelper::getUpdateIntervalMsec());
    QObject::connect(twitchUserLocal, SIGNAL(twitchBookmarkedChannelsDataChanged(const bool)), this, SLOT(onTwitchBookmarkedChannelsDataChanged(const bool)));

    twitchUser = new TwitchUser(this,twitchUserLocal->getStoredOAuthAccessToken(),genericHelper::getUsername(), 5*genericHelper::getUpdateIntervalMsec());
    QObject::connect(twitchUser, SIGNAL(twitchNeedsOAuthSetup()), this, SLOT(on_actionSetup_Twitch_Auth_triggered()));
    QObject::connect(twitchUser, SIGNAL(twitchFollowedChannelsDataChanged(const bool)), this, SLOT(onTwitchFollowedChannelsDataChanged(const bool)));
    QObject::connect(twitchUser, SIGNAL(twitchFollowChannelError(const QString)), this, SLOT(showOnStatusBar(const QString)));
    QObject::connect(twitchUser, SIGNAL(twitchUnfollowChannelSuccess(const QString)), this, SLOT(updateOnUnfollow(const QString)));
    QObject::connect(twitchUser, SIGNAL(twitchUnfollowChannelError(const QString)), this, SLOT(showOnStatusBar(const QString)));
}

void tpMainWindow::setupModelsViews()
{
    AgtvDefItemDelegate =  new AgtvDefaultItemDelegate();

    QStringList horzHeaders = { "Name", "Status", "#V", "#F",
                                "Game", "Status Message"};

    stmodel = new TwitchChannelModel(this, genericHelper::getUpdateIntervalMsec());
    stmodel->setHorizontalHeaderLabels(horzHeaders);
    QObject::connect(stmodel, SIGNAL(notifyByTray(QString, QString)),
                     this, SLOT(on_notifyByTray(QString, QString)));
    stproxymodel = new AdvQSortFilterProxyModel(this);
    stproxymodel->setSourceModel(stmodel);
    stproxymodel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->ui->tableView->horizontalHeader()->show();
    this->ui->tableView->setItemDelegate(AgtvDefItemDelegate);
    this->ui->tableView->setModel(stproxymodel);

    stmodelbookmarks = new TwitchChannelModel(this, genericHelper::getUpdateIntervalMsec());
    QObject::connect(stmodelbookmarks, SIGNAL(notifyByTray(QString, QString)),
                     this, SLOT(on_notifyByTray(QString, QString)));
    stmodelbookmarks->setHorizontalHeaderLabels(horzHeaders);
    stproxymodelbookmarks = new AdvQSortFilterProxyModel(this);
    stproxymodelbookmarks->setSourceModel(stmodelbookmarks);
    stproxymodelbookmarks->setFilterCaseSensitivity(Qt::CaseInsensitive);

    this->ui->tableViewBookmarks->horizontalHeader()->show();
    this->ui->tableViewBookmarks->setItemDelegate(AgtvDefItemDelegate);
    this->ui->tableViewBookmarks->setModel(stproxymodelbookmarks);

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

    if(! genericHelper::getShowFollowerCount()) {
        this->ui->tableView->hideColumn((qint64)TwitchChannelModel::ColumnIndex::followers);
        this->ui->tableViewBookmarks->hideColumn((qint64)TwitchChannelModel::ColumnIndex::followers);
    }
    this->ui->actionShow_Follower_Count->setChecked(genericHelper::getShowFollowerCount());

    ui->tableView->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());
    ui->tableViewBookmarks->horizontalHeader()->setStretchLastSection(genericHelper::getFitAllContentToWindow());

    if(! genericHelper::getFitAllContentToWindow()) {
        QTimer::singleShot(0, this, SLOT(restoreTableViewsManual()));
    }

    QTimer::singleShot(0, this, SLOT(restoreSortModes()));
}

void tpMainWindow::startFromGBrowser(const QString stream)
{
    TwitchChannel *channel = this->stmodel->getChannel(stream);
    if(channel != 0) {
        channel->requestPlaylist();

        prepareDiaLauncher(stream, channel->getChannelLogoUrl());
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

    // Set a minimum of 5 seconds for the update interval
    if (genericHelper::getUpdateInterval() < 5) {
        genericHelper::setUpdateInterval(5);
    }
    this->stmodel->setUpdateInterval(genericHelper::getUpdateIntervalMsec());
    this->stmodelbookmarks->setUpdateInterval(genericHelper::getUpdateIntervalMsec());
    this->twitchUser->setInterval(5*genericHelper::getUpdateIntervalMsec());

#ifdef INTERNALIRC
    if(genericHelper::getInternalChat()) {
        if(ircc == 0) {
            ircc = new IrcClient(0,"irc.twitch.tv",genericHelper::getUsername(),"oauth:"+genericHelper::getOAuthAccessToken());
        }
    } else {
        if(ircc != 0) {
            ircc->deleteLater();
            ircc = 0;
        }
    }
#endif
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

void tpMainWindow::setEnableInput(bool enabled)
{
    QList<QWidget*> widgets = this->findChildren<QWidget*>();

    foreach (QWidget *widget, widgets) {
        if (    (widget->objectName() != "mainToolBar") &&
                (widget->objectName() != "menuBar") &&
                (widget->objectName() != "menuFile") &&
                (widget->objectName() != "pushButtonAuthorizeOnTwitch") &&
                (widget->objectName() != "lineEditOAuthToken") &&
                (widget->objectName() != "pushButtonTestOAuth") &&
                (widget->objectName() != "pushButtonREvoke") &&
                (widget->objectName() != "pushButtonSave") &&
                (widget->objectName() != "dialogOauthSetup")
        ) {
            widget->setEnabled(enabled);
        }
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


    open_in_hexchat = new QAction(tr("Open &Chat"), this);
    connect(open_in_hexchat, SIGNAL(triggered()), this, SLOT(openChatHexChat()));

    open_in_hexchat_bookmark = new QAction(tr("Open &Chat"), this);
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
         QString _hostedfor = this->stmodel->getChannel(_streamer)->getHostedChannel();
         
#ifdef INTERNALIRC
         if(genericHelper::getInternalChat()) {
            ircc->connectAndJoin(QStringList{_streamer, _hostedfor});
            ircc->resize(800, 480);
            ircc->show();
         } else {
             ret = genericHelper::executeAddonHexchat( QStringList{_streamer, _hostedfor} );
         }
#else
            ret = genericHelper::executeAddonHexchat( QStringList{_streamer, _hostedfor} );
#endif
     } else { // genericHelper::isHosting(_status)
#ifdef INTERNALIRC
         if(genericHelper::getInternalChat()) {
             ircc->connectAndJoin(QStringList{_streamer});
             ircc->resize(800, 480);
             ircc->show();
         } else {
             ret = genericHelper::executeAddonHexchat( QStringList{_streamer} );
         }
#else
         ret = genericHelper::executeAddonHexchat( QStringList{_streamer} );
#endif
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
        QString _hostedfor = this->stmodelbookmarks->getChannel(_streamer)->getHostedChannel();
#ifdef INTERNALIRC
        if(genericHelper::getInternalChat()) {
            ircc->connectAndJoin(QStringList{_streamer, _hostedfor});
            ircc->resize(800, 480);
            ircc->show();
        } else {
            ret = genericHelper::executeAddonHexchat( QStringList{_streamer, _hostedfor} );
        }
#else
        ret = genericHelper::executeAddonHexchat( QStringList{_streamer, _hostedfor} );
#endif
    } else {
#ifdef INTERNALIRC
        if(genericHelper::getInternalChat()) {
            ircc->connectAndJoin(QStringList{_streamer});
            ircc->resize(800, 480);
            ircc->show();
        } else {
            ret = genericHelper::executeAddonHexchat( QStringList{_streamer} );
        }
#else
        ret = genericHelper::executeAddonHexchat( QStringList{_streamer} );
#endif        
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
       _hostedfor = this->stmodel->getChannel(_channel)->getHostedChannel();

       if (twitchUserLocal->getBookmarks().count(_hostedfor) <= 0) {
           twitchUserLocal->addBookmark(_hostedfor);
       }

       this->ui->tabWidget->setCurrentIndex(1);
}

void tpMainWindow::deleteBookmark()
{
   const QString _streamer = this->ui->tableViewBookmarks->selectionModel()->selectedRows(0).at(0).data().toString();

   twitchUserLocal->deleteBookmark(_streamer);
   this->stmodelbookmarks->removeChannel(_streamer);
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

void tpMainWindow::executeExternalPlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute, QString quality)
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
        if( (qresBinFile.exists()) && (playerBinFile.exists()) ) {
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
        if( playerBinFile.exists() ) {
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

void tpMainWindow::executePlayer(QString player, QString url, QString channel, int streamWidth, int streamHeight, int xOffset, int yOffset, bool mute, QString quality)
{
#ifdef WINTERNALVLC
    if( genericHelper::getInternalVLC() ) {
        this->executeInternalPlayer(player, url, channel, streamWidth, streamHeight, xOffset, yOffset, mute, quality);
        return;
    }
#endif

    this->executeExternalPlayer(player, url, channel, streamWidth, streamHeight, xOffset, yOffset, mute, quality);
}

void tpMainWindow::onTwitchFollowedChannelsDataChanged(const bool &dataChanged)
{
    QList<QString> currFollowedList;
    for(const auto& channel : twitchUser->getFollowedChannels()) {
        if (channel->getChannelName().length() > 0) {
            if(this->stmodel->addChannel(channel->getChannelName())) {
                QObject::connect(this->stmodel->getChannel(channel->getChannelName()), SIGNAL(TwitchChannelPlaylistUrlReady(const QString, const QString)),
                                 this->diaLaunch, SLOT(setStreamUrl(const QString, const QString)));
                QObject::connect(this->stmodel->getChannel(channel->getChannelName()), SIGNAL(twitchChannelQualityUrlsReady(const QString, const QMap<QString, QString>)),
                                 this->diaLaunch, SLOT(setStreamUrlWithQuality(const QString, const QMap<QString, QString>)));
            }
            currFollowedList.append(channel->getChannelName());
        }
    }

    QList<QString> storedFollowedList = this->stmodel->getChannelList();
    for(auto channel : currFollowedList) {
        if(!storedFollowedList.contains(channel)) {
            this->stmodel->removeChannel(channel);
        }
    }
}

void tpMainWindow::onTwitchBookmarkedChannelsDataChanged(const bool &dataChanged)
{
    for(const auto& channel : twitchUserLocal->getBookmarkedChannels()) {
        this->stmodelbookmarks->addChannel(channel->getChannelName());
        QObject::connect(this->stmodelbookmarks->getChannel(channel->getChannelName()), SIGNAL(TwitchChannelPlaylistUrlReady(const QString, const QString)),
                         this->diaLaunch, SLOT(setStreamUrl(const QString, const QString)));
        QObject::connect(this->stmodelbookmarks->getChannel(channel->getChannelName()), SIGNAL(twitchChannelQualityUrlsReady(const QString, const QMap<QString, QString>)),
                         this->diaLaunch, SLOT(setStreamUrlWithQuality(const QString, const QMap<QString, QString>)));
    }
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

void tpMainWindow::openHexchat() {
    int ret = genericHelper::executeAddonHexchat(genericHelper::getFollows());
    if (ret != 0) {
        QMessageBox::critical(this, genericHelper::getAppName(),
                                       tr("Can't find HexChat, please check options."),
                                       QMessageBox::Ok);
    }
}

void tpMainWindow::on_actionHexChat_triggered()
{
#ifdef INTERNALIRC   
    if(genericHelper::getInternalChat()) {
        ircc->connectAndJoin(twitchUser->getFollowedChannelsList());
        ircc->resize(800, 480);
        ircc->show();
    } else {
        this->openHexchat();
    }
#else
        this->openHexchat();
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
    if( trayIcon->isVisible() && genericHelper::getStreamOnlineNotify() ) {
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
                            "Icons made by <a href=http://www.freepik.com title=Freepik>Freepik</a> from <a href=http://www.flaticon.com title=Flaticon>www.flaticon.com</a><br>"
#ifdef WINTERNALVLC
                            "VLC-Qt made by <a href=http://vlc-qt.tano.si title=Tadej Novak>Tadej Novak</a><br>"
#endif
#ifdef INTERNALIRC
                            "libcommuni made by <a href=http://communi.github.io/ title=J-P Nurmi>J-P Nurmi</a>"
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

void tpMainWindow::prepareDiaLauncher(QString _streamer, QString _logoUrl)
{
    if (this->diaLaunch->getDialogShown() == true) {
        this->diaLaunch->close();
        this->diaLaunch->prepareStream(_streamer, _logoUrl);
        this->diaLaunch->show();
    } else {
        this->diaLaunch->show();
        this->diaLaunch->setDialogShown();
        this->diaLaunch->prepareStream(_streamer, _logoUrl);
    }
}

void tpMainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString _streamer = this->stproxymodel->data(index.sibling(index.row(),0),0).toString();

    TwitchChannel *channel = this->stmodel->getChannel(_streamer);
    if(channel != 0) {
        switch(channel->getChannelOnlineStatus()) {
            case TwitchChannel::ChannelOnlineStatus::online:
                channel->requestPlaylist();
                prepareDiaLauncher(_streamer, channel->getChannelLogoUrl());
                break;
            case TwitchChannel::ChannelOnlineStatus::hosting:
                channel->requestHostedPlaylist();
                prepareDiaLauncher(_streamer + QString(" is hosting ") + channel->getHostedChannel(), channel->getChannelLogoUrl());
                break;
            case TwitchChannel::ChannelOnlineStatus::playlist:
            default:
                break;
        }
    }
}

void tpMainWindow::on_tableViewBookmarks_activated(const QModelIndex &index)
{
   QString _streamer = this->stproxymodelbookmarks->data(index.sibling(index.row(),0),0).toString();

   TwitchChannel *channel = this->stmodelbookmarks->getChannel(_streamer);
   if(channel != 0) {
       switch(channel->getChannelOnlineStatus()) {
           case TwitchChannel::ChannelOnlineStatus::online:
               channel->requestPlaylist();
               prepareDiaLauncher(_streamer, channel->getChannelLogoUrl());
               break;
           case TwitchChannel::ChannelOnlineStatus::hosting:
               channel->requestHostedPlaylist();
               prepareDiaLauncher(_streamer + QString(" is hosting ") + channel->getHostedChannel(), channel->getChannelLogoUrl());
               break;
           case TwitchChannel::ChannelOnlineStatus::playlist:
           default:
               break;
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

void tpMainWindow::on_actionShow_Follower_Count_triggered(bool arg1)
{
    genericHelper::setShowFollowerCount(arg1);
    if(! genericHelper::getShowFollowerCount()) {
        this->ui->tableView->hideColumn((qint64)TwitchChannelModel::ColumnIndex::followers);
        this->ui->tableViewBookmarks->hideColumn((qint64)TwitchChannelModel::ColumnIndex::followers);
    } else {
        this->ui->tableView->showColumn((qint64)TwitchChannelModel::ColumnIndex::followers);
        this->ui->tableViewBookmarks->showColumn((qint64)TwitchChannelModel::ColumnIndex::followers);
    }
}
