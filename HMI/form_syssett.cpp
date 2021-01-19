#include "form_syssett.h"
#include "ui_form_syssett.h"

Form_SysSett::Form_SysSett(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Form_SysSett)
{
    ui->setupUi(this);

    connect(ui->btn_Esc1,SIGNAL(clicked()),this,SLOT(EscButtonSlot()));
    connect(ui->btn_OK1,SIGNAL(clicked()),this,SLOT(OKButtonSlot()));
}

Form_SysSett::~Form_SysSett()
{
    delete ui;
}


void Form_SysSett::EscButtonSlot()
{
    delete this;

}

void Form_SysSett::OKButtonSlot()
{
    return;
}

void Form_SysSett::on_pushButton_clicked()
{

}
