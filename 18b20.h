#ifndef __DS18B20_H__
#define __DS18B20_H__

#include<STC12C5A60S2.h>     //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    端口定义
------------------------------------------------*/
sbit DQ=P2^0;//ds18b20 端口
/*------------------------------------------------
                  函数声明
------------------------------------------------*/
float ReadTemperature(void);
bit Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);

#endif
