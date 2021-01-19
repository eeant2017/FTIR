#include "mod_ioctrl.h"
#include "Pro/pro_fpga.h"
#include "Bsp/Bsp_Gpioctl.h"
CIOCtrl g_clIOCtrl;

CIOCtrl::CIOCtrl()
{

}

//==================================================================================================
//| 函数名称 | process
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 |
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
void CIOCtrl::CIOCtrProcess()
{

        //qDebug("IO process");


        //LED Process
        if( m_stIOCtrl.uch_RdyLed == LED_ON )
        {

                OperateSwitch(ARM_ALARM,SWITCH_ON);
        }
        else if ( m_stIOCtrl.uch_RdyLed == LED_OFF )
        {
                OperateSwitch(ARM_ALARM,SWITCH_OFF);
        }
        else if ( m_stIOCtrl.uch_RdyLed == LED_SHINE )
        {
                if(m_bRdyState == TRUE)
                {
                        m_bRdyState = FALSE;
                        OperateSwitch(ARM_ALARM,SWITCH_ON);
                }
                else
                {
                        m_bRdyState = TRUE;
                        OperateSwitch(ARM_ALARM,SWITCH_OFF);

                }
        }

        //RED ERR
        if( m_stIOCtrl.uch_ErrLed == LED_ON )
        {
                OperateSwitch(ARM_ERROR,SWITCH_ON);
        }
        else if ( m_stIOCtrl.uch_ErrLed == LED_OFF )
        {
                OperateSwitch(ARM_ERROR,SWITCH_OFF);
        }
        else if ( m_stIOCtrl.uch_ErrLed == LED_SHINE )
        {
                if(m_bErrState)
                {
                        OperateSwitch(ARM_ERROR,SWITCH_ON);
                        m_bErrState = FALSE;
                }
                else
                {
                        OperateSwitch(ARM_ERROR,SWITCH_OFF);
                        m_bErrState = TRUE;
                }
        }

        //GREEN RUN

        if ( m_stIOCtrl.uch_RunLed == LED_ON )
        {
             OperateSwitch(ARM_RUN,SWITCH_ON);
        }
        else if ( m_stIOCtrl.uch_RunLed == LED_OFF )
        {
             OperateSwitch(ARM_RUN,SWITCH_OFF);
        }
        else if( m_stIOCtrl.uch_RunLed == LED_SHINE )
        {
                if(m_bRunState)
                {
                     OperateSwitch(ARM_RUN,SWITCH_ON);
                     m_bRunState = FALSE;
                }
                else
                {
                     OperateSwitch(ARM_RUN,SWITCH_OFF);
                     m_bRunState = TRUE;
                }

        }


        return ;
}
