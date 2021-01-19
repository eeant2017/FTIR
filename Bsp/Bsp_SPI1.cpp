#include "Bsp_SPI1.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <QDebug>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))


static void transfer(int fd);

static const char *device = "/dev/spidev32766.0";
static int fd;
static INT8U mode = 0;
static INT8U bits = 8;
static INT32U speed = 500000;
static INT16U delay = 0;
SPI1::SPI1()
{


}


//==================================================================================================
//| 函数名称 |SPI1Init()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | SPI初始化
//|------------------------------------------------------------------------------------------------
//| 输入参数 | 无
//|------------------------------------------------------------------------------------------------
//| 返回参数 |
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.02.30
//==================================================================================================
void SPI1Init()
{

    int ret = 0;
    fd = open(device, O_RDWR);
    if (fd < 0)
    {
        qDebug("can't open device");

    }

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
            qDebug("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
            qDebug("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
            qDebug("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
            qDebug("can't get bits per word");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
            qDebug("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
            qDebug("can't get max speed hz");

    qDebug("spi mode: %d", mode);
    qDebug("bits per word: %d", bits);
    qDebug("max speed: %d Hz (%d KHz)", speed, speed/1000);

//    transfer(fd);  //--test spiflash

//    close(fd);

}


//==================================================================================================
//| 函数名称 | Spi_Send1Byte()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 写入1个byte
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U byte,写入数据 INT8U cs_change true，cs数据操作前拉低，数据操作后维持0
//| ，false：cs 数据操作前拉低，数据操作后拉高
//|------------------------------------------------------------------------------------------------
//| 返回参数 | 读到的数据
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.02.30
//==================================================================================================
INT8U Spi_Send1Byte(INT8U byte,INT8U cs_change)
{
   int rtn = 0;
   int ret;
   uint8_t tx[2]= {byte,0};
   tx[0] = byte;
   uint8_t rx[2] = {0,0};

   spi_ioc_transfer tr;
   memset(&tr,0,sizeof(tr));
   tr.tx_buf = (unsigned long)tx;
   tr.rx_buf = (unsigned long)rx;
   tr.len = 1;
   tr.delay_usecs = delay;
   tr.speed_hz = speed;
   tr.bits_per_word = bits;
   tr.delay_usecs = 0;
   tr.cs_change = cs_change;


   ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
//   qDebug("%d,%d",tx[0],rx[0]);
   if (ret < 1)
   {
       rtn = 1;
       qDebug("can't send spi message");
   }
   else
   {
       rtn = 0;
   }

   return rx[0];


}
//==================================================================================================
//| 函数名称 | Spi_SendMultByte()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 写入1个byte
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U *tx,发送数据指针 INT8U *rx,读取数据指针 INT16U len，数据操作长度
//| INT8U cs_change true:cs数据操作前拉低，数据操作后维持0，false：cs 数据操作前拉低，数据操作后拉高
//|------------------------------------------------------------------------------------------------
//| 返回参数 | 0 正常 1 不正常
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.02.30
//==================================================================================================
INT8U Spi_SendMultByte(INT8U *tx,INT8U *rx,INT16U len,INT8U cs_change)
{
   int rtn = 0;
   int ret;

   spi_ioc_transfer tr;
   memset(&tr,0,sizeof(tr));
   tr.tx_buf = (unsigned long)tx;
   tr.rx_buf = (unsigned long)rx;
   tr.len = len;
   tr.delay_usecs = delay;
   tr.speed_hz = speed;
   tr.bits_per_word = bits;
   tr.delay_usecs = 0;
   tr.cs_change = cs_change;

   ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
   if (ret < 1)
   {
       rtn = 1;
       qDebug("can't send spi message");
   }
   else
   {
       rtn = 0;
   }

   return rtn;
}
//==================================================================================================
//| 函数名称 | Spi_IOMode()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 修改SPI总线作为输入还是正常
//|------------------------------------------------------------------------------------------------
//| 输入参数 | INT8U uch_mode 0 正常 1 spi总线全部作为输入
//|------------------------------------------------------------------------------------------------
//| 返回参数 |
//|------------------------------------------------------------------------------------------------
//| 函数设计 | lyb,2020.02.30
//==================================================================================================
INT8U Spi_IOMode(INT8U uch_mode)
{
    INT8U mode = 0;
    INT8U ret;

    INT8U rtn;
    if(uch_mode == 0)
    {
        ret = ioctl(fd,SPI_IOC_JG_BURN_START,&mode);
    }
    else
    {
        ret = ioctl(fd,SPI_IOC_JG_BURN_FINISH,&mode);
    }
//      if (ret < 1)
//       {
//           rtn = 1;
//           qDebug("can't send spi message");
//       }
//       else
//       {
//           rtn = 0;
//       }
    return ret;


}
//==================================================================================================
//| 函数名称 | transfer()
//|------------------------------------------------------------------------------------------------
//| 函数功能 | 测试spi总线，短接 miso 和mosi 读取的数据和写入的一样
//|------------------------------------------------------------------------------------------------
//| 输入参数 | int fd spi句柄
//|------------------------------------------------------------------------------------------------
//| 返回参数 |
//|------------------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
static void transfer(int fd)
{
       int ret;
       uint8_t tx[] = {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
                0xF0, 0x0D,
        };
       uint8_t rx[ARRAY_SIZE(tx)] = {0, };


       spi_ioc_transfer tr;
       memset(&tr,0,sizeof(tr));
       tr.tx_buf = (unsigned long)tx;
       tr.rx_buf = (unsigned long)rx;
       tr.len = ARRAY_SIZE(tx);
       tr.delay_usecs = delay;
       tr.speed_hz = speed;
       tr.bits_per_word = bits;
       tr.delay_usecs = 0;

        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 1)
             qDebug("can't send spi message");

        for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
                if (!(ret % 6))
                        qDebug("");
                qDebug("%.2X ", rx[ret]);
        }
        qDebug("transfer over.\n");
}



