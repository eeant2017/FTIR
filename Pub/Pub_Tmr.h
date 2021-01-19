//==================================================================================================
//| 文件名称 | Pub_Tmr.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Pub_Tmr.h所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2009, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.1    | 2009.04.24  | Gang  | 规范化
//==================================================================================================
//-----------------------------------------Macro--------------------------------------------------//
#ifndef PUB_TMR_H
#define PUB_TMR_H

#include "types.h"


class CTmr
{
public:
    CTmr(INT32U ul_time)
    {
        ul_tmr = 0;            //开始时候参数复位
        uch_stop = 1;          //停止定时 1停止计时 0 启动定时
        ul_interval = ul_time; //定时周期
        uch_stop1 = 1;
        uch_stop2 = 1;
        uch_stop3 = 1;
        uch_stop4 = 1;
    };
    CTmr()
    {

    };                  //默认构造函数
    ~CTmr(){};                 //定时超时
    INT8S TmrOut(void);        //定时开始
    INT8S TmrOut1(void);
    INT8S TmrOut2(void);
    INT8S TmrOut3(void);
    INT8S TmrOut4(void);
    void Start1(void);
    void Start2(void);
    void Start3(void);
    void Start4(void);
    void Start(void);
    void Start(INT32S ul_init);
    void Set1(INT32U ul_time)
    {
        ul_interval1 = ul_time;
        Start1();
    };
     void Set2(INT32U ul_time)
    {
        ul_interval2 = ul_time;
        Start2();
    };
     void Set3(INT32U ul_time)
    {
        ul_interval3 = ul_time;
        Start3();
    };
    void Set4(INT32U ul_time)
    {
        ul_interval4 = ul_time;
        Start4();
    };
    void Set(INT32U ul_time)
    {
        ul_interval = ul_time;
        Start();
    };
    void Set(INT32U ul_period, INT32S sl_init)
    {
        ul_interval = ul_period;
        Start(sl_init);
    }
      void Stop1(void)          //定时停止函数
    {
        uch_stop1 = 1;
    }
      void Stop2(void)          //定时停止函数
    {
        uch_stop2 = 1;
    }
      void Stop3(void)          //定时停止函数
    {
        uch_stop3 = 1;
    }
    void Stop4(void)          //定时停止函数
    {
        uch_stop4 = 1;
    }
    void Stop(void)          //定时停止函数
    {
        uch_stop = 1;
    }
        INT32U ul_interval;
                INT32U ul_interval1;
        INT32U ul_interval2;
                INT32U ul_interval3;
                INT32U ul_interval4;
private:
    INT32U ul_tmr;
    INT8U  uch_stop;          //定时停止
     INT32U ul_tmr1;
    INT8U  uch_stop1;
    INT32U ul_tmr2;
    INT8U  uch_stop2;
    INT32U ul_tmr3;
    INT8U  uch_stop3;
    INT32U ul_tmr4;
    INT8U  uch_stop4;
};

extern volatile INT32U gul_softTmr;

#endif /* Pub_Tmr_H */

//no more
