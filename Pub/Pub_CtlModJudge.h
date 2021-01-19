#ifndef PUB_CTLMODJUDGE
#define PUB_CTLMODJUDGE


#include "typedefine.h"

//判读结果宏定义
#define JUDGE_RESULT_OK       1
#define JUDGE_RESULT_FALSE    2
#define JUDGE_RESULT_ING      3

class CCtlModJudge
{
public:
    CCtlModJudge(FP32 f_Max, FP32 f_Min, FP32 f_Err)
    {
        uch_RangeErrCnt = 0;//范围超限次数清零
        uch_RangeOkCnt = 0;
        uin_CtlErrCnt = 0;//控制异常次数清零

        f_ValMax = f_Max;
        f_ValMin = f_Min;
        f_CtlJudgeErr = f_Err;
    };

    void Kill(void);//判断参数清零
    INT8U RangeJudge(FP32 f_Pv);//范围判断
    INT8U CtlJudge(FP32 f_Pv, FP32 f_Target);//控制判断

private:
    FP32 f_ValMax;//上限
    FP32 f_ValMin;//下限

    FP32 f_CtlJudgeErr;//控制判断误差

    INT8U uch_RangeErrCnt;//范围超限次数
    INT8U uch_RangeOkCnt;

    INT16U uin_CtlErrCnt;//控制异常次数
};


#endif
