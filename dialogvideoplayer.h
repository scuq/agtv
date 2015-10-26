#ifndef DIALOGVIDEOPLAYER_H
#define DIALOGVIDEOPLAYER_H

#include <QDialog>

#ifdef WINTERNALVLC
#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#endif

namespace Ui {
class DialogVideoPlayer;
}

class DialogVideoPlayer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVideoPlayer(QWidget *parent = 0);
    ~DialogVideoPlayer();

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    void initVLC();

    void openLocal(QString pathtovideo);
    void openUrl(QString url);

    void setTitle(QString channel);

private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonStart_clicked();

    void on_pushButtonPause_clicked();

private:
    enum MediaType{
        MediaFile,
        MediaUrl,
        MediaNone
    };

    Ui::DialogVideoPlayer *ui;

    void closeEvent(QCloseEvent *event);

    QString fileurl;
    MediaType mediaType;

    bool isPaused;

};

#endif // DIALOGVIDEOPLAYER_H
