
//==================================================================================================
//| 文件名称 | Mod_FPGAUpdate.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 通信模块模块
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 | 所有C/C++语言编译环境，包括单片机编译器
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2017, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    |2017.12.20   |  lyb       | 初版
//==================================================================================================

#ifndef __MOD_FPGAUPDATE_H__
#define __MOD_FPGAUPDATE_H__

#include "typedefine.h"
#include "Pub/Pub_MyBus.h"
#include "Bsp/Bsp_Gpioctl.h"
//----------------------------------------Cplusplus-----------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif


//-----------------------驱动设置 必看----------------------


#define FPGAFLASH_CLR_CS()  (gpio_write(FPGA_NCSO,0))
#define FPGAFLASH_SET_CS()  (gpio_write(FPGA_NCSO,1))

#define FPGAFLASH_CLR_SDO() (gpio_write(FPGA_ASDO,0))
#define FPGAFLASH_SET_SDO() (gpio_write(FPGA_ASDO,1))

#define FPGAFLASH_CLR_CLK() (gpio_write(FPGA_DCLK,0))
#define FPGAFLASH_SET_CLK() (gpio_write(FPGA_DCLK,1))

#define FPGAFLASH_SDI_DAT() (gpio_read(FPGA_DATA0))


//-----------------------升级相关定义-----------------------
#define FPGA_UPDATE_SIZE (4*256)
#define FLASH_WAIT_TIMEOUT  40000 //等待FLASH 忙标志位清零

#define EPCS1_ID    0x10
#define EPCS4_ID    0x12
#define EPCS16_ID   0x14
#define EPCS64_ID   0x16
#define EPCS128_ID  0x18

//flash opration 命令
#define WRITE_ENABLE                0x06
#define WRITE_DISABLE               0x04
#define READ_STATUS                 0x05
#define WRITE_STATUS                0x01
#define READ_BYTES                  0x03
#define FAST_READ_BYTES             0x0B
#define PAGE_PROGRAM                0x02
#define ERASE_SECTOR                0xD8
#define ERASE_BULK                  0xC7
#define READ_SILICON_ID             0xAB
#define CHECK_SILICON_ID            0x9F

#define FPGAUPSTATUS_IDLE           0
#define FPGAUPSTATUS_UPSTART        1
#define FPGAUPSTATUS_ONTRANS        2
#define FPGAUPSTATUS_WAITDONE       3
#define FPGAUPSTATUS_UPDONE         4



//----支持聚光协议回调函数调用-------------
INT8U Com71ReadUpdate(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth);
INT8U Com71WriteUpdate(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth);
INT8U Com72WriteUpdate(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth);
INT8U Com73ReadUpdate(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth);
INT8U Com74ReadUpdate(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth);

//----不支持聚光协议回调函数调用-----------

INT8U SetFPGAUpdateStart(INT8U *p);
INT8U EraseDone(void);
INT8U LoadUpdateFlowToFlash(INT8U *puch_Data,INT32U len,INT16U *puin_CurPackBack);
INT8U LookFPGAUpdateStatus(void);
INT8U ReadFlashData(INT8U *puch_Data,INT16U PrePackNum);


//-----必须调用---------
void FPGAUpdateInit(void);

//-----调试使用---------
INT8U FlashSelfTest(void);

#ifdef __cplusplus
}
#endif

#endif
