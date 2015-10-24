#ifndef DIALOGSHOWLOGFILE_H
#define DIALOGSHOWLOGFILE_H

#include <QDialog>
#include <QFile>

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

private:
    Ui::DialogShowLogFile *ui;

    QFile file;
    QString fileName;
};

#endif // DIALOGSHOWLOGFILE_H
