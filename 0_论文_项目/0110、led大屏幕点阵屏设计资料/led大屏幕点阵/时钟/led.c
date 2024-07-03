#include<reg51.h>
#include<absacc.h>
#include<stdio.h>
#include"define.h"
#include"disdata.c"
#include"numbercode.c"
#include"word.c"
#include"1302.h"
#include"autoisp.c"
#include"discontrol.h"

/*******************
使用双倍速下载，波特率为115200bps
 可以通过串口设定时间，格式如下：
 2008-02-25/5/14:35:20
****************/
void main()
{
   unsigned char Row = 0;
   unsigned int count = 0;
   unsigned char Sel = 0;
	timer1_init();
   UpData=0;
   DisSwitch=0;//off led
	Initial_DS1302();
   while(1)
	{  
      for(Row=0;Row<8;Row++)
	   {
		   if(count>300)
		   {
			   switch(Sel)
				{
				   case 0: Out_pic(Row,dis1);	break; //图片1
				   case 1: Out_pic(Row,dis2);  break; //图片2
				   case 2: Out_pic(Row,dis3);  break; //图片3
				   //case 0: Out_count(Row)	 ;  break; //倒计时牌
					default: Out_pic(Row,dis1);Sel=0;break;
				}
			}	
	      else 
			{
			   Out_row(Row); //显示万年历
			}
		   ChangeData(Row);
		 }
		 if(count>450)
		 {
		    count=0;	 
			 Sel++;
		 }
		 count++;		  			     
	}
}




