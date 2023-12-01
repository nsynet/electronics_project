
/***********************************************************************
 名字：  main.c
 作者：  春风  
 创建日：2008-8-15 
 工程：  数控稳压电源  
***********************************************************************/

#include"reg52.h"
//#include"math.h"
#include"stdlib.h"
#include"function.h"


sbit RS = P1^4;
sbit RW = P1^5;
sbit E  = P1^6;

#define DBUS P0
sbit clk_574_1 = P2^3;
//sbit clk_574_2 = P1^3;
//sbit lcd_rs    = P1^4; 

uint idac;
	

idata uchar GOUT[7];
idata float temp1;
idata float dac_temp; 
idata float sheding_temp; 
idata float sheding_temp1;
char  code dx516[3] _at_ 0x003b;//这是为了仿真设置的(大虾仿真器) 

/************************************************************************************
                              把浮点数拆开存储指针型数据  
							  输入浮点数，方式0：设定值，1：检测值  
************************************************************************************/
void chai(float x,bit y)  
{
	uint i;
	for(i = 0;i < 7;i ++)
		GOUT[i] = 0;
	if(y == 0)
	{
		i = x / 10;
		if(i == 0)
			GOUT[0] = ' ';
		else
			GOUT[0]	= i + '0';
		GOUT[1] = (int)x % 10 + '0';
		x = x * 10;
		GOUT[3] = (int)x % 10 + '0';

		x = x * 10;
		if(((int)x % 10) >= 5 )// 防止后面大于4的数被舍去  
		{GOUT[3] = GOUT[3] + 1;}
		else
		{;}

		GOUT[4] = ' ';
		GOUT[5] = ' ';
	}
	else
	{
		i = x / 10;
		if(i == 0)
			GOUT[0] = ' ';
		else
			GOUT[0] = i + '0';
		GOUT[1] = (int)x % 10 + '0';
		x = x * 10;
		GOUT[3] = (int)x % 10 + '0';
		x = x * 10;
		GOUT[4] = (int)x % 10 + '0';
		x = (x * 10);
		GOUT[5] = (unsigned int)x % 10 + '0';
	}
	GOUT[2] =  '.';
	GOUT[6] =  '\0';
}

/************************************************************************************

//向LCM发送一个字符串,长度64字符之内。  

************************************************************************************/
void lcm_w_word(uchar x,uchar y,uchar *str)
{
   x = x+y;
   w_code(x);
  while(*str != '\0')
  {
       w_data(*str++);
   }
  *str = 0;
}
/************************************************************************************
                              显示设定电压  
************************************************************************************/
void  disp_sheding()
{
    lcm_w_word(0x90,0x00,"设定：");
    sheding_temp1 = sheding_temp + 0.0005;//------加0.0005修正有时显示.0999 
	chai(sheding_temp1,1);  
    lcm_w_word(0x90,3,GOUT);
	w_code(0x96);
    w_data('V');	
}
/************************************************************************************
                 AD的16位整形数转换成浮点电压或电流值  
************************************************************************************/
float adjisuan_U(uint x)
{
	temp1 = 11.990 / 0x5e1f;
	temp1 = x * temp1;
	return temp1;
}

float adjisuan_I(uint x)  //
{
	temp1 = 0.899 / 0x253d;
	temp1 = (x * temp1)-0.005;
	return temp1;
}
/************************************************************************************
*
*函数功能:输入浮点数  
*
************************************************************************************/
float scanf_float(void)
{
	static uchar i = 0;
	static uchar x[5] = {'0','0','0','0','0'};
	static bit k = 1;
	uchar temp1,temp2;
	temp1 = key_sm();
	temp2 = key_sm();
	if((temp2 == 0xff) || (temp1 != temp2))
	{
		k = 1;
		return(atof(x));
	}
	else if(k)
	{
		k = 0;
		x[i] = temp1;
		i ++;
		if((temp1 == key_esc) || (i == 4))
		{
			x[0] = '0';
			x[1] = '0';
			x[2] = '0';
			x[3] = '0';
			x[4] = '0';
			i = 0;
		}
		return(atof(x));
	}
}

/************************************************************************************

                              扫描按键，并根据键值修改DAC的电压  

************************************************************************************/
void key_dac()
{
		uchar key_qiao;
	   key_qiao = key_sm();
	switch(key_sm())
		{
	case key_shang:dac_temp = dac_temp + 1.0;break;//---按下上键电压加1V 
	case key_xia  :dac_temp = dac_temp - 1.0;break;//---按下下键电压减1 
	case key_zuo  :dac_temp = dac_temp - 0.1;break;//---按下左键电压减0.1
    case key_you  :dac_temp = dac_temp + 0.1;break;//---按下右键电压加0.1 
    case key_0    :dac_temp = 0;             break;//---按下0键电压归0 
	default: key_qiao = 0xff;
		 }
		if(key_qiao != 0xff)  //  10月1日 这里原来是0x00就读AD不正常  	  
	   		{
       		RS  =  1;			
	   		RW  =  0;	//--------避免液晶屏干扰数据总线 
	   		if(dac_temp < 0){dac_temp = 0;}	//----判断电压低于0大于30时把数据归0 
			if(dac_temp >= 30.10){dac_temp = 0;}	
			sheding_temp = dac_temp ;
			disp_sheding();
			idac = conver(dac_temp);  
	  		dac(idac);
			delay(50000);
			}
}
/************************************************************************************
                                     主函数    
************************************************************************************/
main()
{
 
			lcd_rst(0);
			rst_ad();	
			dac_temp = 0 ;
	       	RS  =  1;			
	       	RW  =  0;	//--------避免液晶屏干扰数据总线  		
	  	   	dac(conver(dac_temp));	   //吸合低压继电器并输出0V 
	       	LED_ONOFF(1);//  打开背光源   
		  	set_7705_ch(0);
		    read_AD_data(0x38);
		    set_7705_ch(1);
		    read_AD_data(0x39);

		   disp_sheding();//显示设定电压   
		   while(1)
		   {
		   lcm_w_word(0x80,0x02,"稳压电源"); 
		   lcm_w_word(0x90,0x00,"设定：");		  
		   lcm_w_word(0x88,0x00,"电压：");   
		   lcm_w_word(0x98,0x00,"电流："); 
		   	
		   write_ad(0x10);
	       write_ad(0x04);
		   adjisuan_U(read_AD_data(0x38));
		   chai(temp1,1);  
           lcm_w_word(0x88,3,GOUT);
		   key_dac();

		   write_ad(0x11);
	       write_ad(0x14); 
		   adjisuan_I(read_AD_data(0x39));
		   chai(temp1,1);  
           lcm_w_word(0x98,3,GOUT);
		   key_dac();
		   
		   }
		   															   
}
