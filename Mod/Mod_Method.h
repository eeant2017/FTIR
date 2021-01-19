//==================================================================================================
//| 文件名称 | Mod_Method.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Mod_Method.h所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2009, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.2    | 2019.08.16  |lyb      | 移植
//==================================================================================================
#ifndef MOD_METHOD_H
#define MOD_METHOD_H

//--------------------------------------------------------------------------------------------------

#include <string.h>
#include "typedefine.h"
#include <QThread>
#ifdef __cplusplus
extern "C" {
#endif

#define MSRF_REFERENCE 10.0

        //对应LC-MS协议扫描参数的字节地址  200个字节
        //以下定义的为相应的参数的起始地址，相应位数参考LC-MS协议 未定义的以注释给出
        //注：定义中的 method 为方法片段， sect 为时序片段
        //0总包数
        //2当前包号
        //----方法片段信息参数----
#define ARM_TOTAL_SECT_NUM		4 //全部时序片段号
#define ARM_TOTAL_METHOD_NUM    8 //全部方法片段号
#define ARM_CURRENT_METHOD_NUM  10//当前方法片段号
#define ARM_START_TIME			12//开始时间
#define ARM_STOP_TIME			14//结束时间
#define ARM_CYCLE_TIME			16//方法片段循环时间
#define ARM_METHOD_PIONT		18//一个方法片段的数据全部点数
                                                                        //保留4字节
        //-----时序片段参数----
#define ARM_TOTAL_SECT_NUM_IN1METHOD 24 //当前方法片段的总时序片段数
#define ARM_CURRENT_SECT_NUM_IN1METHOD 		26//当前方法片段的时序片段号
#define ARM_SECT_TIME			28 //时序片段运行时间

        //----每个时序片段的输出信息---
#define ARM_ENABLE_SCAN			32//扫描使能
#define ARM_SAMP_EXP_TIMES		33//平均次数
#define ARM_MS1_DC_ENABLE		35//DC使能(MS1)
#define ARM_MS2_DC_ENABLE		36//DC使能(MS2)
#define ARM_EM_HIGH_SW			37//EM高压切换标志位
#define ARM_ION_MODE			38//离子源选择
#define ARM_TOTAL_DATA_CNT		39//时序片段数据总点数
        //ms1扫描设置
#define ARM_MS1_RAMP_EN			41//RF Ramp波形标志（MS1）
        //保留4字节
#define ARM_MS1_RAMP_BEGIN_V	46//RF Ramp起始电压（MS1）
#define ARM_MS1_RAMP_END_V		50//RF Ramp终止电压（MS1）
#define ARM_MS1_FIXED_EN		54//RF fixed使能（MS1）
#define ARM_MS1_FIXED_V			55//RF fixed幅值（MS1）
        //ms2扫描设置
#define ARM_MS2_RAMP_EN			59//RF Ramp波形标志（MS2）
        //保留4字节
#define ARM_MS2_RAMP_BEGIN_V	64//RF Ramp起始电压（MS2）
#define ARM_MS2_RAMP_END_V		68//RF Ramp终止电压（MS2）
#define ARM_MS2_FIXED_EN		72//RF fixed使能（MS2）
#define ARM_MS2_FIXED_V			73//RF fixed幅值（MS2）
#define ARM_TRASN_RAMP_BEGIN_V  77//RF Ramp起始电压（TRANS）
#define ARM_TRASN_RAMP_END_V    81//RF Ramp终止电压（TRANS）
#define ARM_TRANS_RAMP_CNT      85//TRANS RF从起始电压到转折电压的点数

        //17字节保留
#define ARM_CONE_V				94//样品锥
#define ARM_CLN_V				98//collision
#define ARM_CAPL_V			102//毛细管高压
#define ARM_CORO_V			106//电晕针电流
#define ARM_EXTRA_V			110//萃取锥
#define ARM_ROD_V			114//传输杆
#define ARM_MS1_PRE_V		118//MS1预杆
#define ARM_MS1_ION_V		122//MS1离子能量
#define ARM_MS1_LM_V		126//MS1LM
#define ARM_MS1_HM_V		130//MS1HM
#define ARM_CID_IN_V		134//CID入口电压
#define ARM_CID_OUT_V		138//CID出口电压
#define ARM_CID_OVER_V		142//CID外壁电压
#define ARM_MS2_PRE_V		146//MS2预杆电压
#define ARM_MS2_ION_V		150//MS2离子能量
#define ARM_MS2_LM_V		154//MS2LM
#define ARM_MS2_HM_V		158//MS2HM
#define ARM_EM_V			162//EM倍增器电压
#define ARM_EM_TURN_V		166//EM偏转电压
#define ARM_EM_IN_V			170//EM入口电压
#define ARM_HV1_V			174//保留2[HV1]
#define ARM_HV2_V			178//保留3[HV2]
#define ARM_DA_CP_V			90//挡板电压
#define ARM_DC1_V			182//DC1
#define ARM_DC2_V			186//DC2
#define ARM_DC3_V			190//DC3
#define ARM_DC4_V			194//DC4

        //18字节预留
#define ARM_NULL_V			255



//FPGA中sdram储存的时序片段信息对应的参数地址 64个word一段  位宽16bit
#define TOTAL_SECT_NUM_H		0//全部时序片段数(所有方法片段的时序片段和)高字节
#define TOTAL_SECT_NUM_L        1//全部时序片段数低字节
#define TOTAL_METHOD_NUM		2//全部方法片段数
#define CURRENT_METHOD_NUM		3//当前方法片段号
#define START_TIME				4//方法片段运行开始时间
#define STOP_TIME				5//方法片段运行停止时间
#define CYCLE_TIME				6//方法片段运行循环时间
#define	METHOD_PIONT			7//方法片段数据点数
                                                                        //保留1个地址
#define TOTAL_SECT_NUM_IN1METHOD 9//当前方法片段的全部时序片段数
#define CURRENT_SECT_NUM_IN1METHOD 10//当前方法片段的当前时序片段号
#define SECT_TIME_H			11//时序片段运行时间高字节
#define SECT_TIME_L			12//时序片段运行时间低字节
#define ENABLE_SCAN			13//扫描使能
#define	CAP_EVERAGE_TIME	14//采集平均次数

//部分参数位数不足16bit 将他们整合在一起，定义中名字在前的为高8位
#define MS1_AND_MS2_DC_ENABLE	15//DC使能（MS1)和DC使能（MS2）
#define EM_HIGH_SW_AND_ION_MOD	16//EM高压切换标志 和 离子源模式

#define MS1_RAMP_BEGIN_V	17//RAMP起始电压（MS1） //如果是fixed电压也放在该位置
#define MS1_RAMP_DELTA_V	18//RAMP电压差值（MS1）
#define MS2_RAMP_BEGIN_V	19//RAMP起始电压（MS2）//如果是fixed电压也放在该位置
#define MS2_RAMP_DELTA_V	20//RAMP电压差值（MS2）
#define TRANS_RAMP_BEGIN_V	25//RAMP起始电压（TRANS）//没有fixed概念了，delta是0或者转折电压点数为1就为起始电压
#define TRANS_RAMP_DELTA_V	26//RAMP电压差值（TRANS）
#define TRANS_RAMP_SEND_POINTS 27//trans rf ramp从起始到转折的点数

#define MS1_AND_MS2_WAVE_TYPE   21//MS1和MS2波形选择
#define RAMP_SEND_POINTS		22//发送点数
#define RAMP_SEND_CYCLE_H		23//RAMP发送周期高字节
#define RAMP_SEND_CYCLE_L		24//RAMP发送周期低字节
        //保留4个地址
        //不用ramp的电压
#define JUNK_CONE_V			33//
#define JUNK_COLN_V			30//
#define JUNK_CAPL_V			31//毛细管高压
#define JUNK_CORO_V			32//电晕针电流
#define JUNK_EXTRA_V		29//  和cone交换
#define JUNK_ROD_V			34//传输杆
#define JUNK_MS1_PRE_V		35//MS1预杆
#define JUNK_MS1_ION_V		36//MS1离子能量
#define JUNK_MS1_LM_V		37//MS1LM
#define JUNK_MS1_HM_V		38//MS1HM
#define JUNK_CID_IN_V		39//CID入口电压
#define JUNK_CID_OUT_V		40//CID出口电压
#define JUNK_CID_OVER_V		41//CID外壁电压
#define JUNK_MS2_PRE_V		42//MS2预杆电压
#define JUNK_MS2_ION_V		43//MS2离子能量
#define JUNK_MS2_LM_V		44//MS2LM
#define JUNK_MS2_HM_V		45//MS2HM
#define JUNK_EM_V			46//EM倍增器电压
#define JUNK_EM_TURN_V		47//EM偏转电压
#define JUNK_EM_IN_V		48//EM入口电压
#define JUNK_HV1_V			49//保留2[HV1]
#define JUNK_HV2_V			50//保留3[HV2]
#define JUNK_DA_CP_V		51//挡板电压
#define JUNK_DC1_V			52//DC1
#define JUNK_DC2_V			53//DC2
#define JUNK_DC3_V			54//DC3
#define JUNK_DC4_V			55//DC4


#define MS1_RAMP_BEGIN_H_V	56//RAMP起始电压（MS1） 高16bit
#define MS1_RAMP_DELTA_H_V	57//RAMP电压差值（MS1） 高16bit
#define MS2_RAMP_BEGIN_H_V	58//RAMP起始电压（MS2） 高16bit
#define MS2_RAMP_DELTA_H_V	59//RAMP电压差值（MS2） 高16bit
#define TRANS_RAMP_BEGIN_H_V	60//RAMP起始电压（TRANS) 高16bit
#define TRANS_RAMP_DELTA_H_V	61//RAMP电压差值（TRANS） 高16bit

#define JUNK_NULL_V			63 //最高位地址



#define SECT_MAX_NUM         60000//最多的时序片段数
#define SECT_BYTE_NUM        200 //时序片段长度
#define MAX_IN_FPGA_LEN      64//一个时序片段FPGA存储的空间
#define RAM_LEN              512//方法下载1k写完，通知FPGA  //容量与fpga开辟的该内存相关

#define MAX_METHOD_NUM       60000//最多的方法片段数
#define MAX_PACK_NUM         100//TODO
#define MAX_DATA_LEN         2048 //最大数据长度 2048
#define MAX_DATA_LEN_HALF    1024//最大数据长度的一半 1024

//1个方法=255时序片段*50个字/时序片段
//地址长度=2*个方法
//外部存储空间16M字
//最多255个方法
#define METHOD_SPACE         0x6800

#define MAX_MTH_CONFIG_LEN	 1000//方法信息最大长度（字节长度）
#define METHOD_CONFIG_LENTH  5//方法信息字长度（一个方法片段需寄存器长度）

typedef struct
{
        INT8U uch_TotalNum;//总段数
        INT8U uch_Para[SECT_MAX_NUM][SECT_BYTE_NUM];//parameter of sect ,the max number is 150//存放一个方法片段的所有时序片段
}STRMethodPara;


class CMethod:public QThread
{
public:
    CMethod();
    ~CMethod()
    {

    };

    void DownLoadMethod();             //下载方法
    INT16U SaveMethod(INT8U *uch_pData,INT16U uin_iDataLen);//save method into memory

    void SetMsState(INT8U uch_iState);//1:run,0:stop


protected:

public:
        INT8U uch_pvPassMethod;//1:success,0:fail

private:  
        INT8U uch_pvTotalMthNum;
        INT32U ul_SaveSectNum;

        INT8U uch_pvMethodNum;

        INT8U uch_pvSendMethodFlag;//1:wait interrupt,0:recved interrupt
        INT8U uch_pvMethodRunFlag;

        INT8U uch_pvRecvMthConfigSucc;

};

extern CMethod cl_gMethod;
#ifdef __cplusplus
}
#endif
#endif









