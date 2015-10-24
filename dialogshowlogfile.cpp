#include "dialogshowlogfile.h"
#include "ui_dialogshowlogfile.h"

DialogShowLogFile::DialogShowLogFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowLogFile)
{
    ui->setupUi(this);

    QObject::connect(this->ui->pushButtonReload, SIGNAL(clicked()), this, SLOT(reloadFile()));
    QObject::connect(this->ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(close()));
}

DialogShowLogFile::~DialogShowLogFile()
{
    delete ui;
}

void DialogShowLogFile::loadFile(const QString filename)
{
    this->fileName = filename;
    file.setFileName(filename);
    if (file.open(QIODevice::ReadOnly)) {
        this->ui->textEditLog->setText(file.readAll());
    }
    file.close();

    // Scroll to bottom
    QTextCursor cursor = this->ui->textEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->ui->textEditLog->setTextCursor(cursor);
}

void DialogShowLogFile::reloadFile()
{
    file.setFileName(this->fileName);
    if (file.open(QIODevice::ReadOnly)) {
        this->ui->textEditLog->setText(file.readAll());
    }
    file.close();

    // Scroll to bottom
    QTextCursor cursor = this->ui->textEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->ui->textEditLog->setTextCursor(cursor);
}
