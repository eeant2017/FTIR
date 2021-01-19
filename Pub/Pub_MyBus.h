//==================================================================================================
//| 文件名称 | Pub_MyBus.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Pub_MyBus.cpp所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2010
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2007.10.20  | gao       | 初版
//==================================================================================================

#ifndef  PUB_MYBUS_H
#define  PUB_MYBUS_H

#include "typedefine.h"
//临时存放ygm
//=========================== Macro,Typestruct, & Class ===================//
//#define MAIN_ASPECTS_EN  1   //1：大端模式, 0:小端模式
#define MAIN_ASPECTS_EN  0   //1：大端模式, 0:小端模式


#if MAIN_ASPECTS_EN > 0   //大端模式
    typedef struct st_TYPE_INT16U
    {
       INT8U  byte0;
       INT8U  byte1;
    }TYPE_INT16U;

    typedef struct st_TYPE_INT16S
    {
       INT8U  byte0;
       INT8U  byte1;
    }TYPE_INT16S;

    typedef struct st_TYPE_INT32U
    {
       INT8U  byte0;
       INT8U  byte1;
       INT8U  byte2;
       INT8U  byte3;
    }TYPE_INT32U;

    typedef struct st_TYPE_INT32S
    {
       INT8U  byte0;
       INT8U  byte1;
       INT8U  byte2;
       INT8U  byte3;
    }TYPE_INT32S;
#else                       //小端模式
    typedef struct st_TYPE_INT16U
    {
       INT16U byte1:8;
       INT16U byte0:8;
    }TYPE_INT16U;

    typedef struct st_TYPE_INT16S
    {
       INT16S byte1:8;
       INT16S byte0:8;
    }TYPE_INT16S;

    typedef struct st_TYPE_INT32U
    {
       INT32U byte3:8;
       INT32U byte2:8;
       INT32U byte1:8;
       INT32U byte0:8;
    }TYPE_INT32U;

    typedef struct st_TYPE_INT32S
    {
       INT32S byte3:8;
       INT32S byte2:8;
       INT32S byte1:8;
       INT32S byte0:8;
    }TYPE_INT32S;
#endif

typedef union u_TYPECHANGE
{
    FP32       f_FloatData;
    INT32S        lg_LongData;
    INT32U        ul_UlongData;
    TYPE_INT32U   bit;
}TYPECHANGE;


typedef union u_TYPECHANGE2
{
    INT16S        in_IntData;
    INT16U        uin_UintData;
    TYPE_INT16U   bit;
}TYPECHANGE2;

typedef union u_TYPE_INT32U
{
    INT32U  ul_UlongData;
    INT8U  byte0;
    INT8U  byte1;
    INT8U  byte2;
    INT8U  byte3;
}U_TYPE_INT32U;

//========================= Variable,Object & Fuction =====================//
INT16U  GetCrc16Bit(INT8U *p_uch_Data, INT16U uin_CrcDataLen);
void    EncodeFloat(FP32 f1, INT8U *p);
FP32    DecodeFloat(INT8U *p);
void    EncodeUlong(INT32U ul_Data, INT8U *p);
INT32U  DecodeUlong(INT8U *p);
void    EncodeLong(INT32S lg_Data, INT8U *p);
INT32S  DecodeLong(INT8U *p);
void    EncodeUint(INT16U uin_Data, INT8U *p);
INT16U  DecodeUint(INT8U *p);
void    EncodeInt(INT16S in_Data, INT8U *p);
INT16S  DecodeInt(INT8U *p);

void    EnSplitNByte(INT32U ul_Data,INT8U *p_uch_Data,INT8U uch_Bytes);
INT32U  DeSplitNByte(INT8U *p_uch_Data,INT8U uch_Bytes);
void    CopyDataFromAToB(INT8U *pAbuf,INT8U *pBbuf,INT16U uin_Len);
FP32 DecodeStrToFloat(INT8U *ar_buf,INT8U uch_len,INT8U uch_digit);
FP32    DecodeINT16UToFloat(INT16U ch_Data);
void    EncodeFloatToINT16U(FP32 f_Data,INT8U *p);

//常用宏定义
#ifndef  m_MIN
#define m_MIN(a, b) (a<b ? a:b)
#endif
#ifndef  m_MAX
#define m_MAX(a, b) (a<b ? b:a)
#endif

#endif
