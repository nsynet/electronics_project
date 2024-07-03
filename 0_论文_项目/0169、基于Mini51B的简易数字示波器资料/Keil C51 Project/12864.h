#include <reg51.H>
#include <absacc.h>
#include <word.H>
/*�������궨��Ϊ�ⲿ��д��ַ*/
#define read_busy XBYTE [0xf6ea]
#define wr_command(b) XBYTE [b]
#define wr_date(c) XBYTE [c]

#define cs1 0x0400//ѡ��Ƭ1��Ƭ2
#define cs2 0x0800
#define lcd_on 0x3f//��ʾ����
#define lcd_off 0x3e
#define word 15	   //ѡ��д16*16
#define number 7   //ѡ��д8*16
#define set_page 0xb8//��ʼҳ��
#define set_x 0x40//��ʼ�е�ַ
extern uchar xdata ram[128];


void Delay(unsigned int count)
{
	unsigned char i,j,k;
	for(i=0;i<count;i++)
	   {
		for(j=0;j<count;j++)
	    for(k=0;k<50;k++);
	   }
}

//���æ��־λ
void wait()		  
{   
	uchar busy ;
	busy=read_busy	   ;
	while ((busy|0x7f)==0xff)
	busy=read_busy ;
}

//дָ��
void write_command(unsigned int cs,uchar input)
{
	unsigned int x  ;
	x=(0xf0ea|cs)	;//ѡ��ƬһƬ��
	wr_command(x)=input  ;
	wait() 		  ;
}
//д������
void write_date(unsigned int cs,uchar input) 
{    	 
	unsigned int x  ;
	x=(0xf1ea|cs)	;//ѡ��ƬһƬ��
	wr_date(x)=input  ;
	wait()           ;
}


/*��������*/
void lcd_clear(unsigned int cs)
{
	uchar i,j,c_page=0xb8  	;
	write_command(cs,c_page);
	write_command(cs,0x40);
	for(i=0;i<=7;i++)
	    {
		 for(j=0;j<=63;j++)
		 write_date(cs,0x00);
		 c_page=++c_page;
		 write_command(cs,c_page);
		 }
	write_command(cs,lcd_on);
}


//д���麺��

/*void wr_word(unsigned int cs,uchar page,uchar lcd_x,uchar n,uchar x[])
{
	unsigned char m,i,k;
	m=0;
	write_command(cs,lcd_on);
	write_command(cs,page);
	write_command(cs,lcd_x);
	for (k=0;k<=1;k++)	//kΪִ�л�ҳ�Ĵ���
	    {
		    for (i=m;i<=n;i++)//iΪһ��д�е�����
	        write_date(cs,x[i]);
			m=n+1 ,n=m+m-1 ;  //m��n����iд������
		    page++;
		    write_command(cs,page);
		    write_command(cs,lcd_x);
		 } 
}*/ 

//���溯��
void wr_pic(uchar x[])
{
	unsigned char i,j,page,lcd_x;
	unsigned int m;
	lcd_clear(cs1);
	lcd_clear(cs2);
	page=0xb8,lcd_x=0x40,m=0;
	write_command(cs1,page);
	write_command(cs1,lcd_x);
	for (j=0;j<8;j++)	//kΪִ�л�ҳ�Ĵ���
	    {
		    for (i=0;i<=63;i++)//iΪһ��д�е�����
	            {
				 write_date(cs1,x[m]);
				 m++;
				 }
			
			write_command(cs2,page);
	        write_command(cs2,lcd_x);			
			for (i=0;i<=63;i++)//iΪһ��д�е�����
	            { 
				 write_date(cs2,x[m]);
				 m++;
				 }
		    page=++page;
		    write_command(cs1,page);
		    write_command(cs1,lcd_x);
		 }
}

//����ɨ�躯��
void scan(uchar y1,uchar y2)
{
    uchar x,page,i,k,lcd_y,z	;
	x=0	  ;
	lcd_clear(cs1);
	lcd_clear(cs2);	
	for (i=0;i<=63;i++)//iΪһ��д�е�����
	     {
			   k=ram[x]	  ;
			   page=k/y1,z=k%y1,lcd_y=z/(y1/8);
	           write_command(cs1,set_page+y2-page);
			   write_date(cs1,dat_y[lcd_y]);
			   x++	 ;
		 }
	   x=64 ;
	for (i=0;i<=63;i++)//iΪһ��д�е�����
	     {
				k=ram[x]	  ;
				page=k/y1,z=k%y1,lcd_y=z/(y1/8);
	            write_command(cs2,set_page+y2-page);
			    write_date(cs2,dat_y[lcd_y]);
				x++;
				 }
}




