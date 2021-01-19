#ifndef BSP_FPGA_H
#define BSP_FPGA_H


#include "Bsp_Include.h"


#define FPGA_DRAM_SIZE		(16*1024)

class CBspFpga
{


public:
    CBspFpga();
    ~CBspFpga();

    INT16U ReadFromFPGA(INT32U ul_Addr,INT16U *ui_Buf,INT32U ul_Len);
    INT32U WriteToFPGA(INT32U ul_Addr,const INT16U *ui_Buf,INT32U ul_Len);
    BOOL FPGAInit(void);
    BOOL InsmodFPGA(void);
private:
    INT16S ul_fd;
};

extern CBspFpga cl_bspFpga;

#endif // BSP_FPGA


