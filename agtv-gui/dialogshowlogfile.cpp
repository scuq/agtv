#include "dialogshowlogfile.h"
#include "ui_dialogshowlogfile.h"

DialogShowLogFile::DialogShowLogFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowLogFile)
{
    ui->setupUi(this);

    restoreGeometry(genericHelper::getGeometry("showlog").toByteArray());


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

void DialogShowLogFile::closeEvent(QCloseEvent *)
{
    genericHelper::saveGeometry("showlog",saveGeometry());
    this->close();
}



void DialogShowLogFile::on_lineEditFilter_textChanged(const QString &arg1)
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if(!ui->textEditLog->isReadOnly())
    {
        ui->textEditLog->moveCursor(QTextCursor::Start);
        QColor color = QColor(Qt::gray).lighter(130);

        while(ui->textEditLog->find(arg1))
        {
            QTextEdit::ExtraSelection extra;
            extra.format.setBackground(color);

            extra.cursor = ui->textEditLog->textCursor();
            extraSelections.append(extra);
        }
    }

    ui->textEditLog->setExtraSelections(extraSelections);
}
