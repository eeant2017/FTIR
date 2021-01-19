#include "formmain.h"
#include "ui_formmain.h"
#include <QTimer>
#include "sqlite.h"

INT8U guch_ready =0;
INT8U guch_RUNflag =0;
Formmain::Formmain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formmain)
{
    ui->setupUi(this);
    //1000ms

    //connect(ui->btn_SysSet,SIGNAL(clicked()),this,SLOT(SystemSetSlot()));
    ui->btn_Runstate->setStyleSheet("border-image: url(:/Images/start.png)");
    //新建一个QTimer对象
    timer3 = new QTimer();
    //设置定时器每个多少毫秒发送一个timeout()信号
    timer3->setInterval(5000);
    //启动定时器
    timer3->start();
    //信号和槽
    connect(timer3, SIGNAL(timeout()), this, SLOT(Timer3Out()));
    //1000ms

   connect(ui->btn_Log,SIGNAL(clicked()),this,SLOT(LogSlot()));
   connect(ui->btn_Inform,SIGNAL(clicked()),this,SLOT(InformSlot()));

}

Formmain::~Formmain()
{
    delete ui;
}


void Formmain::SystemSetSlot()
{
   FormSysSett = new Form_SysSett(this);
   FormSysSett->show();

}

//5S更新一次
void Formmain::Timer3Out()
{
    //guch_ready++;

    //QTime current_time =QTime::currentTime();
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm");
    ui->labelTime->setText(current_date);
    if(guch_ready!=1 )
    {

        guch_ready = 1;
        ui->btn_Runstate->setStyleSheet("border-image: url(:/Images/startReady.png)");
        guch_RUNflag =1;
        connect(ui->btn_Runstate,SIGNAL(clicked()),this,SLOT(NowdateSlot()));

    }







}


//void Widget::timerEvent(QTimerEvent *)
//{

//    //int fd= ::open(")
//    static int fa =0;
//    fa++;
//    if(fa>10)
//    {
//        fa = 0;
//    }
//    ui->lcdNumber->display(fa);


//     return;

//}





/*void Widget::on_pushButton_clicked()
{

}

void Widget::on_btn_SysSet_clicked()
{
   // FormSysSett = new Form_SysSett(this);
   //FormSysSett->show();
}*/


void Formmain::LogSlot()
{
    FormLog = new Form_Log(this);
   FormLog->show();

}

void Formmain::NowdateSlot()
{

    QDateTime current_date_time =QDateTime::currentDateTime();

    FormNowDate = new Form_NowDate(this);
    FormNowDate->show();
    ui->btn_Runstate->setStyleSheet("border-image: url(:/Images/start.png)");
    disconnect(ui->btn_Runstate,SIGNAL(clicked()),this,SLOT(NowdateSlot()));
     m_SQLite.AddlibsHistoryDate(current_date_time);

}
void Formmain::NowdateescSlot()
{


    guch_ready =0;

}

void Formmain::InformSlot()
{
    FormInfrom = new Form_Inform(this);
    FormInfrom->show();

}
