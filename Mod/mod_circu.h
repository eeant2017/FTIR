#ifndef MOD_CIRCU_H
#define MOD_CIRCU_H
#include "Pub/Pub_Tmr.h"
#include <Bsp/Bsp_Time.h>
#include "typedefine.h"
class Circu : public CTmr
{
public:
    Circu();
    void CircuProcess();
    void CircuMntProcess();
    BOOL m_bCommAlarm;


public:
    INT32U m_ulCommCnt;
    INT32U m_ulCommErrCnt;

private:
    INT8U m_uchMntState;

    BOOL m_bCommLock;



};

extern Circu g_clCircu;
#endif // MOD_CIRCU_H
