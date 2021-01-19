#include "mod_Q0Driver.h"
#include "App/App_ComFpi.h"
Q0Driver g_clQ0Driver;

Q0Driver::Q0Driver()
{
    m_uchMntState = 0;
    m_bCommLock = 0;
    Set(1000);

    m_ulCommCnt = 0;
    m_ulCommErrCnt = 0;

}


void Q0Driver::Q0DriverProcess()
{
    Q0DriverMntProcess();
}


void Q0Driver::Q0DriverMntProcess()
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
            st_FpiSend.uch_DestAddr = ADDR_Q0DRIVER;
            st_FpiSend.uch_Cmd = 0x31;
            st_FpiSend.uch_AddCmd = READ;
            st_FpiSend.uch_DataLen = 0;
            gm_Can0Fpi.PushComm(&st_FpiSend);
            //qDebug("Monit Q0Driverboard");
            Set1(50);

            m_ulCommCnt ++;

            m_uchMntState++;
        }

    }break;
    case 2:
    {
        if(a_ComFpi.m_bQ0Driver31ReadCommFlag == 1)
        {
            a_ComFpi.m_bQ0Driver31ReadCommFlag = 0;
            m_uchMntState = 0;
            g_uchCanLock = FALSE;
            m_bCommAlarm = FALSE;
            //qDebug("Get Q0Driverboard");
        }
        else if(TmrOut1())
        {
            a_ComFpi.m_bQ0Driver31ReadCommFlag = 0;
            m_uchMntState = 0;
            g_uchCanLock = FALSE;
            m_bCommAlarm = TRUE;
            m_ulCommErrCnt ++;

            qDebug("Get Q0Driverboard Timeout");
        }

    }break;

    }
}

