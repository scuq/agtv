#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
#include <QTextStream>
#include <QFile>

namespace Ui {
class dialogAbout;
}

class dialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit dialogAbout(QWidget *parent = 0);
    ~dialogAbout();
    void setDialogShown();
    bool getDialogShown();

private:
    Ui::dialogAbout *ui;
    bool dialogShown;
    QString version;
    QString getChangelog();
    QString getTodo();
};

#endif // DIALOGABOUT_H
