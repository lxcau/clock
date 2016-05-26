#ifndef __DS18B20_H__
#define __DS18B20_H__

#include<STC12C5A60S2.h>     //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    �˿ڶ���
------------------------------------------------*/
sbit DQ=P2^0;//ds18b20 �˿�
/*------------------------------------------------
                  ��������
------------------------------------------------*/
float ReadTemperature(void);
bit Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);

#endif
