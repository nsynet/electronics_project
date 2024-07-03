

typedef struct  
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;
	unsigned char Month;
	unsigned char Year;
}SYSTEMTIME;	//定义的时间类型

SYSTEMTIME  CurrentTime;


#define AM(X)	X
#define PM(X)	(X+12)            	  // 转成24小时制
#define DS1302_SECOND	0x81
#define DS1302_MINUTE	0x83
#define DS1302_HOUR		0x85 
#define DS1302_DAY		0x87
#define DS1302_MONTH	0x89
#define DS1302_WEEK  	0x8b
#define DS1302_YEAR		0x8d
#define DS1302_RAM(X)	(0xC0+(X)*2)   	//用于计算 DS1302_RAM 地址的宏 


Write1302(unsigned char com,unsigned char wd_data)
{ 
  unsigned char m;
  DS1302_IO=0;
  DS1302_RST=0;
  DS1302_CLK=0;
  ACC=com;	           //命令字节
  DS1302_RST=1;
  for(m=8;m>0;m--)
  {
     DS1302_CLK=0;
	  ACC=ACC>>1;
	  DS1302_IO=CY;            //写1个比特
	  DS1302_CLK=1;           //上升沿，1302从主cpu读入命令
  }
  ACC=wd_data;
  for(m=8;m>0;m--)
  {
     DS1302_CLK=0;
	  ACC=ACC>>1;
	  DS1302_IO=CY;            //写1个比特
	  DS1302_CLK=1;           //上升沿，1302从主cpu读入命令
  }	  
  DS1302_RST =0;
  DS1302_IO=0;
}

Read1302(unsigned char comand)
{ 
  unsigned char m;
  DS1302_RST=0;
  DS1302_CLK=0;
  ACC=comand;	           //命令字节
  DS1302_RST=1;
  for(m=8;m>0;m--)
  { 
     DS1302_CLK=0;
	  ACC=ACC>>1;
	  DS1302_IO=CY;            //写1个比特
	  DS1302_CLK=1;           //上升沿，1302从主cpu读入命令
	  //_nop_();
  }
  for(m=8;m>0;m--)
  { 
     DS1302_CLK=1;
//	   _nop_();
	  DS1302_CLK=0;	        //1302在每个下降沿输出一位数据       	   
	  ACC=ACC>>1;
	  ACC7=DS1302_IO;         //主cpu读一个比特
  }
  DS1302_RST=0;
  DS1302_IO=0;//////////////////////不加，第二次读数为错误
  return ACC;	       // 返回读出的数据
}

void DS1302_SetProtect(unsigned char flag)       //是否写保护
{
	if(flag)
		Write1302(0x8E,0x10);
	else
		Write1302(0x8E,0x00);
}




void DS1302_SetTime(unsigned char Address, unsigned char Value)        // 设置时间函数
{
	DS1302_SetProtect(0);
	Write1302(Address, (((Value/10)<<4) | (Value%10))); 	
}

idata unsigned char set_time_buf[21] ;

void Serial_SetTime(void)        // 串口设置时间函数
{
   DS1302_SetTime(DS1302_YEAR-1,(set_time_buf[2]-'0')*10 + (set_time_buf[3]-'0'))	;	
	DS1302_SetTime(DS1302_MONTH-1,(set_time_buf[5]-'0')*10 + (set_time_buf[6]-'0'))	;
	DS1302_SetTime(DS1302_DAY-1,(set_time_buf[8]-'0')*10 + (set_time_buf[9]-'0'))	;

	DS1302_SetTime(DS1302_WEEK-1,set_time_buf[11]-'0')	;

	DS1302_SetTime(DS1302_HOUR-1,(set_time_buf[13]-'0')*10 + (set_time_buf[14]-'0'))	;
	DS1302_SetTime(DS1302_MINUTE-1,(set_time_buf[16]-'0')*10 + (set_time_buf[17]-'0'));
	DS1302_SetTime(DS1302_SECOND-1,(set_time_buf[19]-'0')*10 + (set_time_buf[20]-'0'));		
} 

void DS1302_GetTime(SYSTEMTIME *Time)
{
	unsigned char ReadValue;
	ReadValue = Read1302(DS1302_SECOND);

	Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);

	ReadValue = Read1302(DS1302_MINUTE);
	Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);

	ReadValue = Read1302(DS1302_HOUR);
	Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);

	ReadValue = Read1302(DS1302_DAY);
	Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
		
	ReadValue = Read1302(DS1302_WEEK);
	Time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);

	ReadValue = Read1302(DS1302_MONTH);
	Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);

	ReadValue = Read1302(DS1302_YEAR);
	Time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);	
}
CountDay()
{
   unsigned char month=9;
   unsigned char day=3;
   unsigned int count;
   count=(month-CurrentTime.Month)*31-CurrentTime.Day+day;
   return count;
}

void Initial_DS1302( )
{
	unsigned char Second;
	delay(65535);
	Second=Read1302(DS1302_SECOND);
	//P1=Second;	//0x80  stop
	if((Second&0x80)==0x80)
	{
	   delay(65535);
	   Second=Read1302(DS1302_SECOND);	 //读2次，更加可靠。
	   if((Second&0x80)==0x80)
		{		  
	      DS1302_SetTime(DS1302_SECOND-1,0);
	   }
	}
}