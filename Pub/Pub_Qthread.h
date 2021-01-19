//=================================================================================================
//  文件名称  | Pub_Qthread.h
//==================================================================================================
// 文件描述   | Pub_Qthread.cpp所对应的头文件
//==================================================================================================
// 版权声明   | Copyright 2018, 聚光科技(FPI)
//=================================================================================================
//  版本     |  时间        |  作者      | 描述
//==================================================================================================
//  V1.0     | 2019.05.31  |   陈飞华  | 初版
//=================================================================================================
#ifndef PUB_QTHREAD_H
#define PUB_QTHREAD_H

#include"Include.h"
#include "Bsp/Bsp_Include.h"
#include "globalvar.h"


class CPubQthread : public QThread, public CTmr
{

public:
    CPubQthread();
   ~CPubQthread();
     VOID_PFUN_VOID Runtask;

    void Process(void);

   void run();

 private:
    BOOL b_ModEnable;         //模块总使能
    BOOL b_FirstTime;          //第一次运行
   

// ------------------------------函数定义--------------------------//
    BOOL SetModEnable(BOOL b_Set);
    void ModInit(void);
   

};






extern CPubQthread cPubQthread[THREAD_MAX_SOURCE];

#endif // APP_SPECTRUM_H

