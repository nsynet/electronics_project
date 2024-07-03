//ad转换头文件

#define ad_dat(d) XBYTE [d]
#include <reg51.H>
sbit P1_3=0x93;
extern uchar xdata ram[128];
void getdat(unsigned char scantime)
{
    
	uchar j,x=0;
	for(j=0;j<=128;j++)
	     { 
	         ad_dat(0xFFF1)=0xF1;
	         Delay(scantime)	 ;
	         while(P1_3==0);//检测忙标志位
             ram[x]=ad_dat(0xFFF0);
		     x++;	
		  }
}
