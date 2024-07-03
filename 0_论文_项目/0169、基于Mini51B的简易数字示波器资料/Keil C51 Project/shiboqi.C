/****************************************************************
           简易数字示波器
		           制作：鲁湛 2007.4.17
****************************************************************/

#include <12864.H>
#include <getdat.h>
#include <intrins.H>
uchar xdata ram[128];

unsigned char t	 ;	//分别为扫描时间，电压分辨率和暂停
unsigned char y1	 ;
unsigned char y2	 ;
bit s		;

  main()
{ 		
	EA=1,IT0=1,EX0=1,IT1=1,EX1=1,ET0=1;//开启中断
	TMOD=0x04,TL0=0x1f,TH0=0xff ,TR0=1;
	t=1,s=0;
	y1=32,y2=7;  	
	wr_pic(pic);                       //画图片
	while(1)
	   {
		while(s)					   //S控制ad转化的开关
	    {
	        getdat(t);
	        scan(y1,y2)	;
	        Delay(50)	 ;	 
	     }
		}	
}


 changetime()interrupt 0  using 1
{
	 
	 Delay(20);
	 if(IE0==1)goto end;
	 t++;
     end:if(t==4)t=0;
}

 stop()interrupt 1  using 3
{
     Delay(20);
	 if(TF0==1)goto end;
	 s=~s;
	 end:TL0=0x1f,TH0=0xff;
}      

 changey()interrupt 2  using 2
{

	 Delay(20);
	 if(IE1==1)goto end;
	 y1=y1+32;
	 y2=y2-2;
     end:if(y1==96)y1=32,y2=7;
}