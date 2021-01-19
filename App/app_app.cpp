#include "app_app.h"
#include "Mod/mod_detectdata.h"
#include "Mod/Mod_Method.h";
#include "Mod/mod_mnt.h"
#include "Pro/pro_fpga.h"
#include "App_ComFpi.h"
#include "Mod/mod_circu.h"
#include "Pub/pub_MKSVacuometer_V1_0.h"
#include "Mod/mod_ioctrl.h"
#include "Bsp/Bsp_Gpioctl.h"
#include "Mod/mod_fpgaupdate.h"
#include "Mod/mod_EI.h"
#include "Mod/mod_Q0Driver.h"

#include "Pro/pro_led.h"
#include <qdatetime.h>

app_app cl_App;
app_com cl_AppCom;

extern void vMemDump(INT8U *pData, INT16U DataLen);



#if 1  //計算PCM 解码
void run_datatest(INT32U testdata[],INT32S data_change[])
{
    //INT32S  data_change[32768] = {0};
    INT32U  i;

    for (i = 0; i < 32768; i++)
    {

        //data_change[i] = ~a_data2_1[i];
        if (testdata[i] & (0x800000))
        {
            //-
            data_change[i] = testdata[i] - 1;
            data_change[i] =(~testdata[i]) & 0xffffff;
            data_change[i] = data_change[i]*(-1);
        }
        else
        {
            //+
            data_change[i] = testdata[i];
        }
    }

    FILE *fpPCMdata = fopen("outtestdata.txt","w+");
    if (fpPCMdata == NULL)
    {
        qDebug("fopen1 ERR.");
    }
    for (i = 0; i < 32768; i++)
    {
        fprintf(fpPCMdata, "%ld\n", data_change[i]); //将数据写入文件
    }
    fclose(fpPCMdata);

    qDebug("data_change[0] = %ld",data_change[0]);
    qDebug("data_change[1] = %ld",data_change[1]);
    qDebug("data_change[2] = %ld",data_change[2]);
    qDebug("data_change[3] = %ld",data_change[3]);
    qDebug("data_change[4] = %ld",data_change[4]);

}
#endif

//==================================================================================================
//| 函数名称 | buildDateTime
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获取系统编译时间+预处理+必须放在该文件
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | Chensq 2020.08.12
//==================================================================================================
QDateTime buildDateTime(void)
{
    QString dateTime;
    dateTime += __DATE__;
    dateTime += __TIME__;
    return QLocale(QLocale::English).toDateTime(dateTime, "MMM dd yyyyhh:mm:ss");
}

//---------------------------------监控/方法下载，数据获取线程----------------------------
app_app::app_app()
{
    m_uchMSRunStatus = MS_RUNSTATUS_IDLE;
    Set(500);//
}


INT8U Getshellcmd(void)
{
    INT8U cmdstr[256] = {0x31,0x32,0x33};
    INT32U i = 0;

     //--write data to txt;
    FILE *fpPCMdata = fopen("shellcmd.txt","w+");
    if (fpPCMdata == NULL)
    {
        qDebug("fopen1 ERR.");
    }

    //--test
    fwrite(cmdstr,1,8,fpPCMdata);
    fclose(fpPCMdata);


    fpPCMdata = fopen("shellcmd.txt","r+");
    if (fpPCMdata == NULL)
    {
        qDebug("fopen1 ERR.");
    }

    fread(cmdstr,1,8,fpPCMdata);
    for (i = 0; i < 8; i++)
    {
        qDebug("cmdstr[%d] = %s,",i,cmdstr[i]);

    }
    fclose(fpPCMdata);
}



//==================================================================================================
//| 函数名称 | run
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 主要运行程序
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
void app_app::run()
{
    INT16U rfpga_data[32] = {0};
    INT16U uin_Test[2] = {0};
    INT8U  wflag = 0;
    qDebug() << " app_app::run()2\n";
    //qDebug("------------------------------FPGA FLASH TEST -----------------------------start");
    //INT8U uch_Temp = FlashSelfTest();
    //qDebug("------------------------------FPGA FLASH TEST %d----------------------------",uch_Temp);

    static QString buildtime = buildDateTime().toString("yyyy.MM.dd hh:mm:ss");

    qDebug() << "System creat time record:";
    qDebug() << buildtime;                          //打印系统编译时间

    //--test
    //--Getshellcmd();
    LedQuickSpark();

    qDebug(" app_app::run @2020.10.19. for normal data.\n");

//--初始化的时候配置FPGA的环境参数 DA1-4 dealy等
//    uin_Test[0] = 0x6b8-5;                            //DA3 1.05V
//    cl_bspFpga.WriteToFPGA(0x0034,uin_Test,1);
//    cl_bspFpga.WriteToFPGA(0x0036,uin_Test,1);        //high
//    usleep(1000);
//    cl_bspFpga.WriteToFPGA(0x0038,uin_Test,1);        //low

    //time delay set
    //usleep(200000);
//    uin_Test[0] = 0x05;
//    uin_Test[1] = 0xdc;                               //=1500ms
//    cl_bspFpga.WriteToFPGA(0x003a,&uin_Test[1],1);
//    cl_bspFpga.WriteToFPGA(0x003b,&uin_Test[0],1);


//    uin_Test[0] = 41;                               //DA4 12V
//    usleep(200000);
//    cl_bspFpga.WriteToFPGA(0x0035,uin_Test,1);
//    cl_bspFpga.WriteToFPGA(0x0037,uin_Test,1);      //high
//    usleep(1000);
//    cl_bspFpga.WriteToFPGA(0x0039,uin_Test,1);      //low



    while(true)
    {
        if(TmrOut()) //500ms定时
        {
            //-- test FPGA RAM Read --//
//            rfpga_data[0] = 0x55;
//            rfpga_data[1] = 0x66;
//            rfpga_data[2] = 0x77;
//            rfpga_data[3] = 0x88;
//            cl_bspFpga.WriteToFPGA(0x4000,rfpga_data,10);
//            qDebug("MemoryWrite AD_DATA:");
//            vMemDump((INT8U *)rfpga_data,5);

//            cl_bspFpga.ReadFromFPGA(0x000,rfpga_data,32);
//            qDebug("MemoryRead AD_DATA:");
//            vMemDump((INT8U *)rfpga_data,16);

   #if 0
            gpio_ctl(FPGA_EN,1,1); //关闭FPGA

            rfpga_data[0] = 0;
            rfpga_data[1] = 0;


            //--cl_bspFpga.ReadFromFPGA(0x0010,rfpga_data,8);
            //MemoryRead(FPGA_REG, 0x10, rfpga_data,16);
            qDebug("MemoryRead AD_DATA:");
            vMemDump((INT8U *)rfpga_data,8);

            MemoryRead(DSRAM_DATA, 2, uin_Test,2);
            qDebug("MemoryRead DSRAM_DATA:");
            qDebug("%x",uin_Test[0]);
            qDebug("%x",uin_Test[1]);

            MemoryRead(FPGA_REG, 2, uin_Test,2);
            qDebug("MemoryRead FPGA_REG:");
            qDebug("%x",uin_Test[0]);
            qDebug("%x",uin_Test[1]);

            MemoryRead(DSRAM_TEST, 2, uin_Test,2);
            qDebug("MemoryRead DSRAM_TEST:");
            qDebug("%x",uin_Test[0]);
            qDebug("%x",uin_Test[1]);

            MemoryRead(DSRAM_RECT, 2, uin_Test,2);
            qDebug("MemoryRead DSRAM_RECT:");
            qDebug("%x",uin_Test[0]);
            qDebug("%x",uin_Test[1]);
#endif

            //qDebug("FPGA MemoryRead:");
            //vMemDump((INT8U *)rfpga_data,sizeof(rfpga_data));

            //g_clMnt.CMntProcess();
            //qDebug("Mnt");

//            Test
//            qDebug("can comm cnt %d, err cnt %d",g_clCircu.m_ulCommCnt,g_clCircu.m_ulCommErrCnt);//test circu can comm
//            qDebug("MKSVacuometer comm cnt %d, err cnt %d",g_clMKSVacuometerDevice1.m_ulCommCnt,g_clMKSVacuometerDevice1.m_ulCommErrCnt);//test 485 comm

            RUN_LedToggle();                                    // LEDs direct
            CORELED_LedToggle();
#if 0 //--LED ctr
            g_clIOCtrl.CIOCtrProcess();

            if(g_clMnt.m_uchCIDVacuumSelect == UNIT_CIDVACUUMGAUGE_MKS)
            {
                g_clMKSVacuometerDevice1.MKSVacuometerCtrlReadVacuumTrigger();
            }
            //led run
            static BOOL b_LEDState;
            if(b_LEDState)
            {
                b_LEDState = 0;
                //qDebug("led on");
                gpio_ctl(GPIO_LED,1,1);
            }
            else
            {
                b_LEDState = 1;
                //qDebug("led off");
                 gpio_ctl(GPIO_LED,1,0);
            }
#endif
        }

#if 0
        if(g_clMnt.m_uchDeviceModel == DEVICE_MODEL_EXPEC5250)
        {
            g_clEI.EIProcess();
            g_clQ0Driver.Q0DriverProcess();
        }

        g_clCircu.CircuProcess();

        if(g_clMnt.m_uchCIDVacuumSelect == UNIT_CIDVACUUMGAUGE_MKS)
        {
            MKSVacuometerProcess();
        }
//        msleep(1);
#endif

        #if 0
        g_clDetectData.Process();                               // 检测FPGA数据准备情况
        if(uch_MSOpration == MS_OPRATION_START)
        {
            uch_MSOpration = MS_OPRAITON_NULL;

            OperateRunFlag(LC_MS_SYN_CLEAR_FLAG);//每次启动前先关闭
            INT16U uin_Data = 0;
            cl_gMethod.SetMsState(0);

            g_clDetectData.Init();
            OperateRunFlag(METHOD_RUN_FLAG);
            MemoryRead(FPGA_REG,METHOD_RUN_FLAG_addr,&uin_Data);

            qDebug("Get METHOD_RUN_FLAG %d",uin_Data);
            while(uin_Data == 1)
            {
                OperateRunFlag(METHOD_RUN_FLAG);
                MemoryRead(FPGA_REG,METHOD_RUN_FLAG_addr,&uin_Data);

                msleep(5);

            }
            qDebug("START DOWLOAD");
            cl_gMethod.DownLoadMethod();
            qDebug("Download method zz");
            cl_gMethod.SetMsState(1);
            qDebug("MS start");
            m_uchMSRunStatus = MS_RUNSTATUS_RUN;

        }
        else if(uch_MSOpration == MS_OPRATION_STOP)
        {
            uch_MSOpration = MS_OPRAITON_NULL;
            OperateRunFlag(LC_MS_SYN_CLEAR_FLAG);
            cl_gMethod.SetMsState(0);
            qDebug("ENTER_standby");

            m_uchMSRunStatus = MS_RUNSTATUS_IDLE;
        }
        #endif

    }
}

//-----------------------------------通信处理线程-----------------------
app_com::app_com()
{

}





void app_com::run()
{
    //qDebug(" app_com::run.\n");
    while(true)
    {
        a_ComFpi.Process();
        msleep(1);
    }
}


void vMemDump(INT8U *pData, INT16U DataLen)
{
    INT16U i;

    for (i = 0; i < DataLen; i++)
    {
        qDebug("%d:%02x ",i,pData[i]);
    }

}
