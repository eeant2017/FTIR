//==================================================================================================
//| 文件名称 | Pub_ComFpi.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Pub_ComFpi.cpp所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2011, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2011.01.24  |  ygm      | 初版
//==================================================================================================

#ifndef PUB_COMFPI_H
#define PUB_COMFPI_H

#include "typedefine.h"
#include "Pub_Tmr.h"
#include "Pub_Queue.h"

#include <QThread>
#ifdef __cplusplus
extern "C" {
#endif

#define N_NUM 0x5a5a
#define N_MAX_RPONTER  6           //最多8级路由
#define N_COMMU_BUF    12000u         //数据缓冲区
//COM协议标志宏定义
#define RECIEVED     		0x11
#define SENDING     		0x22
#define COM_READY_FOR_SEND 	0x33
#define COM_IDLE 			0x44

//=========================== Macro,Typestruct, & Class ===================//
#define READ                0x55
#define WRITE               0x66
#define READ_ANSWER         0xAA
#define WRITE_ANSWER        0x99

//#define READ_FAILED         0x99
//#define WRITE_FAILED        0x99
#define READ_FAILED        	0xAA
#define WRITE_FAILED        0xAA
//#define WRITE_SUCCESS       0x88
#define WRITE_SUCCESS       0x99
#define WRITE_FAULT         0xAA
//#define LENTH_VOLATILE      0xFFFF
#define READUPDATA          0x77
#define WRITEUPDATA         0x88
#define READUPDATA_ANSWER   0xCC
#define WRITEUPDATA_ANSWER  0xDD

#ifndef CTYPE_FUNC
#define CTYPE_FUNC 0
#endif

#ifndef CTYPE_BOOL
#define CTYPE_BOOL 1
#endif

#ifndef CTYPE_INT8U
#define CTYPE_INT8U 2
#endif

#ifndef CTYPE_INT8S
#define CTYPE_INT8S 3
#endif

#ifndef CTYPE_INT16U
#define CTYPE_INT16U 4
#endif

#ifndef CTYPE_INT16S
#define CTYPE_INT16S 5
#endif

#ifndef CTYPE_INT32U
#define CTYPE_INT32U 6
#endif

#ifndef CTYPE_INT32S
#define CTYPE_INT32S 7
#endif

#ifndef CTYPE_FP32
#define CTYPE_FP32 8
#endif


#define REF_DATA 0x7f
#define REF_REVS 0x80
#define LIMIT 0xff|REF_REVS
#define NO_LIMIT 0xff

#define NO_SAVE	0xffffffff
#define NO_USE  0x0
//=========================== Macro,Typestruct, & Class ===================//
//最大的子板个数，根据52259的can的MB为16个，主动发送用掉1个，剩下最多为15子板
#define DEST_MAX	5

#define COMM_QUEUE_MAX	5100

typedef  INT8U  (*CommExplainFunction)(INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);

typedef struct
{
    INT8U   buf[N_COMMU_BUF];
    INT8U   uch_Status;          //端口状态标志
    INT16U  uin_HaveRecBytes;    //接收时有效
    INT16U  uin_HaveTransBytes;  //发送时有效
    INT16U  uin_TotalTransBytes; //发送时有效
}COMMUNICATE;

typedef struct
{
    //数据帧起始标志位
    INT8U  auch_FrameStart[2];
    //目标地址域
    INT8U  uch_SrcLen;
    INT8U  uch_SrcBuf[N_MAX_RPONTER];
    //源地址域
    INT8U  uch_DestLen;
    INT8U  uch_DestBuf[N_MAX_RPONTER];
    //数据域
    INT8U  uch_Cmd;           //命令码
    INT8U  uch_AddCmd;        //附加命令码
    INT16U uin_DataLen;       //数据区长度
    INT8U  *puch_Data;        //路由地址列表指针
    //CRC校验
    INT16U uin_Crc;
    //数据帧结束标志位
    INT8U  auch_FrameEnd[2];
}FRAMESTRUCT;

typedef struct
{
    INT8U uch_Cmd;					//命令码
    INT8U uch_AddCmd;				//扩展码
    INT16U uin_ReceDataLen;         //接受长度
    INT8U uch_DataNum;				//同一数据类型个数
    CommExplainFunction p_Data;     //变量地址指针
    INT8U uch_DataType;  			//数据类型
    INT32U ul_Min;                  //最大值
    INT32U ul_Max;                  //最小值
    INT32U ul_Default;				//默认值
    INT32U  ul_VariableAddr;  //变量存储地址
}STRFPIDATA;

typedef struct
{
    INT8U uch_DestAddr;		//目标地址
    STRFPIDATA * p_Tab;		//目标子板对应的表格地址
    INT16U uin_TabLen;		//目标子板对应的表格长度
}STRFPIDEST;

typedef struct
{
    BOOL   b_ModEnable;
    INT8U uch_SourAddr;
    INT32U *pull_CanId;
    UCH_PFUN_PUCH ReceFifoP;
    UCH_PFUN_UCH SendFifoP;
    VOID_PFUN_VOID SendEnableP;
    STRFPIDEST st_Dest[DEST_MAX];
}STRFPIINSTALL;

typedef struct
{
    INT8U uch_DestAddr;  			//目标地址
    INT8U uch_Cmd;						//命令码
    INT8U uch_AddCmd;					//扩展码
    INT8U uch_DataLen;				//数据长度
    INT8U puch_Data[100];			//数据地址
}STRFPISEND;

//=========================== Macro,Typestruct, & Class ===================//
class CComFpi : public QThread
{
public:
//------------------------------变量定义--------------------------//
  BOOL   b_ModEnable;                             //模块总使能
  CQueue CommandFifo;
//------------------------------函数定义--------------------------//
  //构造函数和析构函数
  CComFpi();
  ~CComFpi();

  //模块输入<<<
  BOOL Install(STRFPIINSTALL* pst_Data);
  BOOL PushComm(STRFPISEND * pst_Data);
  BOOL b_AckOk;
   VOID_PFUN_VOID Runtask;
   void Process(void);

  void run();
private:
    //被动通信
    //Variable

  COMMUNICATE st_FpiRx;
  COMMUNICATE st_FpiTx;
  INT8U uch_SourAddr;
  INT32U *pull_CanStdId;
  UCH_PFUN_PUCH ReceFifoP;
  UCH_PFUN_UCH SendFifoP;
  VOID_PFUN_VOID SendEnableP;

    STRFPIDATA *FpiDataTabP;
    STRFPIDEST st_Dest[DEST_MAX];
    INT16U uch_FpiDataTabSize;
    INT8U uch_RxThrowFlag;
    //Function
    BOOL CheckFrameCrc(void);
    BOOL SearchDest(INT8U uch_DestAddr);
    INT8U SearchFpiDataTab(INT8U uch_Cmd,INT8U uch_AddCmd);
    INT8U CommComRead(INT8U uch_Index,INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
    INT8U CommComWrite(INT8U uch_Index,INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth);
    BOOL  GetAddrFromRxFrame(FRAMESTRUCT *pst_Frame);
  BOOL  SendFrameData(void);
  BOOL  GetFrameFromQue(void);
  BOOL  ConstructFrame(FRAMESTRUCT *pst_Frame);
  void  DecodeFrameAndDoIt(void);

  BOOL  SendFrameToOther(void);
  INT8U SearchComPort(INT8U uch_Buf);
  //Variable
    STRFPISEND st_Send;
    INT8U  auch_CommandBuf[COMM_QUEUE_MAX];
    INT8U uch_ResendMax;
    INT8U uch_SendCnt;

    INT32U ul_ResendTime;
    CTmr ResendCyc;
    void  SendFrameActice(INT8U uch_DestAddr,INT8U uch_Cmd, INT8U uch_AddCmd,INT8U *p);
    BOOL  PopComm(STRFPISEND * pst_Data);
    void HostProcess(void);
    void SlaveProcess(void);
    BOOL b_resendflag;


    BOOL SendFrameToOtherProcess(void);
    INT8U b_OtherAckOk; //转发
    INT8U b_OtherSendHost; //转发属于主发

    INT8U b_LockSend; //发送锁 主动发送和转发都需要
    INT32U ul_OtherResendTime; //转发重发超时
    CTmr OtherResendCyc;  //转发流程超时
    INT8U uch_StateOther; //转发流程状态
    INT8U uch_StateHost;  //主发流程状态



//private slots:
//  void ResendProcess(void);

};

typedef struct
{
    INT8U   uch_TranAddr;//转发地址

    CComFpi  *strp_ComPort; //通信端口
}STRTRANPORT;

//========================= Variable,Object & Fuction =====================//
//声明模块类对象

extern BOOL SetComPortTab(STRTRANPORT *pst_Data);
extern BOOL SetComPortTabSize(INT8U uch_Data);



#ifdef __cplusplus
}
#endif

#endif

