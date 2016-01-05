#include "dialogvideoplayer.h"
#include "ui_dialogvideoplayer.h"

#include <QFileDialog>
#include <QString>
#include <QCloseEvent>

#include "generichelper.h"

DialogVideoPlayer::DialogVideoPlayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVideoPlayer)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags( flags );

    restoreGeometry(genericHelper::getGeometry("videodialog").toByteArray());
}

DialogVideoPlayer::~DialogVideoPlayer()
{
    delete ui;
}

void DialogVideoPlayer::initVLC()
{
    QStringList args = VlcCommon::args();
    args.append("--network-caching=5000");

    genericHelper::log("DialogVideoPlayer: Using VLC args = " + args.join(" "));

   _instance = new VlcInstance(args, this);
   if(&_instance == 0) {
       genericHelper::log("DialogVideoPlayer: Error creating VLC instance!");
       return;
   }
    _player = new VlcMediaPlayer(_instance);
    if(&_player == 0) {
        genericHelper::log("DialogVideoPlayer: Error creating VLC player!");
        return;
    }
    _player->setVideoWidget(this->ui->video);
    _vlcAudio = _player->audio();

    this->ui->horizontalSliderVolume->setValue(_vlcAudio->volume());

    QObject::connect(_player, SIGNAL(playing()), this, SLOT(onStarted()));
    QObject::connect(_player, SIGNAL(stopped()), this, SLOT(onStopped()));
    QObject::connect(_player, SIGNAL(paused()), this, SLOT(onPaused()));
}

void DialogVideoPlayer::openLocal(QString file)
{
    if (file.isEmpty())
        return;

    mediaType = MediaFile;
    fileurl = file;

    _media = new VlcMedia(file, true, _instance);

    _player->open(_media);
    isPaused = false;
}

void DialogVideoPlayer::openUrl(QString url)
{
    if (url.isEmpty())
        return;

    mediaType = MediaUrl;
    fileurl = url;

    _media = new VlcMedia(url, _instance);

    _player->open(_media);
    isPaused = false;
}

void DialogVideoPlayer::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogVideoPlayer::closeEvent(QCloseEvent *event) {
    this->hide();
    _player->stop();
    genericHelper::saveGeometry("videodialog",saveGeometry());
    event->accept(); // close window
}

void DialogVideoPlayer::setTitle(QString title)
{
    this->setWindowTitle(title);
}

void DialogVideoPlayer::on_pushButtonReload_clicked()
{
    _player->stop();
    this->setEnabledAllButtons(false);

    switch (mediaType) {
        case MediaUrl:
            this->openUrl(this->fileurl);
            break;
        case MediaFile:
            this->openLocal(this->fileurl);
            break;
        case MediaNone:
            break;
    }
}

void DialogVideoPlayer::onStopped()
{
    this->changeUi(RunStatus::Stopped);
}

void DialogVideoPlayer::onStarted()
{
    this->changeUi(RunStatus::Running);

    _vlcAudio->setVolume(this->ui->horizontalSliderVolume->value());
}

void DialogVideoPlayer::onPaused()
{
    this->changeUi(RunStatus::Paused);
}

void DialogVideoPlayer::on_pushButtonStartResume_clicked()
{
    if(this->isStarted) {
        if(! isPaused) {
            _player->pause();
            isPaused = true;
        } else {
            _player->resume();
            isPaused = false;
        }
    } else {
        switch (mediaType) {
            case MediaUrl:
                this->openUrl(this->fileurl);
                break;
            case MediaFile:
                this->openLocal(this->fileurl);
                break;
            case MediaNone:
                break;
        }
    }
}

void DialogVideoPlayer::on_pushButtonStop_clicked()
{
    _player->stop();
}

void DialogVideoPlayer::setEnabledAllButtons(bool enabled)
{
    this->ui->pushButtonClose->setEnabled(enabled);
    this->ui->pushButtonStartResume->setEnabled(enabled);
    this->ui->pushButtonStop->setEnabled(enabled);
    this->ui->pushButtonReload->setEnabled(enabled);
}

void DialogVideoPlayer::changeUi(RunStatus status) {
    switch (status) {
        case Running:
            setEnabledAllButtons(true);
            this->isStarted = true;
            this->isPaused = false;
            this->ui->pushButtonStartResume->setIcon(QIcon(":/16x16/icons/16x16/media-playback-pause.png"));
            break;

        case Paused:
            setEnabledAllButtons(true);
            this->isStarted = true;
            this->isPaused = true;
            this->ui->pushButtonStartResume->setIcon(QIcon(":/16x16/icons/16x16/media-playback-start.png"));
            break;

        case Stopped:
            setEnabledAllButtons(false);
            this->isStarted = false;
            this->isPaused = false;
            this->ui->pushButtonStartResume->setEnabled(true);
            this->ui->pushButtonStartResume->setIcon(QIcon(":/16x16/icons/16x16/media-playback-start.png"));
            break;
    }
}

void DialogVideoPlayer::on_horizontalSliderVolume_valueChanged(int value)
{
    _vlcAudio->setVolume(value);
    if(value == 0) {
         this->ui->pushButtonMute->setIcon(QIcon(":/16x16/icons/16x16/audio-card-off.png"));
    } else {
         this->ui->pushButtonMute->setIcon(QIcon(":/16x16/icons/16x16/audio-card.png"));
    }
}

void DialogVideoPlayer::on_pushButtonMute_clicked()
{
    if (ui->horizontalSliderVolume->value() == 0) {
        this->ui->horizontalSliderVolume->setValue(oldVolume);
    } else {
        oldVolume = this->ui->horizontalSliderVolume->value();
        this->ui->horizontalSliderVolume->setValue(0);
    }
}
