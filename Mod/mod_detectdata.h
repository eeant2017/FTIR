#ifndef MOD_DETECTDATA_H
#define MOD_DETECTDATA_H

#include "typedefine.h"
class CDetectData
{
public:
    CDetectData();
    ~CDetectData();
    void Process();
    bool GetDatasFromFPGA();
    void GetdatasToPC(void * pTarget, INT32U ul_dataLength, INT8U uch_packNo);
    INT32U GetDataLength(INT8U uch_packNo, INT32U ul_dataLength);
    void Init();


private:
    INT32U ul_MaxLength;		//
    INT32U ul_AutoLength;		//2M字节，1M字
    INT8U  uch_PackNo;			//数据包号
    INT16U *puin_Buffer;		//缓存起始地址
    INT32U ul_BufferLength;		//缓存长度
    INT16U *puin_Data;			//有效数据起始地址
    INT32U ul_DataLength;		//有效数据长度
    //INT16U uin_RemainDataLength;	//当前方法片段还需获取的数据长度
    INT32U ul_RemainDataLength;
    //INT16U *puin_MethodPointCount;	//存放各个方法片段的数据点数
    //bool b_isEnd;//是否采集结束
public:
    BOOL b_InitOk;
};

extern CDetectData g_clDetectData;

#endif // MOD_DETECTDATA_H
