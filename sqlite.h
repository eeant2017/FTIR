#ifndef SQLITE_H
#define SQLITE_H

#include "globalvar.h"
#include <QtSql>

typedef struct
{
    INT8U uch_ID;
    FP32 f_Max;
    FP32 f_Min;
    FP32 f_Default;
    FP32 f_SetK;
    FP32 f_SetB;
    FP32 f_ReadK;
    FP32 f_ReadB;

}STRCALIPARA;

class CSQlite
{

public:
    CSQlite();

    QSqlDatabase db;

    INT8U InitSQlite();
    INT8U OpenDB(INT8U uch_Type);
    INT8U InsertCaliPara(STRCALIPARA st_CaliPara);
    INT8U UpdateCaliPara(STRCALIPARA st_CaliPara);

    INT8U GetCali();
    INT8U GetCali(INT8U uch_ID,STRCALIPARA &st_CaliPara);
    INT8U SetDeviceID(char* puchID);
    INT8U GetDeviceID(char* puchID);
    INT8U SetDeviceModel(INT8U uch_Type);
    INT8U GetDeviceModel(INT8U *puch_Type);
    INT8U SetSampleSelect(INT8U uch_Type);
    INT8U GetSampleSelect(INT8U *puch_Type);
    INT8U SetCIDVacuumGaugeSelect(INT8U uch_Type);
    INT8U GetCIDVacuumGaugeSelect(INT8U *puch_Type);

};

extern CSQlite m_SQLite;

#endif // SQLITE_H
