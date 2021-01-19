#include "mod_mnt.h"
#include "Pub/Pub_Alarm.h"
#include "Pro/pro_fpga.h"
#include "Mod/mod_ioctrl.h"
#include "Mod/mod_circu.h"
#include <math.h>
#include "Pub/pub_caltemp.h"
#include <QDateTime>
#include <App/App_ComFpi.h>
#include <App/app_app.h>
#include "Pub/pub_MKSVacuometer_V1_0.h"
CMnt g_clMnt;


//==================================================================================================
//| 函数名称 | CMnt
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 构造函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
CMnt::CMnt()
{

    m_uchRunMode = DEVICE_RUN_MODE_NORMAL;
    //memset(&st_mMntPara,0,sizeof(st_mMntPara));

    //主板电源+5VD异常
    st_mMntPara.ESourcePositive5VDigital.f_MaxVal = 5.5;
    st_mMntPara.ESourcePositive5VDigital.f_MinVal = 4.5;
    st_mMntPara.ESourcePositive5VDigital.uch_FaultCode = 0x81;
    m_pclESourcePositive5VDigital = new CCtlModJudge(st_mMntPara.ESourcePositive5VDigital.f_MaxVal,st_mMntPara.ESourcePositive5VDigital.f_MinVal,0);

    //主板电源+1.2VD
    st_mMntPara.ESourcePositive1Point2VDigital.f_MaxVal = 1.3;
    st_mMntPara.ESourcePositive1Point2VDigital.f_MinVal = 1.1;
    st_mMntPara.ESourcePositive1Point2VDigital.uch_FaultCode = 0x82;
    m_pclESourcePositive1Point2VDigital = new CCtlModJudge(st_mMntPara.ESourcePositive1Point2VDigital.f_MaxVal,st_mMntPara.ESourcePositive1Point2VDigital.f_MinVal,0);

    //主板电源+3.3VD电压
    st_mMntPara.ESourcePositive3Point3VDigital.f_MaxVal = 3.4;
    st_mMntPara.ESourcePositive3Point3VDigital.f_MinVal = 3.0;
    st_mMntPara.ESourcePositive3Point3VDigital.uch_FaultCode = 0x83;
    m_pclESourcePositive3Point3VDigital = new CCtlModJudge(st_mMntPara.ESourcePositive3Point3VDigital.f_MaxVal,st_mMntPara.ESourcePositive3Point3VDigital.f_MinVal,0);

    //主板电源-5VA参考电压
    st_mMntPara.ESourceNegative5VAnalog.f_MaxVal = -4.5;
    st_mMntPara.ESourceNegative5VAnalog.f_MinVal = -5.5;
    st_mMntPara.ESourceNegative5VAnalog.uch_FaultCode = 0x84;
    m_pclESourceNegative5VAnalog = new CCtlModJudge(st_mMntPara.ESourceNegative5VAnalog.f_MaxVal,st_mMntPara.ESourceNegative5VAnalog.f_MinVal,0);

    //主板电源+5VA电压
    st_mMntPara.ESourcePositive5VAnalog.f_MaxVal = 5.5;
    st_mMntPara.ESourcePositive5VAnalog.f_MinVal = 4.5;
    st_mMntPara.ESourcePositive5VAnalog.uch_FaultCode = 0x85;
    m_pclESourcePositive5VAnalog = new CCtlModJudge(st_mMntPara.ESourcePositive5VAnalog.f_MaxVal,st_mMntPara.ESourcePositive5VAnalog.f_MinVal,0);
    //主板电源-14VA参考电压
    st_mMntPara.ESourceNegative14VAnalog.f_MaxVal = -13;
    st_mMntPara.ESourceNegative14VAnalog.f_MinVal = -15;
    st_mMntPara.ESourceNegative14VAnalog.uch_FaultCode = 0x86;
    m_pclESourceNegative14VAnalog = new CCtlModJudge(st_mMntPara.ESourceNegative14VAnalog.f_MaxVal,st_mMntPara.ESourceNegative14VAnalog.f_MinVal,0);

    //主板电源+14VA电压
    st_mMntPara.ESourcePositive14VAnalog.f_MaxVal = 15;
    st_mMntPara.ESourcePositive14VAnalog.f_MinVal = 13;
    st_mMntPara.ESourcePositive14VAnalog.uch_FaultCode = 0x87;
    m_pclESourcePositive14VAnalog = new CCtlModJudge(st_mMntPara.ESourcePositive14VAnalog.f_MaxVal,st_mMntPara.ESourcePositive14VAnalog.f_MinVal,0);

    //主板1 2.5V参考电压
    st_mMntPara.Reference2Point5VOne.f_MaxVal = 2.55;
    st_mMntPara.Reference2Point5VOne.f_MinVal = 2.45;
    st_mMntPara.Reference2Point5VOne.uch_FaultCode = 0x88;
    m_pclReference2Point5VOne = new CCtlModJudge(st_mMntPara.Reference2Point5VOne.f_MaxVal,st_mMntPara.Reference2Point5VOne.f_MinVal,0);
    //主板2 2.5V参考电压
    st_mMntPara.Reference2Point5VTwo.f_MaxVal = 2.55;
    st_mMntPara.Reference2Point5VTwo.f_MinVal = 2.45;
    st_mMntPara.Reference2Point5VTwo.uch_FaultCode = 0x89;



    //RF供电电源电压异常
    st_mMntPara.RFPowerESourceVoltage.f_MaxVal = 65;
    st_mMntPara.RFPowerESourceVoltage.f_MinVal = 55;
    st_mMntPara.RFPowerESourceVoltage.uch_FaultCode = 0x8B;
    m_pclRFPowerESourceVoltage = new CCtlModJudge( st_mMntPara.RFPowerESourceVoltage.f_MaxVal,st_mMntPara.RFPowerESourceVoltage.f_MinVal,0);




    //主板温度异常
    st_mMntPara.st_Temperta.f_MaxVal = 45;
    st_mMntPara.st_Temperta.f_MinVal = 10;
    st_mMntPara.st_Temperta.uch_FaultCode = 0x8D;
    m_pclTemperta = new CCtlModJudge(st_mMntPara.st_Temperta.f_MaxVal,st_mMntPara.st_Temperta.f_MinVal,0);

    //真空归通讯异常
    st_mMntPara.st_VacuCommErr.uch_FaultCode = 0x0D;

    //门控开关打开
    st_mMntPara.st_GateONErr.uch_FaultCode = 0x91;

    //总线自检失败
    st_mMntPara.st_GPMCCheckErr.uch_FaultCode = 0x10;

    m_stEnvMnt.f_IonicTempSet = 5;

    uch_FirstGateOnSet = 1;

    b_MSStateBeforeGateOff = 0;
    f_IonicTempBeforeGateOff = 0;
}

//==================================================================================================
//| 函数名称 | CMntProcess
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 监控进程  数据监控 保护和报警
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.08.14
//==================================================================================================
void CMnt::CMntProcess()
{
    FP32 f_Temp = 0;
    INT16U uin_Data = 0;



    //读取FPGA的AD电压
    FPGAADRead();


    // 真空规兼容
    if(g_clMnt.m_uchCIDVacuumSelect == UNIT_CIDVACUUMGAUGE_MKS)
    {
        st_mMntPara.st_VacuValue3.f_RealVal = g_clMKSVacuometerDevice1.m_fVacuum;//碰撞池压力
    }
    else
    {
        if(g_clCircu.m_bCommAlarm == TRUE)
        {
            st_mMntPara.st_VacuValue3.f_RealVal = 0;
        }
        else
        {
            st_mMntPara.st_VacuValue3.f_RealVal = g_clMnt.m_stEnvMnt.f_VacuValue;//碰撞池压力
        }

    }

    //真空异常报警
    if(m_stEnvMnt.strPumpSpeed.f_RealVal >= 950)
    {
          if( (st_mMntPara.st_VacuValue2.f_RealVal > 0.009)   //分析腔室真空度条件
                  || (st_mMntPara.st_VacuValue3.f_RealVal > 1)  //碰撞池真空度条件
                  )
          {
                  if(g_clMnt.m_uchRunMode == DEVICE_RUN_MODE_NORMAL)
                  {

                       DeviceStandby();
                  }
                  //qDebug("Vaccum Err to standby ERR_VACUUM");
                  PushAlarmCode(ERR_VACUUM);
          }
          else
          {
                  DelAlarmCode(ERR_VACUUM);
          }

    }
    else
    {
          DelAlarmCode(ERR_VACUUM);
    }

    //调试模式下，。如果不满足分析腔室的真空，就会关闭倍增器开关
    if(g_clMnt.m_uchRunMode == DEVICE_RUN_MODE_DEBUG)
    {
        if((st_mMntPara.st_VacuValue2.f_RealVal > 0.009) || (st_mMntPara.st_VacuValue2.f_RealVal == 0))
        {
            if(g_clIOCtrl.m_stIOCtrl.uch_EMIO == 1)
            {

                OperateSwitch(EM_SW,SWITCH_OFF);
                g_clIOCtrl.m_stIOCtrl.uch_EMIO = 0;
                qDebug("Vacuum not meet EMIO close");
            }
        }
    }


    //主板与pc通讯异常
    if(uch_PCCommCnt > 60*2) //1min
    {
          DeviceStandby();
          PushAlarmCode(ERR_PCCOMM);
    }
    else
    {
          uch_PCCommCnt++;

          DelAlarmCode(ERR_PCCOMM);
    }

    //主板与环境板通信异常
    if(g_clCircu.m_bCommAlarm == TRUE)
    {
        if(g_clMnt.m_uchRunMode == DEVICE_RUN_MODE_NORMAL)
        {
            DeviceStandby();
        }
        PushAlarmCode(ERR_CRICUCOM);
    }
    else
    {
        DelAlarmCode(ERR_CRICUCOM);
    }

    INT8U j= 0;
    for(int i=0;i<10;i++)
    {
        if(m_stEnvMnt.auch_AlarmCode[i] == 0x03 //分子泵转速异常
        || m_stEnvMnt.auch_AlarmCode[i] == 0x04 //分子泵有Error
        || m_stEnvMnt.auch_AlarmCode[i] == 0x0e //分子泵通信异常
        || m_stEnvMnt.auch_AlarmCode[i] == 0x0f //环境板与雾化气MFC通信异常
        || m_stEnvMnt.auch_AlarmCode[i] == 0x11 //环境板与反吹气MFC通信异常
        || m_stEnvMnt.auch_AlarmCode[i] == 0x12 //环境板与去溶剂气MFC通信异常
        || m_stEnvMnt.auch_AlarmCode[i] == 0x13 //环境板与去碰撞气MFC通信异常
        )
        {
            if(g_clMnt.m_uchRunMode == DEVICE_RUN_MODE_NORMAL)
            {
               j++;
            }

            //qDebug("Circu Err to standby %d",m_stEnvMnt.auch_AlarmCode[i]);
        }
        else if(m_stEnvMnt.auch_AlarmCode[i] != 0)
        {
            //qDebug("Circu Err to not standby %d",m_stEnvMnt.auch_AlarmCode[i]);
        }


    }
    if(j>0)
    {
         DeviceStandby();
    }


    //MKS真空归通信异常----------------------
    if(g_clMnt.m_uchCIDVacuumSelect == UNIT_CIDVACUUMGAUGE_MKS)
    {


        if(g_clMKSVacuometerDevice1.m_bErrCom == TRUE)
        {
            if(g_clMnt.m_uchRunMode == DEVICE_RUN_MODE_NORMAL)
            {
                DeviceStandby();
            }
            PushAlarmCode(st_mMntPara.st_VacuCommErr.uch_FaultCode);

        }
        else
        {
            DelAlarmCode(st_mMntPara.st_VacuCommErr.uch_FaultCode);
        }
    }
    else
    {
        DelAlarmCode(st_mMntPara.st_VacuCommErr.uch_FaultCode);
    }



    //qDebug("set emio  coro %x ,%x",g_clIOCtrl.m_stIOCtrl.uch_EMIO,g_clIOCtrl.m_stIOCtrl.uch_CoroIO);
    OperateRunFlag(GATE_ON_FLAG);//更新离子源门控，程序设计多余
    MemoryRead(FPGA_REG,GATE_ON_FLAG_ADDR,&uin_Data);
    //qDebug("fpga io ctrl %x",uin_Data);
    g_clIOCtrl.m_stIOCtrl.uch_CoroIO = (uin_Data & 0x0002) >> 1;
    g_clIOCtrl.m_stIOCtrl.uch_EMIO = (uin_Data & 0x0004) >> 2;
    uin_Data = uin_Data & 0x0001; //离子源是否安装标志位
    //qDebug("read emio  coro %x ,%x",g_clIOCtrl.m_stIOCtrl.uch_EMIO,g_clIOCtrl.m_stIOCtrl.uch_CoroIO);





    uch_GateOnCur = uin_Data;

    //-------------------------------------离子源 未安装报警------------------------------------
    STRFPISEND st_FpiSend;

    if(uch_FirstGateOnSet == 1)
    {
        uch_FirstGateOnSet = 0;
        uch_GateOnCur = uin_Data;
        uch_GateOnLast = uin_Data;

    }

    if (uin_Data)
    {

          //离子源插上 恢复 上次状态
          if(uch_GateOnLast == 0 && uch_GateOnCur == 1 )
          {

                if(g_clIOCtrl.m_stIOCtrl.uch_PumpIO == 0) //onlyt stanby can recover
                {
                    st_FpiSend.uch_DestAddr = ADDR_CIRCU;
                    st_FpiSend.uch_Cmd = 0x23;
                    st_FpiSend.uch_AddCmd = WRITE;
                    st_FpiSend.uch_DataLen = 2;
                    st_FpiSend.puch_Data[0] = 0x0c;
                    st_FpiSend.puch_Data[1] = b_MSStateBeforeGateOff*0x11;
                    gm_Can0Fpi.PushComm(&st_FpiSend);


                    g_clIOCtrl.m_stIOCtrl.uch_PumpIO = b_MSStateBeforeGateOff;
                    if(g_clIOCtrl.m_stIOCtrl.uch_PumpIO == 1)
                    {
                        OperateRunFlag(ONE_KEY_ON);
                    }



                    //恢复真空接口加热
                    st_FpiSend.uch_DestAddr = ADDR_CIRCU;
                    st_FpiSend.uch_Cmd = 0x27;
                    st_FpiSend.uch_AddCmd = WRITE;
                    st_FpiSend.uch_DataLen = 3;
                    st_FpiSend.puch_Data[0] = 0x01;
                    EncodeFloat(f_IonicTempBeforeGateOff,st_FpiSend.puch_Data+1);
                    gm_Can0Fpi.PushComm(&st_FpiSend);
                }
                qDebug("ionic source connect");
          }

          DelAlarmCode(st_mMntPara.st_GateONErr.uch_FaultCode );
    }
    else
    {
          //离子源挪开。
          if(uch_GateOnLast == 1 && uch_GateOnCur == 0)
          {
                  //离子源拿开后的联动
                  b_MSStateBeforeGateOff = g_clIOCtrl.m_stIOCtrl.uch_PumpIO;
                  f_IonicTempBeforeGateOff = g_clMnt.m_stEnvMnt.f_IonicTempSet;

                    //关闭真空接口加热
                    st_FpiSend.uch_DestAddr = ADDR_CIRCU;
                    st_FpiSend.uch_Cmd = 0x27;
                    st_FpiSend.uch_AddCmd = WRITE;
                    st_FpiSend.uch_DataLen = 3;
                    st_FpiSend.puch_Data[0] = 0x01;
                    EncodeFloat(5,st_FpiSend.puch_Data+1);
                    gm_Can0Fpi.PushComm(&st_FpiSend);
                    qDebug("ionic source disconnect");
          }

          DeviceStandby();


          PushAlarmCode(st_mMntPara.st_GateONErr.uch_FaultCode );
    }

    uch_GateOnLast = uch_GateOnCur;




    // qDebug("b_MSStateBeforeGateOff %d f_IonicTempBeforeGateOff %f uch_PumpIO %d circu state%d",\
        b_MSStateBeforeGateOff,f_IonicTempBeforeGateOff,g_clIOCtrl.m_stIOCtrl.uch_PumpIO,g_clMnt.m_stEnvMnt.b_CircuStandbyIO);



    //----------------------------------设置电压和反馈电压报警-----------------------
    STRMnt *ptr = new STRMnt;
    memcpy(ptr, &st_mMntPara, sizeof(STRMnt));
    //读取设定电压反馈值
    UpdateVoltageCheckListRead(ptr, QDateTime::currentDateTime().toTime_t());
    delete ptr;

    //检验设定电压反馈是否与设定一致
    INT32U ul_LensErr = CheckVoltage();
    STRCheckVoltageList *pstrCheckVol =  GetCheckVoltageListPtr();

    for(int i=0;i<MAX_VOL_CHANNEL-1;i++) //设置电压报警
    {
          INT32U ul_ErrBit = ul_LensErr&(0x0001<<i);
          if(ul_ErrBit == (0x0001<<i))
          {

                  PushAlarmCode(pstrCheckVol[i].uch_AlarmCode);
          }
          else
          {
                  DelAlarmCode(pstrCheckVol[i].uch_AlarmCode);
          }

    }



    //灯板程序
    LEDAlarmProcess();



}


//==================================================================================================
//| 函数名称 | LED ALARM
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | LED提醒报警
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.10.23
//==================================================================================================
void CMnt::LEDAlarmProcess()
{
    //----------------------------------面板等工作逻辑-------------------------
    INT8U uch_ErrNum = GetErrNum();
    INT8U uch_AlarmNum = GetAlarmNum();
    INT8U uch_CircuAlarmNum = GetCircuAlarmNum();

    //ERR LED
    if ( uch_ErrNum > 0 ) //有ERROR就闪烁
    {
          g_clIOCtrl.m_stIOCtrl.uch_ErrLed = LED_SHINE;
    }
    else if(uch_AlarmNum > 0) //有alarm 就常亮
    {
          g_clIOCtrl.m_stIOCtrl.uch_ErrLed = LED_ON;
    }
    else
    {
          g_clIOCtrl.m_stIOCtrl.uch_ErrLed = LED_OFF;
    }




    //RDY LED

    if(
          (g_clIOCtrl.m_stIOCtrl.uch_PumpIO)  //在非待机状态下
          && (m_stEnvMnt.strPumpSpeed.f_RealVal >  999) //TODO: 分子泵转速大于配置转速R（999）
          && (m_stEnvMnt.strPumpCurrent.f_RealVal < 6)//TODO:分子泵电流小于Ipump（6）

          && uch_CircuAlarmNum == 0//环境板没有错误 （气体和温度无错误）
          && m_stEnvMnt.b_N2Valve == 1 //环境板打开了喷雾器电磁阀开关
          && uch_ErrNum == 0 //仪器无err

          )
    {

          g_clIOCtrl.m_stIOCtrl.uch_RdyLed = LED_ON;//准备好
    }
    else if((g_clIOCtrl.m_stIOCtrl.uch_PumpIO)  //在非待机状态下 满足以下任一条件
          && ((m_stEnvMnt.strPumpSpeed.f_RealVal < 999) //TODO: 分子泵转速小于配置转速R（999）并且大于0
                && (m_stEnvMnt.strPumpSpeed.f_RealVal >  0))
          || (m_stEnvMnt.strPumpCurrent.f_RealVal > 6)
          || (uch_CircuAlarmNum > 0) //环境板有报警
          )
    {
          g_clIOCtrl.m_stIOCtrl.uch_RdyLed = LED_SHINE;//正在准备
    }
    else
    {
          g_clIOCtrl.m_stIOCtrl.uch_RdyLed = LED_OFF;//仪器未进行准备工作
    }


    //RUN LED
    if (
          ( (st_mMntPara.st_Rf1Current.f_RealVal+st_mMntPara.st_Rf2Current.f_RealVal) > 0.22)


          && (st_mMntPara.st_Rf1Current.f_RealVal < 2)

          && (st_mMntPara.st_Rf2Current.f_RealVal < 2)

          )
    {
          g_clIOCtrl.m_stIOCtrl.uch_RunLed = LED_SHINE;

    }
    else
    {
          g_clIOCtrl.m_stIOCtrl.uch_RunLed = LED_ON;
    }
}





//==================================================================================================
//| 函数名称 | FPGAADRead
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得FPGA获取的模拟电压
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.10.23
//==================================================================================================
INT8U CMnt::FPGAADRead()
{
    FP32 f_Temp = 0;
    INT16U uin_Data = 0;

    OperateRunFlag(AD_AD7689_CS1_1_syn);
    //样品锥电压-->采样锥电压==采样锥电压
    ReadAD(AD_AD7689_CS1_1_addr, &f_Temp, AD_AD7689_CS1_1_syn);
    st_mMntPara.st_SampleConeV.f_RealVal = (f_Temp * 2 - 2.5) * 503/3;

    //萃取锥电压 == 萃取锥电压
    ReadAD(AD_AD7689_CS1_2_addr, &f_Temp, AD_AD7689_CS1_2_syn);
    st_mMntPara.st_ExtractorV.f_RealVal = (f_Temp * 2 - 2.5) * 51;

    //rf lens传输杆电压-->传输杆电压（RFLEN 实际上没有用到）
    ReadAD(AD_AD7689_CS1_3_addr, &f_Temp, AD_AD7689_CS1_3_syn);
    st_mMntPara.st_RfLenV.f_RealVal = (f_Temp * 2 - 2.5) * 13 / 3;

    //enternCID入口--->碰撞室入口电压 == 碰撞室入口电压
    ReadAD(AD_AD7689_CS1_4_addr, &f_Temp, AD_AD7689_CS1_4_syn);
    st_mMntPara.st_CIDEntranceV.f_RealVal = ( f_Temp * 2 - 2.5) * 51;

    //em倍增器电压---->毛细管电流（CORO_I_MON）
    ReadAD(AD_AD7689_CS1_5_addr, &f_Temp, AD_AD7689_CS1_5_syn);
    st_mMntPara.st_CapillaryI.f_RealVal = f_Temp/0.0248;

    //打拿极电压->检测器倍增电压
    ReadAD(AD_AD7689_CS1_6_addr, &f_Temp, AD_AD7689_CS1_6_syn);
    st_mMntPara.st_EmHighV.f_RealVal = f_Temp * 23.3 * 800 / 3.3;

    //倍增器入口电压kv-->检测器入口电压
    ReadAD(AD_AD7689_CS1_7_addr, &f_Temp, AD_AD7689_CS1_7_syn);
    st_mMntPara.st_EmHighEntranceV.f_RealVal = f_Temp * 23.3 * 800 / 3.3;

    //毛细管电压kv//电晕针电流uA
    ReadAD(AD_AD7689_CS1_8_addr, &f_Temp, AD_AD7689_CS1_8_syn);
    st_mMntPara.st_CapillaryV.f_RealVal = f_Temp * 23.3 * 800 / 3.3;
    st_mMntPara.st_CoroI.f_RealVal = f_Temp * 13.3 * 10 / 3.3;



    //CID碰撞能量--->碰撞室碰撞能量 == 碰撞室碰撞能量
    ReadAD(AD_AD7689_CS2_1_addr, &f_Temp, AD_AD7689_CS2_1_syn);
    st_mMntPara.st_CIDV.f_RealVal = (f_Temp * 2 - 2.5 ) * 51;

    //CID出口电压--->碰撞池出口电压 == 碰撞池出口电压
    ReadAD(AD_AD7689_CS2_2_addr, &f_Temp, AD_AD7689_CS2_2_syn);
    st_mMntPara.st_CIDEixtV.f_RealVal = (f_Temp * 2 - 2.5 ) *51;

    //MS1_RF电压
    ReadAD(AD_AD7689_CS2_3_addr, &f_Temp, AD_AD7689_CS2_3_syn);
    st_mMntPara.st_Ms1RF.f_RealVal = f_Temp * 3;

    //MS2_RF电压
    ReadAD(AD_AD7689_CS2_4_addr, &f_Temp, AD_AD7689_CS2_4_syn);
    st_mMntPara.st_Ms2RF.f_RealVal = f_Temp * 3;

    //MS1_DCN电压,lz15.07.15
    ReadAD(AD_AD7689_CS2_5_addr, &f_Temp, AD_AD7689_CS2_5_syn);
    st_mMntPara.st_Ms1DCN.f_RealVal = 608.53 * (f_Temp -1.88);


    //MS1_DCP电压,lz15.07.15
    ReadAD(AD_AD7689_CS2_6_addr, &f_Temp, AD_AD7689_CS2_6_syn);
    st_mMntPara.st_Ms1DCP.f_RealVal = 608.53 * (f_Temp -1.88);

    //MS2_DCN电压,lz15.07.15
    ReadAD(AD_AD7689_CS2_7_addr, &f_Temp, AD_AD7689_CS2_7_syn);
    st_mMntPara.st_Ms2DCN.f_RealVal = 608.53 * (f_Temp -1.88);

    //MS2_DCP电压,lz15.07.15
    ReadAD(AD_AD7689_CS2_8_addr, &f_Temp, AD_AD7689_CS2_8_syn);
    st_mMntPara.st_Ms2DCP.f_RealVal = 608.53 * (f_Temp -1.88);

    st_mMntPara.st_Ms1IonEnergy.f_RealVal = st_mMntPara.st_Ms1DCP.f_RealVal;
    st_mMntPara.st_Ms2IonEnergy.f_RealVal = st_mMntPara.st_Ms2DCP.f_RealVal;

    ReadAD(AD_AD7689_CS3_1_addr, &f_Temp, AD_AD7689_CS3_1_syn);
    st_mMntPara.ESourcePositive5VDigital.f_RealVal = f_Temp * 2;

    //主板电源+5VD 异常
    INT8U rtn = m_pclESourcePositive5VDigital->RangeJudge(st_mMntPara.ESourcePositive5VDigital.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.ESourcePositive5VDigital.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.ESourcePositive5VDigital.uch_FaultCode);
    }

    ReadAD(AD_AD7689_CS3_2_addr, &f_Temp, AD_AD7689_CS3_2_syn);
    st_mMntPara.ESourcePositive1Point2VDigital.f_RealVal = f_Temp;

    //主板电源+1.2vd 异常
    rtn = m_pclESourcePositive1Point2VDigital->RangeJudge(st_mMntPara.ESourcePositive1Point2VDigital.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.ESourcePositive1Point2VDigital.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.ESourcePositive1Point2VDigital.uch_FaultCode);
    }


    ReadAD(AD_AD7689_CS3_3_addr, &f_Temp, AD_AD7689_CS3_3_syn);
    st_mMntPara.ESourcePositive3Point3VDigital.f_RealVal = f_Temp * 2;
    //主板电源+3.3VD异常
    rtn = m_pclESourcePositive3Point3VDigital->RangeJudge(st_mMntPara.ESourcePositive3Point3VDigital.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.ESourcePositive3Point3VDigital.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.ESourcePositive3Point3VDigital.uch_FaultCode);
    }

    ReadAD(AD_AD7689_CS3_4_addr, &f_Temp, AD_AD7689_CS3_4_syn);
    st_mMntPara.ESourceNegative5VAnalog.f_RealVal = -1 * f_Temp * 3;
    //主板电源-5VA 异常
    rtn = m_pclESourceNegative5VAnalog->RangeJudge(st_mMntPara.ESourceNegative5VAnalog.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.ESourceNegative5VAnalog.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.ESourceNegative5VAnalog.uch_FaultCode);
    }


    ReadAD(AD_AD7689_CS3_5_addr, &f_Temp, AD_AD7689_CS3_5_syn);
    st_mMntPara.ESourcePositive5VAnalog.f_RealVal = f_Temp * 3;
    //主板电源+5VA异常
    rtn = m_pclESourcePositive5VAnalog->RangeJudge(st_mMntPara.ESourcePositive5VAnalog.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.ESourcePositive5VAnalog.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.ESourcePositive5VAnalog.uch_FaultCode);
    }

    ReadAD(AD_AD7689_CS3_6_addr, &f_Temp, AD_AD7689_CS3_6_syn);
    st_mMntPara.ESourceNegative14VAnalog.f_RealVal = -1 * f_Temp * 10;
    //主板电源-14VA 异常
    rtn = m_pclESourceNegative14VAnalog->RangeJudge(st_mMntPara.ESourceNegative14VAnalog.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.ESourceNegative14VAnalog.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.ESourceNegative14VAnalog.uch_FaultCode);
    }

    ReadAD(AD_AD7689_CS3_7_addr, &f_Temp, AD_AD7689_CS3_7_syn);
    st_mMntPara.ESourcePositive14VAnalog.f_RealVal = f_Temp * 11;
    //主板电源 +14VA 异常
    rtn = m_pclESourcePositive14VAnalog->RangeJudge(st_mMntPara.ESourcePositive14VAnalog.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.ESourcePositive14VAnalog.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.ESourcePositive14VAnalog.uch_FaultCode);
    }

    ReadAD(AD_AD7689_CS3_8_addr, &f_Temp, AD_AD7689_CS3_8_syn);
    //st_mMntPara.ESourcePositive3Point3VAnalog.f_RealVal = f_Temp * 2;

    ReadAD(AD_AD7689_CS4_1_addr, &f_Temp, AD_AD7689_CS4_1_syn);//--->参考电压2.5v //here dft
    st_mMntPara.Reference2Point5VOne.f_RealVal = f_Temp * 2;
    //主板参考电压1， 2.5V 异常
    rtn = m_pclReference2Point5VOne->RangeJudge(st_mMntPara.Reference2Point5VOne.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.Reference2Point5VOne.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.Reference2Point5VOne.uch_FaultCode);
    }

    ReadAD(AD_AD7689_CS4_2_addr, &f_Temp, AD_AD7689_CS4_2_syn);//--->保留== RF1_CUR_MON
    st_mMntPara.st_Rf1Current.f_RealVal = f_Temp * 10.0 / 3.0 ;

    ReadAD(AD_AD7689_CS4_3_addr, &f_Temp, AD_AD7689_CS4_3_syn);//--->参考电压1.25V== 主板温度(temp)(去掉1.25v参考电压的报警码)
    st_mMntPara.st_Temperta.f_RealVal =  CalOnTemp(f_Temp);
    //主板温度异常
    rtn = m_pclTemperta->RangeJudge(st_mMntPara.st_Temperta.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.st_Temperta.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.st_Temperta.uch_FaultCode);
    }

    ReadAD(AD_AD7689_CS4_4_addr, &f_Temp, AD_AD7689_CS4_4_syn);//--->RF源电压 == RF源电压 公式不变
    st_mMntPara.RFPowerESourceVoltage.f_RealVal =  f_Temp * 24;
    //qDebug("stmnt para  %f",st_mMntPara.RFPowerESourceVoltage.f_RealVal);

    //RF供电电源电压异常
    rtn = m_pclRFPowerESourceVoltage->RangeJudge(st_mMntPara.RFPowerESourceVoltage.f_RealVal);
    if(JUDGE_RESULT_FALSE == rtn)
    {
        PushAlarmCode(st_mMntPara.RFPowerESourceVoltage.uch_FaultCode);
    }
    else if(JUDGE_RESULT_OK == rtn)
    {
        DelAlarmCode(st_mMntPara.RFPowerESourceVoltage.uch_FaultCode);
    }


    ReadAD(AD_AD7689_CS4_5_addr, &f_Temp, AD_AD7689_CS4_5_syn);//--->主板温度 ---> RF2电流监控
    st_mMntPara.st_Rf2Current.f_RealVal = f_Temp  *10.0 /3.0;



    ReadAD(AD_AD7689_CS4_6_addr, &f_Temp, AD_AD7689_CS4_6_syn);///--->信号版温度 == 保留
    ReadAD(AD_AD7689_CS4_7_addr, &f_Temp, AD_AD7689_CS4_7_syn);//--->ESI源电压
    ReadAD(AD_AD7689_CS4_8_addr, &f_Temp, AD_AD7689_CS4_8_syn);//--->2.5v参考电压




    ReadAD(AD_AD7689_CS5_1_addr, &f_Temp, AD_AD7689_CS5_1_syn);//--->MS2预杆电压 == MS2预杆电压
    st_mMntPara.st_Ms2PreV.f_RealVal = (f_Temp * 2 - 2.5 ) * 51;

    ReadAD(AD_AD7689_CS5_2_addr, &f_Temp, AD_AD7689_CS5_2_syn);//HV倍增器偏转电压--->检测器偏转电压 == 检测器偏转电压
    st_mMntPara.st_EmHighOffsetV.f_RealVal = (f_Temp * 2 - 2.5) * 503/3;

    ReadAD(AD_AD7689_CS5_3_addr, &f_Temp, AD_AD7689_CS5_3_syn);//HV1(出口透镜)--->传输杆出口透镜 == 传输杆出口透镜
    st_mMntPara.st_HV1.f_RealVal = (f_Temp *2 -2.5)* 13/3;

    ReadAD(AD_AD7689_CS5_4_addr, &f_Temp, AD_AD7689_CS5_4_syn);//HV2--->（？？？？）

    f_Temp = (f_Temp - 1.25) * 200;
    f_Temp = 0.001*f_Temp*f_Temp + 1.038*f_Temp-0.851;
    st_mMntPara.st_HV2.f_RealVal = f_Temp;

    ReadAD(AD_AD7689_CS5_5_addr, &f_Temp, AD_AD7689_CS5_5_syn);//--->高真空归
    f_Temp = f_Temp * 4;

    //100kou 的高真空规---------
    //PRT225LEYBOLD
    //st_mMntPara.st_VacuValue2.f_RealVal = pow(10.0, (f_Temp - 12.66) / 1.33);

    //PTR90NLEYBOLD   分析腔室
    st_mMntPara.st_VacuValue2.f_RealVal = pow(10.0, (f_Temp - 6.8) / 0.6)/1.33; //单位为Torr

    //27kou 的普通真空规  传输杆腔室
    st_mMntPara.st_VacuValue1.f_RealVal = pow(10.0, (f_Temp - 6.143) /1.286);

    ReadAD(AD_AD7689_CS5_6_addr, &f_Temp, AD_AD7689_CS5_6_syn);//---->驱动板1温度????==MS1预感电压
    st_mMntPara.st_Ms1PreV.f_RealVal = (f_Temp *2-2.5)*51;

    ReadAD(AD_AD7689_CS5_7_addr, &f_Temp, AD_AD7689_CS5_7_syn);//---->驱动板2温度 == 保留
    ReadAD(AD_AD7689_CS5_8_addr, &f_Temp, AD_AD7689_CS5_8_syn);//--->保留 ==  保留




    ReadAD(AD_AD7689_CS6_1_addr, &f_Temp, AD_AD7689_CS1_1_syn);//DC1 改为 Focus
    st_mMntPara.st_LenthDC1.f_RealVal = (f_Temp *2 -2.5)*503/3;
    ReadAD(AD_AD7689_CS6_2_addr, &f_Temp, AD_AD7689_CS1_1_syn);//DC2 改为 QJet
    st_mMntPara.st_LenthDC2.f_RealVal = (f_Temp *2 -2.5)*51;
    ReadAD(AD_AD7689_CS6_3_addr, &f_Temp, AD_AD7689_CS1_1_syn);//DC3 改为 Q00b
    st_mMntPara.st_LenthDC3.f_RealVal = (f_Temp *2 -2.5)*13/3;
    ReadAD(AD_AD7689_CS6_4_addr, &f_Temp, AD_AD7689_CS1_1_syn);//DC4 改为 Q00out
    st_mMntPara.st_LenthDC4.f_RealVal = (f_Temp *3 -5)*13/3;
    ReadAD(AD_AD7689_CS6_5_addr, &f_Temp, AD_AD7689_CS1_1_syn);//保留
    ReadAD(AD_AD7689_CS6_6_addr, &f_Temp, AD_AD7689_CS1_1_syn);// 信号版温度
    //st_mMntPara.st_SignBoardTemperatrue.f_RealVal = CalOnTemp(f_Temp);
    ReadAD(AD_AD7689_CS6_7_addr, &f_Temp, AD_AD7689_CS1_1_syn);//模拟量检测器信号采集
    st_mMntPara.st_AnologSignal.f_RealVal = f_Temp;

    ReadAD(AD_AD7689_CS5_7_addr, &f_Temp, AD_AD7689_CS1_1_syn); //挡板电压
    st_mMntPara.st_BaffleVoltage.f_RealVal = (f_Temp * 23.3 * 800 / 3.3);//挡板电压

    ReadAD(AD_AD7689_CS5_8_addr, &f_Temp, AD_AD7689_CS1_1_syn); //挡板电流
    st_mMntPara.st_BaffleCurrunt.f_RealVal = (f_Temp/0.0248f);
    ReadAD(AD_AD7689_CS6_5_addr, &f_Temp, AD_AD7689_CS1_1_syn); //Q0out2
    st_mMntPara.st_Q0out2.f_RealVal= (f_Temp *2 -2.5)*13/3;
    ReadAD(AD_AD7689_CS6_8_addr, &f_Temp, AD_AD7689_CS1_1_syn); //DC7 也就是原理图dc test
    st_mMntPara.st_DC7.f_RealVal = (f_Temp *2 -2.5)*13/3;


}

//==================================================================================================
//| 函数名称 | GetErrNum
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得当前仪器的错误数，主板err+环境板err
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | INT8U ERR number
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.07.15
//==================================================================================================
INT8U CMnt::GetErrNum()
{
    INT8U uch_Cnt = 0;
    for(int i=0; i<TOTAL_ALARM_NUM; i++)
    {
        if(auch_AlarmCode[i] > 0 && auch_AlarmCode[i] < 0X80)
        {
                uch_Cnt++;
        }

    }

    for(int  i=0; i<10; i++)
    {
        if(m_stEnvMnt.auch_AlarmCode[i] > 0 && m_stEnvMnt.auch_AlarmCode[i] < 0X80)
        {
                uch_Cnt++;
        }

    }

    return uch_Cnt;

}


//==================================================================================================
//| 函数名称 | GetAlarmNum
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得当前仪器的ALARM数，主板ALARM+环境板ALARM
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | INT8U ALARM number
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.07.15
//==================================================================================================
INT8U CMnt::GetAlarmNum()
{
    INT8U uch_Cnt = 0;
    for(int i=0; i<TOTAL_ALARM_NUM; i++)
    {
        if(auch_AlarmCode[i] >= 0x81 )
        {
                uch_Cnt++;
        }

    }

    for(int i=0; i<10; i++)
    {
        if(m_stEnvMnt.auch_AlarmCode[i] >= 0x81)
        {
                uch_Cnt++;
        }

    }

    return uch_Cnt;

}


//==================================================================================================
//| 函数名称 | GetCircuAlarmNum
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得环境板ALARM数量
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | INT8U ALARM number
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.07.15
//==================================================================================================
INT8U CMnt::GetCircuAlarmNum()
{
    INT8U uch_Cnt = 0;

    for(int i=0; i<10; i++)
    {
        if(m_stEnvMnt.auch_AlarmCode[i] >= 0x81 && m_stEnvMnt.auch_AlarmCode[i] <= 0xff)
        {
                uch_Cnt++;
        }

    }
    return uch_Cnt;

}


//==================================================================================================
//| 函数名称 | DeviceStandby
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得环境板ALARM数量
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | INT8U ALARM number
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.07.15
//==================================================================================================
void CMnt::DeviceStandby()
{
    //主板待机



    if(g_clIOCtrl.m_stIOCtrl.uch_PumpIO == 1)
    {
        OperateRunFlag(ONE_KEY_OFF);



        g_clIOCtrl.m_stIOCtrl.uch_PumpIO = 0;
        cl_App.uch_MSOpration = MS_OPRATION_STOP;

    }



//    qDebug("cirucstand io %d circu commalrm  %d",g_clMnt.m_stEnvMnt.b_CircuStandbyIO,g_clCircu.m_bCommAlarm);
    if(g_clMnt.m_stEnvMnt.b_CircuStandbyIO && (g_clCircu.m_bCommAlarm == FALSE))
    {
        //环境板待机
        STRFPISEND st_FpiSend;
        st_FpiSend.uch_DestAddr = ADDR_CIRCU;
        st_FpiSend.uch_Cmd = 0x23;
        st_FpiSend.uch_AddCmd = WRITE;
        st_FpiSend.uch_DataLen = 2;
        st_FpiSend.puch_Data[0] = 0x0c;
        st_FpiSend.puch_Data[1] = 0x00;
        gm_Can0Fpi.PushComm(&st_FpiSend);
        qDebug("CIRcu set standby");
    }

}

