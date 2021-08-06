#include "gotocelldialog.h"
#include "ui_gotocelldialog.h"

GoToCellDialog::GoToCellDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToCellDialog)
{
    ui->setupUi(this);

 //  connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));


}

GoToCellDialog::~GoToCellDialog()
{
    delete ui;
}

void GoToCellDialog::on_lineEdit_textChanged(const QString &arg1)
{

}
