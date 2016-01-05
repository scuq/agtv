#ifndef DIALOGLAUNCH_H
#define DIALOGLAUNCH_H

#include <QDialog>
#include "generichelper.h"
#include "imageloader.h"

namespace Ui {
class DialogLaunch;
}

class DialogLaunch : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLaunch(QWidget *parent = 0);
    ~DialogLaunch();
    void setDialogShown();
    bool getDialogShown();
    void refreshUiData();


public slots:
    void setStreamTitle(QString streamtitle, QString position);
    void setStreamUrl(const QString _streamurl);
    void setStreamLogoUrl(QString streamlogourl);
    void setStreamUrlWithQuality(const QMap<QString, QString> streamurlqualitymap);

private slots:
    void on_pushButtonStart_clicked();

    void loadStreamLogoImage();

private:
    Ui::DialogLaunch *ui;
     bool dialogShown;
     void loadPositions();
     bool streamUrlSet;
     QString streamurl;
     int x;
     int y;
     int w;
     int h;

     imageLoader *imgl;
     QPixmap streamLogoImage;
     QString streamLogoUrl;

     QMap<QString, QString> streamurlqualitymap;

signals:
     void startStreamPlay(QString, QString, QString, int, int, int , int, bool, QString);


};

#endif // DIALOGLAUNCH_H
