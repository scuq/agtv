#ifndef DIALOGGAMESTATS_H
#define DIALOGGAMESTATS_H

#include <QDialog>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

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

public slots:
    void updateGameStatsFromJsonResponse(const QJsonObject &jsonObject);
private slots:
    void on_pushButtonClose_clicked();
};

#endif // DIALOGGAMESTATS_H
