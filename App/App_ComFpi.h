//==================================================================================================
//| 文件名称 | App_ComFpi.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | App_ComFpi.cpp所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2018, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者      | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2017.9.21   |  陈飞华   | 初版
//==================================================================================================
#ifndef APP_COMMFPI_H
#define APP_COMMFPI_H

#include "typedefine.h"
//#include "Pub/Pub_MyBus.h"
#include "Pub/Pub_ComFpi.h"

//----------------------------------------Cplusplus-----------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif


class CAppComFpi
{
 public:
//------------------------------变量定义--------------------------//
    //模块输入<<<
    BOOL b_ModEnable;         //模块总使能
    BOOL b_FirstTime;          //第一次运行

    CAppComFpi();

    ~CAppComFpi();

//------------------------------函数定义--------------------------//
    BOOL SetModEnable(BOOL b_Set);
    void CommFpiInit(void);
    void Process(void);


 public:
    BOOL m_bCircu31ReadCommFlag;
    BOOL m_bEI31ReadCommFlag;
    BOOL m_bQ0Driver31ReadCommFlag;

};


extern CAppComFpi a_ComFpi;
extern CComFpi gm_Can0Fpi;
//extern CComFpi gm_Uart1Fpi;
extern CComFpi gm_TcpFpi;


#ifdef __cplusplus
}
#endif


#endif

//no more
