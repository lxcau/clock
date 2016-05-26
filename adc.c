#include "adc.h"

void Adc_Init(void)
{
	P1ASF=0x01;//P1.0��adת����
	ADC_RES=0; //����Ĵ�������
    AUXR1|=0x4;	//����Ĵ���������ʽ
	ADC_RESL=0;
	ADC_CONTR=0x80;//���ù�����ʽ
	DelayMs(2);
}


float Get_Adc(void)
{
	unsigned int temp;
	ADC_CONTR=0x88;	 //��ʼת��
	DelayMs(2);
	while(!ADC_CONTR&0X10);//�ȴ�������
	ADC_CONTR&=0XEF;	   //������־����
	temp=ADC_RES;
	temp<<=8;
	temp=temp+ADC_RESL;	//���10λadc�Ĵ���ֵ
	return((float)temp/204.8);	//��5V�Աȵ�ѹ�����õ�ѹֵ
}