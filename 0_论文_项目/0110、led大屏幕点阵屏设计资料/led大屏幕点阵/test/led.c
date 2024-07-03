#include<reg51.h>
#include<absacc.h>
#include"disdata.c"
//#include"numbercode.c"

#define  WRled   XBYTE[0xfff0]

sbit UpData= P1^3;
sbit DisSwitch  = P1^4;

sbit RowA=P1^5;
sbit RowB=P1^6;
sbit RowC=P1^7;

delay(unsigned int time)
{while(time--);}

ChangeData(unsigned char row)
{switch(row)
         { case 0 : RowA=RowB=RowC=1;  break;
		   case 1 : RowC=RowB=1;RowA=0;break;
		   case 2 : RowA=RowC=1;RowB=0;break;
		   case 3 : RowA=RowB=0;RowC=1;break;
		   case 4 : RowA=RowB=1;RowC=0;break;
		   case 5 : RowA=RowC=0;RowB=1;break;
		   case 6 : RowB=RowC=0;RowA=1;break;
		   case 7 : RowA=RowB=RowC=0;  break;
		   default :break;
		  }
 UpData=1;
 UpData=0;
 DisSwitch=1;
 delay(50);
 DisSwitch=0;
}

void main()
{
 unsigned char count;
 unsigned char Row;
 unsigned char Page;
 UpData=0;
 DisSwitch=0;//off led
 while(1){ 
           for(Row=0;Row<8;Row++)
		       {
			    for(Page=4;Page>0;Page--)
				   {for(count=20;count>0;count--)
				       {WRled=dis[160*(Page-1)+20*Row+count-1];}
				   }
		         ChangeData(Row);
				}			     
		  }
 }