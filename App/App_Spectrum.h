//=================================================================================================
//  文件名称  | App_Spectrum.h
//==================================================================================================
// 文件描述   | App_Spectrum.cpp所对应的头文件
//==================================================================================================
// 版权声明   | Copyright 2018, 聚光科技(FPI)
//=================================================================================================
//  版本     |  时间        |  作者      | 描述
//==================================================================================================
//  V1.0     | 2018.08.16  |  hb  | 初版
//=================================================================================================
#ifndef APP_SPECTRUM_H
#define APP_SPECTRUM_H

#include"Include.h"
#include "Bsp/Bsp_Include.h"

#define DATAREADYADDR           0x01

#define FPGAMODEADDR            0x200
#define FPGACONFADDR            0x20d
#define FPGACONFADDR1           0x20e
#define FPGACONFDNOEADDR        0x20f
#define FPGARUNADDR             0x211
#define FPGARUNADDR1            0x212
#define FPGAREADOVERADDR        0x213
#define FPGAREADOVERADDR1       0x214
#define FPGARERESETADDR         0x215
#define FPGARERESETADDR1        0x216

#define DATABASE      0x400
class CAppSpectrum : public QThread, public CTmr, public CBspFpga
{

public:
    CAppSpectrum();
   ~CAppSpectrum();

    void Process(void);
    void ReadOriginalSpectrum(INT8U uch_CmosSn,INT8U uch_Sn,INT32U *pul_Date);
    void SetExe(INT8U uch_Set){uch_CmdExe = uch_Set;}


    TASKINF st_Task;

 private:
    BOOL b_ModEnable;         //模块总使能
    BOOL b_FirstTime;          //第一次运行
    INT16U buf[100];
// ------------------------------函数定义--------------------------//
    BOOL SetModEnable(BOOL b_Set);
    void ModInit(void);
    void SetFpga(INT16U *puin_Data);
    BOOL SetTask(TASKINF *st_Task,INT16U *puin_TempData);

    INT8U uch_CmdExe;
    INT8U uch_SpectrumState;
    INT16U auin_OriginalSpectrumData[VALIDPOINTS*MAXCMOS]; //原始光谱
    SPARKMODEL st_SparkModle;

    INT16U auin_TempData[50];
    INT16U auin_TempData111[50];
};






extern CAppSpectrum a_Spectrum;

#endif // APP_SPECTRUM_H

