#include "dialogpositioner.h"
#include "ui_dialogpositioner.h"

DialogPositioner::DialogPositioner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPositioner)
{
    ui->setupUi(this);
    currentGeo = "";
   this->loadPositions();
      desktop = QApplication::desktop();
}

DialogPositioner::~DialogPositioner()
{
    delete ui;
}

void DialogPositioner::setDialogShown()
{
    dialogShown = true;
}

bool DialogPositioner::getDialogShown()
{
    return dialogShown;
}

void DialogPositioner::on_pushButtonCurrentPos_clicked()
{



    if (this->ui->checkBoxDpiAware->checkState() == Qt::Checked)
    {

        this->ui->lineEditX->setText( QString::number( (this->pos().x()*100) / (desktop->logicalDpiX()+desktop->PdmDpiX) ) );
        this->ui->lineEditY->setText( QString::number( (this->pos().y()*100) / (desktop->logicalDpiY()+desktop->PdmDpiY) ) );
        this->ui->lineEditW->setText(QString::number(this->size().width()));
        this->ui->lineEditH->setText(QString::number(this->size().height()));

    } else {
        this->ui->lineEditX->setText(QString::number(this->pos().x()));
        this->ui->lineEditY->setText(QString::number(this->pos().y()));
        this->ui->lineEditW->setText(QString::number(this->size().width()));
        this->ui->lineEditH->setText(QString::number(this->size().height()));
    }


    this->currentGeo = this->ui->lineEditX->text() + "," + this->ui->lineEditY->text() + "," + this->ui->lineEditW->text() + "," + this->ui->lineEditH->text();

}

void DialogPositioner::on_pushButtonSavePos_clicked()
{
    if ( (this->ui->comboBoxPosName->currentText() != "") && (this->currentGeo != "")  ) {
        if (genericHelper::setPosition(this->ui->comboBoxPosName->currentText(),  this->currentGeo)) {
            this->successPopup("Saved.");
        } else {
            this->errorPopup("Saved failed.");
        }
    } else {
        this->errorPopup("No position name or geometry set.");
    }
    this->loadPositions();
}

void DialogPositioner::successPopup(QString message)
{
    QMessageBox::information(this, tr("twitcher"),
                                    message,
                                    QMessageBox::Ok
                                    );
}

void DialogPositioner::errorPopup(QString message)
{
    QMessageBox::warning(this, tr("twitcher"),
                                    message,
                                    QMessageBox::Ok
                         );
}

void DialogPositioner::loadPositions()
{
    this->ui->comboBoxPosName->clear();
    QHash<QString, QString> poss;
    poss = genericHelper::getPositions();

    if (poss.keys().length() == 0) {
        this->ui->comboBoxPosName->addItem("<default>");
    } else {

        QHashIterator<QString, QString> i(poss);
        while (i.hasNext()) {
            i.next();
            this->ui->comboBoxPosName->addItem(i.key());
        }
    }

}

void DialogPositioner::setGeoLineEdit(QString geo)
{
    if (geo.count(",") == 3) {

        QStringList _geo;
        _geo = geo.split(",");


        this->ui->lineEditX->setText(_geo[0]);
        this->ui->lineEditY->setText(_geo[1]);
        this->ui->lineEditW->setText(_geo[2]);
        this->ui->lineEditH->setText(_geo[3]);

        this->move(QString(_geo[0]).toInt(),QString(_geo[1]).toInt());
        this->resize(QString(_geo[2]).toInt(),QString(_geo[3]).toInt());
    }
}

void DialogPositioner::on_pushButtonLoadPos_clicked()
{
    this->setGeoLineEdit(genericHelper::getPosition(this->ui->comboBoxPosName->currentText()));
}

void DialogPositioner::on_pushButtonDeletePos_clicked()
{
    if (genericHelper::deletePosition(this->ui->comboBoxPosName->currentText())) {
        this->successPopup(this->ui->comboBoxPosName->currentText()+" deleted.");
        this->loadPositions();
    } else {
        this->errorPopup("Delete failed.");
    }

}
