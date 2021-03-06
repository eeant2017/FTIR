//==================================================================================================
//| 文件名称 | Pro_led.h
//|-----------------------------------------------------------------------------------------------
//| 文件描述 | Pro_led.cpp的头文件
//|-----------------------------------------------------------------------------------------------
//| 版权声明 | Copyright2011, 聚光科技(FPI)
//|-----------------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|---------------------------------------------------------------------------------------------
//|  V1.1   | 2020.08.16    |  chensq  | 规范化
//==================================================================================================

#ifndef PRO_LED_H
#define PRO_LED_H

#include "typedefine.h"
#include <QThread>

void Err_LEDON(void);
void Err_LEDOFF(void);
void RUN_LEDON(void);
void RUN_LEDOFF(void);
void ALARM_LEDON(void);
void ALARM_LEDOFF(void);

void ALARM_LedToggle(void);
void ERR_LedToggle(void);
void RUN_LedToggle(void);
void CORELED_LedToggle(void);

void LedQuickSpark(void);



#endif
