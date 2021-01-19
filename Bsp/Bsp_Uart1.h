#ifndef BSP_UART1_H
#define BSP_UART1_H

#include <QDebug>

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "typedefine.h"

#define UART_BAUDRATE B9600
#define UART_COM "/dev/ttymxc1"
//#define UART_COM "/dev/ttyS1" //pc
//#define UART_COM "/dev/ttyUSB0"

#define UART1FIFOSIZE      100
void Uart1Init(void);
BOOL Uart1RxChar(INT8U *puch_Char);
BOOL Uart1TxChar(INT8U uch_Char);
void Uart1SendEnable(void);
void Uart1SendDisable(void);
void Uart1Process(void);
#endif


