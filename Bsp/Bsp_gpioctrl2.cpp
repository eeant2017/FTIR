#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#include "Bsp_gpioctrl2.h"
 
//char *pin = io第几组 * 32 + 第几号引脚，
//例如：PA8：pin=0*32+8=8;  PF6：pin=6*32+6=198;
 
//char *mode 通常为out/in, out为输出, in为输入
//char *value 通常为0/1, 0为低电平, 1位高电平



//使用Linux通用gpio接口控制io口函数
int Linux_Gpio_Config(char *pin, char *mode, char *value)
{
	static int dev_fd, export_fd, dir_fd, ret;
	char DEV_PATH[64], EXPORT_PATH[64], DIRECT_PATH[64];
	char buf[10];
 
	sprintf(DEV_PATH, "/sys/class/gpio/gpio%s/value", pin);
	sprintf(EXPORT_PATH, "/sys/class/gpio/export");
	sprintf(DIRECT_PATH, "/sys/class/gpio/gpio%s/direction", pin);
 
	export_fd = open(EXPORT_PATH, O_WRONLY);
	if(export_fd == -1)
	{
		perror("export open failed!\n");
		return -1;
	}
 
	write(export_fd, pin, strlen(pin));
 
	dev_fd = open(DEV_PATH, O_RDWR);
	if(dev_fd == -1)
	{
		perror("dev open faield!\n");
		return -1;
	}
 
	dir_fd = open(DIRECT_PATH, O_RDWR);
	if(dir_fd == -1)
	{
		perror("dir open failed!\n");
		return -1;
	}
	
	memset(buf, 0, sizeof(buf));	
	strcpy(buf, mode);
 
	ret = write(dir_fd, buf, strlen(mode));
	if(ret == -1)
	{
		perror("dir write failed!\n");
		close(export_fd);
		close(dir_fd);
		close(dev_fd);
		return -1;
	}
 
	memset(buf, 0, sizeof(buf));
	strcpy(buf, value);
	ret = write(dev_fd, buf, strlen(value));
	if(ret == -1)
	{
		perror("dev write failed!\n");
		return -1;
	}
 
	close(export_fd);
	close(dir_fd);
	close(dev_fd);
 
	return 0;

}

void print_usage()
{
	printf("Usage:./gpio_ctrl pin mode value\n");
	printf("	./gpio_ctrl 137 out 1\n");
	printf("	./gpio_ctrl 137 out 0\n");
}

////demo
//int main(int argc, char **argv)
//{
//	char *pin, *mode, *value;
	
//	if(argc != 4)
//	{
//		print_usage();
//		return -1;
//	}
	
//	pin 	= argv[1];
//	mode 	= argv[2];
//	value	= argv[3];
	
//	printf("pin = %s, mode = %s, value = %s\n", pin, mode, value);
	
//	Linux_Gpio_Config(pin, mode, value);
 
//	return 0;
//}
