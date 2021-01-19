//==================================================================================================
//| 文件名称 | pub_MKSVacuometer.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | pub_MKSVacuometer.cpp所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2019, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2019.10.22  |  LYB       | 初版
//==================================================================================================

#ifndef PUB_MKSVacuometer_V1_0_H
#define PUB_MKSVacuometer_V1_0_H

//包含类型宏定义

#include "typedefine.h"
#include "Pub_MyBus.h"
#include "Pub_Tmr.h"
#include "Pub_Queue.h"
//----------------------------------------Cplusplus-----------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif

#define MKSVacuometer_COM_TIMEOUT 1000 //MFC回复超时
#define N_MKSVacuometer_COMMU_BUF 40
#define MKSVacuometer_1COM_BUF 40
typedef struct
{
    INT8U   buf[N_MKSVacuometer_COMMU_BUF];          //数据缓存
    INT8U   uch_Status;          //端口状态标志
    INT16U  uin_HaveRecBytes;
    INT16U  uin_HaveTransBytes;
    INT16U  uin_TotalTransBytes;
    INT16U  uin_DataLen;
}STRMKSVacuometerCOMMUNICATE; //通信命令结构体

typedef struct
{
    BOOL b_ModEnable;
    INT8U uch_SourAddr;
    BOOL* pb_ComLock;
    UCH_PFUN_PUCH ReceFifoP;
    UCH_PFUN_UCH SendFifoP;
    VOID_PFUN_VOID SendEnableP;
}STRMKSVacuometerINSTALL;  //通信接口装载

//COM协议标志宏定义
#define MKSVacuometer_RECIEVED     		0x11
#define MKSVacuometer_SENDING     		0x22
//#define COM_READY_FOR_SEND 	0x33
#define MKSVacuometer_COM_IDLE 			0x44

//=========================== Macro,Typestruct, & Class ===================//
class CComMKSVacuometer
{
public:
    CComMKSVacuometer ();
    ~CComMKSVacuometer ();
public:
    BOOL ComMKSVacuometerSetModEnable(BOOL b_Set);
    BOOL ComMKSVacuometerInstall(STRMKSVacuometerINSTALL* pst_Data);
    void ComMKSVacuometerSlaveProcess(void);
public:
    void SendFrameData(INT8U* uch_Data,INT16U uin_SendLen,INT8U uch_Addr);
    BOOL GetFrameFromCOM();
    void FrameDeal();
    FP32 CountF(INT8U* uch_Data,INT8U uch_Len);

    void ClearComBackFlag(void){m_bComBack=FALSE;}
    BOOL GetComBackFlag(void){return m_bComBack;}
    void LockCom(void){(*m_pbComLock)=TRUE;m_bFrameGetEnable=TRUE;}
    void UnLockCom(void){(*m_pbComLock)=FALSE;m_bFrameGetEnable=FALSE;}
    BOOL GetComLock(void){return (*m_pbComLock);}
    STRMKSVacuometerCOMMUNICATE st_NoFpiRev;

    INT16U m_uinVacuumRead;
    FP32 m_fVacuum;

 private:
    BOOL b_ModEnable;
    BOOL b_FirstRunFlag;    //模块总使能
    BOOL m_bComBack;
    BOOL* m_pbComLock;
    BOOL m_bFrameGetEnable;
    UCH_PFUN_PUCH  ReceFifoP;
    UCH_PFUN_UCH   SendFifoP;
    VOID_PFUN_VOID SendEnableP;
    INT8U uch_SourAddr;
 private:

    INT8U m_uchSendAddr;

};

//MFC控制类
#define MKSVacuometer_OPRATION_NULL 0
#define MKSVacuometer_OPRATION_READ_Vacuum 1

#define MKSVacuometer_CMD_READ_Vacuum 1


class CMKSVacuometerCtrl : public CTmr
{
public:
    BOOL m_bModEnable;      //模块总使能
    BOOL m_bFirstRunFlag;   //模块第一次运行
    CMKSVacuometerCtrl();
    CMKSVacuometerCtrl(INT8U uch_Addr);
    ~CMKSVacuometerCtrl();
    void MKSVacuometerCtrlConnectCom(CComMKSVacuometer * pcl_MKSVacuometer);
    void MKSVacuometerCtrlProcess(void);
    void MKSVacuometerCtrlReadVacuumTriggerProcess(void);
    void MKSVacuometerCtrlReadVacuum(void);
    void MKSVacuometerCtrlReadVacuumTrigger();

    BOOL IsMKSVacuometerOpraionNull(void);

    INT8U ConstructFrame(INT8U uch_Addr,INT8U uch_Cmd);
    void FlowFinish(void);
public:

    INT8U m_bErrCom;
    FP32 m_fVacuum;



    INT32U m_ulCommCnt;
    INT32U m_ulCommErrCnt;

private:
    CComMKSVacuometer* m_pclMKSVacuometer;
    INT8U m_uchOpration;
    INT8U m_uchSendCnt;
    INT8U m_uchResendMax;
    INT8U m_uchAddr;
    INT8U m_auchSendBuf[MKSVacuometer_1COM_BUF];
    INT16U m_uinSendDataLen;
    INT8U m_uchState;
    BOOL m_bTmrOutFlag;

private:

    INT8U m_uchStateReadVacuumTrigger;
    BOOL m_bReadVacuumSig;



};
void MKSVacuometerProcess(void);
void MKSVacuometerInit(void);
extern CMKSVacuometerCtrl g_clMKSVacuometerDevice1;

#ifdef __cplusplus
}
#endif


#endif

//no more
