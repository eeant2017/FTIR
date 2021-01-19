//==================================================================================================
//| 文件名称 | Pro_FPGA.cpp
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | FPGA接口模块
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2011, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.1    | 2016.8.5 |lz      | 规范化
//==================================================================================================
#include "pro_fpga.h"
#include <QMutex>
#include "Mod/Mod_Method.h"
#include "Pub/Pub_MyBus.h"
#include "Mod/mod_ioctrl.h"
#include <math.h>
#include <QDateTime>
#include <Bsp/Bsp_Fpga.h>
#include <QDebug>
#include "App/app_app.h"


#include <unistd.h>

QMutex mutex_Memory;
QMutex mutex_RunFlag;
STRSetVoltageList g_strSetVoltageList[MAX_VOL_CHANNEL] =
{
        //style  Y= ( ( (x*f_k) *f_a) +f_b) *f_c

        //                                          k               a                   b       c

        //dac124
        DA_DAC124_11_addr,  DA_DAC124_11_syn,   0,  -1.0,           1/60.0,             2.05,   4095.0/4.1, 0x09,   0x10,   1,  ARM_CID_IN_V,   JUNK_CID_IN_V,  "碰撞池入口电压",
        DA_DAC124_15_addr,  DA_DAC124_15_syn,   0,  -1.0,           1/60.0,             2.05,   4095.0/4.1, 0x0b,   0x50,   1,  ARM_CID_OUT_V,  JUNK_CID_OUT_V, "碰撞池出口电压",
        DA_DAC124_19_addr,  DA_DAC124_19_syn,   0,  -1.0,           1/6.0,              2.05,   4095.0/4.1, 0x04,   0x90,   1,  ARM_ROD_V,      JUNK_ROD_V,     "TR3", //传输杆电压
        DA_DAC124_1d_addr,  DA_DAC124_1d_syn,   0,  -1.0,           1/60.0,             2.05,   4095.0/4.1, 0x03,   0xd0,   1,  ARM_EXTRA_V,    JUNK_EXTRA_V,   "L1",  //萃取锥电压
        //dac124
        DA_DAC124_21_addr,  DA_DAC124_21_syn,   0,  1/3200.0,       1.0,                0.0,    4095.0/2.5,     0x13,   0x10,   1,  ARM_EM_IN_V,    JUNK_EM_IN_V,   "检测器入口电压",
        DA_DAC124_25_addr,  DA_DAC124_25_syn,   0,  1/3200.0,       1.0,                0.0,    4095.0/2.5,     0x00,   0x50,   1,  ARM_CAPL_V,     JUNK_CAPL_V,    "毛细管高压",
        DA_DAC124_29_addr,  DA_DAC124_29_syn,   0,  1/40.0  ,       1.0,                0.0,    4095.0/2.5,     0x01,   0x90,   1,  ARM_CORO_V,     JUNK_CORO_V,    "电晕针电流",
        DA_DAC124_2d_addr,  DA_DAC124_2d_syn,   0,  1/3200.0,       1.0,                0.0,    4095.0/2.5,     0x11,   0xd0,   1,  ARM_EM_V,       JUNK_EM_V,      "检测器倍增电压",

        //ltc8043
        DA_LTC8043_1_addr,  DA_LTC8043_1_syn,   0,  -2.2/10.0,      1.0/10.0,           1.0,    2047.5,     0x06,   0x00,   1,  ARM_MS1_ION_V,  JUNK_MS1_ION_V, "ms1离子能量",
        DA_LTC8043_2_addr,  DA_LTC8043_2_syn,   0,  -0.6/10.0,      1.0/10.0,           1.0,    2047.5,     0x0e,   0x00,   1,  ARM_MS2_ION_V,  JUNK_MS2_ION_V, "ms2离子能量",
        DA_LTC8043_7_addr,  DA_LTC8043_7_syn,   0,  1.0,            1.0,                0.0,    4095.0/300.0,   0X08,   0X00,   1,  ARM_MS1_HM_V,   JUNK_MS1_HM_V,  "MS1HM",
        DA_LTC8043_8_addr,  DA_LTC8043_8_syn,   0,  1.0,            1.0,                0.0,    4095.0/300.0,   0x07,   0x00,   1,  ARM_MS1_LM_V,   JUNK_MS1_LM_V,  "MS1LM",
        DA_LTC8043_9_addr,  DA_LTC8043_9_syn,   0,  1.0,            1.0,                0.0,    4095.0/300.0,   0x10,   0x00,   1,  ARM_MS2_HM_V,   JUNK_MS2_HM_V,  "MS2HM",
        DA_LTC8043_10_addr, DA_LTC8043_10_syn,  0,  1.0,            1.0,                0.0,    4095.0/300.0,   0x0f,   0x00,   1,  ARM_MS2_LM_V,   JUNK_MS2_LM_V,  "MS2LM",

        //dac124
        DA_DAC124_31_addr,  DA_DAC124_31_syn,   0,  -1.0,           1/200.0,            2.05,   4095.0/4.1, 0x18,   0x10,   1,  ARM_DC1_V,      JUNK_DC1_V, "TR1外壁", // Focus DC1 原来的名字
        DA_DAC124_35_addr,  DA_DAC124_35_syn,   0,  -1.0,           1/60.0,             2.05,   4095.0/4.1, 0x19,   0x50,   1,  ARM_DC2_V,      JUNK_DC2_V, "TR1",  //Qjet DC2
        DA_DAC124_39_addr,  DA_DAC124_39_syn,   0,  -1.0,           1/6.0,              2.05,   4095.0/4.1, 0x1a,   0x90,   1,  ARM_DC3_V,      JUNK_DC3_V, "TR2",   // Q00 DC3
        DA_DAC124_3d_addr,  DA_DAC124_3d_syn,   0,  -1.0,           1/6.0,              2.05,   4095.0/4.1, 0x1b,   0xd0,   1,  ARM_DC4_V,      JUNK_DC4_V, "L2", //Q00 out DC4

        DA_DAC124_41_addr,  DA_DAC124_41_syn,   0,  -1/200.0,       1.0,                2.05,   4095.0/4.1,     0x02,   0x10,   1,  ARM_CONE_V,     JUNK_CONE_V,    "采样锥电压",
        DA_DAC124_45_addr,  DA_DAC124_45_syn,   0,  1/3200.0,       1.0,                0.0,    4095.0/4.1,     0x20,   0x50,   0,  ARM_NULL_V,     JUNK_NULL_V,    "q0out2",
        DA_DAC124_49_addr,  DA_DAC124_49_syn,   0,  1/3200.0,       1.0,                0.0,    4095.0/4.1,     0x1f,   0x90,   1,  ARM_DA_CP_V,    JUNK_DA_CP_V,   "da_cp", //挡板电压
        DA_DAC124_4d_addr,  DA_DAC124_4d_syn,   0,  -1/6.0,         1.0,                2.05,   4095.0/4.1,     0x14,   0xd0,   1,  ARM_HV1_V,      JUNK_HV1_V,     "L3",

        DA_DAC124_5_addr,   DA_DAC124_5_syn,    0,  -1/60.0,        1.0,                2.05,   4095.0/4.1,     0x05,   0x10,   1,  ARM_MS1_PRE_V,  JUNK_MS1_PRE_V, "ms1预杆电压",
        DA_DAC124_5_addr,   DA_DAC124_5_syn,    0,  -1/60.0,        1.0,                2.05,   4095.0/4.1,     0X0D,   0x50,   1,  ARM_MS2_PRE_V,  JUNK_MS2_PRE_V, "ms2预杆电压",
        DA_DAC124_5_addr,   DA_DAC124_5_syn,    0,  1/200.0,        1.0,                2.05,   4095.0/4.1,     0x12,   0x90,   1,  ARM_EM_TURN_V,  JUNK_EM_TURN_V, "检测器偏转电压",
        DA_DAC124_5_addr,   DA_DAC124_5_syn,    0,  -1/60.0,        1.0,                2.05,   4095.0/4.1,     0x0a,   0xd0,   1,  ARM_CLN_V,      JUNK_COLN_V,    "碰撞池碰撞能量",

        //other
        ION_SWITCH_MS_addr, ION_SWITCH_MS_syn,  0,  1.0,            1.0,                0.0,    1.0,            0x16,   0x00,   0,  ARM_NULL_V,     JUNK_NULL_V,    "正负离子切换时间"

};

 STRCheckVoltageList g_strVoltageCheckList[MAX_VOL_CHANNEL_CHECK]=
 {
        //index,    mapoffset                                                                       read,       write,      delta   timedly alarmcode

        0,      &( (STRMntPtr *) 0)->st_CIDEntranceV    - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xa2, "碰撞池入口电压",  //ok
        1,      &( (STRMntPtr *) 0)->st_CIDEixtV        - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xa3, "碰撞池出口电压",  //ok
        2,      &( (STRMntPtr *) 0)->st_RfLenV          - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xa4, "TR3电压",  //"传输杆电压", ok
        3,      &( (STRMntPtr *) 0)->st_ExtractorV      - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xa5, "L1电压",   //"萃取锥电压", ok
        //U158
        4,      &( (STRMntPtr *) 0)->st_EmHighEntranceV - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  100,    3,  0xa6, "检测器入口电压", //ok
        5,      &( (STRMntPtr *) 0)->st_CapillaryV      - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  100,    3,  0xa7, "毛细管高压", //ok
        6,      0xffff,                                                                             0.0,    0,  0.0,    0,  1.5,    3,  0x00, "电晕针电流",  //取消
        7,      &( (STRMntPtr *) 0)->st_EmHighV         - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  100,    3,  0xa9, "检测器倍增电压",  //ok

        8,     &( (STRMntPtr *) 0)->st_Ms1IonEnergy    - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  15,     3,  0xaa, "ms1离子能量",  //ok
        9,     &( (STRMntPtr *) 0)->st_Ms2IonEnergy    - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  15,     3,  0xab, "ms2离子能量",  //ok


        10,     0xffff,                                                                             0.0,    0,  0.0,    0,  1.5,    3,  0x00, "MS1HM",
        11,     0xffff,                                                                             0.0,    0,  0.0,    0,  1.5,    3,  0x00, "MS1LM",
        12,     0xffff,                                                                             0.0,    0,  0.0,    0,  1.5,    3,  0x00, "MS2HM",
        13,     0xffff,                                                                             0.0,    0,  0.0,    0,  1.5,    3,  0x00, "MS2LM",



         //U248
        14,     &( (STRMntPtr *) 0)->st_LenthDC1        - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  2.5,    3,  0xb1, "TR1外壁",
        15,     &( (STRMntPtr *) 0)->st_LenthDC2        - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xb2, "TR1",
        16,     &( (STRMntPtr *) 0)->st_LenthDC3        - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xb3, "TR2",
        17,     &( (STRMntPtr *) 0)->st_LenthDC4        - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xb4, "L2",

        18,      &( (STRMntPtr *) 0)->st_SampleConeV     - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xa0, "采样锥电压",      //ok
        19,     &( (STRMntPtr *) 0)->st_Q0out2          - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xb6, "q0out2",
        20,     &( (STRMntPtr *) 0)->st_BaffleVoltage   - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  30,     3,  0xb7, "da_cp",
        21,     &( (STRMntPtr *) 0)->st_HV1             - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xae, "L3电压",   //ok

        22,     &( (STRMntPtr *) 0)->st_Ms1PreV         - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xaf, "ms1预杆电压", //ok
        23,     &( (STRMntPtr *) 0)->st_Ms2PreV         - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xac, "ms2预杆电压",  //ok
        24,     &( (STRMntPtr *) 0)->st_EmHighOffsetV   - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  10.0,   3,  0xad, "检测器偏转电压", //ok
        25,      &( (STRMntPtr *) 0)->st_CIDV            - &( (STRMntPtr *)0 )->st_CapillaryV,       0.0,    0,  0.0,    0,  1.5,    3,  0xa1, "碰撞池碰撞能量",  //ok

 };




 //==================================================================================================
 //| 函数名称 | GetSetVoltageListPtr
 //|----------|--------------------------------------------------------------------------------------
 //| 函数功能 | 获取设定电压表首地址
 //|----------|--------------------------------------------------------------------------------------
 //| 输入参数 | 无
 //|----------|--------------------------------------------------------------------------------------
 //| 返回参数 | 设定电压表首地址
 //|----------|--------------------------------------------------------------------------------------
 //| 函数设计 | 李壮 2016.8.5
 //==================================================================================================
STRSetVoltageList *GetSetVoltageListPtr(void)
{
        return &g_strSetVoltageList[0];
}

//==================================================================================================
//| 函数名称 | GetCheckVoltageListPtr
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获取校验电压表首地址
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 校验电压表首地址
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | 李壮 2016.8.5
//==================================================================================================
STRCheckVoltageList *GetCheckVoltageListPtr(void)
{
        return &g_strVoltageCheckList[0];
}

 //==================================================================================================
 //| 函数名称 | FindSetVolListPtr
 //|----------|--------------------------------------------------------------------------------------
 //| 函数功能 | 根据协议号寻找电压配置表绝对地址
 //|----------|--------------------------------------------------------------------------------------
 //| 输入参数 | uch_ProctolFlag -- 上位机协议号
 //|----------|--------------------------------------------------------------------------------------
 //| 返回参数 | 协议号对应电压配置表指针
 //|----------|--------------------------------------------------------------------------------------
 //| 函数设计 | 李壮 2016.4.13
 //==================================================================================================
STRSetVoltageList *FindSetVolListPtr(INT8U uch_ProctolFlag)
{
        STRSetVoltageList *pstr;

        pstr = GetSetVoltageListPtr();

        for (INT8U i =0 ; i< MAX_VOL_CHANNEL; i++)
        {
                if( pstr[i].uch_ProctolFlag == uch_ProctolFlag)
                        return (pstr + i);
        }

        return NULL;

}

//==================================================================================================
//| 函数名称 | FindSetVolListIndex
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 根据协议号寻找电压配置表绝对地址
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pstrList -- ARM透镜电压设定表基址，puch_Method，PC数据基址， uin_RamAddr -- FPGA基址
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 偏移量
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | 李壮 2016.4.13
//==================================================================================================
INT16U FindSetVolListIndex(INT8U uch_ProctolFlag)
{
        STRSetVoltageList *pstr = GetSetVoltageListPtr();

        for (INT8U i =0 ; i< MAX_VOL_CHANNEL-1; i++)
        {
                if(  pstr[i].uch_ProctolFlag == uch_ProctolFlag )
                        return i;
        }

        return 0xffff;
}


//==================================================================================================
//| 函数名称 | MethodVolSet
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 将上位机方法电压数据写入FPGA
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pstrList -- ARM透镜电压设定表基址，puch_Method，PC数据基址， uin_RamAddr -- FPGA基址
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | 李壮 2016.7.7
//==================================================================================================
void MethodVolSet(INT8U *puch_Method ,INT32U uin_RamAddr)
{
        INT8U uch_Size = 0;;

        INT8U uch_MethodEnable = 0;
        INT8U uch_ARM_Num = 0;
        INT8U uch_JUNK_Num = 0;

        STRSetVoltageList *pCurStrList = NULL;

        FP32 f_PcData =0;
        FP32 f_FirstData = 0;
        FP32 f_SecondData = 0;

        INT16U ush_TempData = 0;
        INT16U ush_FpgaData = 0;

        STRSetVoltageList *pStrSetVol = GetSetVoltageListPtr();

        uch_Size = MAX_VOL_CHANNEL; //sizeof(pstrList) / (sizeof(STRVoltageList));

        for ( INT8U i =0; i< uch_Size; i++)
        {
                pCurStrList = pStrSetVol + i;
                if(  pCurStrList->uch_MethodEnable == 1 )
                {
                        pCurStrList->uch_CmdSetFlag = 1;

                        //获取ARM偏移量
                        uch_ARM_Num = pCurStrList->uch_ARM_Num;
                        //获取FPGA偏移量
                        uch_JUNK_Num = pCurStrList->uch_JUNK_Num;
                        //根据ARM偏移量获取pc设定值
                        f_PcData = DecodeFloat( &puch_Method[uch_ARM_Num]);



                        //将设定值写入设定缓存表并记录设定时刻 2016.8.2
                        UpdateVoltageCheckListWrite( i, f_PcData, QDateTime::currentDateTime().toTime_t() ); //TODO:CLK

                        //根据系数计算一级公式计算值
                        f_FirstData = f_PcData * pCurStrList->f_k;
                        //根据系数计算二级公式计算值
                        f_SecondData =( f_FirstData * pCurStrList->f_a + pCurStrList->f_b ) * pCurStrList->f_c;
                        //将二级公式计算值转换成整形无符号
                        ush_TempData = (INT16U)f_SecondData;

                        if(pCurStrList->uch_Mask != 0) //使用多通道的da输出处理
                        {
                             if(ush_TempData > 0xfff)
                             {
                                ush_TempData = 0xfff;
                             }

                        }

                        //将计算出数据连同掩码一起写入FPGA
                        ush_FpgaData = ush_TempData | ( pCurStrList->uch_Mask <<8);


                        MemoryWrite(DSRAM_RECT,uin_RamAddr + uch_JUNK_Num,  &ush_FpgaData);

                }
        }

}


//==================================================================================================
//| 函数名称 | UpdateVoltageCheckListRead
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 更新读取设定电压反馈表
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pstrMnt -- 反馈表首地址 ， ul_Tick -- 最新读取时间点
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | 李壮 2016.8.5
//==================================================================================================
void UpdateVoltageCheckListRead(STRMnt *pstrMnt,INT32U ul_Tick)
{
        INT16U ush_Offset = 0;

        STRCheckVoltageList *pstrCheckVol = GetCheckVoltageListPtr();

        if ( pstrMnt == NULL)
        {
                return;
        }

        for (INT8U i =0; i< MAX_VOL_CHANNEL; i++)
        {
                ush_Offset = pstrCheckVol[i].ush_MemMapOffset;

                //偏移量是否有效
                if( ush_Offset == 0xffff)
                {
                        continue;
                }


                FP32 f_RealVal = 0;

                //ms1 ion
                if( pstrCheckVol[i].ush_VoltageListIndex == 10 )
                {
                        f_RealVal = (pstrMnt->st_Ms1DCP.f_RealVal +  pstrMnt->st_Ms1DCN.f_RealVal)/2.0f;
                }
                //ms2 ion
                else if( pstrCheckVol[i].ush_VoltageListIndex == 11)
                {
                        f_RealVal = (pstrMnt->st_Ms2DCP.f_RealVal +  pstrMnt->st_Ms2DCN.f_RealVal)/2.0f;
                }
                else
                {
                        f_RealVal = ( (STRMntAttr *)( (STRMntAttr *)pstrMnt + ush_Offset) )->f_RealVal;
                }

                pstrCheckVol[i].f_ReadValue = f_RealVal;
                pstrCheckVol[i].ul_ReadTime = ul_Tick;
        }
}

//==================================================================================================
//| 函数名称 | UpdateVoltageCheckListWrite
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 根据接收上位机的协议号来寻址设定电压
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | ush_Index -- 内存映射表索引值 ， f_Write -- 设定电压值，ul_Tick -- 最新设定时间点
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | -1 -- 操作异常， 0 -- 操作成功
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | 李壮 2016.8.2
//==================================================================================================
void UpdateVoltageCheckListWrite(INT16U ush_Index, FP32 f_Write,    INT32U ul_Tick)
{
        STRCheckVoltageList *pstrCheckVol = GetCheckVoltageListPtr();

        if ( ush_Index >= ( MAX_VOL_CHANNEL) )
        {
                return;
        }
        pstrCheckVol[ush_Index].f_WriteValue = f_Write;
        pstrCheckVol[ush_Index].ul_WriteTime = ul_Tick;
}


//==================================================================================================
//| 函数名称 | CheckVoltage
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 |  对电压设置值和实际值进行校验
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 |
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2017.09.05
//==================================================================================================
INT32U CheckVoltage(void)
{
        INT32U ul_RealDeltaTime = 0;
        FP32 f_RealDeltaValue = 0;
        FP32 f_SetDeltaValue = 0;
        INT32U ul_Ret = 0;

        //STRSetVoltageList *pstrSetVol = GetSetVoltageListPtr();
        STRCheckVoltageList *pstrCheckVol =  GetCheckVoltageListPtr();

        for(int i=0;i<MAX_VOL_CHANNEL- 1;i++)
        {
                if ( 0xffff == pstrCheckVol[i].ush_MemMapOffset )
                {
                        continue;
                }

                if(
                          cl_App.m_uchMSRunStatus == MS_RUNSTATUS_RUN
                        && (  pstrCheckVol[i].ush_VoltageListIndex == 0  //cid entre
                           || pstrCheckVol[i].ush_VoltageListIndex == 1 //cid exit
                           || pstrCheckVol[i].ush_VoltageListIndex == 18 //cone
                           || pstrCheckVol[i].ush_VoltageListIndex == 25 //cid energe
                           || pstrCheckVol[i].ush_VoltageListIndex == 23) //ms2 pre
                       ) //在仪器跑方法过程中，将会对部分电压认为是没有设置。
               {
                   pstrCheckVol[i].ul_WriteTime = 0;
                   continue;
               }

                //判断是否设置过
                if ( pstrCheckVol[i].ul_WriteTime == 0 )
                {
                        continue;
                }


                //如果是毛细管高压 需要毛细管高压开启
                if( (pstrCheckVol[i].ush_VoltageListIndex == 5  //capi
                        || pstrCheckVol[i].ush_VoltageListIndex == 20 //da_cp
                        )
                        && g_clIOCtrl.m_stIOCtrl.uch_CoroIO == 0)
                {
                        pstrCheckVol[i].ul_WriteTime = QDateTime::currentDateTime().toTime_t();
                        continue;
                }


                //如果是倍增器高压，
                if( ( pstrCheckVol[i].ush_VoltageListIndex == 4
                        || pstrCheckVol[i].ush_VoltageListIndex == 7
                        || pstrCheckVol[i].ush_VoltageListIndex == 24)
                        && g_clIOCtrl.m_stIOCtrl.uch_EMIO == 0)
                {
                        pstrCheckVol[i].ul_WriteTime = QDateTime::currentDateTime().toTime_t();

                        continue;
                }

                //不检测离子能量  和DAC第四片
               if( pstrCheckVol[i].ush_VoltageListIndex == 8
                       || pstrCheckVol[i].ush_VoltageListIndex == 9

//                       || pstrCheckVol[i].ush_VoltageListIndex == 20
//                       || pstrCheckVol[i].ush_VoltageListIndex == 21
//                       || pstrCheckVol[i].ush_VoltageListIndex == 22
//                       || pstrCheckVol[i].ush_VoltageListIndex == 23

                       )

               {

                       continue;
               }

               if(!g_clIOCtrl.m_stIOCtrl.uch_PumpIO) //待机状态 不断更新write time, 将不会进行 电压比较判断
               {
                       pstrCheckVol[i].ul_WriteTime = QDateTime::currentDateTime().toTime_t();
                       continue;
               }

                //判断设置过后是否读取更新过
                if( pstrCheckVol[i].ul_ReadTime <= pstrCheckVol[i].ul_WriteTime)
                {
                        continue;
                }


                ul_RealDeltaTime = pstrCheckVol[i].ul_ReadTime - pstrCheckVol[i].ul_WriteTime;


                //判断是否达到等待时间
                if( ul_RealDeltaTime < pstrCheckVol[i].ul_SetWaitTime)
                {
                        continue;
                }


                //计算差值  有设置值和反馈值符号不同 直接 比较绝对值的差
                if(
                        ( pstrCheckVol[i].ush_VoltageListIndex ==5)
                       || ( pstrCheckVol[i].ush_VoltageListIndex ==20)
                       || ( pstrCheckVol[i].ush_VoltageListIndex ==4)
                       || ( pstrCheckVol[i].ush_VoltageListIndex ==7)
                       || ( pstrCheckVol[i].ush_VoltageListIndex ==24)


                       )
                {
                       f_RealDeltaValue = FP32 ( fabs(pstrCheckVol[i].f_WriteValue) - fabs(pstrCheckVol[i].f_ReadValue) );
                }
                else
                {
                       f_RealDeltaValue = FP32 ( pstrCheckVol[i].f_WriteValue - pstrCheckVol[i].f_ReadValue );
                }

                f_SetDeltaValue = pstrCheckVol[i].f_SetDeltaValue;



                if ( fabs(f_RealDeltaValue) > f_SetDeltaValue)
                {
                        ul_Ret |= 1<< i;

                          qDebug("index %d  ,writevlaue %f, readvalue %f Setdeltavalue %f",i,pstrCheckVol[i].f_WriteValue,\
                                 pstrCheckVol[i].f_ReadValue ,f_SetDeltaValue);


                }





        }
        return ul_Ret;
}

//==================================================================================================
//| 函数名称 | SetParameterByVoltageList
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 根据接收上位机的协议号来寻址进行电压设置
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | puch_iData -- 接收上位机数据指针
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | -1 -- 操作异常， 0 -- 操作成功
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.16
//==================================================================================================
INT8U SetParameterByVoltageList(INT8U *puch_iData)
{
    INT8U uch_Re = 0;
    INT16U uin_Data = 0;
    INT16U uin_Temp = 0;
    INT16U uin_Addr = 0;
    INT16U uch_SynValue = 0;
    INT16U uin_SynAddr = 12;
    FP32 f_FirstData = 0;
    FP32 f_SecondData = 0;

    INT8U uch_ProctolFlag = puch_iData[0];
    FP32 f_Data = DecodeFloat(&puch_iData[1]);

    //根据协议标志位搜索
    STRSetVoltageList *pstrVoltageList = FindSetVolListPtr(uch_ProctolFlag);
    INT16U ush_Index = FindSetVolListIndex(uch_ProctolFlag);

    if (pstrVoltageList == NULL )
    {
        uch_Re = 1;
    }
    else
    {
        //更新设置值

        //TODO:clock
        UpdateVoltageCheckListWrite(ush_Index,  f_Data, QDateTime::currentDateTime().toTime_t());

        //写设置标志位
        pstrVoltageList->uch_CmdSetFlag = 1;

        //获得硬件地址
        uin_Addr = pstrVoltageList->ush_Addr;
        //一级公式运算
        f_FirstData = f_Data * pstrVoltageList->f_k;
        //二级公式运算
        f_SecondData = (f_FirstData * pstrVoltageList->f_a  + pstrVoltageList->f_b) * pstrVoltageList->f_c;

        uin_Data = (INT16U)f_SecondData;



        if(pstrVoltageList->uch_Mask != 0) //使用多通道的da输出处理
        {
             if(uin_Data > 0xfff)
             {
                uin_Data = 0xfff;
             }

        }
        uin_Data =  uin_Data | ( pstrVoltageList->uch_Mask <<8 );

        //获取写同步标志值
        uch_SynValue = pstrVoltageList->uch_SyncValue;

        //qDebug("PC %.1f,1st %.3f, 2nd %.1f, fp %04x, mk %02x, pt %02x, %s\r\n", \
            f_Data,f_FirstData,f_SecondData,uin_Data,pstrVoltageList->uch_Mask,pstrVoltageList->uch_ProctolFlag, pstrVoltageList->asch_Note);


        //写高字节
        uin_Temp = (uin_Data & 0xff00) >> 8;
        MemoryWrite(FPGA_REG, uin_Addr, &uin_Temp);
        //写低字节
        uin_Temp = uin_Data & 0x00ff;
        MemoryWrite(FPGA_REG, uin_Addr + 1, &uin_Temp);

        OperateRunFlag(uch_SynValue, uin_SynAddr);

    }

        return uch_Re;

}



//==================================================================================================
//| 函数名称 | ReadAD
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 设置fpga的 da值
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | wxb20130820
//==================================================================================================
void WriteDA(INT16U uin_Addr, FP32 f_Data, INT8U uch_SynValue,INT8U uch_iChannel /* = 0 */, INT16U uin_SynAddr /* = 12 */)
{
        INT16U uin_Data = 0, uin_Temp = 0;

        switch(uin_Addr)
        {
        case DA_LTC1595_1_addr:
        case DA_LTC1595_2_addr:
                {
                        uin_Data = (INT16U)(f_Data * 65535 / MSRF_REFERENCE);
                }
                break;

        case DA_AD5546_1_addr:
        case DA_AD5546_2_addr:
                {
                        uin_Data = (INT16U)(f_Data * 32767 / 4.1f);
                }
                break;

        case DA_DAC121_1_addr:
                {
                        uin_Data = (INT16U)(f_Data * 4095 / 4.1f);
                }
                break;

        case DA_DAC121_2_addr:
                {
                        uin_Data = (INT16U)(f_Data * 4095 / 2.5f);
                }
                break;

        case DA_DAC124_11_addr:
                {
                        uin_Data = (INT16U)(f_Data * 4095 / 4.1f);

                        if(1 == uch_iChannel)
                        {
                                uin_Data = (0x1000 | uin_Data);
                        }
                        else if(2 == uch_iChannel)
                        {
                                uin_Data = (0x5000 | uin_Data);
                        }
                        else if(3 == uch_iChannel)
                        {
                                uin_Data = (0x9000 | uin_Data);
                        }
                        else if(4 == uch_iChannel)
                        {
                                uin_Data = (0xd000 | uin_Data);
                        }
                }
                break;

        case DA_DAC124_21_addr:
                {
                        uin_Data = (INT16U)(f_Data * 4095 / 2.5f);
                        if(1 == uch_iChannel)
                        {
                                uin_Data = (0x1000 | uin_Data);
                        }
                        else if(2 == uch_iChannel)
                        {
                                uin_Data = (0x5000 | uin_Data);
                        }
                        else if(3 == uch_iChannel)
                        {
                                uin_Data = (0x9000 | uin_Data);
                        }
                        else if(4 == uch_iChannel)
                        {
                                uin_Data = (0xd000 | uin_Data);
                        }
                }
                break;

        case DA_DAC124_31_addr:
                {
                        //正负值直接由硬件离子开关控制
                        uin_Data = (INT16U)(f_Data * 4095 / 4.1f);
                        //uin_Data =  (INT16U)(2048 - f_Data * 2047 / 4.1f);
                        if(1 == uch_iChannel)
                        {
                                uin_Data = (0x1000 | uin_Data);
                        }
                        else if(2 == uch_iChannel)
                        {
                                uin_Data = (0x5000 | uin_Data);
                        }
                        else if(3 == uch_iChannel)
                        {
                                uin_Data = (0x9000 | uin_Data);
                        }
                        else if(4 == uch_iChannel)
                        {
                                uin_Data = (0xd000 | uin_Data);
                        }
                }
                break;

        case DA_LTC8043_1_addr:
        case DA_LTC8043_2_addr:
                {
                        //lz2015.12.31 ms ionic energy
                        //if (cl_Xml.m_XmlCoonfig.bIonicEnergyConnected == 1)
                        //{
                                //设绝对值
                                uin_Data = (INT16U)(f_Data * 2047 / MSRF_REFERENCE);
                        //}
                        //else
                        //{
                                //正负可设
                         //       uin_Data =  (INT16U)(2048 - f_Data * 2047/MSRF_REFERENCE);
                        //}
                }
                break;

        case DA_LTC8043_3_addr:
                {
                        uin_Data = (INT16U)(f_Data * 2047 / 4.1f);
                        //uin_Data =  (INT16U)(2048 - f_Data * 2047 / 4.1f);
                }
                break;

        case DA_LTC8043_4_addr:
                {
                        uin_Data =  (INT16U)(2048 - f_Data * 2047 / 4.1f);
                }
                break;

        case DA_LTC8043_5_addr:
        case DA_LTC8043_6_addr:
        case DA_LTC8043_11_addr:
        case DA_LTC8043_12_addr:
                {
                        uin_Data =  (INT16U)(2048 - f_Data * 2047 / 4.1f);
                }
                break;

        case DA_LTC8043_7_addr:
        case DA_LTC8043_8_addr:
        case DA_LTC8043_9_addr:
        case DA_LTC8043_10_addr:
                {
                        uin_Data = (INT16U)(f_Data * 4095 / 300);
                }
                break;

        case ION_SWITCH_MS_addr:
                {
                        uin_Data = (INT16U)(f_Data);
                }
                break;

        case ION_CONNECTED_addr:
                {
                        uin_Data = (INT16U)(f_Data);
                }
                break;

        default:
                {
                        ;
                }
                break;
        }



        uin_Temp = (uin_Data & 0xff00) >> 8;
        MemoryWrite(FPGA_REG, uin_Addr, &uin_Temp);
        uin_Temp = uin_Data & 0x00ff;
        MemoryWrite(FPGA_REG, uin_Addr + 1, &uin_Temp);
        //if(DA_DAC124_21_addr == uin_Addr)
        //  RETAILMSG(1,(_T("写地址为【%d】 数值为【%d】 同步信号量为【%d】 \r\n "),uin_Addr,uin_Data,uch_SynValue));
        OperateRunFlag(uch_SynValue, uin_SynAddr);
}

//==================================================================================================
//| 函数名称 | ReadAD
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读取fpga的 ad值
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | wxb20130820
//==================================================================================================
FP32 ReadAD(INT16U uin_Addr, FP32* f_Data, INT8U uch_SynValue, INT16U uin_SynAddr /* = 12 */)
{
        INT16U uin_Data = 0,uin_Temp = 0;
        FP32 f_Temp = 0;


        MemoryRead(FPGA_REG, uin_Addr, &uin_Data);

        //数据转换
        if ((AD_AD7689_CS1_6_addr == uin_Addr)
                || (AD_AD7689_CS1_7_addr == uin_Addr)
                || (AD_AD7689_CS1_8_addr == uin_Addr)
                || (AD_AD7689_CS5_7_addr == uin_Addr))
        {
                if (uin_Data < 32768)
                {
                        uin_Data = uin_Data << 1;
                        f_Temp = uin_Data * 2.5f / 65535;
                }
                else
                {
                        uin_Data = uin_Data << 1;
                        f_Temp = uin_Data * 2.5f / 65535;
                        f_Temp = f_Temp * -1.0f;
                }
        }
        else
        {
                f_Temp = uin_Data * 2.5f / 65535;
        }

        if(f_Data != NULL)
        {
                *f_Data = f_Temp;
        }

        return f_Temp;
}


//==================================================================================================
//| 函数名称 | ResetAllVol
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 将所有电压都设置为0
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.07.15
//==================================================================================================
void ResetAllVol(void)
{
        FP32 f_Data = 0.0;
        INT8U auch_Data[5];

        EncodeFloat(f_Data,auch_Data+1);

        //对应的电压对协议中的寄存器地址

        auch_Data[0] = 0x09;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x0b;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x04;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x03;
        SetParameterByVoltageList(auch_Data);

        auch_Data[0] = 0x13;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x00;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x01;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x11;
        SetParameterByVoltageList(auch_Data);

        auch_Data[0] = 0x06;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x0e;
        SetParameterByVoltageList(auch_Data);


        auch_Data[0] = 0X08;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x07;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x10;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x0f;
        SetParameterByVoltageList(auch_Data);

        auch_Data[0] = 0x18;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x19;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x1a;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x1b;
        SetParameterByVoltageList(auch_Data);

        auch_Data[0] = 0x02;
        SetParameterByVoltageList(auch_Data);
        //auch_Data[0] = 0x20;
        //SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x1f;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x14;
        SetParameterByVoltageList(auch_Data);

        auch_Data[0] = 0x05;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0X0D;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x12;
        SetParameterByVoltageList(auch_Data);
        auch_Data[0] = 0x0a;
        SetParameterByVoltageList(auch_Data);




        return;
}

//==================================================================================================
//| 函数名称 | ReadFPGAVersion
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读取FPGA版本号
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U *puch_Data, 输入32byte数组,程序将赋值给该数组版本号
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.07.15
//==================================================================================================
void ReadFPGAVersion(INT8U *puch_Data)
{
        int i;
        INT16U uin_Addr;
        uin_Addr = TEST_ADDR_VERSION;
        INT16U uin_ReadVersion[32];

        ////qDebug("FPGA Version read star");
        MemoryRead(DSRAM_TEST,uin_Addr, uin_ReadVersion,32);

        INT8U version[33] = "   EXPEC5210.0667D.U116M.T1A.000";
        for(int i = 0;i<32;i++)
        {

            version[i] = (INT8U)uin_ReadVersion[i];
        }
        memcpy(puch_Data, version, 32);

        ////qDebug("version %s",version);



}

//==================================================================================================
//| 函数名称 | FPGABusTest
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 测试FPGA与ARM的总线
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 |
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT32U FPGABusTest()
{

    INT32U ul_Ret = 0;
    INT16U ush_DataWrite = 0;
    INT16U ush_DataRead = 0;
    INT8U uch_Loop = 0;


    //low addr test
    for ( uch_Loop = 0; uch_Loop < 16; uch_Loop ++)
    {
            ush_DataWrite = (1 << uch_Loop);
            MemoryWrite(DSRAM_TEST, TEST_ADDR_LOW, &ush_DataWrite);

            MemoryRead(DSRAM_TEST, TEST_ADDR_LOW, &ush_DataRead);
            if ( ush_DataRead != ush_DataWrite)
            {
                    ul_Ret |= (1 << uch_Loop);
//                    qDebug("ush_DataRead%x,ush_DataWrite%x",ush_DataRead,ush_DataWrite);
            }
    }

    //high addr test
    for ( uch_Loop = 0; uch_Loop < 16; uch_Loop ++)
    {
            ush_DataWrite = (1 << uch_Loop);
            MemoryWrite(DSRAM_TEST, TEST_ADDR_HIGH, &ush_DataWrite);

            MemoryRead(DSRAM_TEST, TEST_ADDR_HIGH, &ush_DataRead);
            if ( ush_DataRead != ush_DataWrite)
            {
                    ul_Ret |= (1 << ( 16 + uch_Loop) );
//                    qDebug("ush_DataRead%x,ush_DataWrite%x",ush_DataRead,ush_DataWrite);

            }
    }



    return ul_Ret;
}


//==================================================================================================
//| 函数名称 | FPGABusPerformanceTest
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 测试FPGA与ARM的总线性能
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 |
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
void FPGABusPerformanceTest(void)
{

        //single wirte and read
//        while(1)
//        {
//            static INT32U ul_ReadWriteCnt = 0;
//            static INT32U ul_ReadWriteErrCnt = 0;

//            INT32U ul_SelfTest = FPGABusTest();
//            ul_ReadWriteCnt++;
//            if ( ul_SelfTest != 0)
//            {
//                  ul_ReadWriteErrCnt++;

//            }
//            if(ul_ReadWriteCnt%1000 == 0)
//            {
//                qDebug("FPGA Test ul_ReadWriteCnt%d,ul_ReadWriteErrCnt%d",ul_ReadWriteCnt,ul_ReadWriteErrCnt);

//            }

//        }

        //multi write and read
        while(1)
        {
            static INT32U ul_ReadWriteCnt = 0;
            static INT32U ul_ReadWriteErrCnt = 0;
            INT16U ush_DataWrite[1024];
            INT16U ush_DataRead[1024];

            INT16U data = ul_ReadWriteCnt&0xffff;
            for(int i=0;i<1024;i++)
            {
                ush_DataWrite[i] = data+i;
            }



            MemoryWrite(DSRAM_TEST, TEST_ADDR_LOW, ush_DataWrite,1024);

            MemoryRead(DSRAM_TEST, TEST_ADDR_LOW, ush_DataRead , 1024);

            if ( ush_DataWrite[0]!= ush_DataRead[0] && ush_DataWrite[1023]!= ush_DataRead[1023])
            {
                  ul_ReadWriteErrCnt++;

            }


            ul_ReadWriteCnt++;
            if(ul_ReadWriteCnt%100 == 0)
            {
                qDebug("FPGA Test ul_ReadWriteCnt%d,ul_ReadWriteErrCnt%d,ReadData%d",ul_ReadWriteCnt,ul_ReadWriteErrCnt,ush_DataRead[1023]);

            }




        }




}


//==================================================================================================
//| 函数名称 | MemoryRead
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | fpga 寄存器读取
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.19
//==================================================================================================
INT8U MemoryRead(INT8U uch_iType,const INT16U uin_Addr,INT16U *uin_pData,INT16U uin_DataLen)
{
        mutex_Memory.lock();
        INT8U uch_Re = 1;
        INT32U ul_Addr = 0;
        INT32U ul_rtn = 0;


        switch(uch_iType)
        {
            case DSRAM_RECT:
            {

            }
            break;
            case FPGA_REG:
            {
                 ul_Addr = ARM_CS2 + uin_Addr;
                 ul_rtn = cl_bspFpga.ReadFromFPGA(ul_Addr,uin_pData,uin_DataLen);
            }
            break;
            case DSRAM_DATA:
            {
                ul_Addr = ARM_CS5 + uin_Addr;
                ul_rtn = cl_bspFpga.ReadFromFPGA(ul_Addr,uin_pData,uin_DataLen);
            }
            break;
            case DSRAM_TEST:
            {
                ul_Addr = ARM_CS6 + uin_Addr;
                ul_rtn = cl_bspFpga.ReadFromFPGA(ul_Addr,uin_pData,uin_DataLen);
            }
            break;

        }


        //qDebug("MemoryRead ul_Addr %d,len %d, retern %d",ul_Addr,uin_DataLen,ul_rtn);
        mutex_Memory.unlock();
        return uch_Re;
}
//==================================================================================================
//| 函数名称 | MemoryWrite
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | fpga 寄存器写
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.19
//==================================================================================================

INT8U MemoryWrite(INT8U uch_iType,const INT16U uin_Addr,const void *uin_pData,INT16U uin_DataLen)
{
        INT8U uch_Re = 1;
        INT32U ul_Addr = 0;
        INT16U *puin_Data = (INT16U *) uin_pData;
        mutex_Memory.lock();
        INT32U ul_rtn = 0;
        switch(uch_iType)
        {
                case DSRAM_RECT:
                {
                    ul_Addr = ARM_CS3 + uin_Addr;
                    ul_rtn = cl_bspFpga.WriteToFPGA(ul_Addr,puin_Data,uin_DataLen);
                }
                break;

                case FPGA_REG:
                {
                    ul_Addr = ARM_CS2 + uin_Addr;
                    ul_rtn = cl_bspFpga.WriteToFPGA(ul_Addr,puin_Data,uin_DataLen);
                }
                break;

                case DSRAM_METHOD:
                {
                    ul_Addr = ARM_CS4 + uin_Addr;
                    ul_rtn = cl_bspFpga.WriteToFPGA(ul_Addr,puin_Data,uin_DataLen);
                }
                break;

                case DSRAM_TEST:
                {
                    ul_Addr = ARM_CS6 + uin_Addr;
                    ul_rtn = cl_bspFpga.WriteToFPGA(ul_Addr,puin_Data,uin_DataLen);
                }
                break;
        }
        //qDebug("MemoryWrite ul_Addr %d,first data %d ,len %d",ul_Addr,(*puin_Data),uin_DataLen);

        mutex_Memory.unlock();
        return uch_Re;
}

//==================================================================================================
//| 函数名称 |
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 |
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.19
//==================================================================================================
void OperateSwitch(INT8U uch_SwitchType,INT8U uch_OperType)
{
        switch(uch_SwitchType)
        {
        case ARM_ERROR:
                switch(uch_OperType)
                {
                case SWITCH_ON:
                        //epcIoSetAPin(ARM_ERROR,1);
                        gpio_ctl(GPIO_ERROR,1,1);

                        break;
                case SWITCH_OFF:
                        gpio_ctl(GPIO_ERROR,1,0);
                        //epcIoSetAPin(ARM_ERROR,0);
                        break;
                case SWITCH_TURN:
                        break;
                }
                break;
        case ARM_RUN:
                switch(uch_OperType)
                {
                case SWITCH_ON:
                        gpio_ctl(GPIO_RUN,1,1);
                        //epcIoSetAPin(ARM_RUN,1);
                        break;
                case SWITCH_OFF:
                        gpio_ctl(GPIO_RUN,1,0);
                        //epcIoSetAPin(ARM_RUN,0);
                        break;
                case SWITCH_TURN:
                        break;

                }
                break;
        case ARM_ALARM:
                switch(uch_OperType)
                {
                case SWITCH_ON:
                        gpio_ctl(GPIO_ALARM,1,1);
                        //epcIoSetAPin(ARM_ALARM,1);
                        break;
                case SWITCH_OFF:
                        gpio_ctl(GPIO_ALARM,1,0);
                        //epcIoSetAPin(ARM_ALARM,0);
                        break;
                case SWITCH_TURN:
                        break;
                }
                break;
        case FPGA_RUN:
                {
                    switch(uch_OperType)
                    {
                        case SWITCH_ON:
                        {

                            INT16U uin_Data = SWITCH_ON;
                            MemoryWrite(FPGA_REG, RUN_SIG, &uin_Data);
                            ////qDebug("SWITCH_ON");
                            //epcIoSetAPin(FPGA_RUN,1);
                            break;
                        }
                        case SWITCH_OFF:
                        {
                            INT16U uin_Data = SWITCH_OFF;
                            MemoryWrite(FPGA_REG, RUN_SIG, &uin_Data);
                            ////qDebug("SWITCH_OFF");
                            //epcIoSetAPin(FPGA_RUN,0);
                            break;
                        }
                        case SWITCH_TURN:
                        {
                            break;
                        }
                    }
                }
                break;
        case MS1_DC_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(MS1_DC_SW_ON);*/
                        OperateRunFlag(SW_MS1_DC_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(MS1_DC_SW_OFF);*/
                        OperateRunFlag(SW_MS1_DC_OFF_syn);
                }
                break;
        case MS2_DC_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(MS2_DC_SW_ON);*/
                        OperateRunFlag(SW_MS2_DC_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(MS2_DC_SW_OFF);*/
                        OperateRunFlag(SW_MS2_DC_OFF_syn);
                }
                break;
        case MS1_RF_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(MS1_RF_SW_ON);*/
                        OperateRunFlag(SW_MS1_RF_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(MS1_RF_SW_OFF);*/
                        OperateRunFlag(SW_MS1_RF_OFF_syn);
                }
                break;
        case MS2_RF_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(MS2_RF_SW_ON);*/
                        OperateRunFlag(SW_MS2_RF_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(MS2_RF_SW_OFF);*/
                        OperateRunFlag(SW_MS2_RF_OFF_syn);
                }
                break;
        case CORO_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(CORO_SW_ON);*/
                        OperateRunFlag(SW_ECorona_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(CORO_SW_OFF);*/
                        OperateRunFlag(SW_ECorona_OFF_syn);
                }
                break;
        case DORA_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(DANA_SW_ON);*/
                        OperateRunFlag(SW_Dynode_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(DANA_SW_OFF);*/
                        OperateRunFlag(SW_Dynode_OFF_syn);
                }
                break;
        case NEGION_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(NEGION_SW_ON);*/
                        OperateRunFlag(SW_VoltSignSwitch_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(NEGION_SW_OFF);*/
                        OperateRunFlag(SW_VoltSignSwitch_OFF_syn);
                }
                break;
        case EM_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(EM_SW_ON);*/
                        OperateRunFlag(SW_EMultipiper_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(EM_SW_OFF);*/
                        OperateRunFlag(SW_EMultipiper_OFF_syn);
                }
                break;
        case EM_PROTECT_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(EM_PROTECT_SW_ON);*/
                        OperateRunFlag(SW_EMultipiper_Safeguard_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(EM_PROTECT_SW_OFF);*/
                        OperateRunFlag(SW_EMultipiper_Safeguard_OFF_syn);
                }
                break;
        case PRE_PUMP_SW://TODO未实现
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(EM_SW_ON);*/
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(EM_SW_OFF);*/
                }
                break;
        case PUMP_SW://TODO未实现
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(PUMP_SW_ON);*/
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(PUMP_SW_OFF);*/
                }
                break;
        case CAPI_SW:
                if(uch_OperType == SWITCH_ON){
                        /*OperateRunFlag(CAPI_SW_ON);*/
                        OperateRunFlag(SW_Capillary_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        /*OperateRunFlag(CAPI_SW_OFF);*/
                        OperateRunFlag(SW_Capillary_OFF_syn);
                }
                break;
        case BIT_SW:
                if(uch_OperType == SWITCH_ON){
                        OperateRunFlag(SW_BIT_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        OperateRunFlag(SW_BIT_OFF_syn);
                }
                break;
        case LBVacuumG_SW:
                if(uch_OperType == SWITCH_ON){
                        OperateRunFlag(SW_LBVacuumG_ON_syn);
                }else if(uch_OperType == SWITCH_OFF){
                        OperateRunFlag(SW_LBVacuumG_OFF_syn);
                }



                break;
        }
}

//==================================================================================================
//| 函数名称 |
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 |
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 |
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.19
//==================================================================================================
void OperateRunFlag(INT8U uch_SyncValue,INT16U uin_SyncAddr)
{
        INT16U uin_Data = 0;
        mutex_RunFlag.lock();


        if(uch_SyncValue == 1 || uch_SyncValue == 2);


        //RETAILMSG(1,(_T("同步地址：【%d】  信号量：【%d】 \r\n"),uin_iAddr,uch_iState));


        OperateSwitch(FPGA_RUN, SWITCH_OFF);
        uin_Data = uch_SyncValue;
        MemoryWrite(FPGA_REG, uin_SyncAddr, &uin_Data);
        OperateSwitch(FPGA_RUN, SWITCH_ON);


        if (uch_SyncValue == STOP_MS_FLAG)
        {
                //Sleep(1); //TODO:sleep
                //ClearRes();//lz2016.4.6 停止扫描后将res清0
                //WriteDA(DA_LTC1595_1_addr, 0, DA_LTC1595_1_syn);
                //WriteDA(DA_LTC1595_2_addr, 0, DA_LTC1595_2_syn);
        }
        //qDebug("OperateRunFlag synvalue %d, syncAddr %d",uch_SyncValue,uin_SyncAddr);
        mutex_RunFlag.unlock();
}

//no more
