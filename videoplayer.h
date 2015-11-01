#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QVideoWidget>
#include <QtGui/QMovie>
#include <QtWidgets/QWidget>

class QAbstractButton;
class QSlider;
class QLabel;

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();
    void setStreamUrl(QString url);

public slots:
    void openFile();
    void play();

private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);

    void handleError();

private:
    QMediaPlayer mediaPlayer;
    QAbstractButton *playButton;
    QSlider *positionSlider;
    QLabel *errorLabel;
    QMediaPlaylist *playlist;
};
#endif // VIDEOPLAYER_H
