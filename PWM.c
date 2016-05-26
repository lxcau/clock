#include "PWM.h"

void PWM_Init()
{
    CCON=0;//���б�־����
	CL=0;
	CH=0;
	CMOD=0X00;//PCAʱ��Ϊϵͳʱ��/12
	CCAP0H=CCAP0L=0X80;//ռ�ձ�Ϊ50%
	PCA_PWM0=0X00;//ռ�ձȵھ�λΪ0
	CCAPM0=0X42;//����PWM���(P1.3)���ر�PCA�ж�
    CR=1;//����PCA������
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
