#ifndef DIALOGPOSITIONER_H
#define DIALOGPOSITIONER_H

#include <QDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include "generichelper.h"

namespace Ui {
class DialogPositioner;
}

class DialogPositioner : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPositioner(QWidget *parent = 0);
    ~DialogPositioner();
    void setDialogShown();
    bool getDialogShown();

private slots:
    void on_pushButtonCurrentPos_clicked();

    void on_pushButtonSavePos_clicked();

    void on_pushButtonLoadPos_clicked();

    void on_pushButtonDeletePos_clicked();

private:
    Ui::DialogPositioner *ui;
    bool dialogShown;
    QString currentGeo;
    void successPopup(QString message);
    void errorPopup(QString message);
    void loadPositions();
    void setGeoLineEdit(QString geo);
    QDesktopWidget *desktop;
};

#endif // DIALOGPOSITIONER_H
