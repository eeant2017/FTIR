//==================================================================================================
//| 文件名称 | Pub_Queue.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 环形队列处理
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 | 所有C/++语言编译器，包括单片机编译器
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2010
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2010.01.01  | 杨光芒ygm |
//==================================================================================================
#ifndef PUB_QUEUE_H
    #define	PUB_QUEUE_H
#ifdef	PUB_QUEUE_GLOBALS
    #define PUB_QUEUE_EXT
#else
    #define PUB_QUEUE_EXT extern
#endif

#include "typedefine.h"

//----------------------------------------Cplusplus-----------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif

#define FIFO_OK 0
#define FIFO_FULL 1
#define FIFO_NULL -1

#define QUEUE_LEN_MAX 10000

class CQueue
{
public:
    CQueue(INT8U *puch_Data,INT16U uin_BufLen);
    CQueue();
    ~CQueue();
    BOOL SetQueue(INT8U *p,INT16U uin_BufLen);
    void IntialQueue(void);
    BOOL PushQueue(INT8U uch_Data);
    BOOL PopQueue( INT8U* p );
    BOOL SetQueueLen(INT16U si_Len);

private:
    INT16U FifoPtrInc(INT16U fifo_ptr);
    INT8S ReadFifoStatus( void );
    BOOL OutQueue( void );

    INT8U *puch_FifoBuf;
    INT16U uin_FifoBufR;       //Fifo队列末尾指针
    INT16U uin_FifoBufF;       //Fifo队列首指针
    INT16U uin_BufSize;        //Fifo队列缓冲深度
};


#ifdef __cplusplus
}
#endif



#endif
