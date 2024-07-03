#include <reg51.H>
#include <absacc.h>
#include <word.H>
/*带参数宏定义为外部读写地址*/
#define read_busy XBYTE [0xf6ea]
#define wr_command(b) XBYTE [b]
#define wr_date(c) XBYTE [c]

#define cs1 0x0400//选择片1，片2
#define cs2 0x0800
#define lcd_on 0x3f//显示开关
#define lcd_off 0x3e
#define word 15	   //选择写16*16
#define number 7   //选择写8*16
#define set_page 0xb8//初始页面
#define set_x 0x40//初始列地址
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

//检测忙标志位
void wait()		  
{   
	uchar busy ;
	busy=read_busy	   ;
	while ((busy|0x7f)==0xff)
	busy=read_busy ;
}

//写指令
void write_command(unsigned int cs,uchar input)
{
	unsigned int x  ;
	x=(0xf0ea|cs)	;//选择片一片二
	wr_command(x)=input  ;
	wait() 		  ;
}
//写入数据
void write_date(unsigned int cs,uchar input) 
{    	 
	unsigned int x  ;
	x=(0xf1ea|cs)	;//选择片一片二
	wr_date(x)=input  ;
	wait()           ;
}


/*清屏函数*/
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


//写方块汉字

/*void wr_word(unsigned int cs,uchar page,uchar lcd_x,uchar n,uchar x[])
{
	unsigned char m,i,k;
	m=0;
	write_command(cs,lcd_on);
	write_command(cs,page);
	write_command(cs,lcd_x);
	for (k=0;k<=1;k++)	//k为执行换页的次数
	    {
		    for (i=m;i<=n;i++)//i为一次写列的列数
	        write_date(cs,x[i]);
			m=n+1 ,n=m+m-1 ;  //m，n控制i写的列数
		    page++;
		    write_command(cs,page);
		    write_command(cs,lcd_x);
		 } 
}*/ 

//画面函数
void wr_pic(uchar x[])
{
	unsigned char i,j,page,lcd_x;
	unsigned int m;
	lcd_clear(cs1);
	lcd_clear(cs2);
	page=0xb8,lcd_x=0x40,m=0;
	write_command(cs1,page);
	write_command(cs1,lcd_x);
	for (j=0;j<8;j++)	//k为执行换页的次数
	    {
		    for (i=0;i<=63;i++)//i为一次写列的列数
	            {
				 write_date(cs1,x[m]);
				 m++;
				 }
			
			write_command(cs2,page);
	        write_command(cs2,lcd_x);			
			for (i=0;i<=63;i++)//i为一次写列的列数
	            { 
				 write_date(cs2,x[m]);
				 m++;
				 }
		    page=++page;
		    write_command(cs1,page);
		    write_command(cs1,lcd_x);
		 }
}

//画面扫描函数
void scan(uchar y1,uchar y2)
{
    uchar x,page,i,k,lcd_y,z	;
	x=0	  ;
	lcd_clear(cs1);
	lcd_clear(cs2);	
	for (i=0;i<=63;i++)//i为一次写列的列数
	     {
			   k=ram[x]	  ;
			   page=k/y1,z=k%y1,lcd_y=z/(y1/8);
	           write_command(cs1,set_page+y2-page);
			   write_date(cs1,dat_y[lcd_y]);
			   x++	 ;
		 }
	   x=64 ;
	for (i=0;i<=63;i++)//i为一次写列的列数
	     {
				k=ram[x]	  ;
				page=k/y1,z=k%y1,lcd_y=z/(y1/8);
	            write_command(cs2,set_page+y2-page);
			    write_date(cs2,dat_y[lcd_y]);
				x++;
				 }
}




