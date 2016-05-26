#include <STC12C5A60S2.h>
#include <stdio.h>
#include "delay.h"
#include "IR.h"
#include "1602.h"
#include "PWM.h"
#include "adc.h"
#include "18b20.h"
#include "DS1302.h"

extern char DisplayTemp0[16];
extern char DisplayTemp1[16];
extern bit irok;
extern bit irpro_ok;
extern unsigned char IrCode[4];
extern bit TimeReadFlag;
extern bit TimeSetFlag;
extern unsigned char i; //for循环用
extern unsigned char time_buf1[14];
extern unsigned char time_buf2[16];
extern unsigned char button;
bit AdFlag;
bit TempFlag;

/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main(void)
{
 unsigned char pwm;
 float temp;
 bit change=1;
 unsigned char TimeCount;

 LCD_Init();           //初始化液晶
 Adc_Init();
 PWM_Init();
 IR_Init();
 Ds1302_Init();
 LCD_Clear();          //清屏
// Ds1302_Write_Time();


 while(1)//主循环
   {
	   /*****adc部分*****/
	   if(AdFlag)
	   {
	   AdFlag=0;
	   temp=Get_Adc();
	   sprintf(&DisplayTemp1[11],"%1.2f",temp);
	   if(temp>2)pwm=255;
	   else
	   pwm=(int)(temp*127);
	   if(pwm<10)pwm=10;
	   CCAP0H=pwm;
	   }
	   /*****温度部分*****/
	   if(TempFlag)
	   {
	   	   TempFlag=0;
		   temp=ReadTemperature();
		   sprintf(DisplayTemp1,"TEMP:%5.2f",temp);
	   }
	   /*****lcd刷新部分*****/
	   LCD_Refresh();
	   /*****红外部分*****/
	   if(irok)
	   {
	   	   IrCordPro();
		   if(irpro_ok)
		   Ir_Work();
	   }
	   /*****时间/日期切换部分*****/
	   if(change)
	   {
	   if(TimeReadFlag)
	   {
	   TimeReadFlag=0;
	   Ds1302_Read_Time();
       sprintf(DisplayTemp0,"TIME:%d%d:%d%d:%d%d  ",(int)time_buf1[6],(int)time_buf1[7],(int)time_buf1[8],(int)time_buf1[9],(int)time_buf1[10],(int)time_buf1[11]);//时分秒
	   }
	   }
	   else
	   {
	   	   sprintf(DisplayTemp0,"DATE:20%d%d-%d%d-%d%d",(int)time_buf1[0],(int)time_buf1[1],(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[4],(int)time_buf1[5]);
	   }
	   /*****时间更新部分*****/
	   if(TimeSetFlag)
	   {
		   TimeSetFlag=0;
	   	   Ds1302_Write_Time();
	   }
	   if(button==0x45)
	   {
	   		button=0;
			change=!change;
	   }
	   /*****菜单部分/按键扫描*****/
	   if(button==0x46)
	   {
	   	   button=0;
		   sprintf(DisplayTemp0,"1. SET TIME  ");
		   sprintf(DisplayTemp1,"2. SET LIGHT ");
		   i=0;
		   LCD_Refresh();
		   button=0x45;
		   while(1)
		   {
		       LCD_Refresh();
			   DelayMs(1000);
			   sprintf(DisplayTemp0,"check      ");
			   sprintf(DisplayTemp1,"count %d ",(int)i);
			   if(i>20)i=0;
			   i++;
			   if(button==0x45) 
			   sprintf(&DisplayTemp0[11],"ok");
//		       if(button==0x0c)
//		       {
//		           button=0;
//				   Ds1302_Read_Time();
//				   sprintf(DisplayTemp0,"20%d%d%d%d%d%d   ",(int)time_buf1[0],(int)time_buf1[1],(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[4],(int)time_buf1[5]);
//				   sprintf(DisplayTemp1,"%d%d%d%d%d%d     ",(int)time_buf1[6],(int)time_buf1[7],(int)time_buf1[8],(int)time_buf1[9],(int)time_buf1[10],(int)time_buf1[11]);
//				   while(1)
//				   {
//				   	   LCD_Refresh();
//					   if(button==0x45)break;
//					   if(TimeCount<8)DisplayTemp0[TimeCount]=' ';
//					   else DisplayTemp1[TimeCount%8]=' ';
//					   DelayMs(200);
//					   switch(button)
//					   {
//					   	   case 0x16:time_buf2[TimeCount]='0';break;
//						   case 0x0c:time_buf2[TimeCount]='1';break;
//						   case 0x18:time_buf2[TimeCount]='2';break;
//						   case 0x5e:time_buf2[TimeCount]='3';break;
//						   case 0x08:time_buf2[TimeCount]='4';break;
//						   case 0x1c:time_buf2[TimeCount]='5';break;
//						   case 0x5a:time_buf2[TimeCount]='6';break;
//						   case 0x42:time_buf2[TimeCount]='7';break;
//						   case 0x52:time_buf2[TimeCount]='8';break;
//						   case 0x4a:time_buf2[TimeCount]='9';break;
//						   case 0x09:time_buf2[TimeCount]++;break;
//						   case 0x15:time_buf2[TimeCount]--;break;
//						   case 0:break;
//						   default:break;
//					   }
//					   TimeCount++;
//					   if(TimeCount<8)DisplayTemp0[TimeCount]=time_buf2[TimeCount];
//					   else DisplayTemp1[TimeCount%8]=time_buf2[TimeCount];
//					   if(TimeCount==15)
//					   {
//					   	   Ds1302_Write_Time();
//						   button=0x45;
//					   }
//				   }
//				   			   button=0x45;
//
//		       }
		   }
	   }
   }//主循环
}