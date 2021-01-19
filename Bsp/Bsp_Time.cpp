#include "Bsp_Time.h"
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include "Pub/Pub_Tmr.h"

CTimer::CTimer(QObject *parent)
    : QObject(parent)
{

    //新建一个QTimer对象
    timer1 = new QTimer();
    //设置定时器每个多少毫秒发送一个timeout()信号
    timer1->setInterval(1000);
    //启动定时器
    timer1->start();

    //信号和槽
    connect(timer1, SIGNAL(timeout()), this, SLOT(Timer1Out()));

    //新建一个QTimer对象
    timer2 = new QTimer();
    //设置定时器每个多少毫秒发送一个timeout()信号
    timer2->setInterval(1);
    //启动定时器
    timer2->start();
    //信号和槽
    connect(timer2, SIGNAL(timeout()), this, SLOT(Timer2Out()));

}

CTimer::~CTimer()
{

}

void CTimer::Timer1Out()
{
  //获取系统当前时间
  QTime time = QTime::currentTime();

}

void CTimer::Timer2Out()
{
    gul_softTmr++;
}
//no more
