#ifndef APP_APP_H
#define APP_APP_H
#include <QThread>
#include "typedefine.h"
#include "types.h"
#include "Pub/Pub_Tmr.h"
#include <Bsp/Bsp_Time.h>
#define MS_OPRAITON_NULL 0
#define MS_OPRATION_START 1
#define MS_OPRATION_STOP 2

#define MS_RUNSTATUS_IDLE 0
#define MS_RUNSTATUS_RUN 1
class app_app : public QThread , public CTmr
{
public:
    app_app();
protected:
    void run();
public:
    INT8U uch_MSOpration;
    INT8U m_uchMSRunStatus;

};

class app_com : public QThread
{
public:
    app_com();
protected:
    void run();

};


extern app_app cl_App;
extern app_com cl_AppCom;
#endif // APP_APP_H
