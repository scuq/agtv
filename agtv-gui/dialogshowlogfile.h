#ifndef DIALOGSHOWLOGFILE_H
#define DIALOGSHOWLOGFILE_H

#include <QDialog>
#include <QFile>
#include "generichelper.h"

namespace Ui {
class DialogShowLogFile;
}

class DialogShowLogFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowLogFile(QWidget *parent = 0);
    ~DialogShowLogFile();

    void loadFile(const QString filename);

public slots:
    void reloadFile();

private slots:
    void on_lineEditFilter_textChanged(const QString &arg1);



private:
    Ui::DialogShowLogFile *ui;

    QFile file;
    QString fileName;

    void closeEvent(QCloseEvent *); // Overriding the window's close event
};

#endif // DIALOGSHOWLOGFILE_H
