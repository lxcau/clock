/*-----------------------------------------------
  ���ƣ�LCD1602
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�
  ���Ŷ������£�1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"

char DisplayTemp0[16]="check ok";//��ʾ��������ʾ����(ȫ�ֱ���)
char DisplayTemp1[16]="no data";
char Display0[16];//��ǰ��ʾ������
char Display1[16];

#define RS_CLR RS=0 
#define RS_SET RS=1

#define RW_CLR RW=0 
#define RW_SET RW=1 

#define EN_CLR EN=0
#define EN_SET EN=1


/*------------------------------------------------
              ��æ����
------------------------------------------------*/
 bit LCD_Check_Busy(void) 
 { 
 DataPort= 0xFF; 
 RS_CLR; 
 RW_SET; 
 EN_CLR; 
 _nop_(); 
 EN_SET;
 return (bit)(DataPort & 0x80);
 }
/*------------------------------------------------
              д�������
------------------------------------------------*/
 void LCD_Write_Com(unsigned char com) 
 {  
 while(LCD_Check_Busy()); //æ��ȴ�
 Delay1us();
 RS_CLR; 
 RW_CLR; 
 EN_SET; 
 DataPort= com; 
 _nop_(); 
 EN_CLR;
 }
/*------------------------------------------------
              д�����ݺ���
------------------------------------------------*/
 void LCD_Write_Data(unsigned char Data) 
 { 
 while(LCD_Check_Busy()); //æ��ȴ�
 Delay1us();
 RS_SET; 
 RW_CLR; 
 EN_SET; 
 DataPort= Data; 
 _nop_();
 EN_CLR;
 }


/*------------------------------------------------
                ��������
------------------------------------------------*/
 void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }
/*------------------------------------------------
              д���ַ�������
------------------------------------------------*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     //��ʾ��һ��
 	}
 else 
 	{      
 	LCD_Write_Com(0xC0 + x);      //��ʾ�ڶ���
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }
/*------------------------------------------------
              д���ַ�����
------------------------------------------------*/
// void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
// {     
// if (y == 0) 
// 	{     
// 	LCD_Write_Com(0x80 + x);     
// 	}    
// else 
// 	{     
// 	LCD_Write_Com(0xC0 + x);     
// 	}        
// LCD_Write_Data( Data);  
// }
/*------------------------------------------------
              ��ʼ������
------------------------------------------------*/
 void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);    /*��ʾģʽ����*/ 
   DelayMs(2); 
   LCD_Write_Com(0x38); 
   DelayMs(2); 
   LCD_Write_Com(0x38); 
   DelayMs(2); 
   LCD_Write_Com(0x38);
   DelayMs(2);  
   LCD_Write_Com(0x08);    /*��ʾ�ر�*/
   DelayMs(2); 
   LCD_Write_Com(0x01);    /*��ʾ����*/
   DelayMs(2); 
   LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/ 
   DelayMs(2); 
   LCD_Write_Com(0x0C);    /*��ʾ�����������*/
   }

/**--------------------------------------
           ��Ļˢ�º���
---------------------------------------*/
void LCD_Refresh(void)
{
	unsigned char i;
	LCD_Write_Com(0x80);//��һ��
	for(i=0;i<16;i++)
	{
		if(DisplayTemp0[i]==Display0[i])
		{
		LCD_Write_Com(0x14);//�������
		continue;
		}
		if(DisplayTemp0[i]==0)DisplayTemp0[i]=' ';
		LCD_Write_Data(DisplayTemp0[i]);
	}
	LCD_Write_Com(0xc0);//�ڶ���
	for(i=0;i<16;i++)
	{
		if(DisplayTemp1[i]==Display1[i])
		{
		LCD_Write_Com(0x14);//�������
		continue;
		}
		if(DisplayTemp1[i]==0)DisplayTemp1[i]=' ';
		LCD_Write_Data(DisplayTemp1[i]);
	}
	for(i=0;i<16;i++)//����Display
	{
		Display0[i]=DisplayTemp0[i];
		Display1[i]=DisplayTemp1[i];
	}

}   

