#ifndef DIALOGGAMESTATS_H
#define DIALOGGAMESTATS_H

#include <QDialog>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QTreeWidgetItem>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class dialogGameStats;
}

class dialogGameStats : public QDialog
{
    Q_OBJECT

public:
    explicit dialogGameStats(QWidget *parent = 0);
    ~dialogGameStats();
    void setDialogShown();
    bool getDialogShown();

private:
    Ui::dialogGameStats *ui;
    bool dialogShown;
    QUrl gamebrowserurl;

public slots:
    void updateGameStatsFromJsonResponse(const QJsonObject &jsonObject);
private slots:
    void on_pushButtonClose_clicked();
    void on_treeWidgetGameStats_itemClicked(QTreeWidgetItem *item, int column);
    void on_treeWidgetGameStats_itemDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif // DIALOGGAMESTATS_H
