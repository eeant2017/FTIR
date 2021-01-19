#include "mod_EI.h"
#include "App/App_ComFpi.h"
EI g_clEI;

EI::EI()
{
    m_uchMntState = 0;
    m_bCommLock = 0;
    Set(1000);

    m_ulCommCnt = 0;
    m_ulCommErrCnt = 0;

}


void EI::EIProcess()
{
    EIMntProcess();
}

void EI::EIMntProcess()
{
    STRFPISEND st_FpiSend;

    switch(m_uchMntState)
    {

    case 0:
    {
        if(TmrOut())
        {
            m_uchMntState++;
        }
    }break;
    case 1:
    {
        if(g_uchCanLock == FALSE)
        {
            g_uchCanLock = TRUE;
            st_FpiSend.uch_DestAddr = ADDR_EI;
            st_FpiSend.uch_Cmd = 0x31;
            st_FpiSend.uch_AddCmd = READ;
            st_FpiSend.uch_DataLen = 0;
            gm_Can0Fpi.PushComm(&st_FpiSend);
            //qDebug("Monit EIboard");
            Set1(50);

            m_ulCommCnt ++;

            m_uchMntState++;
        }
    }break;
    case 2:
    {
        if(a_ComFpi.m_bEI31ReadCommFlag == 1)
        {
            a_ComFpi.m_bEI31ReadCommFlag = 0;
            m_uchMntState = 0;
            g_uchCanLock = FALSE;
            m_bCommAlarm = FALSE;
            //qDebug("Get EIboard");
        }
        else if(TmrOut1())
        {
            a_ComFpi.m_bEI31ReadCommFlag = 0;
            m_uchMntState = 0;
            g_uchCanLock = FALSE;
            m_bCommAlarm = TRUE;
            m_ulCommErrCnt ++;
            qDebug("Get EIboard Timeout");
        }

    }break;

    }
}

void EI::EISampleSelect(INT8U uch_Select)
{
    STRFPISEND st_FpiSend;
    st_FpiSend.uch_DestAddr = ADDR_EI;
    st_FpiSend.uch_Cmd = 0x26;
    st_FpiSend.uch_AddCmd = WRITE;
    st_FpiSend.uch_DataLen = 2;
    st_FpiSend.puch_Data[0] = 0x01;
    if(uch_Select == 0x00)
    {
        st_FpiSend.puch_Data[1] = 0x00;
    }
    else
    {
        st_FpiSend.puch_Data[1] = 0x01;
    }

    gm_Can0Fpi.PushComm(&st_FpiSend);

}



