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
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(this->ui->video);
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
    _player->stop();
    genericHelper::saveGeometry("videodialog",saveGeometry());
    event->accept(); // close window
}

void DialogVideoPlayer::setTitle(QString title)
{
    this->setWindowTitle(title);
}

void DialogVideoPlayer::on_pushButtonStart_clicked()
{
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

void DialogVideoPlayer::on_pushButtonPause_clicked()
{
    if(! isPaused) {
        _player->pause();
        this->ui->pushButtonPause->setText("Resume");
        isPaused = true;
    } else {
        _player->resume();
        this->ui->pushButtonPause->setText("Pause");
        isPaused = false;
    }
}
