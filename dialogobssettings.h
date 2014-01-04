#ifndef DIALOGOBSSETTINGS_H
#define DIALOGOBSSETTINGS_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "generichelper.h"

namespace Ui {
class dialogOBSSettings;
}

class dialogOBSSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dialogOBSSettings(QWidget *parent = 0);
    ~dialogOBSSettings();
    void setDialogShown();
    bool getDialogShown();

private slots:
    void on_pushButtonBrowseOBS_clicked();

    void on_pushButtonApply_clicked();

    void on_pushButtonClose_clicked();

    void on_checkBox_clicked();

private:
    Ui::dialogOBSSettings *ui;
    bool dialogShown;


};

#endif // DIALOGOBSSETTINGS_H
