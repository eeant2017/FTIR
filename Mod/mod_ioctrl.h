#ifndef MOD_IOCTRL_H
#define MOD_IOCTRL_H
#include "typedefine.h"

#define  LED_ON				0
#define  LED_OFF			1
#define  LED_SHINE			2


typedef struct
{
    INT8U uch_CapiIO;
    INT8U uch_PumpIO;
    INT8U uch_EMIO;
    INT8U uch_DoraIO;

    INT8U uch_PrePumpIO;
    INT8U uch_NegionIO;

    INT8U uch_CoroIO;
    INT8U uch_BitIO;//wxb

    INT8U uch_LBVacuumGIO;//wxb
    INT8U uch_ProtectIO;//agan
    INT8U uch_RfTune;//lz

    //EI
    INT8U uch_FilamentIO; //灯丝切换开关
    INT8U uch_Q0HighVIO; //Q0高压开关

    //QODriver
    INT8U uch_Q0AutoTuningIO; //自动调谐开关
    INT8U uch_Q0Relay11;//Q0电压切换继电器11
    INT8U uch_Q0Relay12;//Q0电压切换继电器12
    INT8U uch_Q0Relay21;//Q0电压切换继电器21
    INT8U uch_Q0Relay22;//Q0电压切换继电器22

    INT8U uch_Q0DCRelay1;//Q0DC电压切换继电器1
    INT8U uch_Q0DCRelay2;//Q0DC电压切换继电器2


    INT8U uch_RunLed;
    INT8U uch_RdyLed;
    INT8U uch_ErrLed;
}STRIOCTRL;

class CIOCtrl
{
public:
    CIOCtrl();

public:
    void CIOCtrProcess(void);
    void CIOCtrlInit(void);

public:
    STRIOCTRL m_stIOCtrl;

private:
    BOOL m_bErrState;
    BOOL m_bRdyState;
    BOOL m_bRunState;

};

extern CIOCtrl g_clIOCtrl;

#endif // MOD_IOCTRL_H
