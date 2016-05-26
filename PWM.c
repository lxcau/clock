#include "PWM.h"

void PWM_Init()
{
    CCON=0;//所有标志清零
	CL=0;
	CH=0;
	CMOD=0X00;//PCA时钟为系统时钟/12
	CCAP0H=CCAP0L=0X80;//占空比为50%
	PCA_PWM0=0X00;//占空比第九位为0
	CCAPM0=0X42;//允许PWM输出(P1.3)，关闭PCA中断
    CR=1;//启动PCA计数器
}

void Set_PWM(unsigned char n)
{
	 switch(n)
	 {
	     case 1:
		 CCAP0H=0X19;break;
		 case 2:
		 CCAP0H=0x33;break;
		 case 3:
		 CCAP0H=0x4d;break;
		 case 4:
		 CCAP0H=0x66;break;
		 case 5:
		 CCAP0H=0x80;break;
		 case 6:
		 CCAP0H=0x9a;break;
		 case 7:
		 CCAP0H=0xb3;break;
		 case 8:
		 CCAP0H=0xcd;break;
		 case 9:
		 CCAP0H=0xe6;break;
		 case 10:
		 CCAP0H=0xff;break;
		 default:break;
	 }
}
