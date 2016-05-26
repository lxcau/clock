#include "adc.h"

void Adc_Init(void)
{
	P1ASF=0x01;//P1.0作ad转换用
	ADC_RES=0; //结果寄存器清零
    AUXR1|=0x4;	//结果寄存器工作方式
	ADC_RESL=0;
	ADC_CONTR=0x80;//设置工作方式
	DelayMs(2);
}


float Get_Adc(void)
{
	unsigned int temp;
	ADC_CONTR=0x88;	 //开始转换
	DelayMs(2);
	while(!ADC_CONTR&0X10);//等待检测结束
	ADC_CONTR&=0XEF;	   //结束标志清零
	temp=ADC_RES;
	temp<<=8;
	temp=temp+ADC_RESL;	//获得10位adc寄存器值
	return((float)temp/204.8);	//用5V对比电压换算测得电压值
}