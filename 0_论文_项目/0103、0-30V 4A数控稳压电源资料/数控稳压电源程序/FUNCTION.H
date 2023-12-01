#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

#define key_shang 0x18
#define key_xia   0x19
#define key_zuo   0x1b
#define key_you   0x1a
#define key_enter 0x2f
#define key_1     0x31
#define key_2     0x32
#define key_3     0x33
#define key_4     0x34
#define key_5     0x35
#define key_6     0x36
#define key_7     0x37
#define key_8     0x38
#define key_9     0x39
#define key_esc   0x0c
#define key_0     0x30
#define key_dian  0x2e

extern void  busy(void);//查忙函数 
extern void  w_code(uchar c);//写指令函数 
extern void  w_data(uchar d);//写数据函数  
extern uchar r_data(void);//读数据函数 	 
extern void  address(uchar x_lcd,uchar y_lcd);//写DGRAM:x_lcd横坐标,y_lcd纵坐标	 
extern void  dac(uint c);//DAC数据转换函数 
extern uint  conver(float x);//实际输出电压转换成DA输出值 
extern void  LED_ONOFF(bit i);//控制LCD背光   
extern void  RL_ONOFF(bit i); // 控制继电器吸合  
extern void  BELL_ONOFF(bit i);//蜂鸣器控制 
extern void  lcd_rst(bit a); 
extern void  rst_ad();
extern write_ad(uchar i);
extern void  delay(uint i);
extern float adjisuan(uint x);
extern uint read_AD_data(uchar y);
extern void set_7705_ch(bit a);
extern uchar key_sm();
extern void key_dac();
extern void  write_lcd_full(uint x );
#endif