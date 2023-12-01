//作者QQ:497259320
//网站http://proteus.5d6d.com
//南阳理工学院
//学习是遗忘的最好的方式
//只花了很少时间写的,有很多不足的算法与编程序方式,见谅!
#include"at89x51.h"
#include"key.h"
#include"lcd.h"
uchar code show_0[]="Please input the password:";
uchar code show_1[]="The cipher is:" ;
uchar code show_2[]="********";
uchar code show_3[]="The password is wrong!";
uchar code show_4[]="This is the last time,";
uchar code show_5[]="Please try it tomorrow!";
uchar code show_6[]="you are right!";
uchar code show_7[]="Please input the new cipher:";
uchar code show_8[]="Input it again!";
uchar code show_9[]="Establishes successful! ";
uchar code show_10[]="(^_^)";
uchar  init_pswd[]={8,8,8,8,8,8,8,8};
uchar  pswd[8]={0};
uchar  pswd_1[8]={0};
void play(uchar *j,uchar k)	  //K是显示的个数,j是地址
{
   uchar  i;
   for(i=0;i<k;i++)
   {
    dat_w(*(j+i));
	delay(20);
   }
}
main()
{
  uchar j=0,i=0,sum_1=0;
  loop:init();
  clear();
  gotoxy(1,0);
  play(show_0,16);
  gotoxy(2,0);
  play(show_0+17,10);
  /****密码输入****/
  do
  {
    key();
  }
  while(ki!=15);
  clear();
  gotoxy(1,0);
  play(show_1,15);
  do
  {
    flag=0;
    key();
	gotoxy(2,0);
	if(flag==1&&ki<=9)
	 {
	  pswd[i]=ki;
	  play(show_2,i);
	  i++;
	 }
	if(ki==14)	//14则退出.
	  break;
   }	
   while(i<=8);
   for(i=0;i<=7;i++)
   {
     sum_1=init_pswd[i]-pswd[i]+sum_1;
     if(sum_1!=0)
	 {
	   clear();
	   j++;
	   if(j==3)
	   {
	    clear();
	    gotoxy(1,0);
	    play(show_4,16);
		gotoxy(2,0);
		play(show_4+16,14);
		delay(500);
		delay(500);
		clear();
		gotoxy(1,0);
		play(show_5+7,16);
		gotoxy(2,5);
		play(show_10,5);
	    while(1);   //三次输入密码失败,自锁
	   }
	   gotoxy(1,0);
	   play(show_3,16);
	   gotoxy(2,5);
	   play(show_3+15,7);
	   delay(500);
	   clear();
	   gotoxy(1,0);
	   play(show_8,15);
	   goto loop;
	  }
	  if(sum_1==0) 
	  {
	   
	   clear();
	   gotoxy(1,0);
	   play(show_6,15);
	   do
	   {
	   key();
	   }
	   while(ki!=13);
	   loop1:clear();
	   gotoxy(1,0);
	   play(show_7,13);
	   gotoxy(2,0);
	   play(show_7+13,15);
	   /****第一次输密码***/
	   i=0;
	    do
	    {
		  flag=0;
          key();
		  if(flag==1&&ki<=9)
	      {
	       pswd[i]=ki;
		   i++;
		   clear();
		   gotoxy(1,0);
		   play(show_2,i);
		   }
	   	 }while(i<=7);
		 clear();
		 gotoxy(1,0);
	     play(show_8,15);
	   /***第二次输密码****/
		 i=0;
		 sum_1=0;
	    do
	    {
		  flag=0;
          key();
		  if(flag==1&&ki<=9)
	      {
	       pswd_1[i]=ki;
		   i++;
		   clear();
		   gotoxy(1,0);
		   play(show_2,i);
		   }
	   	 }
		 while(i<=7);
		 for(i=0;i<=7;i++)
		  {
		   sum_1=sum_1+ pswd_1[i]-pswd[i];
		   }
		 if(sum_1==0)
		   {
		    clear();
		   	gotoxy(1,2);
			play(show_9,12);
			gotoxy(2,2);
			play(show_9+13,11);
			for(i=0;i<=7;i++)
			{
			init_pswd[i]=pswd[1];
			 }
			while(1);
		   }
		 if(sum_1!=0)
		   {
		     clear();
		     goto loop1;
		   }
	  }	  
    } 
	   
	 
 }  
  
  
  
