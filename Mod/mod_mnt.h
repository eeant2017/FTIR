#ifndef MOD_MNT_H
#define MOD_MNT_H

#include "typedefine.h"
#include <QThread>
#include "Pub/Pub_CtlModJudge.h"
#define ERR_VACUUM 0x01 //真空异常
#define ERR_PCCOMM 0x0B //PC通讯故障
#define ERR_CRICUCOM 0x0c //环境板通信异常


#define DEVICE_RUN_MODE_NORMAL 0x00
#define DEVICE_RUN_MODE_DEBUG 0X11
#define DEVICE_RUN_MODE_BIT 0X22

#define DEVICE_MODEL_EXPEC5210 0
#define DEVICE_MODEL_EXPEC5250 1

#define SOURCE_ESI 0
#define SOURCE_EI 1

#define UNIT_CIDVACUUMGAUGE_MKS 0
#define UNIT_CIDVACUUMGAUGE_CIRCU 1


//------------------环境板监控量结构体-----------
typedef struct
{
        FP32 f_MaxVal;
        FP32 f_MinVal;
        FP32 f_RealVal;
}STREnvAtt;


typedef struct
{
    INT8U auch_AlarmCode[10];

    STREnvAtt strPumpCurrent;
    STREnvAtt strEnvTemp;
    STREnvAtt strPumpSpeed;
    STREnvAtt strPumpTemp;

    INT8U b_ExhaustGasesValve;
    INT8U b_CircuStandbyIO;
    INT8U b_N2Valve;
    INT8U b_ArValve;
    FP32 f_IonicTempSet;
    FP32 f_VacuValue;


}STREnvMnt;



typedef struct
{
        FP32 f_MinVal;//下限
        FP32 f_MaxVal;//上限
        FP32 f_RealVal;//当前值
        FP32 f_Buf[32];//缓存当前值 用于计算平均值
        INT8U uch_FaultCode;//报警码

}STRMntAttr;

//监控量数据结构
typedef struct
{
        STRMntAttr st_CapillaryV;       //毛细管电压
        STRMntAttr st_CapillaryI;		//毛细管电流  2016.4.18
        STRMntAttr st_CoroI;			//电晕针电流
        STRMntAttr st_SampleConeV;		//采样锥电压
        STRMntAttr st_ExtractorV;		//萃取锥电压
        STRMntAttr st_RfLenV;			//传输杆电压

        STRMntAttr st_Ms1PreV;			//MS1预杆电压
        STRMntAttr st_Ms1IonEnergy;		//MS1离子能量
        STRMntAttr st_Ms1LM;			//MS1LM
        STRMntAttr st_Ms1HM;			//MS1HM

        STRMntAttr st_CIDEntranceV;     //CID入口电压
        STRMntAttr st_CIDV;				//CID碰撞能量
        STRMntAttr st_CIDEixtV;			//CID出口电压
        STRMntAttr st_CIDOverV;			//外壁电压

        STRMntAttr st_Ms2PreV;			//MS2预杆电压
        STRMntAttr st_Ms2IonEnergy;		//MS2离子能量
        STRMntAttr st_Ms2LM;			//MS2LM
        STRMntAttr st_Ms2HM;			//MS2HM

        STRMntAttr st_EmHighV;			//倍增器电压
        STRMntAttr st_EmHighOffsetV;	//倍增器偏转电压
        STRMntAttr st_EmHighEntranceV;	//倍增器入口电压
        STRMntAttr st_Dora;				//打拿极电压

        STRMntAttr st_Temperta;					//主板温度

        STRMntAttr st_HV1;	//HV1 出口透镜
        STRMntAttr st_HV2;	//HV2

        STRMntAttr st_OffsetV;//TODO未使用
        STRMntAttr st_Ms1RF;
        STRMntAttr st_Ms2RF;
        STRMntAttr st_Ms1DCP;
        STRMntAttr st_Ms1DCN;
        STRMntAttr st_Ms2DCP;
        STRMntAttr st_Ms2DCN;
        STRMntAttr ESourcePositive5VDigital;
        STRMntAttr ESourcePositive1Point2VDigital;
        STRMntAttr ESourcePositive3Point3VDigital;
        STRMntAttr ESourceNegative5VAnalog;
        STRMntAttr ESourcePositive5VAnalog;
        STRMntAttr ESourceNegative14VAnalog;
        STRMntAttr ESourcePositive14VAnalog;
        STRMntAttr Reference2Point5VOne;
        STRMntAttr Reference2Point5VTwo;
        STRMntAttr RFPowerESourceVoltage;

        STRMntAttr st_VacuValue1;		//传输杆腔室压力
        STRMntAttr st_VacuValue2;		//分析器腔室压力
        STRMntAttr st_VacuValue3;		//碰撞池腔室压力
        STRMntAttr st_VacuValue4;		//真空规4压力

        STRMntAttr st_LenthDC1;	//DC2透镜电压
        STRMntAttr st_LenthDC2;	//DC2透镜电压
        STRMntAttr st_LenthDC3;
        STRMntAttr st_LenthDC4;
        STRMntAttr st_LenthDC5;
        STRMntAttr st_LenthDC6;
        STRMntAttr st_HEXPoleV; //六级杆电压
        STRMntAttr st_BaffleVoltage;//挡板电压
        STRMntAttr st_BaffleCurrunt;//挡板电流
        STRMntAttr st_Q0out2;
        STRMntAttr st_DC7;

        STRMntAttr st_AnologSignal;//模拟量信号
        STRMntAttr st_Rf1Current;//rf1 电流
        STRMntAttr st_Rf2Current;//rf2 电流
        STRMntAttr st_Rf1Vol;//rf1 电压
        STRMntAttr st_Rf2Vol;//rf2 电压


        STRMntAttr st_VacuCommErr;//真空归通讯无应答
        STRMntAttr st_GateONErr; //门控开关打开报警
        STRMntAttr st_GPMCCheckErr;//总线检测失败报警


         //EI板
        STRMntAttr st_EIRepellerV;   //推斥极
        STRMntAttr st_EIDrawout; //抽出极
        STRMntAttr st_EIFocusV;//聚焦极
        STRMntAttr st_EIDeflectionV; //EI偏转透镜
        STRMntAttr st_Q0UpVHigh; //Q0上邻杆高压
        STRMntAttr st_Q0LowVHigh;//Q0下邻杆高压
        STRMntAttr st_FilamentV; //灯丝电压
        STRMntAttr st_FilamentOffsetV;//灯丝偏转电压
        STRMntAttr st_FilamentI; //灯丝反馈电流


        STRMntAttr st_EIPower14V;     //EI板电源24V
        STRMntAttr st_EIBoardTemp;  //EI板温度
        STRMntAttr st_EISourceTemp; //EI离子源温度（校准后）
        STRMntAttr st_EISourceTempOriginal; //EI离子源温度（校准前）
        STRMntAttr st_EIGCInterfaceTemp; //GC接口温度（校准后）
        STRMntAttr st_EIGCInterfaceTempOriginal; //GC接口温度（校准前）

        //Q0驱动板
        STRMntAttr st_Q0UpVLow; //Q0上邻杆低压
        STRMntAttr st_Q0LowVLow;//Q0下邻杆低压
        STRMntAttr st_DDSOut; //Q0DDS输出
        STRMntAttr st_Q0V; //Q0反馈电压
        STRMntAttr st_Q0I; //Q0反馈电流
        STRMntAttr st_Q0DriverV; //Q0驱动电压
        STRMntAttr st_Q0Power;   //Q0板电源
        STRMntAttr st_Q0BoardTemp;    //Q0板温度


} STRMnt,STRMntPtr;
#define GET_OFFSET(x)	( &( (STRMntPtr *)0 )->x -&( (STRMntPtr *)0 )->st_CapillaryV )



class CMnt:public QThread
{
public:
    CMnt();
    ~CMnt()
    {

    };

    void CMntProcess(void);
    void DeviceStandby();


    INT8U uch_GateOnCur;//当前离子源门控状态。
    INT8U uch_GateOnLast;//上次监离子源门控状态。
    INT8U uch_FirstGateOnSet; //上电将告知环境板离子源门控状态

    INT8U FPGAADRead();
    INT8U GetAlarmNum();
    INT8U GetErrNum();
    INT8U GetCircuAlarmNum();
    void LEDAlarmProcess();

public:
    STRMnt st_mMntPara;//主板监控变量
    STREnvMnt m_stEnvMnt;//环境板监控变量

    INT8U uch_PCCommCnt; //计算pc 多久没有通讯  根据监控命令进行。
    INT8U m_uchRunMode;//仪器运行模式

    BOOL b_MSStateBeforeGateOff; //离子源移除前的设备状态是否待机
    FP32 f_IonicTempBeforeGateOff;//离子源移除前的真空接口加热设置

public:
    INT8U m_uchDeviceModel; //仪器型号
    INT8U m_uchSourceSelect;//进样源切换
    INT8U m_uchCIDVacuumSelect; //碰撞池真空规选择



private:
    CCtlModJudge *m_pclESourcePositive5VDigital;
    CCtlModJudge *m_pclESourcePositive1Point2VDigital;
    CCtlModJudge *m_pclESourcePositive3Point3VDigital;
    CCtlModJudge *m_pclESourceNegative5VAnalog;
    CCtlModJudge *m_pclESourcePositive5VAnalog;
    CCtlModJudge *m_pclESourceNegative14VAnalog;
    CCtlModJudge *m_pclESourcePositive14VAnalog;
    CCtlModJudge *m_pclReference2Point5VOne;
    CCtlModJudge *m_pclTemperta;
    CCtlModJudge *m_pclRFPowerESourceVoltage;




};

extern CMnt g_clMnt;









#endif // MOD_MNT_H
