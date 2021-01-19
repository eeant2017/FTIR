#include "form_nowdate.h"
#include "ui_form_nowdate.h"
#include"types.h"

extern INT8U guch_ready;
Form_NowDate::Form_NowDate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_NowDate)
{
    ui->setupUi(this);
    INT8U i;
    QStringList HeadStringnowdate;
    setGeometry(30,280,400,460);
    ui->nowdate->setGeometry(20,20,380,440);
    ui->nowdate->setColumnCount(5);
    ui->nowdate->setRowCount(5);
    for(i=0;i<5;i++)
    {
       ui->nowdate->setColumnWidth(i,60);

    }
    HeadStringnowdate<<"Al";
    HeadStringnowdate<<"Fe";
    HeadStringnowdate<<"Cu";
    HeadStringnowdate<<"Mg";
    HeadStringnowdate<<"Zn";
    ui->nowdate->setHorizontalHeaderLabels(HeadStringnowdate);
    ui->nowdate->setItem(0,0,new QTableWidgetItem("50"));
    ui->nowdate->setItem(0,1,new QTableWidgetItem("50"));
    ui->nowdate->setItem(0,2,new QTableWidgetItem("50"));
    ui->nowdate->setItem(0,3,new QTableWidgetItem("50"));
    ui->nowdate->setItem(0,4,new QTableWidgetItem("50"));
    ui->ESC->setGeometry(320,350,100,100);
    ui->ESC->setStyleSheet("border-image: url(:/Images/return.png)");


    connect(ui->ESC,SIGNAL(clicked()),this,SLOT(EscButtonSlot()));


}

Form_NowDate::~Form_NowDate()
{
    delete ui;
}

void Form_NowDate::EscButtonSlot()
{
    guch_ready = 0;
    //delete this;
    close();

}



