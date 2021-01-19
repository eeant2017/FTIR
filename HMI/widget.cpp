#include "widget.h"
#include "ui_widget.h"
#include "Pub/Pub_Tmr.h"
#include <QTimer>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);



    //新建一个QTimer对象
    timer2 = new QTimer();
    //设置定时器每个多少毫秒发送一个timeout()信号
    timer2->setInterval(10);
    //启动定时器
    timer2->start();
    //信号和槽
    connect(timer2, SIGNAL(timeout()), this, SLOT(Timer2Out()));
    //1000ms
    //startTimer(1000);

    //connect(ui->btn_SysSet,SIGNAL(clicked()),this,SLOT(SystemSetSlot()));

   connect(ui->f_mainmenu,SIGNAL(clicked()),this,SLOT(SystemSetSlot()));

}

Widget::~Widget()
{
    delete ui;
}


void Widget::SystemSetSlot()
{

    Formmain_sss = new Formmain(this);
   Formmain_sss->show();

  // delete ui;

}


void Widget::Timer2Out()
{
    gul_softTmr++;
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
}


void Widget::LogSlot()
{
    FormLog = new Form_Log(this);
   FormLog->show();

}*/
