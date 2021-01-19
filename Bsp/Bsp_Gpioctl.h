#ifndef DRV_GPIOCTL_H
#define DRV_GPIOCTL_H



#define GPIO_B40 160
#define GPIO_B42 162
#define GPIO_B44 164


#define GPIO_C25 199

#define GPIO_D38 1
#define GPIO_D44 5
#define GPIO_D46 2
#define GPIO_D48 7
#define GPIO_D50 4


#define GPIO_LED    205
#define GPIO_ERROR  GPIO_B40
#define GPIO_RUN GPIO_B42
#define GPIO_ALARM  GPIO_B44
#define UART1_DE GPIO_C25

#define FPGA_EN GPIO_D38
#define FPGA_ASDO GPIO_D44
#define FPGA_DCLK GPIO_D46
#define FPGA_NCSO GPIO_D48
#define FPGA_DATA0 GPIO_D50

int gpio_read(int pin);
int gpio_write(int pin,int dir);
int gpio_direction(int pin,int dir);
int gpio_ctl(int number,int direction,int value);

#endif // DRV_GPIOCTL_H
