
//==================================================================================================
//| 文件名称 | Pub_ComFpi.cpp
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 |
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 | 所有C/C++语言编译环境，包括单片机编译器
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2010, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2010.01.26  |  ygm      | 初版
//==================================================================================================
#include "Pub_ComFpi.h"
#include "Pub_MyBus.h"
#include "Bsp/Bsp_TCP.h"
#include <QDebug>

extern void vMemDump(INT8U *pData, INT16U DataLen);

//定义通信模块
static INT8U uch_ComPortTabSize;
static STRTRANPORT *ComPortTabP;
//定义地址和转发接口表

//==================================================================================================
//| 函数名称 | SetComPortTab()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 设置系统的通信接口和对应的通信地址
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | STRTRANPORT *pst_Data 通信接口表地址
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
BOOL SetComPortTab(STRTRANPORT *pst_Data)
{
    if(NULL == pst_Data)
    {
        return FALSE;
    }

    ComPortTabP = pst_Data;

    return TRUE;
}

//==================================================================================================
//| 函数名称 | SetComPortTabSize()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 设置转发通信接口表格大小
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U uch_Data:表格大小
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
BOOL SetComPortTabSize(INT8U uch_Data)
{
    if(uch_Data > 0xff)
    {
        return FALSE;
    }

    uch_ComPortTabSize = uch_Data;

    return TRUE;
}

//==================================================================================================
//| 函数名称 | SearchComPort()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 寻找转发接口
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 转发地址
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.02.13
//==================================================================================================
INT8U CComFpi::SearchComPort(INT8U uch_Buf)
{
    INT8U i,j;

    j = uch_ComPortTabSize/sizeof(STRTRANPORT);

    for(i=0;i<j;i++)
    {
        if(ComPortTabP[i].uch_TranAddr == uch_Buf)
        {
            break;
        }
    }

    if(i == j)
    {
        i = 0xff;
    }

    return i;
}

//==================================================================================================
//| 函数名称	 | SendFrameToOtherProcess()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能	 | 主动转发需要等待转发反馈，接收到反馈或超时后协议解析才会继续。被动转发不需要反馈
//|----------|--------------------------------------------------------------------------------------
//| 输入参数	 | pRev:   指向COMMUNICATE数据类型的指针(接收)
//|          | pst_Tx: 指向COMMUNICATE数据类型的指针(发送)
//|----------|--------------------------------------------------------------------------------------
//| 返回参数	 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计	 | gao,08.4.10
//|----------|--------------------------------------------------------------------------------------
//| 函数修改	 | lyb,2020.03.17
//==================================================================================================
BOOL CComFpi::SendFrameToOtherProcess(void)
{
        COMMUNICATE *pst_Rx = &st_FpiRx;
        COMMUNICATE *pst_Tx = &st_FpiTx;
        INT8U uch_Index;
        uch_Index = SearchComPort(pst_Rx->buf[4]);

        //--qDebug("SendFrameToOtherProcess\n");
        if(ComPortTabP[uch_Index].strp_ComPort->b_OtherSendHost == 0) //如果转发的类是属与非主动转发就将当前的通信类作为主动转发
        {
                //--qDebug("SendFrameToOtherProcess--128\n");
                if(uch_StateOther == 0)
                {
                        //--qDebug("SendFrameToOtherProcess--131\n");
                        if(ComPortTabP[uch_Index].strp_ComPort->b_LockSend == 0)
                        {
                                uch_StateOther =1;
                                ComPortTabP[uch_Index].strp_ComPort->b_LockSend = 1;//锁

                                b_OtherAckOk = 0;
                                SendFrameToOther();
                                OtherResendCyc.Set(ul_OtherResendTime);
                                qDebug("send to other dest %x\r\n",st_Dest[0].uch_DestAddr);

                                b_OtherSendHost = 1;
                        }
                }
                else
                {
                        //--qDebug("SendFrameToOtherProcess--147\n");
                        if(b_OtherAckOk == 1)
                        {
                                b_OtherAckOk = 0;
                                uch_StateOther = 0;
                                ComPortTabP[uch_Index].strp_ComPort->b_LockSend = 0;//释放锁
                                pst_Rx->uch_Status=COM_IDLE;
                                pst_Rx->uin_HaveRecBytes=0;
                                qDebug("send to other ok----------  dest %x\r\n",st_Dest[0].uch_DestAddr);
                                b_OtherSendHost = 0;
                        }
                        else if(OtherResendCyc.TmrOut())
                        {
                                uch_StateOther = 0;
                                ComPortTabP[uch_Index].strp_ComPort->b_LockSend = 0;//释放锁
                                b_OtherAckOk = 0;
                                qDebug("send to other tmrout-----  dest %x\r\n",st_Dest[0].uch_DestAddr);
                                pst_Rx->uch_Status=COM_IDLE;
                                pst_Rx->uin_HaveRecBytes=0;
                                b_OtherSendHost = 0;
                                return TRUE;
                        }
                }

        }
        else
        {
                if(SendFrameToOther() == TRUE)
                {
                        ComPortTabP[uch_Index].strp_ComPort->b_OtherAckOk = 1;
                }

                b_OtherSendHost = 0;
                qDebug("send to other -----  dest %d\r\n",st_Dest[0].uch_DestAddr);
                pst_Rx->uch_Status=COM_IDLE;
                pst_Rx->uin_HaveRecBytes=0;

        }

}

//==================================================================================================
//| 函数名称 | SendFrameToOther()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数据帧转发
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pRev:   指向COMMUNICATE数据类型的指针(接收)
//|          | pst_Tx: 指向COMMUNICATE数据类型的指针(发送)
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | gao,08.4.10
//|----------|--------------------------------------------------------------------------------------
//| 函数修改 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::SendFrameToOther(void)
{
    COMMUNICATE *pRev = &st_FpiRx;
    //COMMUNICATE *pst_Tx = &st_FpiTx;
    COMMUNICATE *pTransOther = (COMMUNICATE *)0;//NULL;
    INT16U k,k1,k2;
    INT16U uin_Len;
    INT16U uin_CrcResult;
    INT8U uch_Index;


    //ASSERT((pRev != NULL) && (pst_Tx != NULL));

    //修改进行转发的端口
    uch_Index = SearchComPort(pRev->buf[6]);
    ComPortTabP[uch_Index].strp_ComPort = this;
    //qDebug("uch_Index %d strp_ComPort %,addr %d",uch_Index,this,pRev->buf[6]);





    //获取转发端口

    uch_Index = SearchComPort(pRev->buf[4]);




    if(0xff == uch_Index)
    {
        pRev->uch_Status = COM_IDLE;//防错处理
        return FALSE;
    }

    *pull_CanStdId = pRev->buf[4];//接收方滤波器地址


    pTransOther = &(ComPortTabP[uch_Index].strp_ComPort->st_FpiTx);

    //ASSERT(pTransOther != NULL);

    //等待对应发送缓冲区数据为空
    while (pTransOther->uch_Status != COM_IDLE);

    pTransOther->uch_Status = SENDING;

    //数据拷贝
    uin_Len = pRev->uin_HaveRecBytes;
    CopyDataFromAToB(pRev->buf,pTransOther->buf,uin_Len);

    //更改地址域
    k = 2;
    k1 = pRev->buf[2];										   //目标地址域长度
    k2 = pRev->buf[2+k1+1];									   //源地址域长度
    pTransOther->buf[k++] = k1 - 1;							   //新的目标地址域减1
    CopyDataFromAToB(pRev->buf+4,pTransOther->buf+k,k1-1);	   //拷贝新的目标地址域
    k += (k1 - 1);
    pTransOther->buf[k++] = k2 + 1;							   //新的源地址域加1
    pTransOther->buf[k++] = pRev->buf[3];					   //新的源地址域的首地址
    CopyDataFromAToB(pRev->buf+k1+4,pTransOther->buf+k,k2);	   //拷贝新的源地址域

    //重新计算CRC校验码
    uin_CrcResult = GetCrc16Bit(pTransOther->buf+2,uin_Len-6);
    EncodeUint(uin_CrcResult,pTransOther->buf+uin_Len-4);

    //置标志位
    pTransOther->uin_TotalTransBytes = uin_Len;
    pTransOther->uin_HaveTransBytes = 0;
//    pRev->uch_Status = COM_IDLE;
//    pRev->uin_HaveRecBytes = 0;

    //转发数据
    ComPortTabP[uch_Index].strp_ComPort->SendFrameData();

    return TRUE;
}


//==================================================================================================
//主动发送处理函数
//==================================================================================================

//==================================================================================================
//| 函数名称 | SendFrameActice()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 组帧数据并主发
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
void CComFpi::SendFrameActice(INT8U uch_DestAddr,INT8U uch_Cmd, INT8U uch_AddCmd, INT8U *p)
{
    FRAMESTRUCT frame;
    INT8U au_SendBuf[400];
    //INT8U ErrCode;
    INT8U uch_Index = 0;
    INT16U uin_SendDataLenth;//uin_ReceDataLenth,
    INT32U ul_TempP;
    CommExplainFunction stp_Func;

    frame.puch_Data = au_SendBuf;

    if(NULL == p)
    {
        return;
    }

    while (st_FpiTx.uch_Status != COM_IDLE);

    if(TRUE == SearchDest(uch_DestAddr))
    {
        uch_Index = SearchFpiDataTab(uch_Cmd,uch_AddCmd);
    }
    else
    {
        uch_Index = 0xff;
    }


    if(uch_Index != 0xff)
    {
        //*pull_CanStdId = uch_DestAddr;//接收方滤波器地址

        frame.uch_DestLen = 1;//目标地址1
        frame.uch_DestBuf[0] = uch_DestAddr;//目标地址
        frame.uch_SrcLen = 1;//源地址长度
        frame.uch_SrcBuf[0] = uch_SourAddr;//源地址

        frame.uch_Cmd = FpiDataTabP[uch_Index].uch_Cmd; //命令码
        frame.uch_AddCmd  = FpiDataTabP[uch_Index].uch_AddCmd;//扩展码;

        if(CTYPE_FUNC == FpiDataTabP[uch_Index].uch_DataType)
        {

            ul_TempP = (long int)FpiDataTabP[uch_Index].p_Data;
            stp_Func = (CommExplainFunction)ul_TempP;
            //ErrCode =
            stp_Func(p,frame.puch_Data,&uin_SendDataLenth);
        }
        else
        {

            //ErrCode =
      CommComRead(uch_Index,p,frame.puch_Data,&uin_SendDataLenth);
        }

        frame.uin_DataLen = uin_SendDataLenth;//数据长度

        ConstructFrame(&frame);

      SendFrameData();
    }
    else
    {
        st_FpiTx.uch_Status = COM_IDLE;
    }
}

//==================================================================================================
//| 函数名称 | PopCommFifo()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 组帧数据并主发
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::PushComm(STRFPISEND * pst_Data)
{
    if(NULL == pst_Data)
    {
        return FALSE;
    }

    if(TRUE == CommandFifo.PushQueue(pst_Data->uch_DestAddr))
    {
        CommandFifo.PushQueue(pst_Data->uch_Cmd);
        CommandFifo.PushQueue(pst_Data->uch_AddCmd);
        CommandFifo.PushQueue(pst_Data->uch_DataLen);
        for(int i=0;i<pst_Data->uch_DataLen;i++)
        {
            CommandFifo.PushQueue(pst_Data->puch_Data[i]);
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//==================================================================================================
//| 函数名称 | PopCommFifo()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 组帧数据并主发
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::PopComm(STRFPISEND * pst_Data)
{
    if(NULL == pst_Data)
    {
        return FALSE;
    }

    if(TRUE == CommandFifo.PopQueue(&pst_Data->uch_DestAddr))
    {
        CommandFifo.PopQueue(&pst_Data->uch_Cmd);
        CommandFifo.PopQueue(&pst_Data->uch_AddCmd);
        CommandFifo.PopQueue(&pst_Data->uch_DataLen);

        for(int i=0;i<pst_Data->uch_DataLen;i++)
        {
            CommandFifo.PopQueue(&pst_Data->puch_Data[i]);
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//==================================================================================================
//| 函数名称 | SendFrameActice()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 组帧数据并主发
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
void CComFpi::HostProcess(void)
{
        switch(uch_StateHost)
        {
                case 0:
                {
                        if(0 == b_LockSend) //等待锁空闲
                        {
                                if(TRUE == PopComm(&st_Send))
                                {
                                        b_LockSend = 1; //锁住
                                        uch_SendCnt = 0;
                                        uch_StateHost++;
                                }
                        }
                }break;
                case 1: //发送该帧
                {
                        SendFrameActice(st_Send.uch_DestAddr,st_Send.uch_Cmd,st_Send.uch_AddCmd,st_Send.puch_Data);
                        b_AckOk = 0;
                        ResendCyc.Set(ul_ResendTime);
                        uch_StateHost++;


                }break;
                case 2:
                {
                        if(b_AckOk)
                        {

                                b_LockSend = 0; //释放锁
                                uch_StateHost = 0;
                        }
                        else
                        {
                                if(ResendCyc.TmrOut())
                                {
                                        uch_SendCnt++;
                                        if(uch_SendCnt > uch_ResendMax)
                                        {
                                                uch_SendCnt = 0;
                                                b_LockSend = 0; //释放锁
                                                uch_StateHost = 0;

                                        }
                                        else
                                        {
                                                uch_StateHost = 1; //重发
                                        }
                                }
                        }
                }break;
                default:
                {
                        break;
                }
        }
}
//==================================================================================================
//| 函数名称 | Install()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 通信接口驱动安装
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | STRFPIINSTALL* pst_Data 驱动安装指针
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::Install(STRFPIINSTALL* pst_Data)
{
    if(NULL == pst_Data)
    {
        return FALSE;
    }

    //先进行合法性判断
    if((pst_Data->b_ModEnable > 1)\
    ||(pst_Data->uch_SourAddr > 0xff)\
    ||(NULL == pst_Data->ReceFifoP)\
    ||(NULL == pst_Data->SendFifoP)\
    ||(NULL == pst_Data->SendEnableP))
    {
        return FALSE;
    }

    //幅值操作
    b_ModEnable = pst_Data->b_ModEnable;

    uch_SourAddr= pst_Data->uch_SourAddr;
    pull_CanStdId = pst_Data->pull_CanId;

  ReceFifoP= pst_Data->ReceFifoP;
  SendFifoP= pst_Data->SendFifoP;
  SendEnableP= pst_Data->SendEnableP;

  //Runtask = pst_Data->Runtask;

  for(INT8U i=0;i<DEST_MAX;i++)
  {
      st_Dest[i].uch_DestAddr = pst_Data->st_Dest[i].uch_DestAddr;
      st_Dest[i].p_Tab = pst_Data->st_Dest[i].p_Tab;
      st_Dest[i].uin_TabLen = pst_Data->st_Dest[i].uin_TabLen;
  }

  for(INT8U i=0;i<DEST_MAX;i++)
  {
      pst_Data->st_Dest[i].uch_DestAddr = 0;
      pst_Data->st_Dest[i].p_Tab = (STRFPIDATA *)0;
      pst_Data->st_Dest[i].uin_TabLen = 0;
  }

  return TRUE;
}

//==================================================================================================
//| 函数名称 | CComFpi
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | CComFpi够造函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
CComFpi::CComFpi()
{
    //接收状态变量初始化
    st_FpiRx.uch_Status = COM_IDLE;
    st_FpiRx.uin_HaveRecBytes = 0;
    st_FpiRx.uin_HaveTransBytes = 0;
    st_FpiRx.uin_TotalTransBytes = 0;
    //发送状态变量初始化
    st_FpiTx.uch_Status = COM_IDLE;
    st_FpiTx.uin_HaveRecBytes = 0;
    st_FpiTx.uin_HaveTransBytes = 0;
    st_FpiTx.uin_TotalTransBytes = 0;
    uch_RxThrowFlag = 0;

    for(INT8U i=0;i<DEST_MAX;i++)//清零
    {
        st_Dest[i].uch_DestAddr = 0;
        st_Dest[i].p_Tab = (STRFPIDATA *)0;
        st_Dest[i].uin_TabLen = 0;
    }


    CommandFifo.SetQueue(auch_CommandBuf,COMM_QUEUE_MAX);//设置命令发送队列
    b_AckOk = 1;//默认开始已经回应
    uch_SendCnt = 0;//重发计数
    uch_ResendMax = 2;//重发次数为2次
    ul_ResendTime = 100;//重发间隔时间

    ul_OtherResendTime = 200;
    b_OtherAckOk = 0;
    b_LockSend = 0; //发送锁 主动发送和转发都需要
    uch_StateOther = 0; //转发流程状态
    uch_StateHost = 0;  //主发流程状态
}

//==================================================================================================
//| 函数名称 | ~CComFpi()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | ~CComFpi析构函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | yufei,11.02.24
//==================================================================================================
CComFpi::~CComFpi()
{
    //qDebug()<<"QT Demo:~CComFpi:";
    //在此销毁类成员(变量)
}

//==================================================================================================
//| 函数名称 | GetFrameFromQue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 从缓冲区中获取FPI帧
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | gao,08.4.10
//|----------|--------------------------------------------------------------------------------------
//| 函数修改 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::GetFrameFromQue(void)
{
    COMMUNICATE *pst_Rx = &st_FpiRx;
    INT8U uch_Data;

    //判断指针合法性
    //ASSERT((ReceFifoP != NULL) && (pst_Rx != NULL));

    if (ReceFifoP == NULL || pst_Rx == NULL)
    {
        return FALSE;
    }

    if (pst_Rx->uch_Status == RECIEVED)
    {
        return FALSE;
    }

    while (ReceFifoP(&uch_Data))//解析接收队列
    {
        //ASSERT(pst_Rx->uin_HaveRecBytes < N_COMMU_BUF);//判断接收的数据是否超限

        if(pst_Rx->uin_HaveRecBytes >= N_COMMU_BUF)
        {
            pst_Rx->uch_Status=COM_IDLE;
            pst_Rx->uin_HaveRecBytes = 0;

            return FALSE;
        }
        pst_Rx->buf[pst_Rx->uin_HaveRecBytes++] = uch_Data;

        if (pst_Rx->uin_HaveRecBytes == 1 && uch_RxThrowFlag != 1)
        {
            if (uch_Data != 0x7d)
            {
                pst_Rx->uin_HaveRecBytes = 0;
//                //qDebug()<<"no 0x7d";
            }
      //      //qDebug()<<"0x7d";
        }
        else if (pst_Rx->uin_HaveRecBytes == 2)
        {
            if (uch_Data != 0x7b)
            {
                pst_Rx->uin_HaveRecBytes = 0;
//                //qDebug()<<"no 0x7b";
            }
         //   //qDebug()<<"0x7b";
        }
        else if (pst_Rx->buf[pst_Rx->uin_HaveRecBytes-2] == 0x7d)//大于2的情况
        {
            if (uch_Data == 0x7d && uch_RxThrowFlag == 0)
            {
                uch_RxThrowFlag = 0;
                pst_Rx->uch_Status = RECIEVED;
             //   //qDebug()<<"recieved suc";
                return TRUE;
            }
            else if (uch_Data == 0x82 && uch_RxThrowFlag == 0)
            {
                //丢掉数据0x82
                pst_Rx->uin_HaveRecBytes--;
                uch_RxThrowFlag = 1;
            }
            else if (uch_Data == 0x7b && uch_RxThrowFlag == 0)//数据帧重新开始
            {
                uch_RxThrowFlag = 0;
                pst_Rx->buf[0] = 0x7d;
                pst_Rx->buf[1] = 0x7b;
                pst_Rx->uin_HaveRecBytes = 2;
            }
            else if(uch_RxThrowFlag == 1)
            {
                uch_RxThrowFlag = 0;
            }
            else
            {
                uch_RxThrowFlag = 0;
                pst_Rx->uin_HaveRecBytes = 0;
            }
        }
    }

    return FALSE;
}

//==================================================================================================
//| 函数名称 | SendFrameData
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 发送数据帧(需重载)
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | gao,08.4.10
//|----------|--------------------------------------------------------------------------------------
//| 函数修改 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::SendFrameData(void)
{
    //ASSERT((st_FpiTx.uin_TotalTransBytes < N_COMMU_BUF)&&(SendFifoP != NULL));

    if((st_FpiTx.uin_TotalTransBytes > N_COMMU_BUF)||(NULL == SendFifoP))
    {
        st_FpiTx.uch_Status = COM_IDLE;
        return FALSE;
    }

    st_FpiTx.uin_HaveTransBytes = 0;

    for (INT16U i=0; i<st_FpiTx.uin_TotalTransBytes; i++)
    {
        SendFifoP(st_FpiTx.buf[i]);
        st_FpiTx.uin_HaveTransBytes++;

        if ((st_FpiTx.buf[i] == 0x7d) && (i > 1) && (i < st_FpiTx.uin_TotalTransBytes-2))
        {
            SendFifoP(0x82);
            continue;
        }
    }

    st_FpiTx.uch_Status = COM_IDLE;

    SendEnableP();//启动发送；可以为中断/查询

    return TRUE;
}

//==================================================================================================
//| 函数名称 | CheckFrameCrc()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数据帧的CRC校验
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | gao,08.4.10
//|----------|--------------------------------------------------------------------------------------
//| 函数修改 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::CheckFrameCrc(void)
{
    COMMUNICATE *pst_Data = &st_FpiRx;
    INT16U  uin_K1,uin_K2,uin_K3,uin_K;
    INT16U  uin_CrcValue;

    //ASSERT(pst_Data != NULL);

    if(pst_Data == NULL)
    {
        return FALSE;
    }

    uin_K1 = pst_Data->buf[2]; //目标地址长
    uin_K2 = pst_Data->buf[2+uin_K1+1]; //源地址长度
    uin_K3 = DecodeUint(pst_Data->buf+2+(1+uin_K1)+(1+uin_K2)+2); //数据域长度

    if((uin_K1+uin_K2>=N_MAX_RPONTER) || (uin_K3>N_COMMU_BUF-18)\
    || (uin_K3 != (pst_Data->uin_HaveRecBytes-12-uin_K1-uin_K2)))
    {
        return(FALSE);
    }

    uin_K = 2+ (1+uin_K1) + (1+uin_K2) + 4 + uin_K3;

    if(uin_K > N_COMMU_BUF-4)
    {
        return(FALSE);
    }

    uin_CrcValue = DecodeUint(pst_Data->buf+uin_K);
    ////qDebug()<<"GetCrc16Bit";
    if (GetCrc16Bit(pst_Data->buf+2,uin_K-2) == uin_CrcValue)
    {
        //--qDebug()<<"GetCrc16Bit true";
        return(TRUE);
    }
    else
    {
        qDebug()<<"GetCrc16Bit false";
        return(FALSE);
    }

}

//==================================================================================================
//| 函数名称 | ConstructFrame()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 组建数据帧
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Frame: 指向FRAMESTRUCT数据类型的指针
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | gao,08.4.10
//|----------|--------------------------------------------------------------------------------------
//| 函数修改 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::ConstructFrame(FRAMESTRUCT *pst_Frame)
{
    COMMUNICATE *pst_Data = &st_FpiTx;
    INT8U  uch_K1,uch_K2;
    INT16U uin_K3,uin_K;
    INT16U  uin_CrcResult;

    //ASSERT((pst_Frame != NULL) && (pst_Data != NULL));

    if (pst_Frame == NULL || pst_Data == NULL)
    {
        return FALSE;
    }

    //变量赋值
    uch_K1 = pst_Frame->uch_DestLen;
    uch_K2 = pst_Frame->uch_SrcLen;
    uin_K3 = pst_Frame->uin_DataLen;
    uin_K = uch_K1 + uch_K2 + uin_K3;

    //溢出检查
    //ASSERT((uch_K1+uch_K2<N_MAX_RPONTER) && (uin_K < N_COMMU_BUF-14));

    if (uch_K1+uch_K2 > N_MAX_RPONTER) return FALSE;
    if (uin_K > N_COMMU_BUF-14)	   return FALSE;

    //起始标准位
    uin_K = 0;
    pst_Data->buf[uin_K++] = 0x7D;
    pst_Data->buf[uin_K++] = 0x7B;

    //目标地址域
    pst_Data->buf[uin_K++] = uch_K1;
    //ASSERT(uin_K<N_COMMU_BUF);
    CopyDataFromAToB(pst_Frame->uch_DestBuf, pst_Data->buf+uin_K, uch_K1);
    uin_K += uch_K1;

    //源地址域
    //ASSERT(uin_K<N_COMMU_BUF);
    pst_Data->buf[uin_K++] = uch_K2;
    CopyDataFromAToB(pst_Frame->uch_SrcBuf,pst_Data->buf+uin_K,uch_K2);
    uin_K += uch_K2;

    //数据域
    pst_Data->buf[uin_K++] = pst_Frame->uch_Cmd;
    pst_Data->buf[uin_K++] = pst_Frame->uch_AddCmd;
    //ASSERT(uin_K<N_COMMU_BUF);
    EncodeUint(pst_Frame->uin_DataLen,pst_Data->buf+uin_K);
    uin_K += 2;
    CopyDataFromAToB(pst_Frame->puch_Data,pst_Data->buf+uin_K,uin_K3);
    uin_K += uin_K3;



    //ASSERT(uin_K<N_COMMU_BUF);

    //CRC校验码
    uin_CrcResult = GetCrc16Bit(pst_Data->buf+2,uin_K-2);
    EncodeUint(uin_CrcResult,pst_Data->buf+uin_K);
    uin_K += 2;

    //结束标志位
    pst_Data->buf[uin_K++] = 0x7D;
    pst_Data->buf[uin_K++] = 0x7D;
    //ASSERT(uin_K<N_COMMU_BUF);

    pst_Data->uin_TotalTransBytes = uin_K;
    pst_Data->uin_HaveTransBytes = 0;




    return TRUE;
}


//==================================================================================================
//| 函数名称 | GetAddrFromRxFrame()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 从接收数据帧获取返回数据帧的地址
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Frame: 指向FRAMESTRUCT数据类型的指针
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL:TRUE 成功 FALSE 失败
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | gao,08.4.10
//|----------|--------------------------------------------------------------------------------------
//| 函数修改 | ygm,2011.01.24
//==================================================================================================
BOOL CComFpi::GetAddrFromRxFrame(FRAMESTRUCT *pst_Frame)
{
    COMMUNICATE *pst_Data = &st_FpiRx;
    INT8U uch_K1,uch_K2;

    //ASSERT((pst_Frame != NULL) && (pst_Data != NULL));
    if (pst_Frame == NULL || pst_Data == NULL)
    {
        return FALSE;
    }

    for (INT16U i=0; i<N_MAX_RPONTER; i++)
    {
        pst_Frame->uch_DestBuf[i] = 0;
        pst_Frame->uch_SrcBuf[i]  = 0;
    }

    uch_K1 = pst_Data->buf[2];				  //目标地址长度
    uch_K2 = pst_Data->buf[2+uch_K1+1];		  //源地址长度

    //ASSERT(uch_K1<N_MAX_RPONTER && uch_K2<N_MAX_RPONTER);

    pst_Frame->uch_SrcLen = uch_K1;
    CopyDataFromAToB(pst_Data->buf+3,pst_Frame->uch_SrcBuf,uch_K1);

    pst_Frame->uch_DestLen = uch_K2;
    CopyDataFromAToB(pst_Data->buf+uch_K1+4,pst_Frame->uch_DestBuf,uch_K2);

    return TRUE;
}

//==================================================================================================
//| 函数名称 | SearchDestBoard()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 寻找转发接口
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 转发地址
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.02.13
//==================================================================================================
BOOL CComFpi::SearchDest(INT8U uch_DestAddr)
{
    INT8U i;
    for(i=0;i<DEST_MAX;i++)
    {
        if(st_Dest[i].uch_DestAddr == uch_DestAddr)
        {
            break;
        }
    }

    if(i == DEST_MAX)
    {
        return FALSE;
    }

    uch_FpiDataTabSize = st_Dest[i].uin_TabLen;
    FpiDataTabP = st_Dest[i].p_Tab;

    return TRUE;
}

//==================================================================================================
//| 函数名称 | SearchFpiDataTab()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 寻找命令
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 命令码、扩展码
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
INT8U CComFpi::SearchFpiDataTab(INT8U uch_Cmd,INT8U uch_AddCmd)
{
    INT8U i,j;

    j = uch_FpiDataTabSize/sizeof(STRFPIDATA);

    for(i=0;i<j;i++)
    {
        if((FpiDataTabP[i].uch_Cmd == uch_Cmd)&&\
           (FpiDataTabP[i].uch_AddCmd == (uch_AddCmd|FpiDataTabP[i].uch_AddCmd)))
        {
            break;
        }
    }

    if(i == j)
    {
        i = 0xff;
    }

    return i;
}

//==================================================================================================
//| 函数名称 | CommComRead()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 命令解析
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
INT8U CComFpi::CommComRead(INT8U uch_Index,INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_FpiDataTabIndex=0;
    INT16U uin_Index = 0;

    uch_FpiDataTabIndex = uch_Index;

    while((FpiDataTabP[uch_FpiDataTabIndex].uch_Cmd == FpiDataTabP[uch_Index].uch_Cmd)&&\
           (FpiDataTabP[uch_FpiDataTabIndex].uch_AddCmd == FpiDataTabP[uch_Index].uch_AddCmd))
    {
        for(int i=0;i<FpiDataTabP[uch_FpiDataTabIndex].uch_DataNum;i++)
        {
            switch(FpiDataTabP[uch_FpiDataTabIndex].uch_DataType&REF_DATA)
            {
                case CTYPE_BOOL:
                case CTYPE_INT8U:
                {
                    if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                    {
                        #ifdef STOR_COM_ENABLE
                        ReadDataFromSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(INT8U)*i,\
                                     (INT8U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_INT8U);
                        #endif
                    }

                    SendData[uin_Index] = *((INT8U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i);
                    uin_Index++;
                }
                break;

                case CTYPE_INT16U:
                {
                    if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                    {
                        #ifdef STOR_COM_ENABLE
                        ReadDataFromSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(INT16U)*i,\
                                     (INT16U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_INT16U);
                        #endif
                    }

                    EncodeUint(*((INT16U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i),&SendData[uin_Index]);
                    uin_Index+=2;
                }
                break;

                case CTYPE_INT32U:
                {
                    if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                    {
                        #ifdef STOR_COM_ENABLE
                        ReadDataFromSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(INT32U)*i,\
                                     (INT32U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_INT32U);
                        #endif
                    }

                    EncodeUlong(*((INT32U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i),&SendData[uin_Index]);
                    uin_Index+=4;
                }
                break;

                case CTYPE_FP32:
                {
                    if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                    {
                        #ifdef STOR_COM_ENABLE
                        ReadDataFromSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(FP32)*i,\
                                     (FP32 *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_FP32);
                        #endif
                    }

                    EncodeFloat(*((FP32 *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i),&SendData[uin_Index]);
                    uin_Index+=4;
                }
                break;

                default:
                    return READ_FAILED;
                //break;
            }
        }

        uch_FpiDataTabIndex++;
    }

    *uip_SendDataLenth = uin_Index;

    return 0;
}

//==================================================================================================
//| 函数名称 | CommComWrite()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 命令解析
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U* ReceData,INT8U* SendData
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | INT8U 错误码
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
INT8U CComFpi::CommComWrite(INT8U uch_Index,INT8U* ReceData,INT8U* SendData,INT16U *uip_SendDataLenth)
{
    INT8U uch_FpiDataTabIndex=0;
    INT16U uin_Index = 0;

    for(INT8U j=0;j<2;j++)
    {
        uch_FpiDataTabIndex = uch_Index;
        uin_Index = 0;

        while((FpiDataTabP[uch_FpiDataTabIndex].uch_Cmd == FpiDataTabP[uch_Index].uch_Cmd)&&\
               (FpiDataTabP[uch_FpiDataTabIndex].uch_AddCmd == FpiDataTabP[uch_Index].uch_AddCmd))
        {
            for(int i=0;i<FpiDataTabP[uch_FpiDataTabIndex].uch_DataNum;i++)
            {
                switch(FpiDataTabP[uch_FpiDataTabIndex].uch_DataType&REF_DATA)
                {
                    case CTYPE_BOOL:
                    case CTYPE_INT8U:
                    {
                        if(((ReceData[uin_Index]<FpiDataTabP[uch_FpiDataTabIndex].ul_Min)\
                         ||(ReceData[uin_Index]>FpiDataTabP[uch_FpiDataTabIndex].ul_Max))\
                         &&(REF_REVS==(FpiDataTabP[uch_FpiDataTabIndex].uch_DataType&REF_REVS)))
                        {
                            return WRITE_FAILED;
                        }
                        else
                        {
                            if(1 == j)
                            {
                                *((INT8U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i) = ReceData[uin_Index];//写变量
                                if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                                {
                                    #ifdef STOR_COM_ENABLE
                                    WriteDataToSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(INT8U)*i,\
                                                (INT8U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_INT8U);
                                    #endif
                                }
                            }
                        }

                        uin_Index++;
                    }
                    break;

                    case CTYPE_INT16U:
                    {
                        INT16U uin_Data;
                        uin_Data = DecodeUint(&ReceData[uin_Index]);
                        if(((uin_Data<FpiDataTabP[uch_FpiDataTabIndex].ul_Min)\
                         ||(uin_Data>FpiDataTabP[uch_FpiDataTabIndex].ul_Max))
                         &&(REF_REVS==(FpiDataTabP[uch_FpiDataTabIndex].uch_DataType&REF_REVS)))

                        {
                            return WRITE_FAILED;
                        }
                        else
                        {
                            if(1 == j)
                            {
                                *((INT16U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i) = uin_Data;
                                if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                                {
                                    #ifdef STOR_COM_ENABLE
                                    WriteDataToSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(INT16U)*i,\
                                                (INT16U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_INT16U);
                                    #endif
                                }
                            }

                        }

                        uin_Index+=2;
                    }
                    break;

                    case CTYPE_INT32U:
                    {
                        INT32U ul_Data;
                        ul_Data = DecodeUlong(&ReceData[uin_Index]);
                        if(((ul_Data<FpiDataTabP[uch_FpiDataTabIndex].ul_Min)\
                         ||(ul_Data>FpiDataTabP[uch_FpiDataTabIndex].ul_Max))
                         &&(REF_REVS==(FpiDataTabP[uch_FpiDataTabIndex].uch_DataType&REF_REVS)))
                        {
                            return WRITE_FAILED;
                        }
                        else
                        {
                            if(1 == j)
                            {
                                *((INT32U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i) = ul_Data;
                                if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                                {
                                    #ifdef STOR_COM_ENABLE
                                    WriteDataToSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(INT32U)*i,\
                                                (INT32U *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_INT32U);
                                    #endif
                                }
                            }
                        }

                        uin_Index+=4;
                    }
                    break;

                    case CTYPE_FP32:
                    {
                        FP32 f_Data;
                        f_Data = DecodeFloat(&ReceData[uin_Index]);
                        if(((f_Data<FpiDataTabP[uch_FpiDataTabIndex].ul_Min)\
                         ||(f_Data>FpiDataTabP[uch_FpiDataTabIndex].ul_Max))
                         &&(REF_REVS==(FpiDataTabP[uch_FpiDataTabIndex].uch_DataType&REF_REVS)))
                        {
                            return WRITE_FAILED;
                        }
                        else
                        {
                            if(1 == j)
                            {
                                *((FP32 *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i) = f_Data;
                                if(NO_SAVE != FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr)
                                {
                                    #ifdef STOR_COM_ENABLE
                                    WriteDataToSaver(FpiDataTabP[uch_FpiDataTabIndex].ul_VariableAddr+sizeof(FP32)*i,\
                                                (FP32 *)FpiDataTabP[uch_FpiDataTabIndex].p_Data+i,CTYPE_FP32);
                                    #endif
                                }
                            }
                        }

                        uin_Index+=4;
                    }
                    break;

                    default:
                        return WRITE_FAILED;
                    //break;
                }
            }

            uch_FpiDataTabIndex++;
        }
    }

    *uip_SendDataLenth = 1;
    SendData[0] = WRITE_SUCCESS;

    return 0;
}

//==================================================================================================
//| 函数名称 | DecodeFrameAndDoIt()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 处理发给自己的命令帧
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2011.01.24
//==================================================================================================
void CComFpi::DecodeFrameAndDoIt(void)
{
    COMMUNICATE *pRev = &st_FpiRx;
    COMMUNICATE *pst_Tx = &st_FpiTx;
    INT8U *pCmd = (INT8U *)0;//NULL;
    INT8U *pData = (INT8U *)0;//NULL;
//	INT16U uin_DataLen;
    INT16U k1,k2;
    FRAMESTRUCT frame;
    INT16U uin_DataLen;
    INT8U ErrCode;
    INT16U uin_SendDataLenth;//uin_ReceDataLenth
    INT8U uch_Index;
    INT32U ul_TempP;
    CommExplainFunction stp_Func;

    //ASSERT((this != NULL) && (pRev != NULL) && (pst_Tx != NULL));

    while (pst_Tx->uch_Status != COM_IDLE);

    pst_Tx->uch_Status = SENDING;

    k1 = pRev->buf[2];					//目标地址长度
    k2 = pRev->buf[2+k1+1];				//源地址长度
    pCmd = pRev->buf + 4 + k1 + k2;		//主命令指针
    uin_DataLen = DecodeUint(pCmd+2);	//数据长度
    pData = pCmd + 4;					//数据起始位置指针
//    //qDebug()<<"GetAddrFromRxFrame";
    GetAddrFromRxFrame(&frame);
//    //qDebug()<<"GetAddrFromRxFrame end";
    frame.uch_Cmd = 0xff;
    frame.puch_Data = pst_Tx->buf + 8 + k1 + k2;

    //ASSERT((this != NULL) && (pRev != NULL) && (pst_Tx != NULL)\
    && (pCmd != NULL) && (pData != NULL) && (frame.puch_Data != NULL));

//	#ifdef CAN0_COM
//	gst_Can0Id.uch_DestAddr = pRev->buf[5];
//	#endif
//    //qDebug()<<"pull_CanStdId";
//    //qDebug()<<pull_CanStdId;
//    //qDebug()<<(pRev->buf[4+pRev->buf[4]]);
    //*pull_CanStdId = pRev->buf[4+pRev->buf[4]];
   // *pull_CanStdId = 0xF3;
//    //qDebug()<<"pull_CanStdId end";
    if(TRUE == SearchDest(pRev->buf[4+pRev->buf[4]]))
    {
//        //qDebug()<<"SearchDest";
        uch_Index = SearchFpiDataTab(pCmd[0],pCmd[1]);
//        //qDebug()<<"SearchFpiDataTab";
    }
    else
    {
        uch_Index = 0xff;
    }

    if(uch_Index != 0xff)
    {
        frame.uch_Cmd = FpiDataTabP[uch_Index].uch_Cmd;//命令码


        if(READUPDATA == pCmd[1])
        {
            frame.uch_AddCmd = READUPDATA_ANSWER;
        }
        else if(WRITEUPDATA == pCmd[1])
        {
            frame.uch_AddCmd = WRITEUPDATA_ANSWER;
        }

        else
        {
            frame.uch_AddCmd = ~pCmd[1];
    //            //qDebug()<<"frame.uch_AddCmd = ~pCmd[1]";
        }

        if(CTYPE_FUNC == FpiDataTabP[uch_Index].uch_DataType)
        {

//            //qDebug()<<"CTYPE_FUNC ";
            ul_TempP = (long int)FpiDataTabP[uch_Index].p_Data;
            stp_Func = (CommExplainFunction) ul_TempP;



            if(FpiDataTabP[uch_Index].uin_ReceDataLen == N_NUM)
            {

                ErrCode = stp_Func(pData-2,frame.puch_Data,&uin_SendDataLenth);
            }
            else
            {
                ErrCode = stp_Func(pData,frame.puch_Data,&uin_SendDataLenth);
            }
//            //qDebug()<<"READ_ANSWER == pCmd[1])||(WRITE_ANSWER == pCmd[1])";
            if((READ_ANSWER == pCmd[1])||(WRITE_ANSWER == pCmd[1]))
            {

                b_AckOk = 1;
                pRev->uch_Status = COM_IDLE;
                pst_Tx->uch_Status = COM_IDLE;
                pRev->uin_HaveRecBytes = 0;
                return;
            }

        }
        else
        {
            if(READ == pCmd[1])
            {//读操作
                ErrCode = CommComRead(uch_Index,pData,frame.puch_Data,&uin_SendDataLenth);
            }
            else if(WRITE == pCmd[1])
            {//写操作
                ErrCode = CommComWrite(uch_Index,pData,frame.puch_Data,&uin_SendDataLenth);
            }
            else if(READUPDATA == pCmd[1])
            {//读操作
                ErrCode = CommComRead(uch_Index,pData,frame.puch_Data,&uin_SendDataLenth);
            }
            else if(WRITEUPDATA == pCmd[1])
            {//写操作
                ErrCode = CommComWrite(uch_Index,pData,frame.puch_Data,&uin_SendDataLenth);
            }
            else if(READ_ANSWER == pCmd[1])
            {//读回应
                ErrCode = CommComWrite(uch_Index,pData,frame.puch_Data,&uin_SendDataLenth);
                b_AckOk = 1;
                pRev->uch_Status = COM_IDLE;
                pst_Tx->uch_Status = COM_IDLE;
                pRev->uin_HaveRecBytes = 0;
                return;
            }
            else if(WRITE_ANSWER == pCmd[1])
            {//写回应
                ErrCode = CommComRead(uch_Index,pData,frame.puch_Data,&uin_SendDataLenth);
                b_AckOk = 1;
                pRev->uch_Status = COM_IDLE;
                pst_Tx->uch_Status = COM_IDLE;
                pRev->uin_HaveRecBytes = 0;
                return;
            }

        }

//		if(uin_ReceDataLenth == uin_DataLen)//接收数据长度校验zbb2017.6.20
//		{
            if(0 == ErrCode)
            {
                frame.uin_DataLen = uin_SendDataLenth;//数据长度
            }
            else
            {
                frame.uin_DataLen =  1;//数据长度
                frame.puch_Data[0] = ErrCode;
            }
//		}
//		else
//		{
//	         frame.uin_DataLen =  1;//数据长度
//	         frame.puch_Data[0] = WRITE_FAILED;
//		}
    }
    else//命令表格里没有该命令
    {
        frame.uch_Cmd = 0xff;
    }

    if (frame.uch_Cmd != 0xff)
    {
        ConstructFrame(&frame);
        pRev->uch_Status = COM_IDLE;
        pRev->uin_HaveRecBytes = 0;


        SendFrameData();
    }
    else
    {
        pRev->uch_Status = COM_IDLE;
        pRev->uin_HaveRecBytes = 0;

        pst_Tx->uch_Status = COM_IDLE;
    }

}

//==================================================================================================
//| 函数名称 | Process()
//|-----------------------------------------------------------------------------------------------
//| 函数功能 | 处理数据包(上层)
//|-----------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|-----------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|-----------------------------------------------------------------------------------------------
//| 函数设计 | zbb,2018.7.12
//==================================================================================================
void CComFpi::SlaveProcess(void)
{
    COMMUNICATE *pst_Rx = &st_FpiRx;
    COMMUNICATE *pst_Tx = &st_FpiTx;

    if(0 == b_ModEnable)
    {
       return;
    }

    //ASSERT((pst_Rx != NULL) && (pst_Tx != NULL));

    if(pst_Rx == NULL || pst_Tx == NULL)
    {
        return;
    }

    GetFrameFromQue();

    //端口状态判断
    if (pst_Rx->uch_Status != RECIEVED)  return;

    //--qDebug()<<"pst_Rx->uch_Status == RECIEVED";

    if (FALSE == CheckFrameCrc())
    {
        pst_Rx->uch_Status = COM_IDLE;
        pst_Rx->uin_HaveRecBytes = 0;
//        qDebug()<<"check crc flase";
        return;
    }
//    qDebug()<<"CheckFrameCrc end";


    //qDebug("st_FpiRx data:");
    //vMemDump(st_FpiRx.buf,st_FpiRx.uin_HaveRecBytes);

    //数据处理
    if (pst_Rx->buf[3] == uch_SourAddr)
    {
        if (pst_Rx->buf[2] == 1) //数据解析
        {
            //--qDebug("DecodeFrameAndDoIt().\n");
            //--qDebug("pst_Rx->buf[2] %d = uch_SourAddr",pst_Rx->buf[3]);
            DecodeFrameAndDoIt();
        }
        else					//数据转发
        {
            //--qDebug("pst_Rx->buf[3] %d != uch_SourAddr",pst_Rx->buf[3]);

            //--qDebug("SendFrameToOtherProcess().\n");
            SendFrameToOtherProcess();
        }
    }
    else
    {
//        qDebug()<<"no sourceaddr";
        pst_Rx->uch_Status=COM_IDLE;
        pst_Rx->uin_HaveRecBytes=0;
    }
}

//==================================================================================================
//| 函数名称 | run()
//|-----------------------------------------------------------------------------------------------
//| 函数功能 |
//|-----------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|-----------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|-----------------------------------------------------------------------------------------------
//| 函数设计 | zbb,2018.7.12
//==================================================================================================
void CComFpi::Process(void)
{


     HostProcess();

     SlaveProcess();

}
//==================================================================================================
//| 函数名称 | run()
//|-----------------------------------------------------------------------------------------------
//| 函数功能 |
//|-----------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|-----------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|-----------------------------------------------------------------------------------------------
//| 函数设计 | zbb,2018.7.12
//==================================================================================================
void CComFpi::run(void)
{
    while(1)
    {



      Runtask();

    }

}

//no more
