//==================================================================================================
//| 文件名称 | Mod_Method.cpp
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 扫描时序方法
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2019, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.2    | 2019.08.16  |lyb      | 移植
//==================================================================================================
#include "Mod_Method.h"
#include "Pro/pro_fpga.h"
#include "Pub/Pub_MyBus.h"


#include <QMutex>
#include <math.h>


QMutex mutex_FPGARdy;
INT8U guch_SectParam[SECT_MAX_NUM][SECT_BYTE_NUM] = {0};
CMethod cl_gMethod;
//==================================================================================================
//| 函数名称 |CMethod
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 构造函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lxlll 2010.09.06
//==================================================================================================

CMethod::CMethod()
{
        uch_pvPassMethod = 0;
        uch_pvSendMethodFlag = 0;   
}
//==================================================================================================
//| 函数名称 |DownLoadMethod
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 下载方法
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lxlll 2011.04.26
//==================================================================================================
void CMethod::DownLoadMethod(void)
{
        INT16U i = 0,j = 0,k = 0;
        INT8U uch_WriteNum = 0;
        //INT16U uin_SdramOffAddr = 0;
        INT8U* puch_Method = NULL;
        INT16U uin_RamAddr = 0;
        INT8U uin_InRamNum = 0;
        INT8U uch_Data = 0,uch_MethodFlag = 0;
        FP32 f_v = 0.0,f_Temp = 0.0;
        INT32U ul_Bv = 0;
        INT32U ul_Ev = 0;
        INT16U uin_Internal = 0;
        INT16U uin_Data = 0;
        INT16U uin_Temp = 0;
        INT8U uch_v[2];
        INT32U ul_SectTime = 0;
        INT32U ul_SectErrorTime = 0;
        INT32U ul_SdramAddr = 0;
        INT32U ul_SdramOffAddr = 0;
        INT32U ul_Cycle = 0;
        INT32U ul_Data = 0;
        INT16U uin_Result = 0,uin_RResult = 0;
        INT16U uin_data_total_cnt = 0;
        INT16U uin_step = 0;
        INT32U ul_pvTotalSectNum;
        INT16U uin_CurrentMethod;
        INT16U uin_CurrentSectIn1Method;


        //得到全部方法数
        ul_pvTotalSectNum = DecodeUlong(guch_SectParam[0] + ARM_TOTAL_SECT_NUM);
        ////qDebug("ul_pvTotalSectNum %d",ul_pvTotalSectNum);
        for(i=0; i<ul_pvTotalSectNum; i++)
        {
                uin_RamAddr = uin_InRamNum * MAX_IN_FPGA_LEN;
                uch_WriteNum++;
                puch_Method = guch_SectParam[i];

                //所有数据存放为大端模式 所有数据存放顺序与LC-MS协议中扫描参数顺序对应

                //----方法片段信息参数----
                ul_Data = DecodeUlong(puch_Method+ARM_TOTAL_SECT_NUM);
                uin_Data = ul_Data >> 16;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TOTAL_SECT_NUM_H,&uin_Data);
                uin_Data = ul_Data;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TOTAL_SECT_NUM_L,&uin_Data);

                uin_Data = DecodeUint(puch_Method + ARM_TOTAL_METHOD_NUM);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TOTAL_METHOD_NUM,&uin_Data);

                uin_Data = DecodeUint(puch_Method + ARM_CURRENT_METHOD_NUM);
                uin_CurrentMethod = uin_Data;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + CURRENT_METHOD_NUM,&uin_Data);



                uin_Data = DecodeUint(puch_Method + ARM_START_TIME);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + START_TIME,&uin_Data);
                INT16U uin_Start = uin_Data;
                uin_Data = DecodeUint(puch_Method + ARM_STOP_TIME);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + STOP_TIME,&uin_Data);
                INT16U uin_Stop = uin_Data;
                //qDebug("time start %d,time stop %d",uin_Start,uin_Stop);

                uin_Data = DecodeUint(puch_Method + ARM_CYCLE_TIME);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + CYCLE_TIME,&uin_Data);

                uin_Data = DecodeUint(puch_Method + ARM_METHOD_PIONT);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + METHOD_PIONT,&uin_Data);



                //------时序片段参数--------
                uin_Data = DecodeUint(puch_Method + ARM_TOTAL_SECT_NUM_IN1METHOD);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TOTAL_SECT_NUM_IN1METHOD,&uin_Data);

                uin_Data = DecodeUint(puch_Method + ARM_CURRENT_SECT_NUM_IN1METHOD);
                uin_CurrentSectIn1Method = uin_Data;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + CURRENT_SECT_NUM_IN1METHOD,&uin_Data);

                //段运行时间
                f_Temp = DecodeFloat(&puch_Method[ARM_SECT_TIME]); //上位机发送的数据单位为ms
                ul_SectTime = (INT32U)(f_Temp * 1000);				//现在改成us发送给FPGA
                uin_Data = ul_SectTime >> 16;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + SECT_TIME_H,&uin_Data);
                uin_Data = ul_SectTime;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + SECT_TIME_L,&uin_Data);

                //-------个性化控制参数---------


                if((puch_Method[ARM_ENABLE_SCAN] == 0x11) || (puch_Method[ARM_ENABLE_SCAN] == 0x33))
                {
                        uin_Data = 0x11;
                }
                else
                {
                        uin_Data = 0x00;
                }

                //扫描使能标志位放在ram中高字节
                uin_Data = (uin_Data << 8);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + ENABLE_SCAN,&uin_Data);

                //ms1_dc_en and ms2_dc_en 放在一个ram word
                uch_Data = puch_Method[ARM_MS1_DC_ENABLE];
                uin_Data = (uch_Data<<8)| puch_Method[ARM_MS2_DC_ENABLE];
                MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_AND_MS2_DC_ENABLE,&uin_Data);

                //离子源模式放在低字节
                uch_Data = puch_Method[ARM_ION_MODE];
                uin_Data = (puch_Method[ARM_EM_HIGH_SW] << 8) | uch_Data;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + EM_HIGH_SW_AND_ION_MOD,&uin_Data);

                if(puch_Method[ARM_MS1_RAMP_EN] == 0x11)//ms1 ramp en
                {
                        uch_v[0] = 0x22;

                        ul_Bv = DecodeFloat(&puch_Method[ARM_MS1_RAMP_BEGIN_V])*65535/MSRF_REFERENCE;
                        //qDebug("f_Bv%f  f_Ev%f",DecodeFloat(&puch_Method[ARM_MS1_RAMP_BEGIN_V]),DecodeFloat(&puch_Method[ARM_MS1_RAMP_END_V]));
                        uin_Data = ul_Bv;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_RAMP_BEGIN_V,&uin_Data);
                        uin_Data = ul_Bv>>16;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_RAMP_BEGIN_H_V,&uin_Data);
                        //电压差值
                        f_v = DecodeFloat(&puch_Method[ARM_MS1_RAMP_END_V]);
//                        if (f_v > 10.0)
//                        {
//                                f_v = 10.0;
//                        }

                        //ul_Ev = DecodeFloat(&puch_Method[ARM_MS1_RAMP_END_V])*65535/MSRF_REFERENCE;
                        ul_Ev = f_v*65535/MSRF_REFERENCE;
                        uin_Data = ul_Ev - ul_Bv;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_RAMP_DELTA_V,&uin_Data);
                        uin_Data = (ul_Ev - ul_Bv)>>16;
                        //qDebug("ms1 delta h%d  %d ulEv%d,ulBv%d",uin_Data,ul_Ev - ul_Bv,ul_Ev,ul_Bv);
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_RAMP_DELTA_H_V,&uin_Data);
                }
                else if(puch_Method[ARM_MS1_FIXED_EN] == 0x11)//fixed
                {
                        uch_v[0] = 0x11;

                        f_v = DecodeFloat(&puch_Method[ARM_MS1_FIXED_V]);
//                        if (f_v > 10.0)
//                        {
//                                f_v = 10.0;
//                        }

                        //ul_Bv = DecodeFloat(&puch_Method[ARM_MS1_FIXED_V])*65535/MSRF_REFERENCE;
                        ul_Bv = f_v*65535/MSRF_REFERENCE;
                        uin_Data = ul_Bv;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_RAMP_BEGIN_V,&uin_Data);
                        uin_Data = ul_Bv>>16;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_RAMP_BEGIN_H_V,&uin_Data);
                }

                if(puch_Method[ARM_MS2_RAMP_EN] == 0x11)//ms2 ramp en
                {
                        uch_v[1]  = 0x22;

                        ul_Bv = DecodeFloat(&puch_Method[ARM_MS2_RAMP_BEGIN_V])*65535/MSRF_REFERENCE;
                        uin_Data = ul_Bv;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS2_RAMP_BEGIN_V,&uin_Data);
                        uin_Data = ul_Bv>>16;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS2_RAMP_BEGIN_H_V,&uin_Data);
                        //电压差值
                        f_v = DecodeFloat(&puch_Method[ARM_MS2_RAMP_END_V]);
//                        if (f_v > 10.0)
//                        {
//                                f_v = 10.0;
//                        }

                        //ul_Ev = DecodeFloat(&puch_Method[ARM_MS2_RAMP_END_V])*65535/MSRF_REFERENCE;
                        ul_Ev = f_v*65535/MSRF_REFERENCE;
                        uin_Data = ul_Ev - ul_Bv;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS2_RAMP_DELTA_V,&uin_Data);
                        uin_Data = (ul_Ev - ul_Bv)>>16;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS2_RAMP_DELTA_H_V,&uin_Data);
                }
                else if(puch_Method[ARM_MS2_FIXED_EN] == 0x11)//fixed
                {
                        uch_v[1]  = 0x11;

                        f_v = DecodeFloat(&puch_Method[ARM_MS2_FIXED_V]);
//                        if (f_v > 10.0)
//                        {
//                                f_v = 10.0;
//                        }

                        //ul_Bv = DecodeFloat(&puch_Method[ARM_MS2_FIXED_V])*65535/MSRF_REFERENCE;
                        ul_Bv = f_v*65535/MSRF_REFERENCE;
                        uin_Data = ul_Bv;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS2_RAMP_BEGIN_V,&uin_Data);
                        uin_Data = ul_Bv>>16;
                        MemoryWrite(DSRAM_RECT,uin_RamAddr + MS2_RAMP_BEGIN_H_V,&uin_Data);
                }

                //ms1_wave_type 在高字节 ms2_wave_type 在低字节
                uin_Data = uch_v[0];
                uin_Data = (uin_Data << 8) | uch_v[1];
                MemoryWrite(DSRAM_RECT,uin_RamAddr + MS1_AND_MS2_WAVE_TYPE,&uin_Data);

                //发送点数
                uin_data_total_cnt = DecodeUint(&puch_Method[ARM_TOTAL_DATA_CNT]);
                uin_Data = uin_data_total_cnt;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + RAMP_SEND_POINTS,&uin_Data);

                //发送周期
                ul_Cycle = ul_SectTime*100/uin_data_total_cnt;
                ul_Cycle = (ul_Cycle < 1250)?1250:ul_Cycle;
                uin_Data = ul_Cycle >> 16;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + RAMP_SEND_CYCLE_H,&uin_Data);
                uin_Data = ul_Cycle;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + RAMP_SEND_CYCLE_L,&uin_Data);

                //-------增加了trans rf-----------------
                f_v = DecodeFloat(&puch_Method[ARM_TRASN_RAMP_BEGIN_V])/32/3.5;
//                if (f_v > 10.0)
//                {
//                        f_v = 10.0;
//                }
                ul_Bv = f_v*65535/MSRF_REFERENCE;
                uin_Data = ul_Bv;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TRANS_RAMP_BEGIN_V,&uin_Data);
                uin_Data = ul_Bv>>16;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TRANS_RAMP_BEGIN_H_V,&uin_Data);
                f_v = DecodeFloat(&puch_Method[ARM_TRASN_RAMP_END_V])/32/3.5;
//                if (f_v > 10.0)
//                {
//                        f_v = 10.0;
//                }
                ul_Ev = f_v*65535/MSRF_REFERENCE;
                uin_Data = ul_Ev - ul_Bv;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TRANS_RAMP_DELTA_V,&uin_Data);
                uin_Data = (ul_Ev - ul_Bv)>>16;
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TRANS_RAMP_DELTA_H_V,&uin_Data);
                //TRANS RF 从起始到转折电压的点数
                uin_Data =  DecodeUint(&puch_Method[ARM_TRANS_RAMP_CNT]);
                MemoryWrite(DSRAM_RECT,uin_RamAddr + TRANS_RAMP_SEND_POINTS,&uin_Data);

                //------------------------------------------------------------------------------
                MethodVolSet(puch_Method, uin_RamAddr);




                uin_InRamNum++;
                if(uin_InRamNum == (RAM_LEN/MAX_IN_FPGA_LEN))
                {
                       ul_SdramAddr += ul_SdramOffAddr;
                        ul_SdramOffAddr = uin_InRamNum*MAX_IN_FPGA_LEN*2;

                       uin_Data = uin_InRamNum*MAX_IN_FPGA_LEN;
                        MemoryWrite(FPGA_REG,CURRENT_DATA_LEN_addr,&uin_Data);

                        uin_Data  = ul_SdramAddr >> 16;
                        MemoryWrite(FPGA_REG,CURRENT_SDRAM_addr,&uin_Data);

                        uin_Data  = ul_SdramAddr;
                        MemoryWrite(FPGA_REG,CURRENT_SDRAM_addr + 2,&uin_Data);



                        OperateRunFlag(DOWNLOAD_METHOD_FLAG);
                        uin_InRamNum = 0;

                        //等待FPGA完成操作
                        INT16U uin_Data = 1;
                        MemoryRead(FPGA_REG,METHOD_LOAD_RDY,&uin_Data);
                        qDebug("Get METHOD_LOAD_FLAG_addr %d",uin_Data);
                        while(uin_Data == 0)
                        {
                            MemoryRead(FPGA_REG,METHOD_LOAD_RDY,&uin_Data);
                            msleep(5);
                        }


                        /*INT32U ul_TotalMethod = DecodeUlong(puch_Method+ARM_TOTAL_SECT_NUM);
                        INT16U ul_LastSendMethod = DecodeUint(puch_Method + ARM_CURRENT_METHOD_NUM);
                        CString cs_Temp;
                        cs_Temp.Format(L"Down Method %d,%d\n",ul_TotalMethod,ul_LastSendMethod);
                        AddLogWithTime(cs_Temp);*/


                        /*
                        CString cs_Temp;
                        cs_Temp.Format(L"Down Method =4");
                        AddLogWithTime(cs_Temp);
                        */

                }


        }

        if(uin_InRamNum > 0) //个别的时序片段 发送
        {
                ul_SdramAddr += ul_SdramOffAddr;

                uin_Data = uin_InRamNum*MAX_IN_FPGA_LEN;
                MemoryWrite(FPGA_REG,CURRENT_DATA_LEN_addr,&uin_Data);

                uin_Data  = ul_SdramAddr >> 16;
                MemoryWrite(FPGA_REG,CURRENT_SDRAM_addr,&uin_Data);

                uin_Data  = ul_SdramAddr;
                MemoryWrite(FPGA_REG,CURRENT_SDRAM_addr + 2,&uin_Data);


                //qDebug("DOWNLOAD_METHOD_FLAG");
                OperateRunFlag(DOWNLOAD_METHOD_FLAG);

                uin_InRamNum = 0;

                //等待FPGA完成操作
                INT16U uin_Data = 1;
                MemoryRead(FPGA_REG,METHOD_LOAD_RDY,&uin_Data);
                qDebug("seq  METHOD_LOAD_FLAG_addr %d",uin_Data);
                while(uin_Data == 0)
                {
                    MemoryRead(FPGA_REG,METHOD_LOAD_RDY,&uin_Data);
//                    qDebug("seq  METHOD_LOAD_FLAG_addr %d",uin_Data);
                    msleep(5);
                }

                /*INT32U ul_TotalMethod = DecodeUlong(puch_Method+ARM_TOTAL_SECT_NUM);
                INT16U ul_LastSendMethod = DecodeUint(puch_Method + ARM_CURRENT_METHOD_NUM);
                CString cs_Temp;
                cs_Temp.Format(L"Down Method %d,%d\n",ul_TotalMethod,ul_LastSendMethod);
                AddLogWithTime(cs_Temp);*/

                /*CString cs_Temp;
                cs_Temp.Format(L"Down Method <4");
                AddLogWithTime(cs_Temp);
                */
        }


        return ;
}



//==================================================================================================
//| 函数名称 |SaveMethod
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 保存扫描方法到ARM内存中
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 0:error,1:no error
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lxlll 2011.04.26
//==================================================================================================
INT16U CMethod::SaveMethod(INT8U *uch_pData,INT16U uin_iDataLen)//gan140324
{
        INT16U i= 0;
        INT16U uin_TotalMethodNum = 0;
        INT16U uin_CurMethodNum = 0;
        INT16U uin_CurPackNum = 0;
        INT16U uin_TotalPackNumber = 0;
        INT16U uin_CurSectNum = 0;
        INT16U uin_CurSectCnt = 0;
        INT16U uin_TotalSectNum = 0;
        static INT16U uin_BeforePackNum = 0;  //上一次包号
        static INT16U uin_BeforeSaveSectNum = 0;//上一次包 所存到的 数组号
        static INT16U ul_SaveSectNum = 0;
        if(NULL == uch_pData)
        {
                return 0;
        }

        //总包数
        uin_TotalPackNumber = DecodeInt(uch_pData);
        //当前包号
        uin_CurPackNum = DecodeInt(uch_pData+2);
        qDebug("methoid totalpack %d,curpack %d",uin_TotalPackNumber,uin_CurPackNum);
        //当前包数据所含时序片段总数
        uin_CurSectCnt = uin_iDataLen/SECT_BYTE_NUM;

        if(uin_CurPackNum == 1)
        {
                uin_BeforeSaveSectNum = 0;
                ul_SaveSectNum = 0;
        }

        if(uin_BeforePackNum != uin_CurPackNum)
        {
                uin_BeforePackNum = uin_CurPackNum;
                ul_SaveSectNum = ul_SaveSectNum;//包号不同继续 原来的 时序片段计数值
        }
        else
        {
                ul_SaveSectNum = ul_SaveSectNum - uin_BeforeSaveSectNum; //包号相同者恢复原来计数值
        }

        //ul_SaveSectNum = (uin_CurPackNum-1) * 4096 /SECT_BYTE_NUM;
        for(i = 0; i<uin_CurSectCnt; i++)
        {
                //所有方法片段数量
                uin_TotalMethodNum = DecodeUlong(uch_pData+ARM_TOTAL_METHOD_NUM + i*SECT_BYTE_NUM);

                //当前方法片段编号
                uin_CurMethodNum = DecodeInt(uch_pData+ARM_CURRENT_METHOD_NUM + i*SECT_BYTE_NUM);
                //当前方法总时序片段数
                uin_TotalSectNum = DecodeInt(uch_pData+ARM_TOTAL_SECT_NUM_IN1METHOD + i*SECT_BYTE_NUM);
                //当前时序片段号
                uin_CurSectNum = DecodeInt(uch_pData+ARM_CURRENT_SECT_NUM_IN1METHOD + i*SECT_BYTE_NUM);

                qDebug("%d %d %d %d ",uin_CurSectNum,uin_TotalSectNum,uin_CurMethodNum,uin_TotalMethodNum);
                if((uin_CurSectNum <= uin_TotalSectNum)
                        && (uin_CurSectNum > 0)
                        && (uin_TotalSectNum <= SECT_MAX_NUM)
                        && (uin_TotalSectNum > 0)
                        && (uin_CurMethodNum <= MAX_METHOD_NUM)
                        && (uin_CurMethodNum > 0))
                {
                        qDebug("Method seq get %d",ul_SaveSectNum);
                        memcpy(guch_SectParam[ul_SaveSectNum],&uch_pData[SECT_BYTE_NUM*i],SECT_BYTE_NUM);
                }
                else
                {
                        return 0;
                }
                ul_SaveSectNum++;
        }
        uin_BeforeSaveSectNum = uin_CurSectCnt; //记录上一次包中 还有的时序片段数

        return uin_CurPackNum;

}



//==================================================================================================
//| 函数名称 |ReadAGCSeting
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | pass scaning data from MCU to PC
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 0:error,1:no error
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lxlll 2011.04.26
//==================================================================================================
void CMethod::SetMsState(INT8U uch_iState)//1:run,0:stop
{
    switch(uch_iState)
    {
        case 0:
        {
             OperateRunFlag(STOP_MS_FLAG);
        }break;
        case 1:
        {
             OperateRunFlag(START_MS_FLAG);
        }break;
        default:
        break;
    }
    return;
}














