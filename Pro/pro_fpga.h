#ifndef PRO_FPGA_H
#define PRO_FPGA_H


//==================================================================================================
//| 文件名称 | Pro_FPGA.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Pro_FPGA.cpp的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2011, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.1    | 2016.8.5 |lz      | 规范化
//==================================================================================================

#include "typedefine.h"
#include "Mod/mod_mnt.h"

//#define METHOD_NUMBER_FLAG    0	//方法下载标记TODO
//#define METHOD_NUMBER         1//当前方法编号TODO


//----------对FPGA寄存器操作的作用划分---------------------------
#define DSRAM_RECT      1         //保存扫描片段的时候用
#define DSRAM_METHOD    2         //保存方法配置信息的时候用
#define FPGA_REG        3         //访问FPGA REG
#define DSRAM_DATA      4
#define DSRAM_TEST      5         //test use
#define	DSRAM_ADDR_TEST 6

//-----------与arm交接的总线地址划分------------

#define ARM_CS2  0x00000
#define ARM_CS3  0x1000
#define ARM_CS4  0x2000
#define ARM_CS5  0x3000
#define ARM_CS6  0x4000


#define TEST_ADDR_LOW 0
#define TEST_ADDR_HIGH 1023
#define TEST_ADDR_VERSION 1


//--------------------------io操作开关定义-----------------------
//type of switch
#define RED_LAMP     0
#define GREEN_LAMP   1
//type of switch
#define SWITCH_ON     1
#define SWITCH_OFF    2
#define SWITCH_TURN   3
//operating of switch...
#define FPGA_RUN			0
#define ARM_RUN				1
#define ARM_ERROR			2
#define ARM_ALARM			3

#define MS1_DC_SW			10
#define MS2_DC_SW			11
#define MS1_RF_SW			12
#define MS2_RF_SW			13
#define CORO_SW				14
#define DORA_SW				15
#define NEGION_SW			16
#define EM_SW				17
#define EM_PROTECT_SW		18
#define PRE_PUMP_SW			19
#define PUMP_SW				20
#define CAPI_SW				21
#define BIT_SW				30//wxb
#define LBVacuumG_SW		31//wxb


//----------------------------FPGA REG 相关设置地址和内容---------------------------
#define STOP_MS_FLAG			1//停止MS
#define START_MS_FLAG			2//启动MS
#define ARM_READ_DATA_SUCC		3//ARM读数据成功
#define DOWNLOAD_METHOD_FLAG    4//下载方法

#define RUN_SIG                 11
#define METHOD_RUN_FLAG_addr    3//读方法运行标志

#define DETECT_DATA_RDY         14
#define METHOD_LOAD_RDY         15
#define METHOD_RUN_FLAG			19
//#define READ_DATA_FLAG_addr		4//读有效数据标志
//#define READ_DATA_FLAG			20

#define LC_MS_SYN_FLAG_ADDR		16//LC-MS同步标志
#define LC_MS_SYN_FLAG			24
#define LC_MS_SYN_CLEAR_FLAG	23
#define SW_VACUMM_OK			25//OK
#define SW_VACUMM_NO			26
#define ONE_KEY_ON				27
#define ONE_KEY_OFF				28
#define GATE_ON_FLAG			30
#define GATE_ON_FLAG_ADDR		18

#define PROTECT_SYS_ON			31
#define PROTECT_SYS_OFF			32

#define CURRENT_DATA_LEN_addr	6//2,3当前待传数据长度		//当前待传方法数据长度
#define CURRENT_SDRAM_addr		8//10,11,12,13当前配置地址	//当前待传方法数据头地


#define DA_LTC1595_1_addr		40//MS1 RF输出
#define DA_LTC1595_1_syn		50
#define DA_LTC1595_2_addr		42//MS2 RF输出
#define DA_LTC1595_2_syn		51

#define DA_AD5546_1_addr		44//萃取锥电压---->CONE 2016.4.18
#define DA_AD5546_1_syn		    52
#define DA_AD5546_2_addr		46//CID碰撞电压【碰撞能量】
#define DA_AD5546_2_syn		    53

#define DA_DAC121_1_addr		48//RF Lens传输杆电压
#define DA_DAC121_1_syn		    54
#define DA_DAC121_2_addr		50//EM信号调理
#define DA_DAC121_2_syn		    55

#define DA_DAC124_11_addr		52//(透镜)CID入口电压：高位0x1xxx
#define DA_DAC124_11_syn		56
#define DA_DAC124_15_addr		52//(透镜)CID出口电压：高位0x5xxx
#define DA_DAC124_15_syn		56
#define DA_DAC124_19_addr		52//DA_RFLENS(CHC)：高位0x9xxx
#define DA_DAC124_19_syn		56
#define DA_DAC124_1d_addr		52//(样品锥)采样锥电压：高位0xdxxx  cone->extractor
#define DA_DAC124_1d_syn		56

#define DA_DAC124_21_addr		54//倍增器入口电压：高位0x1xxx
#define DA_DAC124_21_syn		60
#define DA_DAC124_25_addr		54//毛细管电压：高位0x5xxx
#define DA_DAC124_25_syn		60
#define DA_DAC124_29_addr		54//电晕针电流：高位0x9xxx
#define DA_DAC124_29_syn		60
#define DA_DAC124_2d_addr		54//倍增器电压：高位0xdxxx
#define DA_DAC124_2d_syn		60

#define DA_LTC8043_1_addr		56//MS1离子能量(主杆DC电压)
#define DA_LTC8043_1_syn		64
#define DA_LTC8043_2_addr		58//MS2离子能量(主杆DC电压)
#define DA_LTC8043_2_syn		65
#define DA_LTC8043_3_addr		60//MS2预杆电压
#define DA_LTC8043_3_syn		66
#define DA_LTC8043_4_addr		62//倍增器偏转电压
#define DA_LTC8043_4_syn		67
#define DA_LTC8043_5_addr		64//HV1
#define DA_LTC8043_5_syn		68
#define DA_LTC8043_6_addr		66//HV2
#define DA_LTC8043_6_syn		69
#define DA_LTC8043_7_addr		68//MS1HM高质量数分辨率调节
#define DA_LTC8043_7_syn		70
#define DA_LTC8043_8_addr		70//MS1LM低质量数分辨率调节
#define DA_LTC8043_8_syn		71
#define DA_LTC8043_9_addr		72//MS2HM高质量数分辨率调节
#define DA_LTC8043_9_syn		72
#define DA_LTC8043_10_addr		74//MS2LM低质量数分辨率调节
#define DA_LTC8043_10_syn		73

#define DA_DAC124_31_addr		76//DC2
#define DA_DAC124_31_syn		74
#define DA_DAC124_35_addr		76//DC3
#define DA_DAC124_35_syn		74
#define DA_DAC124_39_addr		76//DC4
#define DA_DAC124_39_syn		74
#define DA_DAC124_3d_addr		76//MS1预杆
#define DA_DAC124_3d_syn		74

#define DA_DAC124_41_addr		200//dc_test
#define DA_DAC124_41_syn		78
#define DA_DAC124_45_addr		200 //q0out2
#define DA_DAC124_45_syn		78
#define DA_DAC124_49_addr		200 //da_cp(挡板电压)
#define DA_DAC124_49_syn		78
#define DA_DAC124_4d_addr		200 //预留
#define DA_DAC124_4d_syn		78

#define DA_DAC124_5_addr		202
#define DA_DAC124_5_syn         79


#define DA_LTC8043_11_addr		78//DC5
#define DA_LTC8043_11_syn		75
#define DA_LTC8043_12_addr		80//DC6
#define DA_LTC8043_12_syn		76
#define ION_SWITCH_MS_addr		82//正负离子切换时间
#define ION_SWITCH_MS_syn		77
#define ION_CONNECTED_addr		84//离子能量正负是否与极性关联
#define ION_CONNECTED_syn		78


#define AD_AD7689_CS1_1_addr	100//采样锥电压
#define AD_AD7689_CS1_1_syn		100
#define AD_AD7689_CS1_2_addr	102//萃取锥电压
#define AD_AD7689_CS1_2_syn		101
#define AD_AD7689_CS1_3_addr	104//传输杆电压
#define AD_AD7689_CS1_3_syn		102
#define AD_AD7689_CS1_4_addr	106//透镜入口电压
#define AD_AD7689_CS1_4_syn		103
#define AD_AD7689_CS1_5_addr	108//倍增器电压
#define AD_AD7689_CS1_5_syn		104
#define AD_AD7689_CS1_6_addr	110//打拿极电压
#define AD_AD7689_CS1_6_syn		105
#define AD_AD7689_CS1_7_addr	112//检测器电压
#define AD_AD7689_CS1_7_syn		106
#define AD_AD7689_CS1_8_addr	114//毛细管电压/电晕针电流
#define AD_AD7689_CS1_8_syn		107

#define AD_AD7689_CS2_1_addr	116//碰撞池电压
#define AD_AD7689_CS2_1_syn		108
#define AD_AD7689_CS2_2_addr	118//透镜出口电压
#define AD_AD7689_CS2_2_syn		109
#define AD_AD7689_CS2_3_addr	120//RF1扫描电压
#define AD_AD7689_CS2_3_syn		110
#define AD_AD7689_CS2_4_addr	122//RF2扫描电压
#define AD_AD7689_CS2_4_syn		111
#define AD_AD7689_CS2_5_addr	124//RF1 主杆DC电压1
#define AD_AD7689_CS2_5_syn		112
#define AD_AD7689_CS2_6_addr	126//RF1 主杆DC电压2
#define AD_AD7689_CS2_6_syn		113
#define AD_AD7689_CS2_7_addr	128//RF2主杆DC电压1
#define AD_AD7689_CS2_7_syn		114
#define AD_AD7689_CS2_8_addr	130//RF2主杆DC电压2
#define AD_AD7689_CS2_8_syn		115

#define AD_AD7689_CS3_1_addr	132//电源+5VD
#define AD_AD7689_CS3_1_syn		116
#define AD_AD7689_CS3_2_addr	134//电源+1.2VD
#define AD_AD7689_CS3_2_syn		117
#define AD_AD7689_CS3_3_addr	136//电源+3.3VD
#define AD_AD7689_CS3_3_syn		118
#define AD_AD7689_CS3_4_addr	138//电源-5VA
#define AD_AD7689_CS3_4_syn		119
#define AD_AD7689_CS3_5_addr	140//电源+5VA
#define AD_AD7689_CS3_5_syn		120
#define AD_AD7689_CS3_6_addr	142//电源-14VA
#define AD_AD7689_CS3_6_syn		121
#define AD_AD7689_CS3_7_addr	144//电源+14VA
#define AD_AD7689_CS3_7_syn		122
#define AD_AD7689_CS3_8_addr	146//电源3.3VA
#define AD_AD7689_CS3_8_syn		123

#define AD_AD7689_CS4_1_addr	148//参考电压2.5V
#define AD_AD7689_CS4_1_syn		124
#define AD_AD7689_CS4_2_addr	150//保留
#define AD_AD7689_CS4_2_syn		125
#define AD_AD7689_CS4_3_addr	152//参考电压1.25V
#define AD_AD7689_CS4_3_syn		126
#define AD_AD7689_CS4_4_addr	154//RF电源电压
#define AD_AD7689_CS4_4_syn		127
#define AD_AD7689_CS4_5_addr	156//主板温度
#define AD_AD7689_CS4_5_syn		128
#define AD_AD7689_CS4_6_addr	158//信号板温度
#define AD_AD7689_CS4_6_syn		129
#define AD_AD7689_CS4_7_addr	160//ESI源电压
#define AD_AD7689_CS4_7_syn		130
#define AD_AD7689_CS4_8_addr	162//参考电压2.5V2
#define AD_AD7689_CS4_8_syn		131

#define AD_AD7689_CS5_1_addr	164//MS2预杆电压
#define AD_AD7689_CS5_1_syn		132
#define AD_AD7689_CS5_2_addr	166//检测器高压
#define AD_AD7689_CS5_2_syn		133
#define AD_AD7689_CS5_3_addr	168//高压1HV1
#define AD_AD7689_CS5_3_syn		134
#define AD_AD7689_CS5_4_addr	170//高压2HV2
#define AD_AD7689_CS5_4_syn		135
#define AD_AD7689_CS5_5_addr	172//真空规电压
#define AD_AD7689_CS5_5_syn		136
#define AD_AD7689_CS5_6_addr	174//驱动板温度1
#define AD_AD7689_CS5_6_syn		137
#define AD_AD7689_CS5_7_addr	176//驱动板温度2
#define AD_AD7689_CS5_7_syn		138
#define AD_AD7689_CS5_8_addr	178//保留
#define AD_AD7689_CS5_8_syn		139

#define AD_AD7689_CS6_1_addr	180
#define AD_AD7689_CS6_2_addr	182
#define AD_AD7689_CS6_3_addr	184
#define AD_AD7689_CS6_4_addr	186
#define AD_AD7689_CS6_5_addr	188
#define AD_AD7689_CS6_6_addr	190
#define AD_AD7689_CS6_7_addr	192
#define AD_AD7689_CS6_8_addr	194

#define AD_AD7689_AVE_addr		195 //设置ad7689 平均次数的地址 lyb add

#define SW_MS1_DC_ON_syn		200//开
#define SW_MS1_DC_OFF_syn		201//关
#define SW_MS2_DC_ON_syn		202//开
#define SW_MS2_DC_OFF_syn		203//关
#define SW_MS1_RF_ON_syn		204//开
#define SW_MS1_RF_OFF_syn		205//关
#define SW_MS2_RF_ON_syn		206//开
#define SW_MS2_RF_OFF_syn		207//关

#define SW_V430_ON_syn			208//开
#define SW_V430_OFF_syn			209//关
#define SW_V1350_ON_syn			210//开
#define SW_V1350_OFF_syn		211//关

#define SW_Capillary_ON_syn		212//开
#define SW_Capillary_OFF_syn	213//关
#define SW_ECorona_ON_syn		214//开
#define SW_ECorona_OFF_syn		215//关
#define SW_Dynode_ON_syn		216//开
#define SW_Dynode_OFF_syn		217//关
#define SW_EMultipiper_ON_syn	218//开
#define SW_EMultipiper_OFF_syn	219//关
#define SW_EMultipiper_Safeguard_ON_syn		220//开
#define SW_EMultipiper_Safeguard_OFF_syn	221//关
#define SW_VoltSignSwitch_ON_syn			222//开
#define SW_VoltSignSwitch_OFF_syn			223//关
#define SW_RF_ESource_ON_syn				224//开
#define SW_RF_ESource_OFF_syn				225//关
#define SW_ExtractorSignSwitch_ON_syn		226//开
#define SW_ExtractorSignSwitch_OFF_syn		227//关
#define SW_CollisionSignSwitch_ON_syn		228//开
#define SW_CollisionSignSwitch_OFF_syn		229//关
#define SW_MS1_DC_SignSwitch_ON_syn			230//开
#define SW_MS1_DC_SignSwitch_OFF_syn		231//关
#define SW_MS2_DC_SignSwitch_ON_syn			232//开
#define SW_MS2_DC_SignSwitch_OFF_syn		233//关
#define SW_Test1_ON_syn						234//开
#define SW_Test1_OFF_syn					235//关
#define SW_Test2_ON_syn						236//开
#define SW_Test2_OFF_syn					237//关
#define SW_LBVacuumG_ON_syn					238//开【来宝真空规】
#define SW_LBVacuumG_OFF_syn				239//关
#define SW_NebulizeGasChamber_ON_syn		240//开
#define SW_NebulizeGasChamber_OFF_syn		241//关


#define SW_BIT_ON_syn					5//开【单板测试】
#define SW_BIT_OFF_syn					6//关【正常模式】
#define SW_FPGA_Running_LED_ON_syn		7//开
#define SW_FPGA_Running_LED_OFF_syn		8//关
#define SW_Data_Exchanging_LED_ON_syn	9//开
#define SW_Data_Exchanging_LED_OFF_syn	10//关
#define SW_Alarm_LED_ON_syn				11//开
#define SW_Alarm_LED_OFF_syn			12//关
#define SW_Pump1_LED_ON_syn				13//开
#define SW_Pump1_LED_OFF_syn			14//关
#define SW_Pump2_LED_ON_syn				15//开
#define SW_Pump2_LED_OFF_syn			16//关
#define SW_Sampling_LED_ON_syn			17//开
#define SW_Sampling_LED_OFF_syn			18//关






//....enable DAC121

#define MAX_VOL_CHANNEL  27
#define MAX_VOL_CHANNEL_CHECK 26

typedef struct
{
        INT16U	ush_Addr;
        INT8U	uch_SyncValue;
        volatile	INT8U	uch_CmdSetFlag;

        FP32 f_k;
        FP32 f_a;
        FP32 f_b;
        FP32 f_c;

        INT8U uch_ProctolFlag;
        INT8U uch_Mask;
        INT8U uch_MethodEnable;
        INT8U uch_ARM_Num;

        INT8U uch_JUNK_Num;
        INT8S asch_Note[40];
}STRSetVoltageList;

typedef struct
{
        INT16U ush_VoltageListIndex;
        INT16U ush_MemMapOffset;

        FP32 f_ReadValue;
        INT32U ul_ReadTime;

        FP32 f_WriteValue;
        INT32U ul_WriteTime;

        FP32 f_SetDeltaValue;
        INT32U ul_SetWaitTime;

        INT8U uch_AlarmCode;

        char *pstr;
}STRCheckVoltageList;


typedef struct
{
        INT8U uch_index;
        const char ash_max[20];
        const char ash_min[20];
        const char ash_default[20];
        const char ash_setvk[20];
        const char ash_setvb[20];
        const char ash_refvk[20];
        const char ash_refvb[20];
        const char ash_setvpk[20];
        const char ash_setvpb[20];
        const char ash_refvpk[20];
        const char ash_refvpb[20];
}STRXMLPARA;

void WriteDA(INT16U uin_Addr, FP32 f_Data, INT8U uch_SynValue,INT8U uch_iChannel = 0, INT16U uin_SynAddr = 12);
FP32 ReadAD(INT16U uin_Addr, FP32* f_Data, INT8U uch_SynValue, INT16U uin_SynAddr = 12);
void ClearRes(void);

INT8U MemoryRead(INT8U uch_iType,const INT16U uin_Addr,INT16U *uin_pData,INT16U uin_DataLen = 1);
INT8U MemoryWrite(INT8U uch_iType,const INT16U uin_Addr,const void *uin_pData,INT16U uin_DataLen = 1);
void OperateRunFlag(INT8U uch_iState,INT16U uin_iAddr = 12);
void OperateSwitch(INT8U uch_SwitchType,INT8U uch_OperType);
void ReadFPGAVersion(INT8U *puch_Data);
INT32U FPGABusTest();
void FPGABusPerformanceTest(void);
extern void MethodVolSet(INT8U *puch_Method ,INT32U uin_RamAddr);
extern STRCheckVoltageList *GetCheckVoltageListPtr(void);
extern STRSetVoltageList *FindSetVolListPtr(INT8U uch_ProctolFlag);
extern INT16U FindSetVolListIndex(INT8U uch_ProctolFlag);

extern void UpdateVoltageCheckListRead(STRMnt *pstrMnt,	INT32U ul_Tick);
extern void UpdateVoltageCheckListWrite(INT16U ush_Index, FP32 f_Write,	INT32U ul_Tick);
extern INT32U CheckSettedVoltageIsOK(void);
extern INT32U CheckVoltage(void);
extern INT8U SetParameterByVoltageList(INT8U *puch_iData);
extern void ResetAllVol(void);

extern INT8U SetParamtoSetVKB(INT8U *puch_iData);
extern STRXMLPARA g_strxmlparalist[65];



#endif // PRO_FPGA_H
