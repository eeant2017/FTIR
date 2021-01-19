#ifndef SPI1_H
#define SPI1_H
#include "typedefine.h"
#define SPI_IOC_JG_BURN_START _IOW(SPI_IOC_MAGIC,6,__u32)
#define SPI_IOC_JG_BURN_FINISH _IOW(SPI_IOC_MAGIC,7,__u32)


class SPI1
{
public:
    SPI1();


private:


};



extern void SPI1Init();
extern INT8U Spi_Send1Byte(INT8U byte,INT8U cs_change);
extern INT8U Spi_SendMultByte(INT8U *tx,INT8U *rx,INT16U len,INT8U cs_change);
extern INT8U Spi_IOMode(INT8U uch_mode);



#endif // SPI1_H
