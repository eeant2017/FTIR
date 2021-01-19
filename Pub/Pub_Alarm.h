//==================================================================================================
//| 文件名称 |
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 |
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2009, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2011.02.24  |  gao      | 初版
//==================================================================================================

#ifndef PUB_ALARM_H
#define PUB_ALARM_H

#include "typedefine.h"

//=========================== Macro,Typestruct, & Class ===================//

#define TOTAL_ALARM_NUM		10
#define GATE_ON_WARN		0x91
#define VACUUM_ERROR		0x01
#define PC2ARM_COMM_ERR		0x0B
#define ARM2GUAGE_COMM_ERR	0x0D

extern INT8U  auch_AlarmCode[TOTAL_ALARM_NUM];

extern void PushAlarmCode(INT8U uch_alarmcode);
extern void DelAlarmCode(INT8U uch_alarmcode);
extern void AlarmCodePCGetCnt();
extern void ClearPopAlarm();
#endif


