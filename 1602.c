/*-----------------------------------------------
  名称：LCD1602
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：
  引脚定义如下：1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"

char DisplayTemp0[16]="check ok";//显示屏程序显示缓存(全局变量)
char DisplayTemp1[16]="no data";
char Display0[16];//当前显示器内容
char Display1[16];

#define RS_CLR RS=0 
#define RS_SET RS=1

#define RW_CLR RW=0 
#define RW_SET RW=1 

#define EN_CLR EN=0
#define EN_SET EN=1


/*------------------------------------------------
              判忙函数
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
              写入命令函数
------------------------------------------------*/
 void LCD_Write_Com(unsigned char com) 
 {  
 while(LCD_Check_Busy()); //忙则等待
 Delay1us();
 RS_CLR; 
 RW_CLR; 
 EN_SET; 
 DataPort= com; 
 _nop_(); 
 EN_CLR;
 }
/*------------------------------------------------
              写入数据函数
------------------------------------------------*/
 void LCD_Write_Data(unsigned char Data) 
 { 
 while(LCD_Check_Busy()); //忙则等待
 Delay1us();
 RS_SET; 
 RW_CLR; 
 EN_SET; 
 DataPort= Data; 
 _nop_();
 EN_CLR;
 }


/*------------------------------------------------
                清屏函数
------------------------------------------------*/
 void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }
/*------------------------------------------------
              写入字符串函数
------------------------------------------------*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     //表示第一行
 	}
 else 
 	{      
 	LCD_Write_Com(0xC0 + x);      //表示第二行
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }
/*------------------------------------------------
              写入字符函数
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
              初始化函数
------------------------------------------------*/
 void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);    /*显示模式设置*/ 
   DelayMs(2); 
   LCD_Write_Com(0x38); 
   DelayMs(2); 
   LCD_Write_Com(0x38); 
   DelayMs(2); 
   LCD_Write_Com(0x38);
   DelayMs(2);  
   LCD_Write_Com(0x08);    /*显示关闭*/
   DelayMs(2); 
   LCD_Write_Com(0x01);    /*显示清屏*/
   DelayMs(2); 
   LCD_Write_Com(0x06);    /*显示光标移动设置*/ 
   DelayMs(2); 
   LCD_Write_Com(0x0C);    /*显示开及光标设置*/
   }

/**--------------------------------------
           屏幕刷新函数
---------------------------------------*/
void LCD_Refresh(void)
{
	unsigned char i;
	LCD_Write_Com(0x80);//第一行
	for(i=0;i<16;i++)
	{
		if(DisplayTemp0[i]==Display0[i])
		{
		LCD_Write_Com(0x14);//光标右移
		continue;
		}
		if(DisplayTemp0[i]==0)DisplayTemp0[i]=' ';
		LCD_Write_Data(DisplayTemp0[i]);
	}
	LCD_Write_Com(0xc0);//第二行
	for(i=0;i<16;i++)
	{
		if(DisplayTemp1[i]==Display1[i])
		{
		LCD_Write_Com(0x14);//光标右移
		continue;
		}
		if(DisplayTemp1[i]==0)DisplayTemp1[i]=' ';
		LCD_Write_Data(DisplayTemp1[i]);
	}
	for(i=0;i<16;i++)//更新Display
	{
		Display0[i]=DisplayTemp0[i];
		Display1[i]=DisplayTemp1[i];
	}

}   

