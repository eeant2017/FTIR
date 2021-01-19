#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QMutex>


QMutex mutex_gpio;
//export gpio
int gpio_export(int pin)
{

    int fd;
    char *file_add = "/sys/class/gpio/export" ;
    char uch_buff[64];
    int len;
    mutex_gpio.lock();
    fd = open(file_add,O_WRONLY);
    if(fd < 0){
        //printf("/sys/class/gpio/export open faile");
        mutex_gpio.unlock();
        return (-1);
    }

    sprintf(uch_buff,"%d",pin);

    if(write(fd,uch_buff,strlen(uch_buff)) < 0){
        //printf("Failed to export gpio");
        mutex_gpio.unlock();
        return -1;
    }

    close(fd);
    mutex_gpio.unlock();
    return 0;

}

//export gpio
int gpio_unexport(int pin)
{

    int fd;
    char *file_add = "/sys/class/gpio/unexport" ;
    char uch_buff[64];
    mutex_gpio.lock();
    fd = open(file_add,O_WRONLY);
    if(fd < 0){
        //printf("/sys/class/gpio/unexport open faile");
        mutex_gpio.unlock();
        return (-1);
    }

    sprintf(uch_buff,"%d",pin);

    if(write(fd,uch_buff,sizeof(uch_buff))<0){
        //printf("Failed to unexport gpio");
        mutex_gpio.unlock();
        return -1;
    }

    close(fd);
    mutex_gpio.unlock();
    return 0;

}

// 0 -> in   ,1 -> out
int gpio_direction(int pin,int dir)
{
    mutex_gpio.lock();
    int fd;
    //char *addr = "/sys/class/gpio/gpio%d/direction";
    char path[64];
    char *uch_str;

    sprintf(path,"/sys/class/gpio/gpio%d/direction",pin);
    fd = open(path,O_WRONLY);
    if(fd<0){
        //printf("Failed to open gpio direction for writing");
        mutex_gpio.unlock();
        return -1;
    }
    //判断引脚输出方向
    if(dir == 0){
        uch_str = "in";
    }
    else{
        uch_str = "out";
    }

    if(write(fd,uch_str,sizeof(uch_str))<0){
        //printf("Failed to set direction");
        mutex_gpio.unlock();
        return -1;

    }

    close(fd);
    mutex_gpio.unlock();
    return 0;
}

// 0 -> LOw   ,1 -> High
int gpio_write(int pin,int dir)
{

    int fd;

    char path[64];
    char *uch_str;
    mutex_gpio.lock();
    sprintf(path,"/sys/class/gpio/gpio%d/value",pin);
    fd = open(path,O_WRONLY);
    if(fd<0){
        //printf("Failed to open gpio value for writing");
        mutex_gpio.unlock();
        return -1;
    }
    //判断引脚输出方向
    if(dir == 0){
       uch_str = "0";
    }
    else{
       uch_str = "1";
    }

    if(write(fd,uch_str,1)<0){
        //printf("Failed to set value");
        mutex_gpio.unlock();
        return -1;
    }

    close(fd);
    mutex_gpio.unlock();
    return 0;
}

// 0 -> LOw   ,1 -> High
int gpio_read(int pin)
{

    int fd;

    char path[64];
    char uch_str[1];
    mutex_gpio.lock();
    sprintf(path,"/sys/class/gpio/gpio%d/value",pin);
    fd = open(path,O_RDONLY);
    if(fd<0){
        //printf("Failed to open gpio value for read");
        mutex_gpio.unlock();
        return -1;
    }

    if(read(fd,uch_str,1)<0){
        //printf("Failed to read value\n");
        mutex_gpio.unlock();
        return -1;
    }

    close(fd);

    mutex_gpio.unlock();
    return atoi(uch_str);
}

int gpio_ctl(int number,int direction,int value)
{
    int fd;
    int Temp;


    gpio_export(number);
    gpio_direction(number,direction);
    if(direction)//out
        gpio_write(number,value);
    else         //read io
        Temp = gpio_read(number);

    gpio_unexport(number);



    return Temp;
}
