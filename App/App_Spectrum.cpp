//==================================================================================================
//| 文件名称 | App_Spectrum.cpp
//|-----------------------------------------------------------------------------------------------
//| 文件描述 | 通讯处理模块
//|-----------------------------------------------------------------------------------------------
//| 运行环境 | 所有C/C++语言编译环境，包括单片机编译器
//|-----------------------------------------------------------------------------------------------
//| 版权声明 | Copyright2017, 聚光科技(FPI)
//|------------------------------------------------------------------------------------------------
//|  版本    |  时间        |  作者   | 描述
//|--------------------------------------------------------------------------------------------
//|  V1.0   | 2018.08.16  | hb     | 初版
//==================================================================================================
#include "App_Spectrum.h"



CAppSpectrum a_Spectrum;

//==================================================================================================
//| 函数名称 | CAppSpectrum
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | CAppSpectrum构造函数
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | void
//|-------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.16
//==================================================================================================
CAppSpectrum::CAppSpectrum(void)
{
    b_ModEnable = FALSE;
    b_FirstTime = TRUE;
    uch_CmdExe = 0x00;
}

//==================================================================================================
//| 函数名称 | ~CAppSpectrum
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | ~CAppSpectrum析构函数
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | void
//|-------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.16
//==================================================================================================
CAppSpectrum::~CAppSpectrum(void)
{

}
//==================================================================================================
//| 函数名称 | ~CAppSpectrum
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | ~CAppSpectrum析构函数
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | void
//|-------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.16
//==================================================================================================
void CAppSpectrum::ModInit(void)
{
    InsmodFPGA();
    FPGAInit();
}
//==================================================================================================
//| 函数名称 | SetModEnable()
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | 主函数
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | void
//|-------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.17
//==================================================================================================
/* BOOL CAppComFpi::SetModEnable(BOOL b_Set)
 {

   //模块输入变量初始化
    b_ModEnable = b_Set;      //模块使能标志


    return TRUE;
 }*/
//==================================================================================================
//| 函数名称 | SetTask()
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | 主函数
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | void
//|-------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.17
//==================================================================================================
BOOL CAppSpectrum::SetTask(TASKINF *st_Task, INT16U *puin_TempData)
{
    INT8U i;

    if(st_Task->uch_Type == 0x01)
    {
        puin_TempData[0] = 1;
    }
    else
    {
        puin_TempData[0] = 0;
    }

    puin_TempData[1] = 0;
    puin_TempData[2] = 0;

    for(i=0;i<3;i++)
    {
        puin_TempData[3+i*2] = st_Task->uin_AveNum;
        puin_TempData[3+i*2+1] = st_Task->uin_SparkNum;
    }
    return 1;
}
//==================================================================================================
//| 函数名称 | ReadOriginalSpectrum
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | 传输原始光谱
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | NT8U uch_CmosSn,INT16U *puin_Date
//|-------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.20
//==================================================================================================
void CAppSpectrum:: ReadOriginalSpectrum(INT8U uch_CmosSn,INT8U uch_Sn,INT32U *pul_Date)
{
    INT16U i;

    for(i=0;i<PREPOINTS;i++)
    {
        pul_Date[i] = auin_OriginalSpectrumData[uch_CmosSn*VALIDPOINTS+PREPOINTS*uch_Sn+i];
        //qDebug(" Read = %x", puin_Date[i] );
        //qDebug(" Read1 = %x", auin_OriginalSpectrumData[i] );
    }

}
//==================================================================================================
//| 函数名称 | SetFpga
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | 設置Fpga參數
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | NT8U uch_CmosSn,INT16U *puin_Date
//|------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.25
//==================================================================================================
void CAppSpectrum::SetFpga(INT16U *puin_Data)
{
    INT16U uin_Temp[2],i;
        uin_Temp[0] = 1;
        uin_Temp[1] = 1;
    WriteToFPGA(FPGARERESETADDR,uin_Temp,2);
    //for(i=0;i<14;i++)
      // qDebug("  = %d",puin_Data[i]);

    WriteToFPGA(FPGAMODEADDR,puin_Data,14);

    WriteToFPGA(FPGACONFADDR,uin_Temp,2);
    while(1)
    {
       if(ReadFromFPGA(FPGACONFDNOEADDR,uin_Temp,1))
            break;
    }

    WriteToFPGA(FPGARUNADDR,uin_Temp,2);

}
//==================================================================================================
//| 函数名称 | Process
//|-------------------------------------------------------------------------------------------------
//| 函数功能 | 主函数
//|-------------------------------------------------------------------------------------------------
//| 输入参数 | void
//|-------------------------------------------------------------------------------------------------
//| 返回参数 | void
//|-------------------------------------------------------------------------------------------------
//| 函数设计 | hb,2018.08.17
//==================================================================================================
void CAppSpectrum::Process(void)
{

    INT32U i;
    static INT32U ul_cnt= 0;
    static INT16U writedate = 0;


    if(b_FirstTime == TRUE)
    {
         ModInit();
         b_FirstTime = FALSE;
         auin_TempData[0] = 1;
         auin_TempData[1] = 0;
         for(i=2;i<50;i++)
             auin_TempData[i] = 10;

    }


    switch(uch_SpectrumState)
    {
        case 0x00:
        {
           ul_cnt++;
           if(ul_cnt>10000000)
           {

               uch_SpectrumState = 0x01;
               ul_cnt = 0;
               writedate++;
               if(writedate >1000)
               {
                   writedate = 0;
               }
               for(i=0;i<15;i++)
                   auin_TempData111[i] = i+writedate;
           }



        }break;
        case 0x01:
        {

            WriteToFPGA(FPGAMODEADDR,auin_TempData111,14);
            uch_SpectrumState = 0x02;
            //qDebug(" step 1");
        }break;
        case 0x02:
        {
            ReadFromFPGA(FPGAMODEADDR,&auin_TempData[20],14);

            for(i=20;i<34;i++)
            {
               qDebug("第%d次,%d",i,auin_TempData[i]);
               qDebug()<<endl;

            }
            uch_SpectrumState = 0x00;

        }break;
        default:
        break;



    }


}