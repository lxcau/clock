#include "IR.h"
#include "1602.h"

char code Tab[16]="0123456789ABCDEF";
/*------------------------------------------------
                ȫ�ֱ�������
------------------------------------------------*/
unsigned char  irtime;//������ȫ�ֱ���
//unsigned char IrButton;

bit irpro_ok,irok;
unsigned char IrCode[4];
unsigned char irdata[33];
unsigned char button;

extern unsigned char DisplayTemp1[16];//�����ⲿ����
/*------------------------------------------------
                  �ⲿ�ж�0��ʼ��
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   //ָ���ⲿ�ж�0�½��ش�����INT0 (P3.2)
 EX0 = 1;   //ʹ���ⲿ�ж�
 EA = 1;    //�����ж�
}
/*------------------------------------------------
                ��ʱ��0��ʼ��
------------------------------------------------*/
void TIM0init(void)//��ʱ��0��ʼ��
{
  AUXR &= 0x7F;
  TMOD &= 0xF0;		//���ö�ʱ��ģʽ
  TMOD |= 0x02;		//���ö�ʱ��ģʽ  
  TH0=0x00; //����ֵ
  TL0=0x00; //��ʼ��ֵ
  EA=1;
  ET0=1;    //���ж�
  TR0=1;    
}
/*------------------------------------------------
                  ��ʱ��0�жϴ���
------------------------------------------------*/

void tim0_isr(void) interrupt 1 using 1
{
  irtime++;  //���ڼ���2���½���֮���ʱ��
}
/*------------------------------------------------
/*------------------------------------------------
       �ⲿ�ж�0�жϴ���(����)
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //�ⲿ�ж�0������
{
  static unsigned char  i;             //���պ����źŴ���
  static bit startflag;                //�Ƿ�ʼ�����־λ

if(startflag)                         
   {
    if(irtime<60&&irtime>=40)//������ TC9012��ͷ�룬9ms+4.5ms
    i=0;
    irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
    irtime=0;
    i++;
   	if(i==33)//�������
      {
	  	irok=1;
		i=0;
	  }
    }
 else
 {
  irtime=0;
  startflag=1;
 }

}

/*------------������ֵ������------------------------*/
void IrCordPro(void)
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //����4���ֽ�
     {
      for(j=1;j<=8;j++) //����1���ֽ�8λ
         {
          cord=irdata[k];
          if(cord>7)//����ĳֵΪ1������;����о��Թ�ϵ������ʹ��12M���㣬��ֵ������һ�����
             value|=0x80;
          if(j<8)
		    {
			 value>>=1;
			}
           k++;
         }
     IrCode[i]=value;
     value=0;     
     }
	 if(IrCode[0]==0x00&&IrCode[1]==0xff) //�ж��û���
	 {
	 button=IrCode[2]; 
	 irpro_ok=1;//������ϱ�־λ��1
	 irok=0;
	 }
}
/*----------------------------------------------------------------*/
/*------------------------------------------------
                  ��ֵ����
------------------------------------------------*/

void Ir_Work(void)
{
//       TempData[0] = Tab[IRcord[0]/16];   //����ͻ���
//	   TempData[1] = Tab[IRcord[0]%16];
//       TempData[2] = '-';
//	   TempData[3] = Tab[IRcord[1]/16];   //����ͻ���
//	   TempData[4] = Tab[IRcord[1]%16];
//	   TempData[5] = '-';
//	   DisplayTemp1[14] = Tab[button/16];   //����������
//	   DisplayTemp1[15] = Tab[button%16];
//	   TempData[8] = '-';
//	   TempData[9] = Tab[IRcord[3]/16];   //�������ݷ���
//	   TempData[10] = Tab[IRcord[3]%16];
	   irpro_ok=0;//������ɱ�־

  }
/*------------------------------------------------*/
 void IR_Init(void)
 {
	TIM0init();
	EX0init();
 }

