#ifndef MOD_EI_H
#define MOD_EI_H
#include "Pub/Pub_Tmr.h"
#include <Bsp/Bsp_Time.h>
#include "typedefine.h"



typedef struct
{
        //EI板

        FP32 f_EIRepellerVSet;      //推斥极设置
        FP32 f_EIRepellerVMnt;      //推斥极监控
        FP32 f_EIDrawoutSet;        //抽出极设置
        FP32 f_EIDrawoutMnt;        //抽出极监控
        FP32 f_EIFocusVSet;         //聚焦极设置
        FP32 f_EIFocusVMnt;         //聚焦极监控
        FP32 f_EIDeflectionVSet;    //EI偏转透镜设置
        FP32 f_EIDeflectionVMnt;    //EI偏转透镜监控
        FP32 f_Q0UpVHighSet;        //Q0上邻杆高压设置
        FP32 f_Q0UpVHighMnt;        //Q0上邻杆高压监控
        FP32 f_Q0LowVHighSet;       //Q0下邻杆高压设置
        FP32 f_Q0LowVHighMnt;       //Q0下邻杆高压监控
        FP32 f_FilamentVSet;        //灯丝电压设置
        FP32 f_FilamentVMnt;        //灯丝电压监控
        FP32 f_FilamentOffsetVSet;  //灯丝偏转电压设置
        FP32 f_FilamentOffsetVMnt;  //灯丝偏转电压监控
        FP32 f_FilamentIMnt;        //灯丝反馈电流监控
        FP32 f_EIPower14V;     //EI板电源24V监控
        FP32 f_EIBoardTemp;  //EI板温度监控
        INT8U uch_EISourceTempEn; //控温使能
        FP32 f_EISourceTempSet;
        FP32 f_EISourceTempMnt; //EI离子源温度（校准后）监控
        FP32 f_EISourceTempMntOriginal; //EI离子源温度（校准前）监控
        FP32 f_EISourceTempCaliK; //校准参数K
        FP32 f_EISourceTempCaliB; //校准参数B
        FP32 f_EISourceTempKp;
        FP32 f_EISourceTempKi;
        FP32 f_EISourceTempKd;
        FP32 f_EISourceTempLimit;
        FP32 f_EISourceOut; //输出
        INT8U uch_EISourceTempHeatMode;
        INT8U uch_EISourceErrRange;
        INT8U uch_EISourceErrHeatSpeed;
        INT8U uch_EISourceErrReachTar;
        INT8U uch_EIGCInterfaceTempEn; //控温使能
        FP32 f_EIGCInterfaceTempSet; //GC接口温度设置
        FP32 f_EIGCInterfaceTempMnt; //GC接口温度（校准后）
        FP32 f_EIGCInterfaceTempMntOriginal; //GC接口温度（校准前）
        FP32 f_EIGCInterfaceTempCaliK; //校准参数K
        FP32 f_EIGCInterfaceTempCaliB; //校准参数B
        FP32 f_EIGCInterfaceTempKp;
        FP32 f_EIGCInterfaceTempKi;
        FP32 f_EIGCInterfaceTempKd;
        FP32 f_EIGCInterfaceTempLimit;
        FP32 f_EIGCInterfaceOut; //输出
        INT8U uch_EIGCInterfaceTempHeatMode;
        INT8U uch_EIGCInterfaceErrRange;
        INT8U uch_EIGCInterfaceErrHeatSpeed;
        INT8U uch_EIGCInterfaceErrReachTar;
        INT8U uch_FilamentIO; //灯丝切换开关
        INT8U uch_Q0HighVIO; //Q0高压开关
}STREIMNT;


class EI : public CTmr
{
public:
    EI();
    void EIProcess();
    void EIMntProcess();
    void EISampleSelect(INT8U uch_Select);
    BOOL m_bCommAlarm;

    STREIMNT st_Mnt;
public:
    INT32U m_ulCommCnt;
    INT32U m_ulCommErrCnt;

private:
    INT8U m_uchMntState;

    BOOL m_bCommLock;



};

extern EI g_clEI;
#endif // MOD_EI_H
