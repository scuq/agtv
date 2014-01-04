#include "dialogabout.h"
#include "ui_dialogabout.h"

dialogAbout::dialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogAbout)
{
    ui->setupUi(this);

    version = VERSION;


    this->ui->lineEditVersion->setText(version);

    this->ui->textEditChangelog->setText(this->getChangelog());

    this->ui->textEditTodo->setText(this->getTodo());

}

void dialogAbout::setDialogShown()
{
    dialogShown = true;
}

bool dialogAbout::getDialogShown()
{
    return dialogShown;
}


QString dialogAbout::getChangelog()
{
   QString changelog;
   QString _content;

   QFile file("changelog.txt");
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

       return changelog;
   }

   QTextStream in(&file);
   _content = in.readAll();

   file.close();

   changelog = _content;

   return changelog;

}

QString dialogAbout::getTodo()
{
   QString todo;
   QString _content;

   QFile file("todo.txt");
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

       return todo;
   }

   QTextStream in(&file);
   _content = in.readAll();

   file.close();

   todo = _content;

   return todo;

}

dialogAbout::~dialogAbout()
{
    delete ui;
}
