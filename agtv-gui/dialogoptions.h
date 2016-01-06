#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>
#include <QCloseEvent>
#include "generichelper.h"

namespace Ui {
class DialogOptions;
}

class DialogOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOptions(QWidget *parent = 0);
    ~DialogOptions();
    void setDialogShown();
    bool getDialogShown();
    void refreshUiData();

signals:
    void settingsSaved();

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

    void on_vlcPathSelectButton_clicked();

    void on_hexchatPathSelectButton_clicked();

    void on_checkBoxInternalVLC_toggled(bool checked);

private:
    Ui::DialogOptions *ui;
        bool dialogShown;

};

#endif // DIALOGOPTIONS_H
