#include "mod_circu.h"
#include "App/App_ComFpi.h"
Circu g_clCircu;

Circu::Circu()
{
    m_uchMntState = 0;

    Set(1000);

    m_ulCommCnt = 0;
    m_ulCommErrCnt = 0;

}


void Circu::CircuProcess()
{
    CircuMntProcess();
}


void Circu::CircuMntProcess()
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
            st_FpiSend.uch_DestAddr = ADDR_CIRCU;
            st_FpiSend.uch_Cmd = 0x31;
            st_FpiSend.uch_AddCmd = READ;
            st_FpiSend.uch_DataLen = 0;
            gm_Can0Fpi.PushComm(&st_FpiSend);
            //qDebug("Monit Circuboard");
            Set1(50);

            m_ulCommCnt ++;

            m_uchMntState++;
        }

    }break;
    case 2:
    {
        if(a_ComFpi.m_bCircu31ReadCommFlag == 1)
        {
            a_ComFpi.m_bCircu31ReadCommFlag = 0;
            m_uchMntState = 0;
            g_uchCanLock = FALSE;
            m_bCommAlarm = FALSE;
            //qDebug("Get Circuboard");
        }
        else if(TmrOut1())
        {
            a_ComFpi.m_bCircu31ReadCommFlag = 0;
            m_uchMntState = 0;
            g_uchCanLock = FALSE;
            m_bCommAlarm = TRUE;
            m_ulCommErrCnt ++;

            //qDebug("Get Circuboard Timeout");
        }

    }break;

    }
}



