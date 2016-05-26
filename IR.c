#include "IR.h"
#include "1602.h"

char code Tab[16]="0123456789ABCDEF";
/*------------------------------------------------
                全局变量声明
------------------------------------------------*/
unsigned char  irtime;//红外用全局变量
//unsigned char IrButton;

bit irpro_ok,irok;
unsigned char IrCode[4];
unsigned char irdata[33];
unsigned char button;

extern unsigned char DisplayTemp1[16];//申明外部变量
/*------------------------------------------------
                  外部中断0初始化
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{
  AUXR &= 0x7F;
  TMOD &= 0xF0;		//设置定时器模式
  TMOD |= 0x02;		//设置定时器模式  
  TH0=0x00; //重载值
  TL0=0x00; //初始化值
  EA=1;
  ET0=1;    //开中断
  TR0=1;    
}
/*------------------------------------------------
                  定时器0中断处理
------------------------------------------------*/

void tim0_isr(void) interrupt 1 using 1
{
  irtime++;  //用于计数2个下降沿之间的时间
}
/*------------------------------------------------
/*------------------------------------------------
       外部中断0中断处理(接收)
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //外部中断0服务函数
{
  static unsigned char  i;             //接收红外信号处理
  static bit startflag;                //是否开始处理标志位

if(startflag)                         
   {
    if(irtime<60&&irtime>=40)//引导码 TC9012的头码，9ms+4.5ms
    i=0;
    irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    irtime=0;
    i++;
   	if(i==33)//接收完成
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

/*------------红外码值处理函数------------------------*/
void IrCordPro(void)
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //处理4个字节
     {
      for(j=1;j<=8;j++) //处理1个字节8位
         {
          cord=irdata[k];
          if(cord>7)//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
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
	 if(IrCode[0]==0x00&&IrCode[1]==0xff) //判断用户码
	 {
	 button=IrCode[2]; 
	 irpro_ok=1;//处理完毕标志位置1
	 irok=0;
	 }
}
/*----------------------------------------------------------------*/
/*------------------------------------------------
                  键值处理
------------------------------------------------*/

void Ir_Work(void)
{
//       TempData[0] = Tab[IRcord[0]/16];   //处理客户码
//	   TempData[1] = Tab[IRcord[0]%16];
//       TempData[2] = '-';
//	   TempData[3] = Tab[IRcord[1]/16];   //处理客户码
//	   TempData[4] = Tab[IRcord[1]%16];
//	   TempData[5] = '-';
//	   DisplayTemp1[14] = Tab[button/16];   //处理数据码
//	   DisplayTemp1[15] = Tab[button%16];
//	   TempData[8] = '-';
//	   TempData[9] = Tab[IRcord[3]/16];   //处理数据反码
//	   TempData[10] = Tab[IRcord[3]%16];
	   irpro_ok=0;//处理完成标志

  }
/*------------------------------------------------*/
 void IR_Init(void)
 {
	TIM0init();
	EX0init();
 }

