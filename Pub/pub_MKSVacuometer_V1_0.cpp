//==================================================================================================
//| 文件名称 | pub_MKSVacuometerV1_0.cpp
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | MKS真空规驱动
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 | 所有C/C++语言编译环境，包括单片机编译器
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2019, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2019.10.22  |  LYB      | 初版
//|--------- |-------------|-----------|------------------------------------------------------------

//==================================================================================================
#include "pub_MKSVacuometer_V1_0.h"
#include "Bsp/Bsp_Uart1.h"

CComMKSVacuometer  g_clComMKSVacuometer;
CMKSVacuometerCtrl g_clMKSVacuometerDevice1(253);


/*FUNC
函数名称 | MKSVacuometerProcess
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
void MKSVacuometerProcess(void)
{
    g_clComMKSVacuometer.ComMKSVacuometerSlaveProcess();
    g_clMKSVacuometerDevice1.MKSVacuometerCtrlProcess();//响应步骤式操控命令
    g_clMKSVacuometerDevice1.MKSVacuometerCtrlReadVacuumTriggerProcess();//触发式命令
}

/*FUNC 初始化MKS真空规通信和控制模块
函数名称 | MKSVacuometerInit
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
INT8U uch_Uart;
void MKSVacuometerInit(void)
{
        //对通信接口进行装载
    STRMKSVacuometerINSTALL st_Install;
    st_Install.b_ModEnable = TRUE;
    st_Install.ReceFifoP = Uart1RxChar;
    st_Install.SendFifoP = Uart1TxChar;
    st_Install.SendEnableP = Uart1SendEnable;
    st_Install.pb_ComLock = &uch_Uart;
    g_clComMKSVacuometer.ComMKSVacuometerInstall(&st_Install);

    //对器件控制对象设置
    g_clMKSVacuometerDevice1.MKSVacuometerCtrlConnectCom(&g_clComMKSVacuometer); //每个通信类控制器件要连接相应的通信接口


}



//----------------------------------通信类------------------------------------

/*FUNC 构造函数
函数名称 | CComMKSVacuometer
功能详细 | 初始化部分变量
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
CComMKSVacuometer::CComMKSVacuometer(void)
{
    st_NoFpiRev.uch_Status = MKSVacuometer_COM_IDLE;
    st_NoFpiRev.uin_HaveRecBytes = 0;
    st_NoFpiRev.uin_HaveTransBytes = 0;
    st_NoFpiRev.uin_TotalTransBytes = 0;


    m_uinVacuumRead = 0;

    b_FirstRunFlag = TRUE;
    m_bComBack = FALSE;



    return;
}
/*FUNC 析构函数
函数名称 | CComMKSVacuometer
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
CComMKSVacuometer::~CComMKSVacuometer()
{
    //在此销毁类成员(变量)
}

/*FUNC 模块使能
函数名称 | SetModEnable
功能详细 |
输入参数 |
返回参数 |
函数设计 | ,2019.03.11*/
BOOL CComMKSVacuometer::ComMKSVacuometerSetModEnable(BOOL b_Set)
{
    //进行赋值合法性判定并赋值
    if(b_Set == FALSE || b_Set == TRUE)
    {
        b_ModEnable = b_Set;
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*FUNC 通信接口安装
函数名称 | Install
功能详细 |
输入参数 |
返回参数 |
函数设计 | ,2019.03.11*/
BOOL CComMKSVacuometer::ComMKSVacuometerInstall(STRMKSVacuometerINSTALL* pst_Data)
{
    if(NULL == pst_Data)
    {
        return FALSE;
    }

    if((pst_Data->b_ModEnable > 1)\
    ||(pst_Data->uch_SourAddr > 0xff)\
    ||(NULL == pst_Data->ReceFifoP)\
    ||(NULL == pst_Data->SendFifoP)\
    ||(NULL == pst_Data->SendEnableP))
    {
        return FALSE;
    }

    b_ModEnable = pst_Data->b_ModEnable;
    uch_SourAddr= pst_Data->uch_SourAddr;
        m_pbComLock = pst_Data->pb_ComLock;
    ReceFifoP= pst_Data->ReceFifoP;
    SendFifoP= pst_Data->SendFifoP;
    SendEnableP= pst_Data->SendEnableP;

    return TRUE;
}

/*FUNC 通信接收命令运行程序
函数名称 |
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
void CComMKSVacuometer::ComMKSVacuometerSlaveProcess(void)
{
    STRMKSVacuometerCOMMUNICATE *pst_Rx = &st_NoFpiRev;

    if(0 == b_ModEnable)
    {
        return;
    }
    if(pst_Rx == NULL)
    {
        return;
    }

        if(m_bFrameGetEnable == TRUE)
        {
                GetFrameFromCOM();
        }
    //端口状态判断
    if (pst_Rx->uch_Status != MKSVacuometer_RECIEVED)  return;

    //数据处理
    FrameDeal();

    pst_Rx->uin_HaveRecBytes = 0;
    pst_Rx->uch_Status = MKSVacuometer_COM_IDLE;
}





/*FUNC 将发送命令输出给发送缓存
函数名称 | SendFrameData
功能详细 |
输入参数 |
返回参数 | void
函数设计 | lyb,2019.03.11*/
void CComMKSVacuometer::SendFrameData(INT8U* uch_Data,INT16U uin_SendLen,INT8U uch_Addr)
{
    INT16U i;
    m_uchSendAddr = uch_Addr;
    for (i=0; i < uin_SendLen; i++)
    {
        SendFifoP(*(uch_Data+i));
    }
    SendEnableP();//启动发送；可以为中断/查询
    return;
}

/*FUNC 得到正确的帧
函数名称 | GetFrameFromCOM
功能详细 |
输入参数 |
返回参数 | INT8U FALSE 失败， TRUE 成功获取
函数设计 | lyb,2019.03.11*/
INT8U CComMKSVacuometer::GetFrameFromCOM(void)
{
    STRMKSVacuometerCOMMUNICATE *pst_Rcv = &st_NoFpiRev;
    INT8U uch_Chr;


    if (pst_Rcv->uch_Status == MKSVacuometer_RECIEVED)
    {
        return FALSE;
    }

    //获取接收到得数据
    while (ReceFifoP(&uch_Chr))
    {

        if(pst_Rcv->uin_HaveRecBytes >= N_MKSVacuometer_COMMU_BUF)
        {
            pst_Rcv->uch_Status=MKSVacuometer_COM_IDLE;
            pst_Rcv->uin_HaveRecBytes = 0;

            return FALSE;
        }

        pst_Rcv->buf[pst_Rcv->uin_HaveRecBytes++] = uch_Chr;

        //qDebug("get chr %x buf %x recbyte num %d",uch_Chr,pst_Rcv->buf[pst_Rcv->uin_HaveRecBytes],pst_Rcv->uin_HaveRecBytes);
        if (pst_Rcv->uin_HaveRecBytes == 1)
        {
            if (uch_Chr != '@')
            {
                pst_Rcv->uin_HaveRecBytes = 0;
            }
        }
        else if (pst_Rcv->uin_HaveRecBytes == 2)
        {
            if (uch_Chr != (m_uchSendAddr/100+'0'))
            {
                pst_Rcv->uin_HaveRecBytes = 0;
            }
        }
        else if (pst_Rcv->uin_HaveRecBytes == 3)
        {
            if (uch_Chr != (m_uchSendAddr%100/10+'0'))
            {
                pst_Rcv->uin_HaveRecBytes = 0;
            }
        }
        else if (pst_Rcv->uin_HaveRecBytes == 4)
        {
            if (uch_Chr != (m_uchSendAddr%10+'0'))
            {
                pst_Rcv->uin_HaveRecBytes = 0;
            }
        }
        else if(pst_Rcv->uin_HaveRecBytes > 4)
        {

            //qDebug("last buf %x prev bug %x",pst_Rcv->buf[pst_Rcv->uin_HaveRecBytes-1],pst_Rcv->buf[pst_Rcv->uin_HaveRecBytes-2]);
            if((pst_Rcv->buf[pst_Rcv->uin_HaveRecBytes-1] ==  'F')
                &&(pst_Rcv->buf[pst_Rcv->uin_HaveRecBytes-2] == 'F'))
            {

                pst_Rcv->uch_Status = MKSVacuometer_RECIEVED;
//                qDebug("mks vacuometer get  back %d",pst_Rcv->uin_HaveRecBytes );
                pst_Rcv->uin_HaveRecBytes = 0;

                return TRUE;
            }



        }
    }
    return FALSE;
}

/*FUNC 解析返回数据
函数名称 | FrameDeal
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
void CComMKSVacuometer::FrameDeal(void)
{
    STRMKSVacuometerCOMMUNICATE *pst_Rcv = &st_NoFpiRev;
    m_bComBack = TRUE;

    m_fVacuum = CountF(pst_Rcv->buf+7,7);

    pst_Rcv->uin_HaveRecBytes = 0;
    pst_Rcv->uch_Status=MKSVacuometer_COM_IDLE;
    return;


}

//==================================================================================================
//| 函数名称 | CountF
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 计算真空度
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | BOOL TRUE:处理过程没有错误，FALSE:有错误
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lxlll 2010.09.2
//==================================================================================================
FP32 CComMKSVacuometer::CountF(INT8U* uch_Data,INT8U uch_Len)
{

        INT8U uch_lPoint = 0;
        INT8U uch_lE = 0;
        INT8U uch_lF = 0;
        FP32 f_lTemp = 0;
        INT8U j = 0,k = 1,l = 0;
        INT8U uch_Exp = 0;
        INT8U i = 0;
        INT8U uch_ExpFlag = 0;
        static FP32 P = 0.0;
        for(i = 0;i<uch_Len;i++)
        {
                if(uch_lPoint == 0)
                {
                        if(uch_Data[i] == '.')
                                uch_lPoint = 1;
                        else
                                f_lTemp = f_lTemp*10.0 + uch_Data[i]- 48;//0x30或48，'0'
                }
                else
                {
                        if(uch_lE == 0)
                        {
                                if(uch_Data[i] == 'E')
                                        uch_lE = 1;
                                else
                                {
                                        memset(&P,0,sizeof(P));
                                        // P = 0.00;
                                        if(k == 1)
                                                P = (1.0000)/(10.0);
                                        else
                                                P = (1.0000)/(100.0);
                                        P = ((FP32)(uch_Data[i]- 48))*P;
                                        f_lTemp =  f_lTemp +  P;
                                        k++;
                                }
                        }
                        else if(uch_lE == 1)
                        {
                                if(uch_lF == 0)
                                {
                                        if(uch_Data[i] == '+')
                                        {
                                                uch_lF = 1;j = 0;uch_ExpFlag = 1;continue;
                                        }
                                        else if(uch_Data[i] == '-')
                                        {
                                                uch_lF = 1;j = 0;uch_ExpFlag = 2; continue;
                                        }
                                        else
                                                return 0;
                                }
                                else if(uch_lF == 1)
                                {
                                        uch_Exp = uch_Exp*j +uch_Data[i]- 48;
                                        j++;
                                }
                        }

                }
        }
        if(uch_Exp != 0)
        {
                if(uch_ExpFlag == 1)
                {
                        for(i = 1;i<=uch_Exp;i++)
                        {
                                f_lTemp *= 10.0;
                        }
                }
                else if(uch_ExpFlag == 2)
                {
                        for(i = 1;i<=uch_Exp;i++)
                        {
                                f_lTemp /= 10.0;
                        }
                }


        }

        return f_lTemp;

}



//----------------------------------控制类--------------------------
/*FUNC 构造函数
函数名称 | CMKSVacuometerCtrl
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
CMKSVacuometerCtrl::CMKSVacuometerCtrl(INT8U uch_Addr)
{
    int i = 0;
    m_uchAddr = uch_Addr;


    m_bErrCom = FALSE;
    m_uchSendCnt = 0;
    m_uchResendMax = 3;
    m_uinSendDataLen = 0;
    m_pclMKSVacuometer = NULL;
    for(i=0;i<MKSVacuometer_1COM_BUF;i++)
    {
            m_auchSendBuf[i] = 0;
    }
    m_uchOpration = MKSVacuometer_OPRATION_NULL;
}

/*FUNC 构造函数 无参数
函数名称 | CMKSVacuometerCtrl
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
CMKSVacuometerCtrl::CMKSVacuometerCtrl()
{
    int i = 0;
    m_uchAddr = 253; //默认地址

    m_bErrCom = FALSE;
    m_uchSendCnt = 0;
    m_uchResendMax = 3;
    m_uinSendDataLen = 0;
    m_pclMKSVacuometer = NULL;
    for(i=0;i<MKSVacuometer_1COM_BUF;i++)
    {
        m_auchSendBuf[i] = 0;
    }
    m_uchOpration = MKSVacuometer_OPRATION_NULL;


    m_ulCommCnt = 0;
    m_ulCommErrCnt = 0;
}

/*FUNC
函数名称 |
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
CMKSVacuometerCtrl::~CMKSVacuometerCtrl()
{

}

/*FUNC 连接通信接口
函数名称 | MKSVacuometerCtrlConnectCom
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
void CMKSVacuometerCtrl::MKSVacuometerCtrlConnectCom(CComMKSVacuometer * pcl_MKSVacuometer)
{
        m_pclMKSVacuometer = pcl_MKSVacuometer;
}

/*FUNC 设置流量流程触发进程
函数名称 |
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
void CMKSVacuometerCtrl::MKSVacuometerCtrlReadVacuumTriggerProcess(void)
{
        switch(m_uchStateReadVacuumTrigger)
        {
                case 0:
                {
                    if(m_bReadVacuumSig == TRUE)
                    {
                        if(IsMKSVacuometerOpraionNull())
                        {
                            m_bReadVacuumSig = FALSE;
                            m_uchStateReadVacuumTrigger++;
                            MKSVacuometerCtrlReadVacuum();
                        }
                    }
                }break;
                case 1:
                {
                    if(IsMKSVacuometerOpraionNull())
                    {
                         m_uchStateReadVacuumTrigger = 0;
                    }
                }break;
                default:
                break;
        }
}

/*FUNC 读取真空值流程触发
函数名称 | MKSVacuometerCtrlReadVacuumTrigger
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
void CMKSVacuometerCtrl::MKSVacuometerCtrlReadVacuumTrigger(void)
{
    m_bReadVacuumSig = TRUE;
}


/*FUNC 主动发送数据进程
函数名称 | MKSVacuometerCtrlProcess
功能详细 |
输入参数 |
返回参数 |
函数设计 | lyb,2019.03.11*/
void CMKSVacuometerCtrl::MKSVacuometerCtrlProcess(void)
{
    if(m_pclMKSVacuometer == NULL) //没有加载通信接口
    {
            return;
    }
    switch(m_uchState)
    {
        case 0:
        {
            if(m_uchOpration != MKSVacuometer_OPRATION_NULL && m_pclMKSVacuometer->GetComLock() == FALSE)
            {
                m_pclMKSVacuometer->LockCom();
                m_pclMKSVacuometer->ClearComBackFlag();

                switch(m_uchOpration)
                {

                    case MKSVacuometer_OPRATION_READ_Vacuum:
                    {
                        ConstructFrame(m_uchAddr,MKSVacuometer_CMD_READ_Vacuum);
                        m_ulCommCnt++;
                    }break;

                    default:
                    return;
                }
                Set(MKSVacuometer_COM_TIMEOUT);
                m_bTmrOutFlag = FALSE;
                m_uchState++;
            }
        }break;
        case 1:
        {
            if(m_pclMKSVacuometer->GetComBackFlag())
            {
                m_bErrCom = FALSE;//消除报错
                m_pclMKSVacuometer->UnLockCom();
                m_pclMKSVacuometer->ClearComBackFlag();

                switch(m_uchOpration) //根据不同命令校对反馈内容
                {
                    case MKSVacuometer_OPRATION_READ_Vacuum:
                    {

                        m_fVacuum = m_pclMKSVacuometer->m_fVacuum;

                    }break;

                    default:
                    break;
                }
                FlowFinish();
            }
            else
            {

                if(TmrOut())
                {
                    //qDebug("MKSVacum COM err");
                    m_fVacuum = 0;
                    m_pclMKSVacuometer->UnLockCom();
                    m_uchSendCnt++;
                    m_ulCommErrCnt++;
                    if(m_uchSendCnt>m_uchResendMax)
                    {
                        m_uchSendCnt = 0;
                        m_bErrCom = TRUE;//超时报错
                        FlowFinish();
                    }
                    else
                    {
                        m_uchState = 0;
                    }
                }
            }
        }break;

    }


}


/*FUNC 流程结束
函数名称 | FlowFinish
功能详细 |
输入参数 | void
返回参数 | void
函数设计 | lyb,2019.03.11*/
void CMKSVacuometerCtrl::FlowFinish(void)
{
    m_uchSendCnt = 0;
    m_uchState = 0;
    m_uchOpration = MKSVacuometer_OPRATION_NULL;
}

BOOL CMKSVacuometerCtrl::IsMKSVacuometerOpraionNull(void)
{
    if(m_uchOpration==MKSVacuometer_OPRATION_NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



/*FUNC 读取流量
函数名称 | MKSVacuometerCtrlReadFlow
功能详细 |
输入参数 | void
返回参数 | void
函数设计 | lyb,2019.03.11*/
void CMKSVacuometerCtrl::MKSVacuometerCtrlReadVacuum(void)
{
    m_uchOpration = MKSVacuometer_OPRATION_READ_Vacuum;
}


/*FUNC 将发送命令输出给发送缓存
函数名称 | ConstructFrame
功能详细 |
输入参数 |
返回参数 | INT8U
函数设计 | lyb,2019.03.11*/
INT8U CMKSVacuometerCtrl::ConstructFrame(INT8U uch_Addr,INT8U uch_Cmd)
{
    INT8U i;
    INT16U uin_CkeckSum = 0;
    INT16U uin_Byte = 0;
    //m_auchSendBuf[uin_Byte++] = uch_Addr;
    switch(uch_Cmd)
    {
        case MKSVacuometer_CMD_READ_Vacuum: //读取流量
        {

           m_auchSendBuf[uin_Byte++] = '@';
           m_auchSendBuf[uin_Byte++] = uch_Addr/100 + '0';
           m_auchSendBuf[uin_Byte++] = uch_Addr%100/10 + '0';
           m_auchSendBuf[uin_Byte++] = uch_Addr%10 + '0';
           m_auchSendBuf[uin_Byte++] = 'P';
           m_auchSendBuf[uin_Byte++] = 'R';
           m_auchSendBuf[uin_Byte++] = '1';
           m_auchSendBuf[uin_Byte++] = '?';
           m_auchSendBuf[uin_Byte++] = ';';
           m_auchSendBuf[uin_Byte++] = 'F';
           m_auchSendBuf[uin_Byte++] = 'F';
           m_uinSendDataLen = uin_Byte;
        }break;

        default:
        return FALSE;
    }
    m_pclMKSVacuometer->SendFrameData(m_auchSendBuf,m_uinSendDataLen, uch_Addr);

    return TRUE;
}
