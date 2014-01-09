#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include "generichelper.h"

namespace Ui {
class dialogSettings;
}

class dialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dialogSettings(QWidget *parent = 0);
    ~dialogSettings();
    void setDialogShown();
    bool getDialogShown();


private slots:
    void on_pushButtonApply_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::dialogSettings *ui;
    bool dialogShown;

signals:
void valueChanged();

};

#endif // DIALOGSETTINGS_H
