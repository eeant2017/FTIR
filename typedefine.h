#ifndef TYPESDEFINE_H
#define TYPESDEFINE_H

#include "types.h"
#include <string.h>



#define     TOTALALARMNUM  25
#define     MAXTASKNUM 20
#define     PREPOINTS    1024
#define     VALIDPOINTS    2048
#define     TOTAL_ALARM_NUM   10  //报警码总个数

//通信地址
#define     ADDR_PC             0xf3
#define     ADDR_MAIN           0xf5

#define     ADDR_CIRCU          0xf4
#define     ADDR_EI             0xf6
#define     ADDR_Q0DRIVER       0xf7

#define     ADDR_VACU           253
#define     ADDR_NULL           0x00

//board property 单板属性
typedef struct
{
  INT8U uch_IncAddr;                  //当前板子通讯地址
  INT8U auch_MainVersion[33];         //主板版本号
}STBOARDPROPERTY;

//仪器状态
typedef struct
{
  INT8U pch_ID[12];                   //仪器序列号
  INT8U	auch_AlarmCode[TOTAL_ALARM_NUM];//报警码
}SDEVICESTATE;


typedef struct {
    INT8U		   uch_Type;
    INT16U		   uin_SparkNum;
    INT16U   	   uin_AveNum;
    INT16U  	   uin_Freq;
    INT8U		   uch_Valve;
    INT16U         uin_DelayTime;
    INT16U         uin_PreTime;
    INT16U         uin_AlyTime;
}TASKINF;

typedef struct {
INT16U		 	uin_ModelSN;
INT16U		 	uin_Len;
INT8U   	 	uch_TaskTN;
TASKINF  		st_TaskInf[MAXTASKNUM];
}SPARKMODEL;

//函数类型(通信结构中使用)类似钩子函数
typedef INT8U (* UCH_PFUN_UCH)(INT8U);

typedef INT8U (* UCH_PFUN_PUCH)(INT8U *);


typedef FP32 (* FP32_PFUN_VOID)(void);

typedef void(* VOID_PFUN_VOID)(void);

#endif
