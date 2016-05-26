#include "ds1302.h"

unsigned char time_buf1[14];//读取用
unsigned char time_buf2[16]={2,0,1,4,0,8,0,8,1,6,0,0,0,5,0,7};//串口更新用

extern char DisplayTemp0[16];//申明外部变量
extern char DisplayTemp1[16];
extern bit AdFlag;
extern bit TempFlag;

static unsigned char time1=0;//计时器1用
bit TimeReadFlag;
bit TimeSetFlag;

unsigned char i;//for循环用


/*------------------------------------------------
           向DS1302写入一字节数据
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	RST_SET;	
	
	//写入目标地址：addr
	addr = addr & 0xFE;     //最低位置零
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
	
	//写入数据：d
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
	RST_CLR;					//停止DS1302总线
}
/*------------------------------------------------
           从DS1302读出一字节数据
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char temp;
	RST_SET;	

	//写入目标地址：addr
	addr = addr | 0x01;//最低位置高
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
	
	//输出数据：temp
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
	
	RST_CLR;	//停止DS1302总线
	return temp;
}

/*------------------------------------------------
           向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
	for(i=2;i<15;i=i+2)
	    {
		    time_buf2[i]<<=4;
			time_buf2[i]+=time_buf2[i+1];              
	    }
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//关闭写保护 
	Ds1302_Write_Byte(ds1302_sec_add,0x80);				//暂停 
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//涓流充电 
	Ds1302_Write_Byte(ds1302_year_add,time_buf2[2]);		//年 
	Ds1302_Write_Byte(ds1302_month_add,time_buf2[4]);	//月 
	Ds1302_Write_Byte(ds1302_date_add,time_buf2[6]);		//日 
	Ds1302_Write_Byte(ds1302_hr_add,time_buf2[8]);		//时 
	Ds1302_Write_Byte(ds1302_min_add,time_buf2[10]);		//分
	Ds1302_Write_Byte(ds1302_sec_add,time_buf2[12]);		//秒
	Ds1302_Write_Byte(ds1302_day_add,time_buf2[14]);		//周 
	Ds1302_Write_Byte(ds1302_control_add,0x80);			//打开写保护 
}

/*------------------------------------------------
           从DS1302读出时钟数据
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
	unsigned char temp;
	time_buf1[0]=Ds1302_Read_Byte(ds1302_year_add);	//年 
	time_buf1[2]=Ds1302_Read_Byte(ds1302_month_add)&0x1f;//月 
	time_buf1[4]=Ds1302_Read_Byte(ds1302_date_add)&0x3f;//日 
	time_buf1[6]=(Ds1302_Read_Byte(ds1302_hr_add))&0x3f;//时 
	time_buf1[8]=Ds1302_Read_Byte(ds1302_min_add)%0x7f;//分 
	time_buf1[10]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//秒 
	time_buf1[12]=Ds1302_Read_Byte(ds1302_day_add)&0x07;//周 


	for(i=0;i<14;i=i+2)
	   {           //BCD处理
		   temp=time_buf1[i];
		   time_buf1[i+1]=temp&0x0f;
		   time_buf1[i]>>=4;
	   }
}

/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void)
{
	
	RST_CLR;			//RST脚置低
	SCK_CLR;			//SCK脚置低
	Timer1Init();
	UartInit();
//	P3M0=0x70;		   //相关引脚设为强推挽输出
//	Ds1302_Write_Time();
}

/*---------------------------------------
           定时器1初始化
-----------------------------------------*/
void Timer1Init(void)		//50毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0xB0;		//设置定时初值
	TH1 = 0x3C;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	EA=1;
	ET1=1;
	TR1 = 1;		//定时器1开始计时
}

/*------------------------------------------------
                定时器1中断
------------------------------------------------*/
void Timer1_isr(void) interrupt 3
{
    time1++;
	TL1 = 0xB0;		//设置定时初值
	TH1 = 0x3C;		//设置定时初值
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
            串口初始化
------------------------------------------------*/
void UartInit(void)		//9600bps@12.000MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0xD9;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	ES=1;
}

/*------------------------------------------------
              串口中断
------------------------------------------------*/
void UART_SER (void) interrupt 4 //串行中断服务程序
{
    unsigned char Temp;          //定义临时变量 
    static unsigned char n=0;
    if(RI)                        //判断是接收中断产生
     {
	  RI=0;
	  Temp=SBUF;                      //标志位清零
	  time_buf2[n]=Temp&0x0f;
	  n++;
	  if(n==16)                  //连续接收16个字符信息
	   {
	    n=0;
		TimeSetFlag=1;               //接收完成标志位置1
	   }
      SBUF=Temp; //把接收到的值再发回电脑端
	 }
   if(TI)  //如果是发送标志位，清零
     TI=0;
}
