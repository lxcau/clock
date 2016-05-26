#include "ds1302.h"

unsigned char time_buf1[14];//��ȡ��
unsigned char time_buf2[16]={2,0,1,4,0,8,0,8,1,6,0,0,0,5,0,7};//���ڸ�����

extern char DisplayTemp0[16];//�����ⲿ����
extern char DisplayTemp1[16];
extern bit AdFlag;
extern bit TempFlag;

static unsigned char time1=0;//��ʱ��1��
bit TimeReadFlag;
bit TimeSetFlag;

unsigned char i;//forѭ����


/*------------------------------------------------
           ��DS1302д��һ�ֽ�����
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	RST_SET;	
	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;     //���λ����
	for (i = 0; i < 8; i ++) 
	    { 
		if (addr & 0x01) 
		    {
			IO_SET;
			_nop_();
			_nop_();
			}
		else 
		    {
			IO_CLR;
			}
		SCK_SET;
		_nop_();
		_nop_();
		SCK_CLR;
		addr = addr >> 1;
		}
	
	//д�����ݣ�d
	for (i = 0; i < 8; i ++) 
	   {
		if (d & 0x01) 
		    {
			IO_SET;
			_nop_();
			_nop_();
			}
		else 
		    {
			IO_CLR;
			}
		SCK_SET;
		_nop_();
		_nop_();
		SCK_CLR;
		d = d >> 1;
		}
	RST_CLR;					//ֹͣDS1302����
}
/*------------------------------------------------
           ��DS1302����һ�ֽ�����
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char temp;
	RST_SET;	

	//д��Ŀ���ַ��addr
	addr = addr | 0x01;//���λ�ø�
	for (i = 0; i < 8; i ++) 
	    {
	     
		if (addr & 0x01) 
		   {
			IO_SET;
			_nop_();
			_nop_();
			}
		else 
		    {
			IO_CLR;
			}
		SCK_SET;
		_nop_();
		_nop_();
		SCK_CLR;
		addr = addr >> 1;
		}
	
	//������ݣ�temp
	for (i = 0; i < 8; i ++) 
	    {
		temp = temp >> 1;
		IO_SET;
		_nop_();
		_nop_();
		if (IO_R) 
		   {
			temp |= 0x80;
			}
		else 
		   {
			temp &= 0x7F;
			}
		SCK_SET;
		_nop_();
		_nop_();
		SCK_CLR;
		}
	
	RST_CLR;	//ֹͣDS1302����
	return temp;
}

/*------------------------------------------------
           ��DS1302д��ʱ������
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
	for(i=2;i<15;i=i+2)
	    {
		    time_buf2[i]<<=4;
			time_buf2[i]+=time_buf2[i+1];              
	    }
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//�ر�д���� 
	Ds1302_Write_Byte(ds1302_sec_add,0x80);				//��ͣ 
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//������ 
	Ds1302_Write_Byte(ds1302_year_add,time_buf2[2]);		//�� 
	Ds1302_Write_Byte(ds1302_month_add,time_buf2[4]);	//�� 
	Ds1302_Write_Byte(ds1302_date_add,time_buf2[6]);		//�� 
	Ds1302_Write_Byte(ds1302_hr_add,time_buf2[8]);		//ʱ 
	Ds1302_Write_Byte(ds1302_min_add,time_buf2[10]);		//��
	Ds1302_Write_Byte(ds1302_sec_add,time_buf2[12]);		//��
	Ds1302_Write_Byte(ds1302_day_add,time_buf2[14]);		//�� 
	Ds1302_Write_Byte(ds1302_control_add,0x80);			//��д���� 
}

/*------------------------------------------------
           ��DS1302����ʱ������
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
	unsigned char temp;
	time_buf1[0]=Ds1302_Read_Byte(ds1302_year_add);	//�� 
	time_buf1[2]=Ds1302_Read_Byte(ds1302_month_add)&0x1f;//�� 
	time_buf1[4]=Ds1302_Read_Byte(ds1302_date_add)&0x3f;//�� 
	time_buf1[6]=(Ds1302_Read_Byte(ds1302_hr_add))&0x3f;//ʱ 
	time_buf1[8]=Ds1302_Read_Byte(ds1302_min_add)%0x7f;//�� 
	time_buf1[10]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//�� 
	time_buf1[12]=Ds1302_Read_Byte(ds1302_day_add)&0x07;//�� 


	for(i=0;i<14;i=i+2)
	   {           //BCD����
		   temp=time_buf1[i];
		   time_buf1[i+1]=temp&0x0f;
		   time_buf1[i]>>=4;
	   }
}

/*------------------------------------------------
                DS1302��ʼ��
------------------------------------------------*/
void Ds1302_Init(void)
{
	
	RST_CLR;			//RST���õ�
	SCK_CLR;			//SCK���õ�
	Timer1Init();
	UartInit();
//	P3M0=0x70;		   //���������Ϊǿ�������
//	Ds1302_Write_Time();
}

/*---------------------------------------
           ��ʱ��1��ʼ��
-----------------------------------------*/
void Timer1Init(void)		//50����@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x10;		//���ö�ʱ��ģʽ
	TL1 = 0xB0;		//���ö�ʱ��ֵ
	TH1 = 0x3C;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	EA=1;
	ET1=1;
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
}

/*------------------------------------------------
                ��ʱ��1�ж�
------------------------------------------------*/
void Timer1_isr(void) interrupt 3
{
    time1++;
	TL1 = 0xB0;		//���ö�ʱ��ֵ
	TH1 = 0x3C;		//���ö�ʱ��ֵ
	if(time1%5==0)
	TimeReadFlag=1;
	if(time1%10==0)
	TempFlag=1;
	if(time1==20)
	{
		time1=0;
		AdFlag=1;
	}
}

/*------------------------------------------------
            ���ڳ�ʼ��
------------------------------------------------*/
void UartInit(void)		//9600bps@12.000MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	BRT = 0xD9;		//�趨���������ʷ�������װֵ
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	ES=1;
}

/*------------------------------------------------
              �����ж�
------------------------------------------------*/
void UART_SER (void) interrupt 4 //�����жϷ������
{
    unsigned char Temp;          //������ʱ���� 
    static unsigned char n=0;
    if(RI)                        //�ж��ǽ����жϲ���
     {
	  RI=0;
	  Temp=SBUF;                      //��־λ����
	  time_buf2[n]=Temp&0x0f;
	  n++;
	  if(n==16)                  //��������16���ַ���Ϣ
	   {
	    n=0;
		TimeSetFlag=1;               //������ɱ�־λ��1
	   }
      SBUF=Temp; //�ѽ��յ���ֵ�ٷ��ص��Զ�
	 }
   if(TI)  //����Ƿ��ͱ�־λ������
     TI=0;
}
