#include "mod_detectdata.h"
#include "Mod/Mod_Method.h"
#include "Pro/pro_fpga.h"
#include <QMutex>

QMutex mutex_DataBuff;

CDetectData g_clDetectData;
CDetectData::CDetectData()
{

    ul_MaxLength = 10* 1024 * 1024;//10M INT16
    ul_AutoLength = 1024 * 1024;//1M INT16
    puin_Buffer = new INT16U[ul_AutoLength];
    ul_BufferLength = ul_AutoLength;
    puin_Data = puin_Buffer;
    ul_DataLength = 0;
    uch_PackNo = 1;

    ul_RemainDataLength = 0;
    //puin_MethodPointCount = new INT16U[SECT_MAX_NUM];
    //b_isEnd = false;
    b_InitOk = FALSE;

    Init();
}


CDetectData::~CDetectData()
{
    delete[] puin_Buffer;
    //delete[] puin_MethodPointCount;
    puin_Buffer = NULL;
    puin_Data = NULL;

    //puin_MethodPointCount = NULL;
}

void CDetectData::Process()
{
    //查询是否有检测器数据
    INT16U uin_Data = 1;
    MemoryRead(FPGA_REG,DETECT_DATA_RDY,&uin_Data);

    //qDebug("Get DETECT_DATA_RDY %d",uin_Data);
    if(uin_Data == 0)
    {
        GetDatasFromFPGA();
        OperateRunFlag(ARM_READ_DATA_SUCC);

    }

}

bool CDetectData::GetDatasFromFPGA()
{
        bool b_result = false;
        INT16U uin_tmpDataLength = MAX_DATA_LEN*2;
        INT16U uin_current_method_point;


        /*if(b_isEnd)
        {
                return false;
        }*/


        mutex_DataBuff.lock(); //修改到线程中执行该锁
        //if(0 == uin_RemainDataLength)
        if (0 == ul_RemainDataLength)
        {//上次已获取方法片段的所有数据
                INT16U methodNo;
                MemoryRead(DSRAM_DATA,0,&methodNo);//获取方法片段号
                MemoryRead(DSRAM_DATA,1,&uin_current_method_point); //获取该方法片段所有的点数

                INT16U uin_Test[2];
                MemoryRead(DSRAM_DATA, 2, uin_Test,2);


                INT32U ul_curtime = uin_Test[0] + (uin_Test[1]<<16);

                //qDebug("get data from fpga ,methodNo %d method point %d,startime %d stop time%d",methodNo,uin_current_method_point,uin_Test[0],uin_Test[1]);

                //qDebug("methodNo %d method_point %d",methodNo,uin_current_method_point);
                if((methodNo <= 0) || (methodNo > SECT_MAX_NUM) || (uin_current_method_point <= 0))
                {
                        //uin_RemainDataLength = 0;
                        ul_RemainDataLength = 0;
                        mutex_DataBuff.unlock();
                        //Sleep(1);
                        return true;
                }
                else
                {
                        //uin_RemainDataLength = puin_MethodPointCount[methodNo - 1] + 1;//获取数据长度
                        ul_RemainDataLength = uin_current_method_point*2 + 2; //数据内容+ 1个方法片段号+1个数据大小 （16bit）
                }
        }

        //if(uin_tmpDataLength > uin_RemainDataLength)
        if(uin_tmpDataLength > ul_RemainDataLength)
        {
                //uin_tmpDataLength = uin_RemainDataLength;
                uin_tmpDataLength = ul_RemainDataLength;
        }


        //最大内存限制
        if(((INT32U)(puin_Data - puin_Buffer)  + ul_DataLength + uin_tmpDataLength) > ul_BufferLength && ul_BufferLength + ul_AutoLength <= ul_MaxLength)
        {//缓存内存不足，需重新申请
                INT16U *puch_tmp = puin_Buffer;
                puin_Buffer = new INT16U[ul_BufferLength + ul_AutoLength];
                memset(puin_Buffer, 0, (ul_BufferLength + ul_AutoLength) * 2);
                memcpy(puin_Buffer, puch_tmp, ul_BufferLength * 2);
                puin_Data = puin_Buffer + (puin_Data - puch_tmp);
                ul_BufferLength += ul_AutoLength;

//                 qDebug("date buff be long" );


                delete[] puch_tmp;
        }

        if((INT32U)(puin_Data - puin_Buffer) + ul_DataLength + uin_tmpDataLength <= ul_BufferLength)
        {//获取数据
            //qDebug("read data ,data addr ,data len %d",uin_tmpDataLength);
            MemoryRead(DSRAM_DATA, 0, puin_Data + ul_DataLength, uin_tmpDataLength);
            ul_DataLength += uin_tmpDataLength;
            ul_RemainDataLength -= uin_tmpDataLength;


//            qDebug("ul_DataLength %d uin_tmpDataLength %d",ul_DataLength,uin_tmpDataLength);
            b_result = true;
        }

        mutex_DataBuff.unlock();

        return b_result;
}//end GetDatasFromFPGA()


void CDetectData::GetdatasToPC(void * pTarget, INT32U ul_dataLength, INT8U uch_packNo)
{
        mutex_DataBuff.lock();
        if(uch_packNo == uch_PackNo)
        {//包号一致，需重新发送
                ul_DataLength += puin_Data - puin_Buffer;
                puin_Data = puin_Buffer;

                /*CString cs_Temp;
                cs_Temp.Format(L"GETDATA NO%d Resend",uch_PackNo);
                AddLogWithTime(cs_Temp);*/
        }
        else
        {//包号不一致，说明上次已发送成功，删除上次发送的数据
                //memset(puin_Buffer,0,(puin_Data - puin_Buffer)*2);
                memcpy(puin_Buffer, puin_Data, ul_DataLength * 2);//注意：memcpy中的长度是以字节为单位
                puin_Data = puin_Buffer;



        }
        INT32U ul_tmpDataLength = ul_dataLength;
        if (ul_tmpDataLength > ul_DataLength)
                ul_tmpDataLength = ul_DataLength;
        if(ul_tmpDataLength > 0)
        {
                memcpy(pTarget, puin_Data, ul_tmpDataLength * 2);
                puin_Data += ul_tmpDataLength;
                ul_DataLength -= ul_tmpDataLength;
                uch_PackNo = uch_packNo;
        }
        mutex_DataBuff.unlock();
        return;
}
INT32U CDetectData::GetDataLength(INT8U uch_packNo, INT32U ul_dataLength)
{
        mutex_DataBuff.lock();
        if (!b_InitOk)
        {
                return 0;
        }

        if(uch_packNo == uch_PackNo)
        {//包号一致，需重新发送
                mutex_DataBuff.unlock();
                return (ul_dataLength > ul_DataLength + puin_Data - puin_Buffer) ? (ul_DataLength + puin_Data - puin_Buffer) : ul_dataLength;
        }
        else
        {//包号不一致，说明上次已发送成功
                mutex_DataBuff.unlock();
                return (ul_dataLength > ul_DataLength) ? ul_DataLength : ul_dataLength;
        }


}
void CDetectData::Init()
{
        mutex_DataBuff.lock();
        puin_Data = puin_Buffer;
        ul_DataLength = 0;
        uch_PackNo = 1;
        ul_RemainDataLength = 0;
        //b_isEnd = false;
        //memset(puin_MethodPointCount, 0, SECT_MAX_NUM * 2);
        //memset(puin_Buffer, 0, ul_BufferLength * 2);
        b_InitOk = TRUE;
        mutex_DataBuff.unlock();

}


