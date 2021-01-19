#ifndef MOD_Q0Driver_H
#define MOD_Q0Driver_H
#include "Pub/Pub_Tmr.h"
#include <Bsp/Bsp_Time.h>
#include "typedefine.h"

typedef struct
{

        INT8U uch_SourceSelect;
        FP32 f_Q0UpVLowSet; //Q0上邻杆低压设置
        FP32 f_Q0UpVLowMnt; //Q0上邻杆低压监控
        FP32 f_Q0LowVLowSet;//Q0下邻杆低压设置
        FP32 f_Q0LowVLowMnt; //Q0上邻杆低压监控
        FP32 f_DDSOutSet; //Q0DDS输出
        FP32 f_Q0VMnt; //Q0反馈电压
        FP32 f_Q0IMnt; //Q0反馈电流
        FP32 f_Q0DriverVMnt; //Q0驱动电压
        FP32 f_Q0Power;   //Q0板电源
        FP32 f_Q0BoardTemp;    //Q0板温度
        INT8U uch_Q0AutoTuningIOSet; //自动调谐开关设置
        INT8U uch_Q0AutoTuningIOMnt; //自动调谐开关读取
        INT8U uch_Q0Relay11;//Q0电压切换继电器11
        INT8U uch_Q0Relay12;//Q0电压切换继电器12
        INT8U uch_Q0Relay21;//Q0电压切换继电器21
        INT8U uch_Q0Relay22;//Q0电压切换继电器22

        INT8U uch_Q0DCRelay1;//Q0DC电压切换继电器1
        INT8U uch_Q0DCRelay2;//Q0DC电压切换继电器2
}STRQ0DRIVERMNT;


class Q0Driver : public CTmr
{
public:
    Q0Driver();
    void Q0DriverProcess();
    void Q0DriverMntProcess();
    BOOL m_bCommAlarm;


public:
    INT32U m_ulCommCnt;
    INT32U m_ulCommErrCnt;
    STRQ0DRIVERMNT st_Mnt;
private:
    INT8U m_uchMntState;

    BOOL m_bCommLock;



};

extern Q0Driver g_clQ0Driver;
#endif // MOD_Q0Driver_H
