

#ifndef TCPSEF_H
#define TCPSEF_H

#include "typedefine.h"

#include<stdlib.h>

#include <strings.h>   //for bzero
#include <string.h>



#include<sys/socket.h>

//for select
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

#include <stdio.h>
#include <netinet/in.h>   //for souockaddr_in
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/tcp.h>

#define TCPFIFOSIZE  12000

#define TCP_BUF 4096

void TCPAcceptProcess(void);
void TCPInit(void);
void TCPSendEnable1(void);
void TCPSendDisable1(void);
BOOL TCPTxChar1(INT8U uch_Char);
BOOL TCPRxChar1(INT8U *puch_Char);
void TCPSendEnable2(void);
void TCPSendDisable2(void);
BOOL TCPTxChar2(INT8U uch_Char);
BOOL TCPRxChar2(INT8U *puch_Char);
void TCPSendEnable3(void);
void TCPSendDisable3(void);
BOOL TCPTxChar3(INT8U uch_Char);
BOOL TCPRxChar3(INT8U *puch_Char);

 void TCPsendProcess(void);

#endif /* TCP */


