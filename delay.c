#include "delay.h"
#include <intrins.h>
/*------------------------------------------------
��STC-ISP��д���ֱ�����������ʱ����
------------------------------------------------*/
void Delay1us()		//@12.000MHz
{
	_nop_();
	_nop_();
}

//void Delay100us()		//@12.000MHz
//{
//	unsigned char i, j;
//
//	_nop_();
//	_nop_();
//	i = 2;
//	j = 38;
//	do
//	{
//		while (--j);
//	} while (--i);
//}

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}

void DelayUs(int n)  //������ʱn us
{
	while(n--)
	Delay1us();
}

void DelayMs(int n)  //������ʱn ms
{
	while(n--)
	Delay1ms();
}
