//==================================================================================================
//| 文件名称 | Pub_Queue.cpp
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
#define PUB_QUEUE_GLOBALS

#include <string.h>
#include "Pub_Queue.h"


//==================================================================================================
//| 函数名称 | CQueue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 构造函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
CQueue::CQueue(INT8U * puch_Data,INT16U uin_BufLen)
{
    puch_FifoBuf = puch_Data;
    uin_BufSize = uin_BufLen;//默认长度

    uin_FifoBufR = uin_BufSize - 1;
    uin_FifoBufF = uin_BufSize - 1;
    memset(puch_FifoBuf,0,uin_BufSize);
}
CQueue::CQueue()
{

}
CQueue::~CQueue()
{
    //在此销毁类成员(变量)
}
//==================================================================================================
//| 函数名称 | SetQueue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 |
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
BOOL CQueue::SetQueue(INT8U * puch_Data,INT16U uin_BufLen)
{
    if((NULL == puch_Data)||(uin_BufLen > QUEUE_LEN_MAX)||(0 == uin_BufLen))
    {
        return FALSE;
    }

    puch_FifoBuf = puch_Data;
    uin_BufSize = uin_BufLen;//默认长度

    uin_FifoBufR = uin_BufSize - 1;
    uin_FifoBufF = uin_BufSize - 1;
    memset(puch_FifoBuf,0,uin_BufSize);

    return TRUE;
}

//==================================================================================================
//| 函数名称 | IntialQueue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 初始化函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
void CQueue::IntialQueue(void)
{
    uin_FifoBufR = uin_BufSize - 1;
    uin_FifoBufF = uin_BufSize - 1;

    memset(puch_FifoBuf,0,uin_BufSize);
}

//==================================================================================================
//| 函数名称 | IntialQueue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 初始化函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
BOOL CQueue::SetQueueLen(INT16U uin_Len)
{
    if((uin_Len > QUEUE_LEN_MAX)||(0 == uin_Len))
    {
        return(FALSE);
    }

    uin_BufSize = uin_Len;

    return(TRUE);
}


//==================================================================================================
//| 函数名称 | FifoPtrInc
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 队列指针递增函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT16U fifo_ptr
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | INT16U
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
INT16U CQueue::FifoPtrInc(INT16U uin_FifoPtr)
{
    if(uin_FifoPtr >= uin_BufSize-1)  //判断超范围否
    {
        return(0);                     //返回到起点
    }
    else
    {
        return(uin_FifoPtr+1);
    }
}

//==================================================================================================
//| 函数名称 | ReadFifoStatus
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 判断是否为空队列
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | return 0 ok
//|		     | return 1 full
//|		     | return -1 null
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
INT8S CQueue::ReadFifoStatus( void )
{
    //判断是否满队
    if(uin_FifoBufF == FifoPtrInc(uin_FifoBufR))
    {
        return (FIFO_FULL);
    }
    //判断是否空队
    else if(uin_FifoBufR == uin_FifoBufF)
    {
        return (FIFO_NULL);
    }
    //正常
    else
    {
        return (FIFO_OK);
    }
}


//==================================================================================================
//| 函数名称 | PushQueue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 入队
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | return FALSE 满队
//|		     | return TRUE正常入队
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
BOOL CQueue::PushQueue(INT8U uch_Data)
{
    if(ReadFifoStatus() == FIFO_FULL)
    {
        return FALSE;
    }

    uin_FifoBufR = FifoPtrInc(uin_FifoBufR); //返回到起点
    puch_FifoBuf[uin_FifoBufR] = uch_Data;         //

    //donot need to check
    /*for(i=0;i<uin_BufSize;i++)
    {
        if(uch_Data == puch_FifoBuf[i])
        {
            return 0;
        }
    }

    return FALSE;*/
    return TRUE;
}

//==================================================================================================
//| 函数名称 | OutQueue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 出队,删除当前队列的任务，头指针后移
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | return FALSE 空队
//|		     | return TRUE 正常出队
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
BOOL CQueue::OutQueue( void )
{
    INT16U uin_CurFifoPtr;

    if(ReadFifoStatus() == FIFO_NULL)
    {
        return FALSE;
    }

    uin_CurFifoPtr = FifoPtrInc(uin_FifoBufF);

    puch_FifoBuf[uin_CurFifoPtr] = 0;  //清空
    uin_FifoBufF = uin_CurFifoPtr;
    return TRUE;      //正常
}

//==================================================================================================
//| 函数名称 | PopQueue
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读当前队列
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | void
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | return FALSE 出错
//|		     | return TRUE 正常
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | ygm,2010.01.01
//==================================================================================================
BOOL CQueue::PopQueue( INT8U* puch_Data )
{
    INT16U uin_Comptr;
    INT16U uin_CurFifoPtr;

    if(NULL == puch_Data)
    {
        return FALSE;
    }

    if(ReadFifoStatus() == FIFO_NULL)
    {
        return FALSE;
    }

    uin_Comptr = FifoPtrInc(uin_FifoBufF);
    (*puch_Data )= puch_FifoBuf[uin_Comptr];

    uin_CurFifoPtr = FifoPtrInc(uin_FifoBufF);

    puch_FifoBuf[uin_CurFifoPtr] = 0;  //清空
    uin_FifoBufF = uin_CurFifoPtr;

    return TRUE;      //正常
}

//no more