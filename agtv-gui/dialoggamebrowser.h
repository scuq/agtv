#ifndef DIALOGGAMEBROWSER_H
#define DIALOGGAMEBROWSER_H

#include <QDialog>
#include <QClipboard>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSystemTrayIcon>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QTableView>
#include <QTreeWidgetItem>
#include <QThread>

#include "advqsortfilterproxylistmodel.h"
#include "agtvdefaultitemdelegate.h"
#include "twitchapi.h"
#include "htmldelegate.h"

namespace Ui {
class DialogGameBrowser;
}

class DialogGameBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGameBrowser(QWidget *parent = 0);
    ~DialogGameBrowser();

    void showEvent(QShowEvent *e);

private slots:
    void updateFromJsonResponseTopGames(const QJsonDocument &jsonResponseBuffer);
    void updateFromJsonResponseStreamsForGame(const QJsonDocument &jsonResponseBuffer);

    void on_pushButtonClose_clicked();

    void on_tableViewTopGames_activated(const QModelIndex &index);

    void onTableViewGamesScrolled(int);

    void on_tableViewGame_activated(const QModelIndex &index);

    void on_lineEditFilterGames_textChanged(const QString &arg1);

signals:
    void startStream( const QString streamer );

private:
    Ui::DialogGameBrowser *ui;

    TwitchApi *tw;

    QStandardItemModel *stmodelTopGames;
    AdvQSortFilterProxyListModel *stproxymodelTopGames;
   //  AgtvDefaultItemDelegate *agtvDefItemDelegate;
    HTMLDelegate *htmlDelegate;

    QStandardItemModel *stmodelGame;
    AdvQSortFilterProxyListModel *stproxymodelGame;

    int offset;
};

#endif // DIALOGGAMEBROWSER_H
