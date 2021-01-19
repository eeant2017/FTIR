




/*
 * linux/can.h
 *
 * Definitions for CAN network layer (socket addr / CAN frame / CAN filter)
 *
 * Authors: Oliver Hartkopp <oliver.hartkopp@volkswagen.de>
 *          Urs Thuermann   <urs.thuermann@volkswagen.de>
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Send feedback to <socketcan-users@lists.berlios.de>
 *
 */

#ifndef CANSEF_H
#define CANSEF_H

//#include <linux/types.h>
//#include <linux/socket.h>
#include "typedefine.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


#include <getopt.h>
#include <libgen.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <linux/can.h>
#include <linux/can/raw.h>


#define CAN0FIFOSIZE      5100


#define CAN_INV_FILTER 0x20000000U /* to be set in can_filter.can_id */



#define PF_CAN				29




void Can0TxProcess(void);
void Can0RxProcess(void);
void Can0Init(void);
void Can0SendEnable(void);
void Can0SendDisable(void);
BOOL Can0TxChar(INT8U uch_Char);
BOOL Can0RxChar(INT8U *puch_Char);
extern INT8U g_uchCanLock;

#endif /* CAN_H */


