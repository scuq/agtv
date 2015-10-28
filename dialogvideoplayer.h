#ifndef DIALOGVIDEOPLAYER_H
#define DIALOGVIDEOPLAYER_H

#include <QDialog>

#ifdef WINTERNALVLC
#include <VLCQtCore/Audio.h>
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

    void initVLC();

    void openLocal(QString pathtovideo);
    void openUrl(QString url);

    void setTitle(QString channel);

private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonReload_clicked();

    void on_pushButtonStartResume_clicked();

    void on_pushButtonStop_clicked();

    void on_horizontalSliderVolume_valueChanged(int value);

    void onStopped();
    void onStarted();
    void onPaused();

    void on_pushButtonMute_clicked();

private:
    enum MediaType{
        MediaFile,
        MediaUrl,
        MediaNone
    };

    enum RunStatus{
        Running,
        Paused,
        Stopped
    };

    int oldVolume;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    VlcAudio *_vlcAudio;

    Ui::DialogVideoPlayer *ui;

    void closeEvent(QCloseEvent *event);

    QString fileurl;
    MediaType mediaType;

    bool isPaused, isStarted;

    void setEnabledAllButtons(bool enabled);
    void changeUi(RunStatus status);

};

#endif // DIALOGVIDEOPLAYER_H
