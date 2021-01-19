//==================================================================================================
//| 文件名称 | Bsp_Uart1.cpp
//|-----------------------------------------------------------------------------------------------
//| 文件描述 | 初始化模块
//|-----------------------------------------------------------------------------------------------
//| 运行环境 | 所有C/C++语言编译环境，包括单片机编译器
//|-----------------------------------------------------------------------------------------------
//| 版权声明 | Copyright2017, 聚光科技(FPI)
//|------------------------------------------------------------------------------------------------
//|  版本   |  时间       |  作者     | 描述
//|--------- -------------|-----------|------------------------------------------------------------
//|  V1.0   | 2019.05.07  |   cfh     | 初版
//==================================================================================================

#include "Bsp_TCP.h"
#include "Pub/Pub_Queue.h"
#include "Pub/Pub_Qthread.h"
#include <QMutex>
static int fd_com =-1;
static int    ser_souck_fd;


#define BUFF_SIZE 4096
char input_message[BUFF_SIZE];
char resv_message[BUFF_SIZE];

static INT8U uch_SendEn1 = 0;
static INT8U uch_SendEn2 = 0;
static INT8U uch_SendEn3 = 0;
static INT8U sauch_TxFifo1[TCPFIFOSIZE];
static INT8U sauch_RxFifo1[TCPFIFOSIZE];
static INT8U sauch_TxFifo2[TCPFIFOSIZE];
static INT8U sauch_RxFifo2[TCPFIFOSIZE];
static INT8U sauch_TxFifo3[TCPFIFOSIZE];
static INT8U sauch_RxFifo3[TCPFIFOSIZE];

static CQueue m_TxFifo1(sauch_TxFifo1,sizeof(sauch_TxFifo1));
static CQueue m_RxFifo1(sauch_RxFifo1,sizeof(sauch_RxFifo1));
static CQueue m_TxFifo2(sauch_TxFifo2,sizeof(sauch_TxFifo2));
static CQueue m_RxFifo2(sauch_RxFifo2,sizeof(sauch_RxFifo2));
static CQueue m_TxFifo3(sauch_TxFifo3,sizeof(sauch_TxFifo3));
static CQueue m_RxFifo3(sauch_RxFifo3,sizeof(sauch_RxFifo3));

QMutex mutex_Txfifo;
QMutex mutex_Rxfifo;

#define CLI_NUM 3

int client_fds[CLI_NUM];


//==================================================================================================
//| 函数名称 | Can0Init
//|-----------------------------------------------------------------------------------------------
//| 函数功能 | 初始化
//|-----------------------------------------------------------------------------------------------
//| 调用模块 | 无
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|------------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|------------------------------------------------------------------------------------------------
//| 函数设计 | CFH 2019.5.7
//==================================================================================================
void TCPInit(void)
{


    char    buff[TCP_BUF];
    int     n;
    static struct sockaddr_in     servaddr;
    if( (ser_souck_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    printf("ser_souck_fd1 = %d",ser_souck_fd);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(5000);
 
    if( bind(ser_souck_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
    printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
       printf("ser_souck_fd2 = %d",ser_souck_fd);
    if( listen(ser_souck_fd, 10) == -1){
    printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }
    printf("ser_souck_fd3 = %d",ser_souck_fd);
    printf("======waiting for client's request======\n");
    /* if( (fd_com = accept(ser_souck_fd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        //continue;
	exit(0);
    }*/



    int keepalive = 1; // 开启keepalive属性
    int keepidle = 15; // 如该连接在60秒内没有任何数据往来,则进行探测
    int keepinterval = 2; // 探测时发包的时间间隔为5 秒
    int keepcount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
    setsockopt(ser_souck_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
    setsockopt(ser_souck_fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
    setsockopt(ser_souck_fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
    setsockopt(ser_souck_fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));

    // printf("tcpfdcom = %d",fd_com);
}

//==================================================================================================
//| 函数名称 | Uart1RxChar
//|-----------------------------------------------------------------------------------------------
//| 函数功能 | 片内UART1接收一个字节
//|------------------------------------------------------------------------------------------------
//| 调用模块 | 无
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U *uch_Char
//|------------------------------------------------------------------------------------------------
//| 返回参数 | TRUE or FALSE
//|------------------------------------------------------------------------------------------------
//| 函数设计 | zbb 2018.7.12
//==================================================================================================
BOOL TCPRxChar1(INT8U *puch_Char)
{
    if(NULL == puch_Char)
    {
        return FALSE;
    }

    //mutex_Rxfifo.lock();
    if(FALSE == m_RxFifo1.PopQueue(puch_Char)) //从接收缓冲区中弹出一个字节,若失败则开中断并返回
    {
      //mutex_Rxfifo.unlock();
      return FALSE;
    }
    //mutex_Rxfifo.unlock();
    return TRUE;
}

BOOL TCPRxChar2(INT8U *puch_Char)
{
    if(NULL == puch_Char)
    {
        return FALSE;
    }

    //mutex_Rxfifo.lock();
    if(FALSE == m_RxFifo2.PopQueue(puch_Char)) //从接收缓冲区中弹出一个字节,若失败则开中断并返回
    {
      //mutex_Rxfifo.unlock();
      return FALSE;
    }
    //mutex_Rxfifo.unlock();
    return TRUE;
}

BOOL TCPRxChar3(INT8U *puch_Char)
{
    if(NULL == puch_Char)
    {
        return FALSE;
    }

    //mutex_Rxfifo.lock();
    if(FALSE == m_RxFifo3.PopQueue(puch_Char)) //从接收缓冲区中弹出一个字节,若失败则开中断并返回
    {
      //mutex_Rxfifo.unlock();
      return FALSE;
    }
    //mutex_Rxfifo.unlock();
    return TRUE;
}


//==================================================================================================
//| 函数名称 | Uart1TxChar
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 写一个数据到发送缓冲区(不使能发送中断)
//|------------------------------------------------------------------------------------------------
//| 调用模块 | 无
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 待发送的数据
//|------------------------------------------------------------------------------------------------
//| 返回参数 | TRUE or FALSE
//|------------------------------------------------------------------------------------------------
//| 函数设计 | zbb 2018.7.12
//==================================================================================================
BOOL TCPTxChar1(INT8U uch_Char)
{
    //mutex_Txfifo.lock();
    if(FALSE == m_TxFifo1.PushQueue(uch_Char)) //将数据压入发送FIFO,若失败则开中断并返回
    {
        //qDebug()<<"queue full";
        //mutex_Txfifo.unlock();
        return FALSE;
    }
    //mutex_Txfifo.unlock();
    return TRUE;
}

BOOL TCPTxChar2(INT8U uch_Char)
{
    //mutex_Txfifo.lock();
    if(FALSE == m_TxFifo2.PushQueue(uch_Char)) //将数据压入发送FIFO,若失败则开中断并返回
    {
        //qDebug()<<"queue full";
        //mutex_Txfifo.unlock();
        return FALSE;
    }
    //mutex_Txfifo.unlock();
    return TRUE;
}

BOOL TCPTxChar3(INT8U uch_Char)
{
    //mutex_Txfifo.lock();
    if(FALSE == m_TxFifo3.PushQueue(uch_Char)) //将数据压入发送FIFO,若失败则开中断并返回
    {
        //qDebug()<<"queue full";
        //mutex_Txfifo.unlock();
        return FALSE;
    }
    //mutex_Txfifo.unlock();
    return TRUE;
}


//==================================================================================================
//| 函数名称 | Uart1SendEnable
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 启动发送
//|------------------------------------------------------------------------------------------------
//| 调用模块 | 无
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|------------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|------------------------------------------------------------------------------------------------
//| 函数设计 | zbb 2018.7.12
//==================================================================================================
void TCPSendEnable1(void)
{
    uch_SendEn1 = 1;
}
void TCPSendEnable2(void)
{
    uch_SendEn2 = 1;
}
void TCPSendEnable3(void)
{
    uch_SendEn3 = 1;
}
//==================================================================================================
//| 函数名称 | Uart1SendDisable
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 关闭发送
//|------------------------------------------------------------------------------------------------
//| 调用模块 | 无
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|------------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|------------------------------------------------------------------------------------------------
//| 函数设计 | zbb 2018.7.12
//==================================================================================================
void TCPSendDisable1(void)
{
    uch_SendEn1 = 0;
}
void TCPSendDisable2(void)
{
    uch_SendEn2 = 0;
}
void TCPSendDisable3(void)
{
    uch_SendEn3 = 0;
}

//==================================================================================================
//| 函数名称 | Uart1Tx
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 发送
//|------------------------------------------------------------------------------------------------
//| 调用模块 | 无
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|------------------------------------------------------------------------------------------------
//| 返回参数 |
//|------------------------------------------------------------------------------------------------
//| 函数设计 | zbb 2018.7.12
//==================================================================================================
static BOOL TCPTx(void)
{
    INT8U uch_char = 0;
    INT16U uin_datalen = 0;
    INT8U i;
    INT8U uch_Buf[BUFF_SIZE];

    for(i=0;i<CLI_NUM;i++)
    {
        uin_datalen = 0;
        bzero(uch_Buf,BUFF_SIZE);
        if(i ==0)
        {
            while(uch_SendEn1)
            {

                if(FALSE == m_TxFifo1.PopQueue(&uch_char))                        //从发送缓冲区中弹出一个字节
                {
                    TCPSendDisable1();

                    break;
                }
                else
                {

                    uch_Buf[uin_datalen++] = uch_char;

                    if(uin_datalen >TCP_BUF)
                    {

                      break;
                    }
                }
            }
        }
        else if(i ==1)
        {
            while(uch_SendEn2)
            {

                if(FALSE == m_TxFifo2.PopQueue(&uch_char))                        //从发送缓冲区中弹出一个字节
                {
                    TCPSendDisable2();

                    break;
                }
                else
                {

                    uch_Buf[uin_datalen++] = uch_char;

                    if(uin_datalen >TCP_BUF)
                    {

                      break;
                    }
                }
            }
        }
        else if(i == 2)
        {

            while(uch_SendEn3)
            {

                if(FALSE == m_TxFifo3.PopQueue(&uch_char))                        //从发送缓冲区中弹出一个字节
                {
                    TCPSendDisable3();

                    break;
                }
                else
                {

                    uch_Buf[uin_datalen++] = uch_char;

                    if(uin_datalen >TCP_BUF)
                    {

                      break;
                    }
                }
            }
        }

        if(uin_datalen > 0)
        {

             send(client_fds[i], uch_Buf, uin_datalen, 0);
        }

    }


    return TRUE;
}
//==================================================================================================
//| 函数名称 | Uart1Process
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 串口处理函数
//|------------------------------------------------------------------------------------------------
//| 调用模块 |
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|------------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|------------------------------------------------------------------------------------------------
//| 函数设计 | zbb 2018.7.12
//==================================================================================================
void TCPAcceptProcess(void)
{

    fd_set ser_fdset;
    int i;
    int max_fd=1;
    struct timeval mytime;
    printf("wait for client connnect!\n");

    while(1)
    {
        mytime.tv_sec=27;
        mytime.tv_usec=0;

        FD_ZERO(&ser_fdset);
        if(max_fd < 0)
        {
            max_fd=0;
        }
        FD_SET(ser_souck_fd,&ser_fdset);
        if(max_fd < ser_souck_fd)
        {
            max_fd = ser_souck_fd;
        }

        //printf("ser_souck_fd %d\r\n",ser_souck_fd);

        //add client
        for(i=0;i<CLI_NUM;i++)  //用数组定义多个客户端fd
        {
            if(client_fds[i]!=0)
            {
                FD_SET(client_fds[i],&ser_fdset);
                if(max_fd < client_fds[i])
                {
                    max_fd = client_fds[i];

                }
               // printf("client_fds %d\r\n",client_fds[i]);
            }
        }

        //select多路复用
        int ret = select(max_fd + 1, &ser_fdset, NULL, NULL, &mytime);


        if(ret < 0)
        {
            perror("select failure\n");
            continue;
        }
        else if(ret == 0)
        {
            //printf("TCP time out!\n");
            /*for(i=0; i<CLI_NUM; i++)
            {
                if(client_fds[i] != 0)
                {
                    FD_CLR(client_fds[i], &ser_fdset);
                    client_fds[i] = 0;
                }
            }*/
            continue;
        }

        else
        {


            if(FD_ISSET(ser_souck_fd, &ser_fdset))
            {
                struct sockaddr_in client_address;
                socklen_t address_len;
                int client_sock_fd = accept(ser_souck_fd,(struct sockaddr *)&client_address, &address_len);
                if(client_sock_fd > 0)
                {
                    int flags=-1;
                    //一个客户端到来分配一个fd，CLI_NUM=3，则最多只能有三个客户端，超过4以后跳出for循环，flags重新被赋值为-1
                    for(i=0;i<CLI_NUM;i++)
                    {
                        if(client_fds[i] == 0)
                        {
                            flags=i;
                            client_fds[i] = client_sock_fd;
                            break;
                        }
                    }


                    if (flags >= 0)
                    {
                        printf("new user client[%d] add sucessfully!\n",flags);

                    }
                    else //flags=-1
                    {
                        char full_message[]="the client is full!can't join!\n";
                        bzero(input_message,BUFF_SIZE);
                        strncpy(input_message, full_message,100);
                        send(client_sock_fd, input_message, BUFF_SIZE, 0);
                    }
                }
            }

        }
        //deal with the message

        for(i=0; i<CLI_NUM; i++)
        {
            if(client_fds[i] != 0)
            {
                if(FD_ISSET(client_fds[i],&ser_fdset))
                {
                    bzero(resv_message,BUFF_SIZE);
                    int byte_num=read(client_fds[i],resv_message,BUFF_SIZE);
                    if(byte_num > 0)
                    {

                        for(int j =0; j<byte_num; j++)
                        {

                            if(i==0)
                            {
                                if(FALSE == m_RxFifo1.PushQueue(resv_message[j]))//将数据压入接受FIFO,若失败则调用发送FIFO满钩子函数
                                {

                                    break;
                                }

                            }
                            else if(i==1)
                            {
                                if(FALSE == m_RxFifo2.PushQueue(resv_message[j]))//将数据压入接受FIFO,若失败则调用发送FIFO满钩子函数
                                {

                                    break;
                                }
                            }
                            else if(i==2)
                            {
                                if(FALSE == m_RxFifo3.PushQueue(resv_message[j]))//将数据压入接受FIFO,若失败则调用发送FIFO满钩子函数
                                {

                                    break;
                                }
                            }
                        }


                    }
                    else if(byte_num < 0)
                    {
                        printf("byte_num %d",byte_num);
                        printf("rescessed error!");
                    }

                    //某个客户端退出
                    else  //cancel fdset and set fd=0
                    {
                        printf("byte_num %d",byte_num);
                        printf("clien[%d] exit!\n",i);
                        FD_CLR(client_fds[i], &ser_fdset);
                        client_fds[i] = 0;
                        // printf("clien[%d] exit!\n",i);
                        continue;  //这里如果用break的话一个客户端退出会造成服务器也退出。
                    }
                }
            }
        }
    }

}




//==================================================================================================
//| 函数名称 | Uart1Process
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 串口处理函数
//|------------------------------------------------------------------------------------------------
//| 调用模块 |
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|------------------------------------------------------------------------------------------------
//| 返回参数 | 无
//|------------------------------------------------------------------------------------------------
//| 函数设计 | 陈飞华  2019.6.27
//==================================================================================================
void TCPsendProcess(void)
{
        TCPTx();
}
