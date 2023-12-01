#include "reg52.h"
#include"function.h"

sbit clk_574_2 = P1^3;
sbit lcd_rs    = P1^4;
sbit RS = P1^4;
sbit RW = P1^5;

sbit key1 = P1^1;
sbit key2 = P1^2;

uchar key_data;

#define DBUS P0
/************************************************************************************
*按键扫描 
*返回键值    
************************************************************************************/
uchar key_sm()
{
	RS  =  1;			
	RW  =  0;	//--------避免液晶屏干扰数据总线 
	key1 = 1;
	key2 = 1;	//--------按键置高电平  
	key_data = 0xff;
	   DBUS = 0xff;
	   clk_574_2 = 0;	
	   clk_574_2 = 1;
	if(key1 == 0){key_data = key_enter;}
	   DBUS = 0xfe;
	   clk_574_2 = 0;	
       clk_574_2 = 1;
	if(key1 == 0){key_data = key_9;}
	if(key2 == 0){key_data = key_dian;}
	   DBUS = 0xfd;
	   clk_574_2 = 0;	
	   clk_574_2 = 1;
	if(key1 == 0){key_data = key_6;}
	if(key2 == 0){key_data = key_3;}
	   DBUS = 0xfb;
	   clk_574_2 = 0;	
	   clk_574_2 = 1;
	if(key1 == 0){key_data = key_5;}
	if(key2 == 0){key_data = key_2;}
	   DBUS = 0xf7;
	   clk_574_2 = 0;	
	   clk_574_2 = 1;
	if(key1 == 0){key_data = key_8;}
	if(key2 == 0){key_data = key_0;}
	   DBUS = 0xef;
	   clk_574_2 = 0;	
	   clk_574_2 = 1;
	if(key1 == 0){key_data = key_4;}
	if(key2 == 0){key_data = key_1;}
		DBUS = 0xdf;
		clk_574_2 = 0;	
		clk_574_2 = 1;	
	if(key1 == 0){key_data = key_7;}
	if(key2 == 0){key_data = key_esc;}
		DBUS = 0xbf;
		clk_574_2 = 0;	
		clk_574_2 = 1;	
	if(key1 == 0){key_data = key_you;}
	if(key2 == 0){key_data = key_xia;}
		DBUS = 0x7f;
		clk_574_2 = 0;	
		clk_574_2 = 1;		
	if(key1 == 0){key_data = key_zuo;}
	if(key2 == 0){key_data = key_shang;}

	return 	key_data;    	
}
