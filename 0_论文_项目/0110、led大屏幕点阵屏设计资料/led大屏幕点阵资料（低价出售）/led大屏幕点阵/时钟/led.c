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
ʹ��˫�������أ�������Ϊ115200bps
 ����ͨ�������趨ʱ�䣬��ʽ���£�
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
				   case 0: Out_pic(Row,dis1);	break; //ͼƬ1
				   case 1: Out_pic(Row,dis2);  break; //ͼƬ2
				   case 2: Out_pic(Row,dis3);  break; //ͼƬ3
				   //case 0: Out_count(Row)	 ;  break; //����ʱ��
					default: Out_pic(Row,dis1);Sel=0;break;
				}
			}	
	      else 
			{
			   Out_row(Row); //��ʾ������
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




