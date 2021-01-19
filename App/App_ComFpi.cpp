//==================================================================================================
//| 文件名称 | App_ComFpi.cpp
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 通讯处理模块
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 | 所有C/C++语言编译环境，包括单片机编译器
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2017, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2017.06.25  |  zbb      | 初版
//==================================================================================================
#include "App_ComFpi.h"
#include "Bsp/Bsp_Can0.h"
#include "Pub/Pub_Qthread.h"
#include "Mod/mod_ioctrl.h"
#include "Mod/mod_mnt.h"
#include "Mod/Mod_Method.h"
#include "Mod/mod_detectdata.h"
#include "Pub/Pub_Alarm.h"
#include "Pro/pro_fpga.h"
#include <QDebug>
#include "Include.h"
#include "App/app_app.h"
#include "Pub/pub_MKSVacuometer_V1_0.h"
#include "sqlite.h"
#include "Mod/mod_fpgaupdate.h"
#include "Mod/mod_EI.h"
#include "Mod/mod_Q0Driver.h"

//#include "data1.h"
#include "data2_1.h"

#define ALLDATA_SIZE                        (32768)                 //  all data point to PC
#define ALLBAGS_TOPC                        (128)                   // set all data bags to PC
#define EVERYBAGS_SIZE                      (1024)                  //(ALLDATA_SIZE/ALLBAGS_TOPC*4)
#define EVERYBAGS_LOOP                      (256)                   //(ALLDATA_SIZE/ALLBAGS_TOPC)

//#define ALLDATA_SIZE                        (32768)                 //  all data point to PC
//#define ALLBAGS_TOPC                        (128)                   // set all data bags to PC
//#define EVERYBAGS_SIZE                      (1024)                  //(ALLDATA_SIZE/ALLBAGS_TOPC*4)
//#define EVERYBAGS_LOOP                      (256)                   //(ALLDATA_SIZE/ALLBAGS_TOPC)


#define ADC_CH3                             0x13


CAppComFpi a_ComFpi;

//CComFpi gm_Uart1Fpi;
CComFpi gm_Can0Fpi;
CComFpi gm_TcpFpi1;
CComFpi gm_TcpFpi2;
CComFpi gm_TcpFpi3;

extern void run_datatest(INT32U testdata[],INT32S data_change[]);
extern void vMemDump(INT8U *pData, INT16U DataLen);
extern uint32_t *p_srcdata;

//--存放光谱数据
INT16U updata_dataL[32768*2] = {0};
INT16U updata_dataR[32768*2] = {0};


static STRTRANPORT au_ComPortTab[4]=
{
   ADDR_PC,         &gm_TcpFpi1,
   ADDR_CIRCU,      &gm_Can0Fpi,
   ADDR_EI,         &gm_Can0Fpi,
   ADDR_Q0DRIVER,   &gm_Can0Fpi,

};

//-------------- PC FPI通信协议命令静态函数申明区 ------------//
//被动通信声明区

static INT8U Comm20Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm20Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm21Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm22Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm22Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm23Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm24Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm25Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm26Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm26Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm27Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm30Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm40Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm41Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm43Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm47Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm50Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm50Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm52Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm52Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm53Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm53Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm60Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm60Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm71Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //
static INT8U Comm71Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth); //

//--配置 DA3 DA4
static INT8U Comm7aRead(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm7aWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);

static INT8U Comm7bWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);


static INT8U Comm81Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm91Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);


const STRFPIDATA au_PCFpi[]=
{
//Cmd   AddComm   ReceDataLen      DataNum      VariableAddress     DataType        Min     Max     DefineVal   SaveAddr
//----------------------被动命令区 --------------------//
0x20,   READ,        0,           0,          Comm20Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x20,   WRITE,       0,           0,          Comm20Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x21,   READ,        0,           0,          Comm21Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x22,   READ,        0,           0,          Comm22Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x22,   WRITE,       0,           0,          Comm22Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x23,   WRITE,       0,           0,          Comm23Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x24,   WRITE,       0,           0,          Comm24Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x25,   WRITE,       0,           0,          Comm25Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x26,   READ,        0,           0,          Comm26Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x26,   WRITE,       0,           0,          Comm26Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x27,   WRITE,       0,           0,          Comm27Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x30,   READ,        0,           0,          Comm30Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,

//0x33,   WRITE,       0,           0,          Comm33Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,

0x40,   WRITE,       0,           0,          Comm40Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x41,   WRITE,       N_NUM,       0,          Comm41Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x43,   WRITE,       0,           0,          Comm43Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x47,   READ,        0,           0,          Comm47Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x50,   READ,        0,           0,          Comm50Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x50,   WRITE,       0,           0,          Comm50Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x52,   READ,        0,           0,          Comm52Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x52,   WRITE,       0,           0,          Comm52Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x53,   READ,        0,           0,          Comm53Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x53,   WRITE,       0,           0,          Comm53Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x60,   READ,        0,           0,          Comm60Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x60,   WRITE,       0,           0,          Comm60Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x71,   READ,        0,           0,          Comm71Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x71,   WRITE,       0,           0,          Comm71Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x71,   READUPDATA,  0,           1,          Com71ReadUpdate,    CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x71,   WRITEUPDATA, 8,           1,          Com71WriteUpdate,   CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x72,   WRITEUPDATA, 1028,        3,          Com72WriteUpdate,   CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x73,   READUPDATA,  0,           1,          Com73ReadUpdate,    CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x74,   READUPDATA,  6,           1027,       Com74ReadUpdate,    CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,

//------------- add by chensq ---------------------------
//--配置 DA3 - DA4
0x7a,   READ,       0,           0,           Comm7aRead,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x7a,   WRITE,       0,           0,          Comm7aWrite,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x7b,   WRITE,       0,           0,          Comm7bWrite,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,


//--for test 模拟 光谱 ----
0x81,   READ,        0,           0,          Comm81Read,         CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
//0x81,   WRITE,       0,           0,          Comm81Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,
0x91,   WRITE,       0,           0,          Comm91Write,        CTYPE_FUNC,     NO_USE, NO_USE, NO_USE,     NO_SAVE,


};

//主动通信区--------------------------------
//环境板

static INT8U Comm31ReadAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm23CircuWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm23CircuWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm27CircuWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm27CircuWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
const STRFPIDATA au_CircuFpi[]=
{

    0x31,READ,          0,0,NO_USE          ,CTYPE_INT8U,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x31,READ_ANSWER,   0,0,Comm31ReadAck   ,CTYPE_FUNC, NO_USE, NO_USE, NO_USE, NO_SAVE,

    0x23,WRITE,         0,0,Comm23CircuWrite     ,CTYPE_FUNC ,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x23,WRITE_ANSWER,  0,0,Comm23CircuWriteAck  ,CTYPE_FUNC, NO_USE, NO_USE, NO_USE, NO_SAVE,

    0x27,WRITE,         0,0,Comm27CircuWrite     ,CTYPE_FUNC ,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x27,WRITE_ANSWER,  0,0,Comm27CircuWriteAck  ,CTYPE_FUNC, NO_USE, NO_USE, NO_USE, NO_SAVE,
};

//EI板
static INT8U Comm23EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm23EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm24EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm24EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm25EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm25EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm27EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm27EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm31EIReadAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
//static INT8U Comm52EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
//static INT8U Comm52EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
//static INT8U Comm53EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
//static INT8U Comm53EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
//static INT8U Comm71EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
//static INT8U Comm71EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
const STRFPIDATA au_EIFpi[]=
{
    //设置开关量
    0x23,WRITE,          0,0,Comm23EIWrite      ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x23,WRITE_ANSWER,   0,0,Comm23EIWriteAck   ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    //设置电压
    0x24,WRITE,          0,0,Comm24EIWrite      ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x24,WRITE_ANSWER,   0,0,Comm24EIWriteAck   ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    //一次性设置电压
    0x25,WRITE,          0,0,Comm25EIWrite      ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x25,WRITE_ANSWER,   0,0,Comm25EIWriteAck   ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    //设置温度
    0x27,WRITE,          0,0,Comm27EIWrite      ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x27,WRITE_ANSWER,   0,0,Comm27EIWriteAck   ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    //监控
    0x31,READ,           0,0,NO_USE              ,CTYPE_INT8U,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x31,READ_ANSWER,    0,0,Comm31EIReadAck     ,CTYPE_FUNC, NO_USE, NO_USE, NO_USE, NO_SAVE,
//    //设置控温模式
//    0x52,WRITE,          0,0,Comm52EIWrite       ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
//    0x52,WRITE_ANSWER,   0,0,Comm52EIWriteAck    ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
//    //设置控温参数
//    0x53,WRITE,          0,0,Comm53EIWrite       ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
//    0x53,WRITE_ANSWER,   0,0,Comm53EIWriteAck    ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
//    //设置温度校准
//    0x71,WRITE,          0,0,Comm71EIWrite       ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
//    0x71,WRITE_ANSWER,   0,0,Comm71EIWriteAck    ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
};
//Q0DRIVER板
static INT8U Comm23Q0Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm23Q0WriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm24Q0Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm24Q0WriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
static INT8U Comm31Q0ReadAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
const STRFPIDATA au_Q0DriverFpi[]=
{

    //设置开关量
    0x23,WRITE,          0,0,Comm23Q0Write      ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x23,WRITE_ANSWER,   0,0,Comm23Q0WriteAck   ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    //设置电压
    0x24,WRITE,          0,0,Comm24Q0Write      ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x24,WRITE_ANSWER,   0,0,Comm24Q0WriteAck   ,CTYPE_FUNC,NO_USE, NO_USE, NO_USE, NO_SAVE,

    //数据监控
    0x31,READ,          0,0,NO_USE           ,CTYPE_INT8U ,NO_USE, NO_USE, NO_USE, NO_SAVE,
    0x31,READ_ANSWER,   0,0,Comm31Q0ReadAck  ,CTYPE_FUNC, NO_USE, NO_USE, NO_USE, NO_SAVE,
};


INT8U delay(INT32U delay)
{
    INT32U j = 32;
    while (j-- > 0)
    {
        for (INT32U i = 0; i < delay; i++)
        {
            ;
        }
    }
}

//==================================================================================================
//| 函数名称 | Comm20Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设备ID读取
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb,2018.03.26
//==================================================================================================
INT8U Comm20Read(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth)
{

    char ID[12] ;
    m_SQLite.GetDeviceID(ID);
    qDebug(ID);
    memcpy(SendData,ID,11);

    *uip_SendDataLenth = 11;

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm20Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设备ID设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.16
//==================================================================================================
INT8U Comm20Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    char ID[12];
    ID[11] = '\0';
    memcpy(ID,ReceData,11);
    m_SQLite.SetDeviceID(ID);

    SendData[0] = WRITE_SUCCESS;
    *uip_SendDataLenth = 1;
    return 0;
}

//==================================================================================================
//| 函数名称 | Comm7aRead()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置DA3 DA4 -- 激光 Sic电压
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
INT8U Comm7aRead(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT16U uin_Test[8] = {0};

    qDebug("Comm7aRead.%d",*ReceData);
    if(*ReceData == 16)
    {
        qDebug("ReadFromFPGA.");
        cl_bspFpga.ReadFromFPGA(ADC_CH3,uin_Test,1);
        SendData[0] = INT8U (uin_Test[0] >> 8);
        SendData[1] = INT8U (uin_Test[0]);
        qDebug("ReadFromFPGA.%d",uin_Test[0]);
        *uip_SendDataLenth = 2;
    }
    else
    {
        *uip_SendDataLenth = 1;
        SendData[0] = READ_FAILED;
    }

    return TRUE;
}

//==================================================================================================
//| 函数名称 | Comm7aWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置DA3 DA4 -- 激光 Sic电压
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | 使用sql数据库保存 设置过一次后保存参数会写入保存，上电的时候读取数据库，如果有配置过，及时写入FPGA
//==================================================================================================
INT8U Comm7aWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    vMemDump(ReceData,8);

    INT8U uch_channel = *(ReceData++);            //da3 or da4
    INT16U uin_dax_val = 0;                       //da3 or da4
    INT8U uch_datatemp[8] = {0};

    uch_datatemp[0] = *(ReceData++);
    uch_datatemp[1] = *(ReceData++);
    uin_dax_val     = (uch_datatemp[0] << 8) + uch_datatemp[1];

    if (uch_channel == 3)                        //da3
    {
        qDebug("Comm7aWrite.DA3-1 = %x",uin_dax_val);

        //uin_dax_val = uin_dax_val / 1638;
        qDebug("Comm7aWrite.DA3-2 = %x",uin_dax_val);
        if (uin_dax_val < 0x06b9)
        {
            cl_bspFpga.WriteToFPGA(0x0034,&uin_dax_val,1);
            cl_bspFpga.WriteToFPGA(0x0036,&uin_dax_val,1);  //high
            usleep(1000);
            cl_bspFpga.WriteToFPGA(0x0038,&uin_dax_val,1);  //low
        }
        else
        {
            SendData[0] = WRITE_FAILED;                            
            *uip_SendDataLenth = 1;
            return TRUE;
        }
    }
    else if (uch_channel == 4)
    {
        qDebug("Comm7aWrite.DA4-1 = %x",uin_dax_val);

        uin_dax_val = uin_dax_val / 1638;
        qDebug("Comm7aWrite.DA4-2 = %x",uin_dax_val);
        if (uin_dax_val == 12)
            uin_dax_val = 41;
//        else if (uin_dax_val == 115)
//            uin_dax_val = 41*5;
        else if (uin_dax_val == 11)
            uin_dax_val = 41*10;
        else if (uin_dax_val == 10)
            uin_dax_val = 41*20;
        else if (uin_dax_val == 9)
            uin_dax_val = 41*30;
        else if (uin_dax_val == 8)
            uin_dax_val = 41*39;
        else if (uin_dax_val == 7)
            uin_dax_val = 41*48;
        else
            uin_dax_val = 0;            //others all use 12V
        qDebug("Comm7aWrite.DA4-3 = %x",uin_dax_val);

        cl_bspFpga.WriteToFPGA(0x0035,&uin_dax_val,1);
        cl_bspFpga.WriteToFPGA(0x0037,&uin_dax_val,1);      //high
        usleep(1000);
        cl_bspFpga.WriteToFPGA(0x0039,&uin_dax_val,1);      //low
    }

    SendData[0] = WRITE_SUCCESS;
    *uip_SendDataLenth = 1;

    return TRUE;
}


//==================================================================================================
//| 函数名称 | Comm7bWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置Vc1.0版本波峰的延时时间
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
INT8U Comm7bWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT32U ul_delay =  DecodeUlong(ReceData);
    INT16U auin_temp[2] = {0};


    qDebug("Comm7bWrite,%d,",ul_delay);
    if(ul_delay > 60000 || ul_delay < 1)
    {
        SendData[0] = WRITE_FAILED;
        *uip_SendDataLenth = 1;
        return TRUE;
    }
    else
    {
        ul_delay = (ul_delay - 335) * 94;        //量出来的是总间隔，减去一半671ms，乘上系数（没乘25*10）
        auin_temp[0] = (INT16U)(ul_delay >> 16);
        auin_temp[1] = (INT16U)ul_delay;
        qDebug("Comm7bWrite,%x,%x",auin_temp[0],auin_temp[1]);

        cl_bspFpga.WriteToFPGA(0x003a,&auin_temp[1],1);
        cl_bspFpga.WriteToFPGA(0x003b,&auin_temp[0],1);

    }
    SendData[0] = WRITE_SUCCESS;
    *uip_SendDataLenth = 1;

    return TRUE;
}

//==================================================================================================
//| 函数名称 | Comm91Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置音圈电机参数
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
INT8U Comm91Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT32U uin_motor_data[16] = {0};
    INT16U uin_motor_send[16] = {0};
    INT32U i;

    qDebug("Comm91Write.");

    for (i = 0; i < 22; i++)
    {
        qDebug("ReceData[%d] = %x ", i, ReceData[i]);

    }

    uin_motor_data[0] = DecodeUlong(ReceData+0);    //div1
    uin_motor_data[1] = DecodeUlong(ReceData+4);    //div2
    uin_motor_data[2] = DecodeUlong(ReceData+8);   //div3
    uin_motor_data[3] = DecodeUlong(ReceData+12);   //div4
    uin_motor_data[4] = DecodeUlong(ReceData+16);   //cycle

    if((uin_motor_data[0] > 65535) || (uin_motor_data[1] > 65535)
     ||(uin_motor_data[2] > 65535) || (uin_motor_data[3] > 65535))
    {
        SendData[0] = WRITE_FAILED;
        *uip_SendDataLenth = 1;
        return 0;
    }

    uin_motor_send[0] = uin_motor_data[0];
    uin_motor_send[1] = uin_motor_data[1];
    uin_motor_send[2] = uin_motor_data[2];
    uin_motor_send[3] = uin_motor_data[3];
    uin_motor_send[4] = (INT16U)(uin_motor_data[4] >> 16);
    uin_motor_send[5] = (INT16U)uin_motor_data[4];

    //-- 设置已经写完的标志write_done  允许写配置i- o
//    cl_bspFpga.WriteToFPGA(0x20d,&uin_motor_send[5],1);
//    delay(5000);
    qDebug("delay over.");

    cl_bspFpga.WriteToFPGA(0x001b,&uin_motor_send[0],1);
    cl_bspFpga.WriteToFPGA(0x001c,&uin_motor_send[1],1);
    cl_bspFpga.WriteToFPGA(0x001d,&uin_motor_send[2],1);
    cl_bspFpga.WriteToFPGA(0x001e,&uin_motor_send[3],1);
    cl_bspFpga.WriteToFPGA(0x001f,&uin_motor_send[4],1);
    cl_bspFpga.WriteToFPGA(0x0020,&uin_motor_send[5],1);

//    delay(5000);
    qDebug("delay over.");

    cl_bspFpga.WriteToFPGA(0x20d,&uin_motor_send[5],1);     //--tell TAS512 config para ok

    SendData[0] = WRITE_SUCCESS;
    *uip_SendDataLenth = 1;
    return 0;
}


//==================================================================================================
//| 函数名称 | Comm81Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 上传光谱数据，采用正常总线[地址+数据]的方式
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
INT8U Comm81Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_dataport = 0;
    INT32U  uch_allbyte = 0;
    INT32U uch_curbyte = 0;
    INT32U i,j;
    INT8U a_sendata[4] = {0};
    //uint32_t a_testdata[32768] = {0};
    INT16U uin_Test[16] = {0};

    static INT8U update_flag = 0;
    INT8U *p_str = SendData;
    INT32U num = 0;

    //--qDebug("Comm81Read:");
    //vMemDump(ReceData,16);

    uch_dataport = *ReceData;
    uch_allbyte = *(ReceData+1);
    if (uch_allbyte == 0)
    {
        uch_allbyte = ALLBAGS_TOPC;                 // tell pc all bags 0x80=128
        qDebug("sizeof ca_data1 = %d",uch_allbyte);
    }

    uch_curbyte = *(ReceData+2);

    *p_str++ = uch_dataport;                // channl
    *p_str++ = uch_allbyte;                 // all bag size
    *p_str++ = uch_curbyte;                 // cur bag size
    //--qDebug("uch_dataport:%d,uch_allbyte:%d, uch_curbyte: %d",uch_dataport,uch_allbyte,uch_curbyte);

//----------------------------------------------------------------------------------------------------------
    if (update_flag == 0)
    {
        cl_bspFpga.ReadFromFPGA(0x18,uin_Test,1);                          // get data can read ?
        while (uin_Test[0] == 0)
        {
            cl_bspFpga.ReadFromFPGA(0x18,uin_Test,1);
        }
        qDebug("uch_work_mode = 0; 0x18 can read.");

        memset(updata_dataL,0,sizeof(updata_dataL));
        memset(updata_dataR,0,sizeof(updata_dataR));

        //--test write
//        uin_Test[0] = 0x5555;
//        uin_Test[1] = 0x6666;
//        cl_bspFpga.WriteToFPGA(0x0000,uin_Test,2);


        #if 1  //--bugs @
        //--ram1_dada blcok
        //--cl_bspFpga.ReadFromFPGA((0x1000),updata_dataL,32768);           //start read ram1 data
        uin_Test[0] = 0;
        cl_bspFpga.WriteToFPGA(0x0030,uin_Test,1);                           //通道1

        cl_bspFpga.WriteToFPGA(0x0032,uin_Test,1);
        cl_bspFpga.ReadFromFPGA((0x1000),updata_dataL,32768);               //第一个块 读32768地址--对应16384个数据
        //--再读取一次
        //cl_bspFpga.ReadFromFPGA((0x1000),updata_dataL,32768);               //第一个块 读32768地址--对应16384个数据


        cl_bspFpga.WriteToFPGA(0x0033,uin_Test,1);
        cl_bspFpga.ReadFromFPGA((0x1000),updata_dataL+32768,32768);               //第二个块 读32768地址--对应16384个数据
        //--再读取一次
        //cl_bspFpga.ReadFromFPGA((0x1000),updata_dataL+32768,32768);               //第二个块 读32768地址--对应16384个数据
#if 1
        //--ram2_dada blcok
        cl_bspFpga.WriteToFPGA(0x0031,uin_Test,1);                           //通道2

        cl_bspFpga.WriteToFPGA(0x0032,uin_Test,1);
        //--cl_bspFpga.ReadFromFPGA((0x9000),updata_dataR,32768);           //start read ram2 data
        cl_bspFpga.ReadFromFPGA((0x1000),updata_dataR,32768);               //start read ram2 data
        //--再读取一次
        //cl_bspFpga.ReadFromFPGA((0x1000),updata_dataR,32768);               //start read ram2 data

        cl_bspFpga.WriteToFPGA(0x0033,uin_Test,1);
        cl_bspFpga.ReadFromFPGA((0x1000),updata_dataR+32768,32768);
        //--再读取一次
        //cl_bspFpga.ReadFromFPGA((0x1000),updata_dataR+32768,32768);

#endif
        //cl_bspFpga.ReadFromFPGA((0x9000 +0x8000),updata_dataR+32768,32768);           //start read ram2 data
        #endif

        cl_bspFpga.ReadFromFPGA(0x19,uin_Test,1);                          //  wr ok set falg done
        cl_bspFpga.ReadFromFPGA(0x1a,uin_Test,1);                          //  wr ok set falg done

        update_flag = 1;
                                                       // read ok ,wait for update
        //set data
        updata_dataL[0] = updata_dataL[2];
        updata_dataL[1] = updata_dataL[3];

        updata_dataL[65534] = updata_dataL[65532];
        updata_dataL[65535] = updata_dataL[65533];

        updata_dataR[0] = updata_dataR[2];
        updata_dataR[1] = updata_dataR[3];

        updata_dataR[65534] = updata_dataR[65532];
        updata_dataR[65535] = updata_dataR[65533];

#if  0
       //--test debug
        qDebug("uch_work_mode = 0; 0x18 can read.");
        qDebug("updata_dataL[0] = %ld",updata_dataL[0]);
        qDebug("updata_dataL[1] = %ld",updata_dataL[1]);

        qDebug("updata_dataL[2] = %ld",updata_dataL[2]);
        qDebug("updata_dataL[3] = %ld",updata_dataL[3]);

        qDebug("updata_dataL[4] = %ld",updata_dataL[4]);
        qDebug("updata_dataL[5] = %ld",updata_dataL[5]);

        qDebug("updata_dataL[6] = %ld",updata_dataL[6]);
        qDebug("updata_dataL[7] = %ld",updata_dataL[7]);


        qDebug("updata_dataL[65532] = %ld",updata_dataL[65532]);
        qDebug("updata_dataL[65533] = %ld",updata_dataL[65533]);

        qDebug("updata_dataL[65534] = %ld",updata_dataL[65534]);
        qDebug("updata_dataL[65535] = %ld",updata_dataL[65535]);

        for(i = 0; i < 32; i++)
        {
            qDebug("updata_dataL[%ld] = %04x",i,updata_dataL[i]);
            //qDebug("updata_dataL[%ld] = %ld",i+39456,updata_dataL[i+39456]);
        }
        qDebug("\r\n");
        for(i = 0; i < 32; i++)
        {
            qDebug("updata_dataL[%04x] = %04x",i+32760,updata_dataL[i+32760]);
            //qDebug("updata_dataR[%ld] = %ld",i+39430,updata_dataR[i+39430]);
        }
    //-test debug
#endif
    }

    #if 1//-- update to PC < 128bags
    if (uch_dataport == 1)              // channal1
    {
        for (i = 0; i < EVERYBAGS_LOOP; i++)
        {
            //--data make
            //EncodeUlong(updata_dataL[4*i], a_sendata);
            //memcpy(a_sendata,&updata_dataL[4*i],sizeof(a_sendata));

            num = (EVERYBAGS_LOOP*2)*(uch_curbyte-1) + 2*i;             //every bag include EVERYBAGS_LOOP INT32U, means every bag include (EVERYBAGS_LOOP*2) INT16U

            a_sendata[0] = (INT8U)(updata_dataL[num] >> 8);
            a_sendata[1] = (INT8U)updata_dataL[num];
            num += 1;
            a_sendata[2] = (INT8U)(updata_dataL[num] >> 8);
            a_sendata[3] = (INT8U)updata_dataL[num];

            //memcpy(a_sendata,&updata_dataL[4*i],sizeof(a_sendata));
            //qDebug("a_sendata[0][1][2][3]:%x,%x,%x,%x",a_sendata[0],a_sendata[1],a_sendata[2],a_sendata[3]);
            memcpy(p_str,a_sendata,sizeof(a_sendata));
            p_str = p_str + 4;
        }
    }

    else if (uch_dataport == 2)         // channal2
    {
        for (i = 0; i < EVERYBAGS_LOOP; i++)
        {
            //--data make
            //EncodeUlong(updata_dataR[4*i], a_sendata);

            num = (EVERYBAGS_LOOP*2)*(uch_curbyte-1) + 2*i;

            a_sendata[0] = (INT8U)(updata_dataR[num] >> 8);
            a_sendata[1] = (INT8U)updata_dataR[num];
            num += 1;
            a_sendata[2] = (INT8U)(updata_dataR[num] >> 8);
            a_sendata[3] = (INT8U)updata_dataR[num];
            memcpy(p_str,a_sendata,sizeof(a_sendata));
            p_str = p_str + 4;
        }
        if (uch_curbyte >= ALLBAGS_TOPC)                 // all bags update finish (left + right = 256)
        {
            update_flag = 0;
        }
    }

     #endif

#if 0
INT32U Sigdata_dataL[32768] = {0};
INT32U Sigdata_dataR[32768] = {0};

INT32S outSigdata_dataL[32768] = {0};
INT32S outSigdata_dataR[32768] = {0};

    //--save 16bit data --first
    FILE *fpPCMdata = fopen("firstSignaldataL.txt","w+");
    if (fpPCMdata == NULL)
    {
        qDebug("fopen1 ERR.");
    }
    for (i = 0; i < 65536; i++)
    {
        //fprintf(fpPCMdata, "%ld\n", updata_dataL[i]); //将数据写入文件
        fprintf(fpPCMdata, "%ld\n", updata_dataL[i]); //将原始数据写入firstSignaldataL
    }
    fclose(fpPCMdata);

    //--make 32bit data
    for (i = 0; i < 32768; i++)
    {
        Sigdata_dataL[i] = (updata_dataL[2*i] << 16) + updata_dataL[2*i+1];
        Sigdata_dataR[i] = (updata_dataR[2*i] << 16) + updata_dataR[2*i+1];
    }

    //--save 32bit data --second
    fpPCMdata = fopen("secondSignaldataL.txt","w+");
    if (fpPCMdata == NULL)
    {
        qDebug("fopen1 ERR.");
    }
    for (i = 0; i < 32768; i++)
    {
        fprintf(fpPCMdata, "%ld\n", Sigdata_dataL[i]); //将24bits数据写入secondSignaldataL
        //fprintf(fpPCMdata, "%ld\n", Sigdata_dataR[i]); //将24bits数据写入右通道secondSignaldataR
    }
    fclose(fpPCMdata);

    //--make PCM data
    run_datatest(Sigdata_dataL,outSigdata_dataL);
    run_datatest(Sigdata_dataR,outSigdata_dataR);

    //--write data to txt;
    //--save PCM data --third
    fpPCMdata = fopen("thirdSignaldataL.txt","w+");
    if (fpPCMdata == NULL)
    {
        qDebug("fopen1 ERR.");
    }
    //for (i = 0; i < 65536; i++)
    for (i = 0; i < 32768; i++)
    {
        //fprintf(fpPCMdata, "%ld\n", updata_dataL[i]); //将数据写入文件
        fprintf(fpPCMdata, "%ld\n", outSigdata_dataL[i]); //将解PCM码数据写入thirdSignaldataL

    }
    fclose(fpPCMdata);

    fpPCMdata = fopen("thirdSignaldataR.txt","w+");
    if (fpPCMdata == NULL)
    {
        qDebug("fopen2 ERR.");
    }
    for (i = 0; i < 32768; i++)
    {
        //fprintf(fpPCMdata, "%ld\n", updata_dataR[i]); //将数据写入文件
        fprintf(fpPCMdata, "%ld\n", outSigdata_dataR[i]); //将数据写入文件
    }
    fclose(fpPCMdata);

    qDebug("Finish.");
    while(1); //-*-
#endif

    *uip_SendDataLenth = (EVERYBAGS_SIZE+3);                            // every bag size

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm21Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 读取版本号
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
static INT8U uch_mainBoardSoftID[] = "   EXPEC5210.1631A.U0263.T1A.012"; //代码修改后必须修改版本号
INT8U Comm21Read(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth)
{

    if(ReceData[0] == 0x00) //主板版本号
    {
        memcpy(SendData, uch_mainBoardSoftID, 32);
    }
    else if(ReceData[0] == 0x01) //FPGA
    {
        INT8U uch_FPGAVersion[32];
        ReadFPGAVersion(uch_FPGAVersion);
        memcpy(SendData, uch_FPGAVersion, 32);
    }
//    else if(ReceData[0] == 0x02) //EI
//    {
//       // memcpy(SendData, g_clEI.st_Mnt.uch_Version, 32);
//    }
//    else if(ReceData[0] == 0x03) //Q0
//    {
//        memcpy(SendData, g_clQ0Driver.st_Mnt.uch_Version, 32);
//    }
    else
    {
        return 1;
    }

    *uip_SendDataLenth = 32;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm22Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 仪器型号选择read
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm22Read(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth)
{
    SendData[0] = g_clMnt.m_uchDeviceModel;
    *uip_SendDataLenth = 1;
    return 0;

}

//==================================================================================================
//| 函数名称 | Comm22Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 仪器型号选择
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm22Write(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth)
{
    INT8U uch_model = ReceData[0];
    SendData[0] = WRITE_SUCCESS;
    switch(uch_model)
    {
        case 0x01://EXPEC5210
        {
            m_SQLite.SetDeviceModel(DEVICE_MODEL_EXPEC5210);

        }break;
        case 0x02://EXPEC5250
        {
            m_SQLite.SetDeviceModel(DEVICE_MODEL_EXPEC5250);
        }break;
        default:
        {
            SendData[0] = WRITE_FAILED;
        }break;
    }

    *uip_SendDataLenth = 1;

    return 0;

}
//==================================================================================================
//| 函数名称 | Comm23Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 开关量控制
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm23Write(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth)
{
    INT8U uch_type = ReceData[0];
    INT8U uch_switch = ReceData[1];
    SendData[0] = WRITE_SUCCESS;

    qDebug("Comm23Write,SWICH TYPE:%d,state:%d",uch_type,uch_switch);
    switch(uch_type)
    {
        case 0x00://倍增器入口高压
        {
            if(uch_switch == 0x00)
            {
                OperateSwitch(CAPI_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_CapiIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateSwitch(CAPI_SW,SWITCH_ON);
                g_clIOCtrl.m_stIOCtrl.uch_CapiIO = 1;
            }
        }break;
        case 0x01://毛细管高压（电晕针）
        {
            if(uch_switch == 0x00)
            {
                OperateSwitch(CORO_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_CoroIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateSwitch(CORO_SW,SWITCH_ON);
                g_clIOCtrl.m_stIOCtrl.uch_CoroIO = 1;
            }
        }break;
        case 0x02://倍增器
        {
            if(uch_switch == 0x00)
            {
                OperateSwitch(EM_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_EMIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                if(g_clMnt.m_uchRunMode == DEVICE_RUN_MODE_DEBUG)
                {
                    if((g_clMnt.st_mMntPara.st_VacuValue2.f_RealVal > 0.009) || (g_clMnt.st_mMntPara.st_VacuValue2.f_RealVal == 0))
                    {
                         return 1;
                    }
                }
                OperateSwitch(EM_SW,SWITCH_ON);
                g_clIOCtrl.m_stIOCtrl.uch_EMIO = 1;

            }
        }break;
        case 0x03://打拿极开关
        {
            if(uch_switch == 0x00)
            {
                OperateSwitch(DORA_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_DoraIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateSwitch(DORA_SW,SWITCH_ON);
                g_clIOCtrl.m_stIOCtrl.uch_DoraIO = 1;
            }
        }break;
        case 0x04://离子极性开关
        {
            if(uch_switch == 0x00)
            {
                OperateSwitch(NEGION_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_NegionIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateSwitch(NEGION_SW,SWITCH_ON);
                g_clIOCtrl.m_stIOCtrl.uch_NegionIO = 1;
            }
        }break;
        case 0x05://一键待机
        {
            if(uch_switch == 0x00)
            {
                OperateRunFlag(ONE_KEY_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_PumpIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateRunFlag(ONE_KEY_ON);
                g_clIOCtrl.m_stIOCtrl.uch_PumpIO = 1;
            }
        }break;
        case 0x0b://BIT单板测试开关
        {
            if(uch_switch == 0x00)
            {
                OperateSwitch(BIT_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_BitIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateSwitch(BIT_SW,SWITCH_ON);
                g_clIOCtrl.m_stIOCtrl.uch_BitIO = 1;
            }
        }break;
        case 0x0c://来宝真空规开关
        {
            if(uch_switch == 0x00)
            {
                OperateSwitch(LBVacuumG_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_LBVacuumGIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateSwitch(LBVacuumG_SW,SWITCH_ON);
                g_clIOCtrl.m_stIOCtrl.uch_LBVacuumGIO = 1;
            }
        }break;
        case 0x0d://保护开关
        {
            if(uch_switch == 0x00)
            {
                OperateRunFlag(PROTECT_SYS_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_ProtectIO = 0;
            }
            else if(uch_switch == 0x11)
            {
                OperateRunFlag(PROTECT_SYS_ON);
                g_clIOCtrl.m_stIOCtrl.uch_ProtectIO = 1;
            }
        }break;
        case 0x0e://rf调谐 lz2016.7.12
        {
            if(uch_switch == 0x00)
            {
                g_clIOCtrl.m_stIOCtrl.uch_RfTune = 0;

                //TODO TIMER
                //cl_gApp.SetTimer(MNT_TIMER_ID,500);
            }
            else if( uch_switch == 0x11 )
            {
                g_clIOCtrl.m_stIOCtrl.uch_RfTune = 1;

                //TODO TIMER
                //cl_gApp.SetTimer(MNT_TIMER_ID,50);
            }
        }break;
        case 0x30://消除弹窗报警码
        {
            if(uch_switch == 0x11)
            {
                ClearPopAlarm();
            }
        }break;
        case 0x31:
        {
            if(uch_switch == 0x00)
            {
                g_clMnt.m_uchRunMode = DEVICE_RUN_MODE_NORMAL;
            }
            else if(uch_switch == 0x11)
            {
                g_clMnt.m_uchRunMode = DEVICE_RUN_MODE_DEBUG;
            }
            else if(uch_switch == 0x22)
            {
                g_clMnt.m_uchRunMode = DEVICE_RUN_MODE_BIT;
            }

        }break;
        case 0x40: //灯丝开关
        {
            if(uch_switch == 0x00)
            {
                g_clEI.st_Mnt.uch_FilamentIO = 0;
            }
            else if( uch_switch == 0x11 )
            {
                g_clEI.st_Mnt.uch_FilamentIO = 1;
            }
        }break;
        case 0x41: //Q0高压开关
        {
            if(uch_switch == 0x00)
            {
                g_clEI.st_Mnt.uch_Q0HighVIO = 0;
            }
            else if( uch_switch == 0x11 )
            {
                g_clEI.st_Mnt.uch_Q0HighVIO = 1;
            }
        }break;
        case 0x50: //Q0驱动板自动调谐开关
        {
            if(uch_switch == 0x00)
            {
                g_clQ0Driver.st_Mnt.uch_Q0AutoTuningIOSet = 0;
            }
            else if( uch_switch == 0x11 )
            {
                g_clQ0Driver.st_Mnt.uch_Q0AutoTuningIOSet = 1;
            }
        }break;
        default:
        {
            SendData[0] = WRITE_FAILED;
        }break;
    }

    *uip_SendDataLenth = 1;

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm24Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 电压设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm24Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_Type = ReceData[0];
    FP32 f_Data = DecodeFloat(ReceData+1);
    if(uch_Type < 0x30) //主板FPGA电压设置
    {
        if( 0 == SetParameterByVoltageList(ReceData) )
        {
                *SendData = WRITE_SUCCESS;
        }
        else
        {
                *SendData = WRITE_FAILED;
        }
    }
    else //发送给其他板
    {
        switch(uch_Type)
        {
            case 0x30: //推斥极设置
            {
                g_clEI.st_Mnt.f_EIRepellerVSet = f_Data;
            }break;
            case 0x31://抽出极设置
            {
                g_clEI.st_Mnt.f_EIDrawoutSet = f_Data;
            }break;
            case 0x32://聚焦极设置
            {
                g_clEI.st_Mnt.f_EIFocusVSet = f_Data;
            }break;
            case 0x33://EI偏转透镜设置
            {
                g_clEI.st_Mnt.f_EIDeflectionVSet = f_Data;
            }break;
            case 0x34://Q0上邻杆高压设置
            {
                g_clEI.st_Mnt.f_Q0UpVHighSet = f_Data;
            }break;
            case 0x35://Q0下邻杆高压设置
            {
                g_clEI.st_Mnt.f_Q0LowVHighSet = f_Data;
            }break;
            case 0x36://灯丝电压设置
            {
                g_clEI.st_Mnt.f_FilamentVSet = f_Data;
            }break;
            case 0x37://灯丝偏转电压设置
            {
                g_clEI.st_Mnt.f_FilamentOffsetVSet = f_Data;
            }break;
            case 0x40: //Q0上邻杆低压设置
            {
                g_clQ0Driver.st_Mnt.f_Q0UpVLowSet = f_Data;
            }break;
            case 0x41://Q0下邻杆低压设置
            {
                g_clQ0Driver.st_Mnt.f_Q0LowVLowSet = f_Data;
            }break;
            case 0x42://Q0DDS输出
            {
                g_clQ0Driver.st_Mnt.f_DDSOutSet = f_Data;
            }break;

        }

    }
    *uip_SendDataLenth = 1;

    return WRITE_SUCCESS;
}

//==================================================================================================
//| 函数名称 | Comm25Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 电压一次性设置 （当前未使用）
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm25Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    //if(0 == SetParameterOneTimes(puch_iData,uin_iLen))
    if (TRUE)
    {
         *SendData = WRITE_SUCCESS;
    }
    else
    {
         *SendData = WRITE_FAILED;
    }


    *uip_SendDataLenth = 1;

    return WRITE_SUCCESS;
}


//==================================================================================================
//| 函数名称 | Comm26Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 仪器功能切换read
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm26Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    switch(uch_Type)
    {
        case 0x01:
        {
            *SendData  = g_clMnt.m_uchSourceSelect;
        }break;
        default:
        {
            *SendData = READ_FAILED;
        }break;

    }
    *uip_SendDataLenth = 1;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm26Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 仪器功能切换
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm26Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    INT8U uch_Data = ReceData[1];
    *SendData = WRITE_SUCCESS;
    switch(uch_Type)
    {
        case 0x01:
        {
            if(uch_Data == 0x00)
            {

                m_SQLite.SetSampleSelect(SOURCE_ESI);
                g_clEI.EISampleSelect(SOURCE_ESI);

            }
            else if(uch_Data == 0x01)
            {
                m_SQLite.SetSampleSelect(SOURCE_EI);
                g_clEI.EISampleSelect(SOURCE_EI);
            }
            else
            {
                *SendData = WRITE_FAILED;
            }
        }break;
        default:
        {
            *SendData = WRITE_FAILED;
        }break;

    }



    *uip_SendDataLenth = 1;

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm27Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 温度控制
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm27Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    FP32 f_Temp = DecodeFloat(ReceData+1);
    *SendData = WRITE_SUCCESS;
    switch(uch_Type)
    {
        case 0x04:
        {
            g_clEI.st_Mnt.uch_EISourceTempEn = 1;
            g_clEI.st_Mnt.f_EISourceTempSet = f_Temp;
        }break;
        case 0x05:
        {
            g_clEI.st_Mnt.uch_EIGCInterfaceTempEn = 1;
            g_clEI.st_Mnt.f_EIGCInterfaceTempSet = f_Temp;
        }break;
        default:
        {
            *SendData = WRITE_FAILED;
        }break;

    }



    *uip_SendDataLenth = 1;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm30Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 监控命令
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm30Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT16U uin_Index = 0;
    INT16U uin_Data;
    AlarmCodePCGetCnt(); //对已有的报警吗进行技术,防止报警码未读取就自动消除
    g_clMnt.uch_PCCommCnt = 0; //计数 关联 PC通信断开

    memcpy(SendData,auch_AlarmCode,TOTAL_ALARM_NUM);
    uin_Index += 10;
    EncodeFloat(g_clMnt.st_mMntPara.st_CapillaryV.f_RealVal,SendData+uin_Index);    // 毛细管高压
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_CapillaryI.f_RealVal,SendData+uin_Index);    //电晕针电流 st_CapillaryI--->改成毛细管电流
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_SampleConeV.f_RealVal,SendData+uin_Index);   //采样锥电压
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_ExtractorV.f_RealVal,SendData+uin_Index);    //萃取锥电压
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_RfLenV.f_RealVal,SendData+uin_Index);        //传输杆电压
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms1PreV.f_RealVal,SendData+uin_Index);       //ms1 pre
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms1IonEnergy.f_RealVal,SendData+uin_Index);  //ms1 ion energy
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms1LM.f_RealVal,SendData+uin_Index);         //MS1LM
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms1HM.f_RealVal,SendData+uin_Index);         //MS1HM
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_CIDEntranceV.f_RealVal,SendData+uin_Index);  //CID ENTRANCE
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_CIDV.f_RealVal,SendData+uin_Index);          //CID V
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_CIDEixtV.f_RealVal,SendData+uin_Index);      //CID EXIT
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_CIDOverV.f_RealVal,SendData+uin_Index);      //碰撞池 外壁电压
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms2PreV.f_RealVal,SendData+uin_Index);       //MS2 PRE
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms2IonEnergy.f_RealVal,SendData+uin_Index);  //MS2 ION ENERGY
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms2LM.f_RealVal,SendData+uin_Index);         //MS2 LM
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms2HM.f_RealVal,SendData+uin_Index);         //MS2 HM
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_EmHighV.f_RealVal,SendData+uin_Index);           //倍增器电压？？？
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_EmHighOffsetV.f_RealVal,SendData+uin_Index);//倍增器偏转电压
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Dora.f_RealVal,SendData+uin_Index);      //打拿极电压？？？
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms1RF.f_RealVal,SendData+uin_Index);         //MS1 RF
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms2RF.f_RealVal,SendData+uin_Index);         //MS2 RF
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms1DCP.f_RealVal,SendData+uin_Index);        //MS1 DCP
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms1DCN.f_RealVal,SendData+uin_Index);        //MS1 DCN
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms2DCP.f_RealVal,SendData+uin_Index);        //MS2 DCP
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Ms2DCN.f_RealVal,SendData+uin_Index);        //MS2 DCN
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.ESourcePositive5VDigital.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.ESourcePositive1Point2VDigital.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.ESourcePositive3Point3VDigital.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.ESourceNegative5VAnalog.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.ESourcePositive5VAnalog.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.ESourceNegative14VAnalog.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.ESourcePositive14VAnalog.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat((FP32)0,SendData+uin_Index);//预留
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.Reference2Point5VOne.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.Reference2Point5VTwo.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat((FP32)0,SendData+uin_Index); //预留
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.RFPowerESourceVoltage.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Temperta.f_RealVal,SendData+uin_Index);              //主板温度
    uin_Index += 4;
    EncodeFloat((FP32)0,SendData+uin_Index);    //预留
    uin_Index += 4;
    EncodeFloat((FP32)0,SendData+uin_Index);    //预留
    uin_Index += 4;
    EncodeFloat((FP32)0,SendData+uin_Index);    //预留
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_VacuValue1.f_RealVal,SendData+uin_Index);            //传输杆腔室压力
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_VacuValue2.f_RealVal,SendData+uin_Index);            //分析器腔室压力
    uin_Index += 4;

    EncodeFloat(g_clMnt.st_mMntPara.st_VacuValue3.f_RealVal,SendData+uin_Index);         //碰撞池压力
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_VacuValue4.f_RealVal,SendData+uin_Index);            //
    uin_Index += 4;

    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_CapiIO*0x11;
    uin_Index += 1;
    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_CoroIO*0x11;
    uin_Index += 1;
    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_EMIO*0x11;
    //qDebug("g_clIOCtrl.m_stIOCtrl.uch_EMIO* %x",g_clIOCtrl.m_stIOCtrl.uch_EMIO);
    uin_Index += 1;
    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_DoraIO*0x11;
    uin_Index += 1;
    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_NegionIO*0x11;
    uin_Index += 1;
    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_PumpIO*0x11; //一键待机开关
    uin_Index += 1;
    /*uch_Data = cl_gApp.cl_puIOCtr.GetSTRIOObject()->uch_PrePumpIO*0x11;
    AddData(&uch_Data,1);*/
    *(SendData+uin_Index) = g_clMnt.m_uchRunMode; //运行模式
    //qDebug("Cur DeviceRunMode %x",g_clMnt.m_uchRunMode);
    uin_Index += 1;


    EncodeFloat(g_clMnt.st_mMntPara.st_EmHighEntranceV.f_RealVal,SendData+uin_Index);//倍增器入口电压
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_HV1.f_RealVal,SendData+uin_Index);//HV1
    uin_Index += 4;
    //EncodeFloat(g_clMnt.st_mMntPara.st_HV2.f_RealVal,SendData+uin_Index);//HV2
    EncodeFloat(g_clMnt.st_mMntPara.st_RfLenV.f_RealVal,SendData+uin_Index);///HV2
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_BaffleVoltage.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_BaffleCurrunt.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Q0out2.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    //TODO XML修改
    //*(SendData+uin_Index) = cl_Xml.m_XmlCoonfig.bIonicEnergyConnected * 0x11;
    uin_Index += 1;
    //*(SendData+uin_Index) = cl_Xml.m_XmlCoonfig.bUseSpellMan* 0x11;
    uin_Index += 1;
    //*(SendData+uin_Index) = cl_Xml.m_XmlCoonfig.bUseDieBan* 0x11;
    *(SendData+uin_Index) = cl_App.m_uchMSRunStatus*0x11;
    uin_Index += 1;

    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_ProtectIO*0x11;//保护开关
    uin_Index += 1;

    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_BitIO*0x11;//BIT测试状态
    uin_Index += 1;
    *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_LBVacuumGIO*0x11;//来宝真空规开关状态
    uin_Index += 1;

    OperateRunFlag(LC_MS_SYN_FLAG);
    MemoryRead(FPGA_REG,LC_MS_SYN_FLAG_ADDR,&uin_Data); //读取LC 同步标志
    *(SendData+uin_Index) = (uin_Data > 0)?0x11:0x00;
    uin_Index += 1;
    EncodeFloat(g_clMnt.st_mMntPara.st_LenthDC2.f_RealVal,SendData+uin_Index);  //有问题 通信协议需要修改2016.4.12
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_LenthDC3.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_LenthDC4.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Rf1Current.f_RealVal,SendData+uin_Index);//EncodeFloat(g_clMnt.st_mMntPara.st_LenthDC5.f_RealVal);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_Rf2Current.f_RealVal,SendData+uin_Index);//EncodeFloat(g_clMnt.st_mMntPara.st_LenthDC6.f_RealVal);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_HEXPoleV.f_RealVal,SendData+uin_Index);
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_LenthDC1.f_RealVal,SendData+uin_Index);//2016.6.17
    uin_Index += 4;
    EncodeFloat(g_clMnt.st_mMntPara.st_DC7.f_RealVal,SendData+uin_Index);
    uin_Index += 4;


    if(g_clMnt.m_uchDeviceModel == DEVICE_MODEL_EXPEC5250) //该型号多了一些字节
    {
        //1字节（INT8U）GC-MS同步标志
        OperateRunFlag(LC_MS_SYN_FLAG);//当前可能是同一个信号
        MemoryRead(FPGA_REG,LC_MS_SYN_FLAG_ADDR,&uin_Data); //读取GC-MS 同步标志
        *(SendData+uin_Index) = (uin_Data > 0)?0x11:0x00;
        uin_Index += 1;
        *(SendData+uin_Index) = g_clMnt.m_uchSourceSelect; //1字节（INT8U）GC/LC进样选择
        uin_Index += 1;

        uin_Index += 2;//2字节（）预留
        uin_Index += 8;//8字节（）预留
        EncodeFloat(g_clMnt.st_mMntPara.st_EIPower14V.f_RealVal,SendData+uin_Index);//4字节（FP32）EI板电源24V
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EIBoardTemp.f_RealVal,SendData+uin_Index);//4字节（FP32）EI板温度
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EISourceTemp.f_RealVal,SendData+uin_Index);//4字节（FP32）EI源温度（校准后）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EISourceTempOriginal.f_RealVal,SendData+uin_Index);//4字节（FP32）EI源温度（校准前）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EIGCInterfaceTemp.f_RealVal,SendData+uin_Index);//4字节（FP32）GC接口温度（校准后）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EIGCInterfaceTempOriginal.f_RealVal,SendData+uin_Index);//4字节（FP32）GC接口温度（校准前）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EIRepellerV.f_RealVal,SendData+uin_Index);//4字节（FP32）EI推斥极
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EIDrawout.f_RealVal,SendData+uin_Index);//4字节（FP32）EI抽出极
        uin_Index += 4;

        //Q0驱动板
        EncodeFloat(g_clMnt.st_mMntPara.st_EIFocusV.f_RealVal,SendData+uin_Index);//4字节（FP32）EI聚焦极
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_EIDeflectionV.f_RealVal,SendData+uin_Index);//4字节（FP32）EI偏转透镜（圆柱体）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0UpVHigh.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0上邻杆（高压）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0LowVHigh.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0下邻杆（高压）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_FilamentV.f_RealVal,SendData+uin_Index);//4字节（FP32）灯丝电压
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_FilamentOffsetV.f_RealVal,SendData+uin_Index);//4字节（FP32）灯丝偏置电压
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_FilamentI.f_RealVal,SendData+uin_Index);//4字节（FP32）灯丝反馈电流
        uin_Index += 4;
        uin_Index += 12;//12字节（）预留
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_FilamentIO;//1字节（INT8U）灯丝切换开关
        uin_Index += 1;
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_Q0HighVIO;//1字节（INT8U）Q0高压开关
        uin_Index += 1;
        uin_Index += 2;//2字节（）预留


        EncodeFloat(g_clMnt.st_mMntPara.st_Q0Power.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0驱动板电源
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0BoardTemp.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0驱动板温度
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0UpVLow.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0上邻杆（低压）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0LowVLow.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0下邻杆（低压）
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_DDSOut.f_RealVal,SendData+uin_Index);//4字节（FP32）DDS输出
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0V.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0反馈电压
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0I.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0反馈电流
        uin_Index += 4;
        EncodeFloat(g_clMnt.st_mMntPara.st_Q0DriverV.f_RealVal,SendData+uin_Index);//4字节（FP32）Q0驱动电压
        uin_Index += 4;
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_Q0Relay11;//1字节（INT8U）Q0电压切换继电器11
        uin_Index += 1;
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_Q0Relay12;//1字节（INT8U）Q0电压切换继电器12
        uin_Index += 1;
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_Q0Relay21;//1字节（INT8U）Q0电压切换继电器21
        uin_Index += 1;
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_Q0Relay22;//1字节（INT8U）Q0电压切换继电器22
        uin_Index += 1;
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_Q0DCRelay1;//1字节（INT8U）Q0DC电压切换继电器1
        uin_Index += 1;
        *(SendData+uin_Index) = g_clIOCtrl.m_stIOCtrl.uch_Q0DCRelay2;//1字节（INT8U）Q0DC电压切换继电器2
        uin_Index += 1;
        uin_Index += 20;//20字节预留



    }


    *uip_SendDataLenth = uin_Index;


    return 0;
}

//==================================================================================================
//| 函数名称 | Comm33Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 振镜电机
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
INT8U Comm33Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    //--发送给can总线
    SendData[0] = WRITE_SUCCESS;
    *uip_SendDataLenth = 1;

    return 0;
}


//==================================================================================================
//| 函数名称 | Comm40Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 启动MS采样
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm40Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_Switch = ReceData[0];
    SendData[0] = WRITE_SUCCESS;
    //CAppComFpi* pcl_ComFpi = &a_ComFpi;
    if(uch_Switch == 0x00)//STOP MS
    {
        qDebug("COM get stop ms");
        cl_App.uch_MSOpration = MS_OPRATION_STOP;
    }
    else if(uch_Switch == 0x11) //START MS
    {
        qDebug("COM get start ms");
        cl_App.uch_MSOpration = MS_OPRATION_START;
    }
    else
    {
        SendData[0] = WRITE_FAILED;
    }



    *uip_SendDataLenth = 1;

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm41Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 扫描片段参数设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.17
//==================================================================================================
INT8U Comm41Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT16U uin_iLen = DecodeUint(ReceData);

    INT16U uin_Data = cl_gMethod.SaveMethod(ReceData+2,uin_iLen);
    EncodeUint(uin_Data,SendData);
    if(DecodeUint(ReceData) == uin_Data) //说明收到了最后一个包
    {

    }
    *uip_SendDataLenth = 2;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm43Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 扫描扩展电压
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.17
//==================================================================================================
INT8U Comm43Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_Index = 0;
    g_clEI.st_Mnt.f_EIRepellerVSet = DecodeFloat(ReceData+uch_Index);//4字节（FP32）EI推斥极
    uch_Index += 4;
    g_clEI.st_Mnt.f_EIDrawoutSet = DecodeFloat(ReceData+uch_Index);//4字节（FP32）EI抽出极
    uch_Index += 4;
    g_clEI.st_Mnt.f_EIFocusVSet = DecodeFloat(ReceData+uch_Index); //4字节（FP32）EI聚焦极
    uch_Index += 4;
    g_clEI.st_Mnt.f_EIDeflectionVSet = DecodeFloat(ReceData+uch_Index); //4字节（FP32）EI偏转透镜（圆柱体）
    uch_Index += 4;
    g_clEI.st_Mnt.f_Q0UpVHighSet = DecodeFloat(ReceData+uch_Index);//4字节（FP32）Q0上邻杆（高压）
    uch_Index += 4;
    g_clEI.st_Mnt.f_Q0LowVHighSet = DecodeFloat(ReceData+uch_Index);//4字节（FP32）Q0下邻杆（高压）
    uch_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0UpVLowSet = DecodeFloat(ReceData+uch_Index);//4字节（FP32）Q0上邻杆（低压）
    uch_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0LowVLowSet = DecodeFloat(ReceData+uch_Index);    //4字节（FP32）Q0下邻杆（低压）
    uch_Index += 4;
    //32字节（）预留

    SendData[0] = WRITE_SUCCESS;


    *uip_SendDataLenth = 1;

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm47Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 读取检测器数据
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2019.08.15
//==================================================================================================
INT8U Comm47Read(INT8U* ReceData, INT8U* SendData, INT16U *uip_SendDataLenth)
{

    INT32U ul_dataLength = g_clDetectData.GetDataLength(ReceData[0], MAX_DATA_LEN*2);
    //qDebug("read datalen %d",ul_dataLength);
    if(ul_dataLength > 0)
    {//有数据

        SendData[0] = 0x0;//有数据
        SendData[1] = ReceData[0];//当前包号
        g_clDetectData.GetdatasToPC(SendData + 2, ul_dataLength, ReceData[0]);

    }
    else
    {//无数据
        SendData[0] = 0x01;//无数据
        SendData[1] = 0x00;
    }
    *uip_SendDataLenth = ul_dataLength*2+2;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm50Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 电压设置/反馈校准 读取
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.17
//==================================================================================================
INT8U Comm50Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT16U uin_Index = 0;
    INT8U uch_IonFlag = ReceData[0];
    INT8U uch_VolNum = ReceData[1];
    INT8U uch_Offset = 0;
    INT8U uch_VType = 0;

    STRCALIPARA st_CaliPara;
    if(uch_VolNum == 0xff)
    {
        uch_VolNum = 40;
        uch_VType = 0xff;

    }
    else
    {
        uch_VType = uch_VolNum;
        uch_VolNum = 1;
    }


    if(uch_IonFlag == 0) //负离子偏移40
    {
        uch_VType = uch_VType + 40;
        uch_Offset = 40;
    }

    //qDebug(" read uch_IonFlag %d, uch_VolNum , %d",uch_IonFlag,uch_VolNum);

    SendData[uin_Index] = uch_IonFlag;
    uin_Index++;
    SendData[uin_Index] = ReceData[1];
    uin_Index++;

    for(int i=0;i<uch_VolNum;i++)
    {
        if(uch_VolNum == 1)
        {
            m_SQLite.GetCali(uch_VType,st_CaliPara);
        }
        else
        {
            m_SQLite.GetCali(i + uch_Offset,st_CaliPara);
            //qDebug("calipara id %d",st_CaliPara.uch_ID);
        }

        SendData[uin_Index] = st_CaliPara.uch_ID;
        uin_Index++;
        EncodeFloat(st_CaliPara.f_Max,SendData+uin_Index);
        uin_Index+=4;
        EncodeFloat(st_CaliPara.f_Min,SendData+uin_Index);
        uin_Index+=4;
        EncodeFloat(st_CaliPara.f_Default,SendData+uin_Index);
        uin_Index+=4;
        EncodeFloat(st_CaliPara.f_SetK,SendData+uin_Index);
        uin_Index+=4;
        EncodeFloat(st_CaliPara.f_SetB,SendData+uin_Index);
        uin_Index+=4;
        EncodeFloat(st_CaliPara.f_ReadK,SendData+uin_Index);
        uin_Index+=4;
        EncodeFloat(st_CaliPara.f_ReadB,SendData+uin_Index);
        uin_Index+=4;

        //qDebug("%d,%f,%f,%f,%f,%f,%f,%f",st_CaliPara.uch_ID,st_CaliPara.f_Max,st_CaliPara.f_Min,\
        st_CaliPara.f_Default,st_CaliPara.f_SetK,st_CaliPara.f_SetB,st_CaliPara.f_ReadK,st_CaliPara.f_ReadB);
    }




    *uip_SendDataLenth = uin_Index;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm50Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 电压设置/反馈校准 设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.17
//==================================================================================================
INT8U Comm50Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_IonFlag = ReceData[0];
    INT8U uch_VolNum = ReceData[1];

    INT8U i;

    STRCALIPARA st_CaliPara;
    //qDebug("uch_IonFlag %d, uch_VolNum , %d",uch_IonFlag,uch_VolNum);
    if(uch_IonFlag == 0) //负离子
    {
        for(i=0;i<uch_VolNum;i++)
        {

            st_CaliPara.uch_ID = ReceData[2+29*i] + 40; //负离子偏移40ID
            st_CaliPara.f_Max = DecodeFloat(ReceData + (3+29*i));
            st_CaliPara.f_Min = DecodeFloat(ReceData + (7+29*i));
            st_CaliPara.f_Default = DecodeFloat(ReceData + (11+29*i));
            st_CaliPara.f_SetK = DecodeFloat(ReceData + (15+29*i));
            st_CaliPara.f_SetB = DecodeFloat(ReceData + (19+29*i));
            st_CaliPara.f_ReadK = DecodeFloat(ReceData + (23+29*i));
            st_CaliPara.f_ReadB = DecodeFloat(ReceData + (27+29*i));
            m_SQLite.UpdateCaliPara(st_CaliPara);
        }
    }
    else //正离子
    {
        for(i=0;i<uch_VolNum;i++)
        {
            st_CaliPara.uch_ID = ReceData[2+29*i];
            st_CaliPara.f_Max = DecodeFloat(ReceData + (3+29*i));
            st_CaliPara.f_Min = DecodeFloat(ReceData + (7+29*i));
            st_CaliPara.f_Default = DecodeFloat(ReceData + (11+29*i));
            st_CaliPara.f_SetK = DecodeFloat(ReceData + (15+29*i));
            st_CaliPara.f_SetB = DecodeFloat(ReceData + (19+29*i));
            st_CaliPara.f_ReadK = DecodeFloat(ReceData + (23+29*i));
            st_CaliPara.f_ReadB = DecodeFloat(ReceData + (27+29*i));
            m_SQLite.UpdateCaliPara(st_CaliPara);
        }
    }

    SendData[0] = WRITE_SUCCESS;
    *uip_SendDataLenth = 1;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm52Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 温度控制模式切换读取
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm52Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];

    SendData[0] = uch_Type;
    SendData[1] = 0; //预留
    SendData[2] = 0; //预留
    SendData[3] = 0; //预留

    switch(uch_Type)
    {
        case 0x03:
        {
             if(g_clEI.st_Mnt.uch_EISourceTempHeatMode == 0x02)
             {
                 SendData[4] = 0x11;
             }
             else
             {
                 SendData[4] = 0x0;
             }

        }break;
        case 0x04:
        {
             if(g_clEI.st_Mnt.uch_EIGCInterfaceTempHeatMode == 0x02)
             {
                 SendData[4] = 0x11;
             }
             else
             {
                 SendData[4] = 0x0;
             }
        }break;
        default:
        {
            return READ_FAILED;
        }break;

    }
    *uip_SendDataLenth = 5;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm52Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 温度控制模式切换设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm52Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    INT8U uch_Mode = ReceData[1];
    *SendData = WRITE_SUCCESS;
    switch(uch_Type)
    {
        case 0x03:
        {
            if(uch_Mode = 0x11)
            {
                g_clEI.st_Mnt.uch_EISourceTempHeatMode = 0x02;
            }
            else
            {
                g_clEI.st_Mnt.uch_EISourceTempHeatMode = 0;
            }


        }break;
        case 0x04:
        {

            if(uch_Mode = 0x11)
            {
                g_clEI.st_Mnt.uch_EIGCInterfaceTempHeatMode = 0x02;
            }
            else
            {
                g_clEI.st_Mnt.uch_EIGCInterfaceTempHeatMode = 0;
            }

        }break;
        default:
        {
            *SendData = WRITE_FAILED;
        }break;

    }
    *uip_SendDataLenth = 1;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm53Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 温控信息读取
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm53Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    INT8U uch_Index = 0;

    SendData[0] = uch_Type;
    uch_Index += 1;
    switch(uch_Type)
    {
        case 0x03:
        {


             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempSet,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempMnt,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempKp,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempKi,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempKd,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempLimit,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EISourceOut,SendData+uch_Index);
             uch_Index += 4;
        }break;
        case 0x04:
        {
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempSet,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempMnt,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempKp,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempKi,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempKd,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempLimit,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceOut,SendData+uch_Index);
             uch_Index += 4;
        }break;
        default:
        {
            return READ_FAILED;
        }break;

    }
    *uip_SendDataLenth = 5;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm53Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 温控信息设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm53Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    FP32 f_KP = DecodeFloat(ReceData+1);
    FP32 f_KI = DecodeFloat(ReceData+5);
    FP32 f_KD = DecodeFloat(ReceData+9);
    FP32 f_KLimit = DecodeFloat(ReceData+13);


    *SendData = WRITE_SUCCESS;
    switch(uch_Type)
    {
        case 0x03:
        {
            g_clEI.st_Mnt.f_EISourceTempKp = f_KP;
            g_clEI.st_Mnt.f_EISourceTempKi = f_KI;
            g_clEI.st_Mnt.f_EISourceTempKd = f_KD;
            g_clEI.st_Mnt.f_EISourceTempLimit = f_KLimit;
        }break;
        case 0x04:
        {
            g_clEI.st_Mnt.f_EIGCInterfaceTempKp = f_KP;
            g_clEI.st_Mnt.f_EIGCInterfaceTempKi = f_KI;
            g_clEI.st_Mnt.f_EIGCInterfaceTempKd = f_KD;
            g_clEI.st_Mnt.f_EIGCInterfaceTempLimit = f_KLimit;
        }break;
        default:
        {
            *SendData = WRITE_FAILED;
        }break;

    }
    *uip_SendDataLenth = 1;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm60Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 仪器组件配置读取
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm60Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_Type = ReceData[0];
    INT8U uch_Index = 0;

    SendData[0] = uch_Type;
    uch_Index += 1;
    switch(uch_Type)
    {
        case 0x10:
        {
            INT8U type;
            m_SQLite.GetCIDVacuumGaugeSelect(&type);
            SendData[1] = type;

        }break;
        default:
        {
            return READ_FAILED;
        }break;
    }
    *uip_SendDataLenth = 2;

    return 0;
}


//==================================================================================================
//| 函数名称 | Comm60Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 仪器组件配置设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm60Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_Type = ReceData[0];
    INT8U uch_SetInfo = ReceData[1];

    *SendData = WRITE_SUCCESS;
    switch(uch_Type)
    {
        case 0x10:
        {
            if(uch_SetInfo == UNIT_CIDVACUUMGAUGE_MKS)
            {
                m_SQLite.SetCIDVacuumGaugeSelect(UNIT_CIDVACUUMGAUGE_MKS);
            }
            else
            {
                m_SQLite.SetCIDVacuumGaugeSelect(UNIT_CIDVACUUMGAUGE_CIRCU);
            }

        }break;
        default:
        {
            *SendData = WRITE_FAILED;
        }break;
    }
    *uip_SendDataLenth = 1;

    return 0;

}


//==================================================================================================
//| 函数名称 | Comm71Read()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 环境类校准参数读取
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm71Read(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    INT8U uch_Index = 0;

    SendData[0] = uch_Type;
    uch_Index += 1;
    switch(uch_Type)
    {
        case 0x03:
        {
             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempCaliK,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EISourceTempCaliB,SendData+uch_Index);
             uch_Index += 4;

        }break;
        case 0x04:
        {
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempCaliK,SendData+uch_Index);
             uch_Index += 4;
             EncodeFloat(g_clEI.st_Mnt.f_EIGCInterfaceTempCaliB,SendData+uch_Index);
             uch_Index += 4;

        }break;
        default:
        {
            return READ_FAILED;
        }break;

    }
    //预留 16byte
    EncodeFloat(0,SendData+uch_Index);
    uch_Index += 1;
    EncodeFloat(0,SendData+uch_Index);
    uch_Index += 1;
    EncodeFloat(0,SendData+uch_Index);
    uch_Index += 1;
    EncodeFloat(0,SendData+uch_Index);
    uch_Index += 1;

    *uip_SendDataLenth = 25;

    return 0;
}

//==================================================================================================
//| 函数名称 | Comm71Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 环境类校准参数设置
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
INT8U Comm71Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT8U uch_Type = ReceData[0];
    FP32 f_K = DecodeFloat(ReceData+1);
    FP32 f_B = DecodeFloat(ReceData+5);

    *SendData = WRITE_SUCCESS;
    switch(uch_Type)
    {
        case 0x03:
        {
            g_clEI.st_Mnt.f_EISourceTempCaliK = f_K;
            g_clEI.st_Mnt.f_EISourceTempCaliB = f_B;
        }break;
        case 0x04:
        {
            g_clEI.st_Mnt.f_EIGCInterfaceTempCaliK = f_K;
            g_clEI.st_Mnt.f_EIGCInterfaceTempCaliB = f_B;
        }break;
        default:
        {
            *SendData = WRITE_FAILED;
        }break;

    }

    *uip_SendDataLenth = 1;

    return 0;
}


//==================================================================================================
//| 函数名称 | Comm31ReadAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 |
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm31ReadAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    a_ComFpi.m_bCircu31ReadCommFlag = 1;

    CopyDataFromAToB(ReceData,g_clMnt.m_stEnvMnt.auch_AlarmCode,10);

    g_clMnt.m_stEnvMnt.strPumpCurrent.f_RealVal = DecodeFloat(ReceData+10);
    g_clMnt.m_stEnvMnt.strPumpSpeed.f_RealVal = DecodeFloat(ReceData+18);
    g_clMnt.m_stEnvMnt.strPumpTemp.f_RealVal = DecodeFloat(ReceData+22);
    g_clMnt.m_stEnvMnt.b_CircuStandbyIO = *(ReceData + 55);
    if(g_clMnt.m_stEnvMnt.b_CircuStandbyIO == 0x11)
    {
        g_clMnt.m_stEnvMnt.b_CircuStandbyIO = 1;
    }
    else
    {
        g_clMnt.m_stEnvMnt.b_CircuStandbyIO = 0;
    }
    g_clMnt.m_stEnvMnt.b_N2Valve = *(ReceData + 56);
    if(g_clMnt.m_stEnvMnt.b_N2Valve == 0x11)
    {
        g_clMnt.m_stEnvMnt.b_N2Valve = 1;

    }
    else
    {
        g_clMnt.m_stEnvMnt.b_N2Valve = 0;
    }
    //qDebug("b_N2Value  %d",g_clMnt.m_stEnvMnt.b_N2Valve);
    g_clMnt.m_stEnvMnt.b_ExhaustGasesValve = *(ReceData + 68);
    if(g_clMnt.m_stEnvMnt.b_ExhaustGasesValve == 0x11)
    {
        g_clMnt.m_stEnvMnt.b_ExhaustGasesValve = 1;
    }
    else
    {
        g_clMnt.m_stEnvMnt.b_ExhaustGasesValve = 0;
    }
    g_clMnt.m_stEnvMnt.f_IonicTempSet = DecodeFloat(ReceData+78);
    g_clMnt.m_stEnvMnt.f_VacuValue = DecodeFloat(ReceData+95);


//    qDebug("f_IonicTempSet %f",g_clMnt.m_stEnvMnt.f_IonicTempSet);


    return 0;
}

//==================================================================================================
//| 函数名称 | Comm23CircuWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 环境板开关量控制
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm23CircuWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U i;
    INT8U uch_Index = 0;
    for(i=0;i<2;i++)
    {
        SendData[uch_Index] = ReceData[i];
        uch_Index++;
    }
    *uip_SendDataLenth = uch_Index;
    return 0;
}


//==================================================================================================
//| 函数名称 | Comm23CircuWriteAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 环境板开关量控制反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm23CircuWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    //qDebug("23circu wirte ok");

    return 0;
}


//==================================================================================================
//| 函数名称 | Comm27CircuWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置环境板 真空接口温度
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm27CircuWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U i;
    INT8U uch_Index = 0;
    for(i=0;i<5;i++)
    {
        SendData[uch_Index] = ReceData[i];
        uch_Index++;
    }
    *uip_SendDataLenth = uch_Index;
    return 0;
}

//==================================================================================================
//| 函数名称 | Comm27CircuWriteAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 环境板开关量控制反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm27CircuWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    qDebug("27circu wirte ok");

    return 0;
}


//------------------------------------------------------------EI板------------------------------------------------------



//==================================================================================================
//| 函数名称 | Comm23EIWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置开关量
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm23EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    SendData[0] = 0;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm23EIWriteAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置开关量反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm23EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm24EIWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置电压
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm24EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    SendData[0] = WRITE_FAILED;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm24EIWriteAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置电压反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm24EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm25EIWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 一次性设置电压
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm25EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    SendData[0] = WRITE_FAILED;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm25EIWriteAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 一次性设置电压反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm25EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    SendData[0] = WRITE_FAILED;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm27EIWrite()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置温度
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm27EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    SendData[0] = WRITE_FAILED;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm27EIWriteAck
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置温度反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm27EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    *uip_SendDataLenth = 0;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm31EIReadAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 监控
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm31EIReadAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    INT16U uin_Index = 0;
    a_ComFpi.m_bEI31ReadCommFlag = 1;

    g_clEI.st_Mnt.f_EIRepellerVSet = DecodeFloat(ReceData+uin_Index);      //推斥极设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIRepellerVMnt = DecodeFloat(ReceData+uin_Index);      //推斥极监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIDrawoutSet = DecodeFloat(ReceData+uin_Index);        //抽出极设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIDrawoutMnt = DecodeFloat(ReceData+uin_Index);        //抽出极监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIFocusVSet = DecodeFloat(ReceData+uin_Index);         //聚焦极设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIFocusVMnt = DecodeFloat(ReceData+uin_Index);         //聚焦极监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIDeflectionVSet = DecodeFloat(ReceData+uin_Index);    //EI偏转透镜设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIDeflectionVMnt = DecodeFloat(ReceData+uin_Index);    //EI偏转透镜监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_Q0UpVHighSet = DecodeFloat(ReceData+uin_Index);        //Q0上邻杆高压设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_Q0UpVHighMnt = DecodeFloat(ReceData+uin_Index);        //Q0上邻杆高压监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_Q0LowVHighSet = DecodeFloat(ReceData+uin_Index);       //Q0下邻杆高压设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_Q0LowVHighMnt = DecodeFloat(ReceData+uin_Index);       //Q0下邻杆高压监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_FilamentVSet = DecodeFloat(ReceData+uin_Index);        //灯丝电压设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_FilamentVMnt = DecodeFloat(ReceData+uin_Index);        //灯丝电压监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_FilamentOffsetVSet = DecodeFloat(ReceData+uin_Index);  //灯丝偏转电压设置
    uin_Index += 4;
    g_clEI.st_Mnt.f_FilamentOffsetVMnt = DecodeFloat(ReceData+uin_Index);  //灯丝偏转电压监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_FilamentIMnt = DecodeFloat(ReceData+uin_Index);        //灯丝反馈电流监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIPower14V = DecodeFloat(ReceData+uin_Index);     //EI板电源24V监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIBoardTemp = DecodeFloat(ReceData+uin_Index);  //EI板温度监控
    uin_Index += 4;


    //EI源控温信息
    g_clEI.st_Mnt.uch_EISourceTempEn = ReceData[uin_Index]; //控温使能
    uin_Index += 1;
    g_clEI.st_Mnt.f_EISourceTempSet = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempMnt = DecodeFloat(ReceData+uin_Index); //EI离子源温度（校准后）监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempMntOriginal = DecodeFloat(ReceData+uin_Index); //EI离子源温度（校准前）监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempCaliK = DecodeFloat(ReceData+uin_Index); //校准参数K
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempCaliB = DecodeFloat(ReceData+uin_Index); //校准参数B
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempKp = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempKi = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempKd = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceTempLimit = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EISourceOut = DecodeFloat(ReceData+uin_Index); //输出
    uin_Index += 4;
    g_clEI.st_Mnt.uch_EISourceTempHeatMode = ReceData[uin_Index];
    uin_Index += 1;
    g_clEI.st_Mnt.uch_EISourceErrRange = ReceData[uin_Index];
    uin_Index += 1;
    g_clEI.st_Mnt.uch_EISourceErrHeatSpeed = ReceData[uin_Index];
    uin_Index += 1;
    g_clEI.st_Mnt.uch_EISourceErrReachTar = ReceData[uin_Index];
    uin_Index += 1;

    //GC Interface 控温信息
    g_clEI.st_Mnt.uch_EIGCInterfaceTempEn = ReceData[uin_Index]; //控温使能
    uin_Index += 1;
    g_clEI.st_Mnt.f_EIGCInterfaceTempSet = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempMnt = DecodeFloat(ReceData+uin_Index); //EI离子源温度（校准后）监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempMntOriginal = DecodeFloat(ReceData+uin_Index); //EI离子源温度（校准前）监控
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempCaliK = DecodeFloat(ReceData+uin_Index); //校准参数K
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempKp = DecodeFloat(ReceData+uin_Index); //校准参数B
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempKp = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempKi = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempKd = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceTempLimit = DecodeFloat(ReceData+uin_Index);
    uin_Index += 4;
    g_clEI.st_Mnt.f_EIGCInterfaceOut = DecodeFloat(ReceData+uin_Index); //输出
    uin_Index += 4;
    g_clEI.st_Mnt.uch_EIGCInterfaceTempHeatMode = ReceData[uin_Index];
    uin_Index += 1;
    g_clEI.st_Mnt.uch_EIGCInterfaceErrRange = ReceData[uin_Index];
    uin_Index += 1;
    g_clEI.st_Mnt.uch_EIGCInterfaceErrHeatSpeed = ReceData[uin_Index];
    uin_Index += 1;
    g_clEI.st_Mnt.uch_EIGCInterfaceErrReachTar = ReceData[uin_Index];
    uin_Index += 1;


    g_clEI.st_Mnt.uch_FilamentIO = ReceData[uin_Index]; //灯丝切换开关
    uin_Index += 1;
    g_clEI.st_Mnt.uch_Q0HighVIO = ReceData[uin_Index]; //Q0高压开关
    uin_Index += 1;



    *uip_SendDataLenth = 0;
    return 0;

}
////==================================================================================================
////| 函数名称 | Comm52EIWrite()
////|------------------------------------------------------------------------------------------------
////| 函数功能 | 设置控温模式
////|------------------------------------------------------------------------------------------------
////| 输入参数 | INT8U* ReceData,INT8U* SendData
////|------------------------------------------------------------------------------------------------
////| 返回参数 | INT8U 错误码
////|------------------------------------------------------------------------------------------------
////| 函数设计 | lyb,2020.03.24
////==================================================================================================
//static INT8U Comm52EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
//{

//    return 0;
//}
////==================================================================================================
////| 函数名称 | Comm52EIWriteAck()
////|------------------------------------------------------------------------------------------------
////| 函数功能 | 设置控温模式反馈
////|------------------------------------------------------------------------------------------------
////| 输入参数 | INT8U* ReceData,INT8U* SendData
////|------------------------------------------------------------------------------------------------
////| 返回参数 | INT8U 错误码
////|------------------------------------------------------------------------------------------------
////| 函数设计 | lyb,2020.03.24
////==================================================================================================
//static INT8U Comm52EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
//{
//    *uip_SendDataLenth = 0;
//    return 0;
//}
////==================================================================================================
////| 函数名称 | Comm53EIWrite()
////|------------------------------------------------------------------------------------------------
////| 函数功能 | 设置温控参数
////|------------------------------------------------------------------------------------------------
////| 输入参数 | INT8U* ReceData,INT8U* SendData
////|------------------------------------------------------------------------------------------------
////| 返回参数 | INT8U 错误码
////|------------------------------------------------------------------------------------------------
////| 函数设计 | lyb,2020.03.24
////==================================================================================================
//static INT8U Comm53EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
//{

//    return 0;
//}
////==================================================================================================
////| 函数名称 | Comm53EIWriteAck()
////|------------------------------------------------------------------------------------------------
////| 函数功能 | 设置温控参数反馈
////|------------------------------------------------------------------------------------------------
////| 输入参数 | INT8U* ReceData,INT8U* SendData
////|------------------------------------------------------------------------------------------------
////| 返回参数 | INT8U 错误码
////|------------------------------------------------------------------------------------------------
////| 函数设计 | lyb,2020.03.24
////==================================================================================================
//static INT8U Comm53EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
//{
//    *uip_SendDataLenth = 0;
//    return 0;
//}
////==================================================================================================
////| 函数名称 | Comm71EIWrite()
////|------------------------------------------------------------------------------------------------
////| 函数功能 | 设置温控参数
////|------------------------------------------------------------------------------------------------
////| 输入参数 | INT8U* ReceData,INT8U* SendData
////|------------------------------------------------------------------------------------------------
////| 返回参数 | INT8U 错误码
////|------------------------------------------------------------------------------------------------
////| 函数设计 | lyb,2020.03.24
////==================================================================================================
//static INT8U Comm71EIWrite(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
//{

//    return 0;
//}
////==================================================================================================
////| 函数名称 | Comm71EIWriteAck()
////|------------------------------------------------------------------------------------------------
////| 函数功能 |设置温控参数返回
////|------------------------------------------------------------------------------------------------
////| 输入参数 | INT8U* ReceData,INT8U* SendData
////|------------------------------------------------------------------------------------------------
////| 返回参数 | INT8U 错误码
////|------------------------------------------------------------------------------------------------
////| 函数设计 | lyb,2020.03.24
////==================================================================================================
//static INT8U Comm71EIWriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
//{
//    return 0;
//}

//--------------------------Q0Driver板-------------------------


//==================================================================================================
//| 函数名称 | Comm23Q0Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置开关量
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm23Q0Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    SendData[0] = 0;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm23Q0WriteAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置开关量反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm23Q0WriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    return 0;
}
//==================================================================================================
//| 函数名称 | Comm24Q0Write()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置电压
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm24Q0Write(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    SendData[0] = WRITE_FAILED;
    return 0;
}
//==================================================================================================
//| 函数名称 | Comm24EIWriteAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 设置电压反馈
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm24Q0WriteAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{

    return 0;
}


//| 函数名称 | Comm31Q0ReadAck()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 数据监控
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|------------------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.03.24
//==================================================================================================
static INT8U Comm31Q0ReadAck(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT16U uin_Index = 0;
    a_ComFpi.m_bQ0Driver31ReadCommFlag = 1;
    g_clQ0Driver.st_Mnt.uch_SourceSelect = ReceData[uin_Index];
    uin_Index += 1;
    uin_Index += 3;//预留
    g_clQ0Driver.st_Mnt.f_Q0UpVLowSet= DecodeFloat(ReceData+uin_Index); //Q0上邻杆低压设置
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0UpVLowMnt = DecodeFloat(ReceData+uin_Index); //Q0上邻杆低压监控
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0LowVLowSet = DecodeFloat(ReceData+uin_Index);//Q0下邻杆低压设置
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0LowVLowMnt = DecodeFloat(ReceData+uin_Index); //Q0上邻杆低压监控
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_DDSOutSet = DecodeFloat(ReceData+uin_Index); //Q0DDS输出
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0VMnt = DecodeFloat(ReceData+uin_Index); //Q0反馈电压
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0IMnt = DecodeFloat(ReceData+uin_Index); //Q0反馈电流
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0DriverVMnt = DecodeFloat(ReceData+uin_Index); //Q0驱动电压
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0Power = DecodeFloat(ReceData+uin_Index);   //Q0板电源
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.f_Q0BoardTemp = DecodeFloat(ReceData+uin_Index);    //Q0板温度
    uin_Index += 4;
    g_clQ0Driver.st_Mnt.uch_Q0AutoTuningIOSet = ReceData[uin_Index]; //自动调谐开关设置
    uin_Index += 1;
    g_clQ0Driver.st_Mnt.uch_Q0AutoTuningIOMnt = ReceData[uin_Index]; //自动调谐开关读取
    uin_Index += 1;
    g_clQ0Driver.st_Mnt.uch_Q0Relay11 = ReceData[uin_Index];      //Q0电压切换继电器11
    uin_Index += 1;
    g_clQ0Driver.st_Mnt.uch_Q0Relay12 = ReceData[uin_Index];      //Q0电压切换继电器12
    uin_Index += 1;
    g_clQ0Driver.st_Mnt.uch_Q0Relay21 = ReceData[uin_Index];      //Q0电压切换继电器21
    uin_Index += 1;
    g_clQ0Driver.st_Mnt.uch_Q0Relay22 = ReceData[uin_Index];      //Q0电压切换继电器22
    uin_Index += 1;
    g_clQ0Driver.st_Mnt.uch_Q0DCRelay1 = ReceData[uin_Index];     //Q0DC电压切换继电器1
    uin_Index += 1;
    g_clQ0Driver.st_Mnt.uch_Q0DCRelay2 = ReceData[uin_Index];     //Q0DC电压切换继电器2
    uin_Index += 1;

    *uip_SendDataLenth = 0;
    return 0;
}


//==================================================================================================
//| 函数名称 | CAppComFpi
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | CComFpi够造函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
CAppComFpi::CAppComFpi()
{
    b_ModEnable = FALSE;
    b_FirstTime = TRUE;
    m_bCircu31ReadCommFlag = 0;
}
//==================================================================================================
//| 函数名称 | ~CAppComFpi()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | ~CAppComFpi
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | yufei,11.02.24
//==================================================================================================
CAppComFpi::~CAppComFpi()
{

    //在此销毁类成员(变量)
}
//==================================================================================================
//| 函数名称 | SetModEnable()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 模块使能设置
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
 BOOL CAppComFpi::SetModEnable(BOOL b_Set)
 {

   //模块输入变量初始化
    b_ModEnable = b_Set;      //模块使能标志


    return TRUE;
 }
//==================================================================================================
//| 函数名称 | CommFpiInit()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 命令解析
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
void CAppComFpi::CommFpiInit(void)
{
    INT8U i ;
    STRFPIINSTALL st_FpiInstall;
    //设置转发表格信息
    SetComPortTabSize(sizeof(au_ComPortTab));
    SetComPortTab((STRTRANPORT *)au_ComPortTab);

    //Dest清零(防止写入异常值)
    for(INT8U i=0;i<DEST_MAX;i++)
    {
        st_FpiInstall.st_Dest[i].uch_DestAddr = 0;
        st_FpiInstall.st_Dest[i].p_Tab = (STRFPIDATA *)0;
        st_FpiInstall.st_Dest[i].uin_TabLen = 0;
    }

//    st_FpiInstall.uch_SourAddr =ADDR_MAIN ;
//    st_FpiInstall.ReceFifoP = Uart1RxChar;
//    st_FpiInstall.SendFifoP = Uart1TxChar;
//    st_FpiInstall.SendEnableP = Uart1SendEnable;
//    st_FpiInstall.pull_CanId = (INT32U *)0;
//    st_FpiInstall.st_Dest[0].uch_DestAddr = ADDR_VACU;
//    st_FpiInstall.st_Dest[0].p_Tab = (STRFPIDATA *)au_PCFpi;
//    st_FpiInstall.st_Dest[0].uin_TabLen = sizeof(au_PCFpi);
//    st_FpiInstall.b_ModEnable = TRUE;
//    gm_Uart1Fpi.Install(&st_FpiInstall);
//    gm_Uart1Fpi.QThread::start();//启动线程


    st_FpiInstall.uch_SourAddr =ADDR_MAIN ;
    st_FpiInstall.ReceFifoP = Can0RxChar;
    st_FpiInstall.SendFifoP = Can0TxChar;
    st_FpiInstall.SendEnableP = Can0SendEnable;
    st_FpiInstall.pull_CanId = new INT32U();
    st_FpiInstall.st_Dest[0].uch_DestAddr = ADDR_CIRCU;
    st_FpiInstall.st_Dest[0].p_Tab = (STRFPIDATA *)au_CircuFpi;
    st_FpiInstall.st_Dest[0].uin_TabLen = sizeof(au_CircuFpi);
    st_FpiInstall.st_Dest[1].uch_DestAddr = ADDR_EI;
    st_FpiInstall.st_Dest[1].p_Tab = (STRFPIDATA *)au_EIFpi;
    st_FpiInstall.st_Dest[1].uin_TabLen = sizeof(au_EIFpi);
    st_FpiInstall.st_Dest[2].uch_DestAddr = ADDR_Q0DRIVER;
    st_FpiInstall.st_Dest[2].p_Tab = (STRFPIDATA *)au_Q0DriverFpi;
    st_FpiInstall.st_Dest[2].uin_TabLen = sizeof(au_Q0DriverFpi);
    st_FpiInstall.b_ModEnable = TRUE;
    gm_Can0Fpi.Install(&st_FpiInstall);
    i = GetFreethreadID();
    if(i!=0xff)

    cPubQthread[i].Runtask = Can0TxProcess;

    cPubQthread[i].start();

    i = GetFreethreadID();
    if(i!=0xff)

    cPubQthread[i].Runtask = Can0RxProcess;

    cPubQthread[i].start();


    st_FpiInstall.uch_SourAddr =ADDR_MAIN ;
    st_FpiInstall.ReceFifoP = TCPRxChar1;
    st_FpiInstall.SendFifoP = TCPTxChar1;
    st_FpiInstall.SendEnableP = TCPSendEnable1;
    st_FpiInstall.pull_CanId = new INT32U();
    st_FpiInstall.st_Dest[0].uch_DestAddr = ADDR_PC;
    st_FpiInstall.st_Dest[0].p_Tab = (STRFPIDATA *)au_PCFpi;
    st_FpiInstall.st_Dest[0].uin_TabLen = sizeof(au_PCFpi);
    st_FpiInstall.b_ModEnable = TRUE;
    gm_TcpFpi1.Install(&st_FpiInstall);

    st_FpiInstall.uch_SourAddr =ADDR_MAIN ;
    st_FpiInstall.ReceFifoP = TCPRxChar2;
    st_FpiInstall.SendFifoP = TCPTxChar2;
    st_FpiInstall.SendEnableP = TCPSendEnable2;
    st_FpiInstall.pull_CanId = new INT32U();
    st_FpiInstall.st_Dest[0].uch_DestAddr = ADDR_PC;
    st_FpiInstall.st_Dest[0].p_Tab = (STRFPIDATA *)au_PCFpi;
    st_FpiInstall.st_Dest[0].uin_TabLen = sizeof(au_PCFpi);
    st_FpiInstall.b_ModEnable = TRUE;
    gm_TcpFpi2.Install(&st_FpiInstall);

    st_FpiInstall.uch_SourAddr =ADDR_MAIN ;
    st_FpiInstall.ReceFifoP = TCPRxChar3;
    st_FpiInstall.SendFifoP = TCPTxChar3;
    st_FpiInstall.SendEnableP = TCPSendEnable3;
    st_FpiInstall.pull_CanId = new INT32U();
    st_FpiInstall.st_Dest[0].uch_DestAddr = ADDR_PC;
    st_FpiInstall.st_Dest[0].p_Tab = (STRFPIDATA *)au_PCFpi;
    st_FpiInstall.st_Dest[0].uin_TabLen = sizeof(au_PCFpi);
    st_FpiInstall.b_ModEnable = TRUE;
    gm_TcpFpi3.Install(&st_FpiInstall);


    i = GetFreethreadID();


    if(i!=0xff)
     //tcp服务器监控
    cPubQthread[i].Runtask = TCPAcceptProcess;

    cPubQthread[i].start();

    i = GetFreethreadID();
    if(i!=0xff)
     //tcp发送
    cPubQthread[i].Runtask = TCPsendProcess;

    cPubQthread[i].start();


    i = GetFreethreadID();
    if(i!=0xff)
     //串口收发
    cPubQthread[i].Runtask = Uart1Process;

    cPubQthread[i].start();
    return;
}


//==================================================================================================
//| 函数名称 | Process()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 |
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
void CAppComFpi::Process(void)
{
    //STRFPISEND st_FpiSend;
    if(b_FirstTime == TRUE)
    {
       CommFpiInit();
       //Set(100);
       b_FirstTime = FALSE;
    }
    if(FALSE==b_ModEnable)
    {
        return;
    }

    //以太网fpi通信
    gm_TcpFpi1.Process();

    gm_TcpFpi2.Process();
    gm_TcpFpi3.Process();
    //CAN通信处理
    gm_Can0Fpi.Process();

}


// no more
