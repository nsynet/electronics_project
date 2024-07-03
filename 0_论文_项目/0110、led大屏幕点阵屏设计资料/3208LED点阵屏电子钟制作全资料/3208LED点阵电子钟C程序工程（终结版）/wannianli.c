/*-------------------------------------------
项目名：　　　2006年毕业设计项目
程序名： 　　 32*8点阵屏电子钟
编写人： 　　 杜洋　
初写时间：　　2005-12-15~2005-12-20　　
CPU说明：　　 MCS-51（AT89S52）　12MHZ
接口说明：　　DS1302、DS18B20
修改日志：　　
　　NO.1-20051215　完成LED驱动、显示切换、时钟温度读取
	NO.2-20051216　完成整点报时、填加注释信息
　	NO.3-20051220　完成了调时、按键音等所以的功能（没有闹钟）
    NO.3-20051221  增加了十位为0时的消隐功能
--------------------------------------------*/

//------------------------------定义头文件
#include <reg51.h>//MCS-51单片机
//------------------------------定义缩写字符
#define  uchar unsigned char
#define  uint  unsigned int
//------------------------------定义扬声器接口，低电平使能
sbit Bell_Out  = P1 ^ 5;//扬声器驱动
//------------------------------定义DS18B20音
sbit DQ        = P1 ^ 6;//ds1820data（单总线）
//------------------------------定义DS1302时钟接口
sbit clock_clk = P3 ^ 5;//ds1302_clk（时钟线）
sbit clock_dat = P3 ^ 6;//ds1302_dat（数据线）
sbit clock_Rst = P3 ^ 7;//ds1302_Rst（复位线）
//-----------------------------定义数据口
#define Led_13     P0  //第1、3屏数据口
#define Led_24     P2  //第2、4屏数据口
#define Led_E      P1  //74HC154的BCD列驱动（P1.0~P1.4）
sbit Led_EA    =   P1 ^ 4;//显示列驱动器（74HC154）使能
//-----------------------------独立键盘定义
sbit Add_Key   = P3 ^ 1;//前位加1键
sbit Add2_Key  = P3 ^ 0;//后位加1键
sbit OK_Key    = P3 ^ 2;//确定键
sbit int_Key   = P3 ^ 3;//中断入口
//-----------------------------定义累加器A中的各位
sbit a0   = ACC ^ 0;
sbit a1   = ACC ^ 1;
sbit a2   = ACC ^ 2;
sbit a3   = ACC ^ 3;
sbit a4   = ACC ^ 4;
sbit a5   = ACC ^ 5;
sbit a6   = ACC ^ 6;
sbit a7   = ACC ^ 7;
//------------------------------------定义全局变量
bit txx = 0;
uchar settime;//定义标志位
uchar yy,mo,dd,xq,hh,mm,bn;//定义时间映射全局变量（专用寄存器）
static uchar timecount = 0;//定义静态软件计数器变量
static uchar pp = 0;//定义静态小时更新用数据变量
//------------------------------------函数声明(按字母顺序排列） 
void Beep(void);
void Beep_key(void);
void Beep_set(void);
void clearRAM (void);
uchar clock_in(void);
void clock_out(uchar dd);
void Delay(int num);
void DelayM(uint a);
void display (void);
void display_s (void);
void Init_1302(void);
void int1 (void);
void putin (uchar u);
uchar read_clock(uchar ord);
void read_clockS(void);
void Set_time(unsigned char sel);
void setput (uchar k);
void Timer0(void);
void Timer0_Init(void);
void write_clock(uchar ord, uchar dd);
uchar DL (uchar);
uchar DSS (void);

//-----------------------------定义显示缓冲寄存器（32个字节全局变量）
uchar Ledplay [35];//
//-----------------------------定义字符数据表单（二维数组）
uchar code no[][10]={
//-----------------------------------数字字符表 
{0x7E,0xFF,0x81,0x81,0xFF,0x7E,0},//0
{0x82,0xFF,0xFF,0x80,0}, //1
{0xC6,0xE7,0xB1,0x99,0x8F,0x86,0},//2
{0x42,0xC3,0x89,0x89,0xFF,0x76,0},//3
{0x30,0x28,0xA6,0xFF,0xFF,0xA0,0},//4
{0x4F,0xCF,0x89,0x89,0xF9,0x71,0},//5
{0x7E,0xFF,0x89,0x8B,0xFB,0x70,0},//6
{0x03,0x03,0xF9,0xFF,0x07,0},//7
{0x76,0xFF,0x89,0x89,0xFF,0x76,0},//8
{0x0E,0xDF,0xD1,0x91,0xFF,0x7E,0},//9
//-----------------------------------星期字符表
{0x10,0x10,0x10,0x10,0x10,0x10,0x18,0x10,0},//一10
{0x40,0x44,0x44,0x44,0x44,0x44,0x44,0x40,0},//二
{0x82,0x92,0x92,0x92,0x92,0x92,0x82,0x80,0},//三
{0xFE,0xA2,0x9E,0x82,0x9E,0xA2,0xFE,0},//四
{0x80,0x89,0xFF,0x89,0x89,0x89,0xF9,0x80,0},//五
{0x88,0x68,0x38,0x0E,0x38,0x68,0x88,0},//六
{0xFE,0x92,0x92,0x92,0x92,0xFE,0},//日			10+6
//-----------------------------------特殊字符表
{0x6C,0x6C,0},//“:”								17
{0x18,0x18,0x18,0x18,0},//“-”						  18
{0x06,0x06,0x78,0xFC,0x84,0x84,0x84,0x80,0},//“oC”	19
{0x02,0x8E,0xFC,0xFC,0x8E,0x02,0},//“Y”（在调时时表示年）20
{0x80,0x7E,0x15,0x15,0x95,0xFF,0},//“月”21
{0xFE,0x92,0x92,0x92,0x92,0xFE,0}//“日”22
};
//-----------------------------------开机画面，显示“DYDIY”
uchar code dydiy[]={//显示“DYDIY”
0x82,0xFE,0xFE,0x82,0xFE,0x7C,0,
0x02,0x8E,0xFC,0xFC,0x8E,0x02,0,
0x82,0xFE,0xFE,0x82,0xFE,0x7C,0,
0x82,0xFE,0xFE,0x82,0,
0x02,0x8E,0xFC,0xFC,0x8E,0x02,0,0x99
}; 
//---------------------------------------延时函数 2uS/次
void Delay(int num){
	for (;num>0;num--);
}
//---------------------------------------延时函数 1MS/次
void DelayM(unsigned int a){	
		unsigned char i;
		while( --a != 0){		
			for(i = 0; i < 125; i++);  //一个 ; 表示空语句,CPU空转。
		}   				   //i 从0加到125，CPU大概就耗时1毫秒
	}
//---------------------------------------1302驱动程序（底层协议）
void clock_out(unsigned char dd){
	ACC=dd;
	clock_dat=a0; clock_clk=1; clock_clk=0;
	clock_dat=a1; clock_clk=1; clock_clk=0;
	clock_dat=a2; clock_clk=1; clock_clk=0;
	clock_dat=a3; clock_clk=1; clock_clk=0;
	clock_dat=a4; clock_clk=1; clock_clk=0;
	clock_dat=a5; clock_clk=1; clock_clk=0;
	clock_dat=a6; clock_clk=1; clock_clk=0;
	clock_dat=a7; clock_clk=1; clock_clk=0;
}
//---------------------------------------DS1302写入字节（底层协议）
unsigned char clock_in(void){
	clock_dat=1;
	a0=clock_dat;
	clock_clk=1; clock_clk=0; a1=clock_dat;
	clock_clk=1; clock_clk=0; a2=clock_dat;
	clock_clk=1; clock_clk=0; a3=clock_dat;
	clock_clk=1; clock_clk=0; a4=clock_dat;
	clock_clk=1; clock_clk=0; a5=clock_dat;
	clock_clk=1; clock_clk=0; a6=clock_dat;
	clock_clk=1; clock_clk=0; a7=clock_dat;
	return(ACC);
}
//----------------------------------------DS1302读数据（底层协议）
uchar read_clock(uchar ord){
	uchar dd=0;
	clock_clk=0;
	clock_Rst=0;
	clock_Rst=1;
	clock_out(ord);
	dd=clock_in();
	clock_Rst=0;
	clock_clk=1;
	return(dd);
}
//---------------------------------------常用时钟数据读取
void read_clockS(void){
	mm = read_clock(0x83);//读取分钟数据
	hh = read_clock(0x85);//小时
	dd = read_clock(0x87);//日
	mo = read_clock(0x89);//月
	xq = read_clock(0x8b);//星期
	yy = read_clock(0x8d);//年
}
//*---------------------------------------DS1302写数据（底层协议）
void write_clock(uchar ord, uchar dd){
	clock_clk=0;
	clock_Rst=0;
	clock_Rst=1;
	clock_out(ord);
	clock_out(dd);
	clock_Rst=0;
	clock_clk=1;
}
//--------------------------------------------------调时用加1程序
void Set_time(unsigned char sel){ //根据选择调整的相应项目加1并写入DS1302
  signed char address,item;
  signed char max,mini;
  if(sel==7)  {address=0x80; max=0;mini=0;}     //秒7
  if(sel==6)  {address=0x82; max=59;mini=0;}    //分钟6
  if(sel==5)  {address=0x84; max=23;mini=0;}    //小时5
  if(sel==3)  {address=0x86; max=31;mini=1;}    //日3
  if(sel==2)  {address=0x88; max=12;mini=1;}    //月2
  if(sel==1)  {address=0x8c; max=99; mini=0;}    //年1
  if(sel==4)  {address=0x8a; max=7;mini=1;}    //星期4  
  //读取1302某地址上的数值转换成10进制赋给item 
  item=((read_clock(address+1))/16)*10 + (read_clock(address+1))%16;
  	item++;//数值加1  
  if(item>max) item=mini;//查看数值有效范围   
  if(item<mini) item=max;           
  write_clock(0x8e,0x00);//允许写操作
  write_clock(address,(item/10)*16+item%10);//转换成16进制写入1302
  write_clock(0x8e,0x80);//写保护，禁止写操作  
}
//--------------------------------------------设置1302的初始时间
void Init_1302(void){//（2005年12月20日12时00分00秒星期二）
	write_clock(0x8e,0x00);//允许写操作
	write_clock(0x8c,0x05);//年
	write_clock(0x8a,0x02);//星期
	write_clock(0x88,0x12);//月
	write_clock(0x86,0x20);//日
	write_clock(0x84,0x12);//小时
	write_clock(0x82,0x00);//分钟
	write_clock(0x80,0x00);//秒
	write_clock(0x90,0xa5);//充电	
	write_clock(0x8e,0x80);//禁止写操作
}
//--------------------------------------------扬声器--整点报时
void Beep(void){//BELL 
	uchar a;//定义变量用于发声的长度设置
	Led_EA = 1;//关列显示驱动，防止屏幕出现乱码
	for(a=60;a>0;a--){//第一个声音的长度
		Bell_Out = ~Bell_Out;//取反扬声器驱动口，以产生音频
		Delay(100);//音调设置延时
	}
	for(a=100;a>0;a--){//同上
		Bell_Out = ~Bell_Out;
		Delay(80);//
	}
	for(a=100;a>0;a--){//同上
		Bell_Out = ~Bell_Out;
		Delay(30);//
	}
}
//--------------------------------------------扬声器--设置报警
void Beep_set(void){//BELL 
	uchar a;//定义变量用于发声的长度设置
	Led_EA = 1;//关列显示驱动，防止屏幕出现乱码
	for(a=250;a>0;a--){//第一个声音的长度
		Bell_Out = ~Bell_Out;//取反扬声器驱动口，以产生音频
		Delay(20);//音调设置延时
	}
	for(a=100;a>0;a--){//同上
		Bell_Out = ~Bell_Out;
		Delay(80);//
	}
}	
//----------------------------------------扬声器--按键音
void Beep_key(void){
	uchar a;//定义变量用于发声的长度设置
	Led_EA = 1;//关列显示驱动，防止屏幕出现乱码
	for(a=100;a>0;a--){//声音的长度
		Bell_Out = ~Bell_Out;
		Delay(50);//音调设置延时
	}
}
//----------------------------------------32个寄存器清空（0x00）
void clearRAM (void){//寄存器清空函数
	uchar a;//定义变量用于清空数据指针
	for(a=0;a<32;a++){//指向32个寄存器
		Ledplay[a] = 0;//将指向的寄存器清空
	}
}
//----------------------------------------将字符装入显示寄存器
void putin (uchar u){//字符载入函数
	uchar a = 0;//定义变量用于数据提取指针
	do{		
		Ledplay[bn] = no[u][a];//将二维数组中的一组数据放入显示缓冲区
		a++;//换下一个提取字节	
		bn++;//换下一个显示缓冲字节			
	}
	while(no[u][a] != 0);//当显示数据为0时结束循环
	bn++;//换下一个显示缓冲字节
	Ledplay[bn] = 0;//显示一列的空位，便于字符区分	
}
//------------------------------------调时切换
void setput (uchar k){
	switch(k){//K为0调整日期，K为1调整时间，K为2调整年和星期
		case 0:{
			read_clockS();//调用时钟芯片驱动函数，读出时钟数据并放入相应的寄存器
			clearRAM();//清空显示缓冲区数据
			bn = 0;//显示位累加寄存器清零（从头开始写入显示缓冲区）
			if(mo/16 != 0){
				putin(mo/16);//显示月值（十位，为0时消隐）
			}
			putin(mo%16);//显示月值（个位）
			putin(21);//显示横杠“月”
			if(dd/16 != 0){
				putin(dd/16);//显示日值（十位，为0时消隐）
			}
			putin(dd%16);//显示日值（个位）
			putin(22);//显示“日”
		}break;	
		case 1:{
			read_clockS();//调用时钟芯片驱动函数，读出时钟数据并放入相应的寄存器
			clearRAM();//清空显示缓冲区数据
			bn = 0;//显示位累加寄存器清零（从头开始写入显示缓冲区）
			//if(hh/16 != 0){
				putin(hh/16);//显示小时值（十位,为0时消隐）
			//}
			putin(hh%16);//显示小时值（个位）
			putin(17);//显示冒号“：”
			putin(mm/16);//显示分钟值（十位）
			putin(mm%16);//显示分钟值（个位）
		}break;	
		case 2:{
			read_clockS();//调用时钟芯片驱动函数，读出时钟数据并放入相应的寄存器
			clearRAM();//清空显示缓冲区数据
			bn = 0;//显示位累加寄存器清零（从头开始写入显示缓冲区）
			putin(yy/16);//显示年（十位）
			putin(yy%16);//显示年（个位）
			putin(20);//显示“Y”表示年
			putin(xq+9);//显示星期数据
		}break;
		default:{//数据不附时退出
		}
	}
	Beep_key();//按键音
}
//----------------------------------------32*8点阵显示驱动
void display (void){//点阵屏驱动函数
	uchar ac,aa=7;//定义两个数据变量
	for(ac=0x28;ac<0x30;ac++){//扫描后两块LED屏（由于P1.5接扬声器所以高4位为2，旨在关声音）		
		Led_E  = ac;//用ac值驱动使能列信号		
		Led_24 = DL(Ledplay[aa+8]);//将显示寄存器数据写入2、4屏（嵌入DL汇编是为了翻转字节数据，如果接线正确可省去）
		Led_13 = Ledplay[aa];//将显示寄存器数据写入1、3屏
		Delay(60);//延时1.2微秒
		Led_13 = 0;//数据总线清空
		Led_24 = 0;
		aa--;//aa减1，换下一列显示
	}
	aa = 23;//指定后16个数据的提取	
	for(ac=0x20;ac<0x28;ac++){//扫描前两块LED屏		
		Led_E  = ac;//用ac值驱动使能列信号		
		Led_24 = DL(Ledplay[aa+8]);//将显示寄存器数据写入2、4屏
		Led_13 = Ledplay[aa];//将显示寄存器数据写入1、3屏				
		Delay(60);//延时1.2微秒
		Led_13 = 0;//数据总线清空
		Led_24 = 0;
		aa--;//aa减1，换下一列显示
	}
}
//--------------------------------------------32*8点阵反色显示驱动（表示调时）
void display_s (void){//点阵屏驱动函数
	uchar ac,aa=7;//定义两个数据变量
	for(ac=0x28;ac<0x30;ac++){//扫描后两块LED屏（由于P1.5接扬声器所以高4位为2，旨在关声音）		
		Led_E  = ac;//用ac值驱动使能列信号		
		Led_24 = ~DL(Ledplay[aa+8]);//将显示寄存器数据反色写入2、4屏（嵌入DL汇编是为了翻转字节数据，如果接线正确可省去）
		Led_13 = ~Ledplay[aa];//将显示寄存器数据反色写入1、3屏
		Delay(60);//延时1.2微秒
		Led_13 = 0;//数据总线清空
		Led_24 = 0;
		aa--;//aa减1，换下一列显示
	}
	aa = 23;//指定后16个数据的提取	
	for(ac=0x20;ac<0x28;ac++){//扫描前两块LED屏		
		Led_E  = ac;//用ac值驱动使能列信号		
		Led_24 = ~DL(Ledplay[aa+8]);//将显示寄存器数据反色写入2、4屏
		Led_13 = ~Ledplay[aa];//将显示寄存器数据反色写入1、3屏				
		Delay(60);//延时1.2微秒
		Led_13 = 0;//数据总线清空
		Led_24 = 0;
		aa--;//aa减1，换下一列显示
	}
}
//-------------------------------------------------------定时器0初始化
void Timer0_Init(void){//定时器0初始（主函数中被调用一次）
	TMOD=0x01;//定时器方式设置（定时器工作方式1-16位定时）
	TH0=(65535-50000)/256;//50MS
	TL0=(65535-50000)%256;
	EA=1;//开总中断
	ET0=1;//开定时器0中断允许 
	TR0=1;//启动定时器0
}
//===============================================定时器中断函数（产生显示切换）
void Timer0(void) interrupt 1 using 0{//定时器中断入口,使用第1工作组寄存器	
	TH0=(65535-50000)/256; //50ms定时（定时器工作方式1）
  	TL0=(65535-50000)%256;  
  	timecount++;//软件计数加1
	if(timecount == 20){//软件计数加到20时显示日期（1S）
		Led_EA  = 1;//关列显示驱动，防止屏幕出现乱码
		read_clockS();//调用时钟芯片驱动函数，读出时钟数据并放入相应的寄存器
		clearRAM();//清空显示缓冲区数据
		bn = 0;//显示位累加寄存器清零（从头开始写入显示缓冲区）
		if(mo/16 != 0){//为0时消隐
			putin(mo/16);//显示月值（十位，为0时消隐）
		}
		putin(mo%16);//显示月值（个位）
		putin(21);//显示“月”
		if(dd/16 != 0){//为0时消隐
			putin(dd/16);//显示日值（十位，为0时消隐）
		}
		putin(dd%16);//显示日值（个位）
		putin(22);//显示“日”					
	}
	if(timecount == 40){//软件计数加到40时显示时间（2S）				
		Led_EA  = 1;//关列显示驱动，防止屏幕出现乱码		 
		clearRAM();//清空显示缓冲区数据
		bn = 0;//显示位累加寄存器清零（从头开始写入显示缓冲区）
		if(hh > 6 && hh < 22){//整点报时设为早6时到晚22时之间
			if(pp != hh){//当小时值更新时则启动报时　
				pp = hh;//将新的小时值放入更新寄存器
				Beep();//启动报时
			}			
		}
		//if(hh/16){//为0时消隐 
			putin(hh/16);//显示小时值（十位，为0时消隐）
		//}
		putin(hh%16);//显示小时值（个位）
		putin(17);//显示冒号“：”
		putin(mm/16);//显示分钟值（十位）
		putin(mm%16);//显示分钟值（个位）		
	}
  	if(timecount > 80){////软件计数加到80时显示时间（1S） 
		uchar ty = 0;//定义温度寄存器变量  	    			
		timecount = 0;//软件计数清零
		Led_EA  = 1;//关列显示驱动，防止屏幕出现乱码							
		ty = DSS();//将DSS汇编程序取得的温度值放入温度寄存器变量
			clearRAM();//清空显示缓冲区数据
			bn = 0;//显示位累加寄存器清零（从头开始写入显示缓冲区）			
		if(ty >99){//当温度高于99度或未发现温度传感器（DS18B20）时显示“--”		 
			putin(18);//显示“--”
			putin(18);
		}
		else{//温度小于99度并发现温度传感器（DS18B20）时，显示温度值 
			if(ty/10 != 0){//为0时消隐
				putin(ty/10);//温度值的十位
			}
			putin(ty%10);//温度值的个位
		}
		putin(19);//显示摄氏度符号“oC”
		putin(xq+9);//显示星期数据	    		          
    }
}
//===============================================外中断0，调时开关（P3.3）
void int1 (void) interrupt 2 {//中断入口2（INT1）
	uchar a;//定义延时长度
	for(a=100;a>0;a--){//延时处理，防止误按（长按2秒后有效）
		display_s ();//反色显示表示按键有效
	}
	if(int_Key == 0){//进入闹钟设置界面
		Beep_set();//调时提示音	
		if(settime == 0x77){//如果调时标志为0说明当正为正常显示状态	
			ET0 = 0;//关定时器中断（停止显示切换）
			TR0 = 0;
			settime = 0;//变为调时的反色显示
			setput(0);//显示清0后的内容			
		}
		else{//如果调时标志为1说明当正为调时显示状态
			ET0 = 1;//开定时器中断（开启显示切换）
			TR0 = 1;
			settime = 0x77;//变为正常显示
		}
		while(int_Key == 0){//等待调时键释放
			display ();//不释放时调用屏幕显示
		}
	}
}
//===============================================主函数，产生点阵显示
void main(void){ //主函数 
	uchar a = 0;//定义变量用于数据提取指针
	Beep();//启动发出报时音（测试扬声器）
	do{//显示开机画面“DYDIY”		
		Ledplay[bn] = dydiy[a];//将二维数组中的一组数据放入显示缓冲区
		a++;//换下一个提取字节	
		bn++;//换下一个显示缓冲字节			
	}
	while(dydiy[a] != 0x99);//当显示数据为0时结束循环
	timecount = 41;//将显示切换值调到41，使开机画面停留2秒
	Timer0_Init();//定时器初始化
	IT1 = 0;//外中断1下沉触发
	EX1 = 1;//开外中断1（调时键）
	settime = 0x77;//显示标志为0表示正常显示
//-------------------------------------主循环体
	while(1){//	
		static uchar k = 0;
		if(settime == 0x77) {//显示标志为0表示正常显示
			display ();//正常显示驱动
			//正常显示时同时按下Add、Add2、OK三个键则初始化时间
			if(Add_Key == 0 && Add2_Key ==0 && OK_Key == 0 ){
				DelayM(100);//延时去抖动
				if(Add_Key == 0 && Add2_Key ==0 && OK_Key == 0 ){			
					Beep_set();//提示音
					Init_1302();//调用时间初始化函数
				}
			}
		}
		else{//显示标志为1表示调时状态				
			display_s();//调时显示驱动
			if(OK_Key == 0) {//调时切换键按下切换调时的内容
				DelayM(20);//去抖
				if(OK_Key == 0){									
					k++;//切换累加变量					
					setput(k);//显示切换的内容
				    if(k > 2){//切换值超出后清0
						k = 0;//清0
						setput(0);//显示清0后的内容
					}
				while(OK_Key == 0 );//等待键放开
				}
			}
			if(Add_Key == 0){//前位加1键（使屏幕前位的月，年，小时加1）
				DelayM(20);//去抖
				if(Add_Key == 0){
					switch(k){//依照显示内容数据调整
						case 0:{
							Set_time(2);//月调整	
							setput(k);//更新显示数据
						}break;
						case 1:{
							Set_time(5);//小时调整
							setput(k);//更新显示数据
						}break;
						case 2:{
							Set_time(1);//年调整
							setput(k);//更新显示数据
						}break;
						default:{//数值不附的处理（无动作）
						}
					}
				while(Add_Key == 0 );//等待键释放
				}
			}
			if(Add2_Key == 0){//后位加1键（使屏幕前位的日、星期、分钟加1）
				DelayM(20);
				if(Add2_Key == 0){
					switch(k){
						case 0:{
							Set_time(3);//日调整	
							setput(k);//更新显示数据
						}break;
						case 1:{
							Set_time(6);//分调整
							setput(k);//更新显示数据
						}break;
						case 2:{
							Set_time(4);//星期调整
							setput(k);//更新显示数据
						}break;
						default:{//数值不附的处理（无动作）
						}
					}
				while(Add2_Key == 0 );//等待键释放
				}
			}	
		}
	}
}




