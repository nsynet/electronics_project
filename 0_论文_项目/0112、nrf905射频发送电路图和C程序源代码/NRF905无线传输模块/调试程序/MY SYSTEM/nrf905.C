/*******************************************************************************************
                      TY905-SMA V1.0  /*NRD905无线收发模块应用程序
/*******************************************************************************************/
#include<reg52.h>
#include<intrins.h>

#define WC		0x00			// Write configuration register command
#define RC		0x10 			// Read  configuration register command
#define WTP		0x20 			// Write TX Payload  command
#define RTP		0x21			// Read  TX Payload  command
#define WTA		0x22			// Write TX Address  command
#define RTA		0x23			// Read  TX Address  command
#define RRP		0x24			// Read  RX Payload  command
/*******************************************************************************************
                                       基本配置
/*******************************************************************************************/
typedef struct RFConfig
{	unsigned char  n;
	unsigned char  buf[10];
}RFConfig;
code RFConfig RxTxConf =
{
	10,
 	0x01, 0x0c, 0x44, 0x20, 0x20, 0xcc, 0xcc, 0xcc,0xcc, 0x58	
};
/*
10--------------------------------10 地址
0x01------------------------------配置频段CH_NO=1  fRF = ( 422.4 + CH_NOd /10)*(1+HFREQ_PLLd) MHz=422.4
0x0c------------------------------HFREQ_PLLd=0 ,433MHZ,Reduced power,6db 
0x44------------------------------地址宽度设置，为4字节 rx tx address width is 4 Bytes
0x20------------------------------接收发送有效数据长度rx payload width
0x20------------------------------接收发送有效数据长度tx payload width
0xcc, 0xcc, 0xcc,0xcc-------------//接收地址
0x58 -----------------------------//CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
*/
/*******************************************************************************************/
unsigned char  data TxBuf[32];	// 发送暂存数组
unsigned char  data RxBuf[32];	// 接收暂存数组
/*******************************************************************************************/
unsigned char key;   //键值
unsigned char temp1; //键值函数
unsigned char rsbrf; //串口地址数据 
unsigned char delayf;//接收标示位
unsigned char tf;	 //发送标示位
/*******************************************************************************************/
unsigned char  bdata DATA_BUF;
sbit	flag	=DATA_BUF^7;
sbit	flag1	=DATA_BUF^0;
/*******************************************************************************************
                      TY905-SMA V1.0 版本管脚定义  /*NRD905无线收发模块应用程序
/*******************************************************************************************/
sbit	TX_EN	=P2^6; //3收发状态选择：1发射，0接收
sbit	TRX_CE	=P2^5; //4使能芯片接收和发送
sbit	PWR_UP	=P2^4; //5工作状态控制：1正常工作，0微功耗
sbit	MISO	=P2^3; //10SPI 输出,MCU 由此口从RF 芯片读入资料
sbit	MOSI	=P2^2; //11SPI 输入,MCU 由此口向RF 芯片写入资料
sbit	SCK	=P2^1;	   //12SPI 时钟 */
sbit	CSN	=P2^0;	   //13SPI 使能，低激活

sbit	AM	=P3^2;	   //8地址匹配
sbit	DR	=P3^3;	   //9接收或发送就绪
sbit	CD	=P3^5;	   //7载波监听
/*******************************************************************************************
                      TY905-SMA V2.0 版本管脚定义  /*NRD905无线收发模块应用程序
/*******************************************************************************************/
/*sbit	TX_EN	=P1^0;
sbit	TRX_CE	=P1^1;
sbit	PWR_UP	=P1^2;
sbit	MISO	=P1^7;
sbit	MOSI	=P3^2;
sbit	SCK	=P3^3;
sbit	CSN	=P3^4;

sbit	AM	=P1^5;
sbit	DR	=P1^6;
sbit	CD	=P1^4;//new
/*******************************************************************************************
                                    函数声明
/*******************************************************************************************/
void InitIO(void);			// Initialize IO port
void Inituart(void);			// initialize 232 uart
void Config905(void);			// Config nRF905 module配置NRF905模式
void SetTxMode(void);			// Set nRF905 in Tx mode
void SetRxMode(void);			// Set nRF905 in Rx mode
void TxPacket(void);			// Send data by nRF905
void RxPacket(void);			// Recive data by nRF905
void SpiWrite(unsigned char );			// Write data to nRF905
unsigned char  SpiRead(void);			// Read data to nRF905
void Delay(unsigned char  n);			// Delay 100us
void Scankey(void);				// Scan key
void TxData (unsigned char  x);			// Send key_value to CRT display
void comdebug(void);
/*******************************************************************************************
                               按键宏定义
/*******************************************************************************************/
/*******输入*******/

#define KEY1 0xfd
#define KEY2 0xf7
#define KEY3 0xdf
#define KEY4 0x7f
/*******输出*******/
#define LED1 0xFE
#define LED2 0xFB
#define LED3 0xEF
#define LED4 0xBF
/***********************串口发送数据********************************************************************/
void TxData (unsigned char x)
{
	SBUF=x;	
	while(TI==0);
		TI=0;
}
/*******************************************************************************************/
//function Delay100us();Delay 100us
/*******************************************************************************************/
void Delay(unsigned char  n)
{
	unsigned int i;
	while(n--)
	for(i=0;i<80;i++);	
}
/*******************************************************************************************/
void serial () interrupt 4
{  
    unsigned char temp;
     if(RI)
       { 
		    
		temp=SBUF;
                rsbrf=temp;
	        RI=0;
	}				
}
/*******************************************************************************************/
void main()
{
    unsigned int cyc=0;
    unsigned int distan=0;

    SCON=0x50;  //模式1
    TMOD=0x21;  //定时模式2
    IE=0x90;

    //=====9600=======设置串口通讯寄存器
    PCON=0x00;   //波特率9600BPS，晶振11.0592MHZ
    TH1=0xFD;  //
    TL1=0xFD;  //计数器初值
    //================NRF905初始化
	CSN=1;						// Spi 	disable						
	SCK=0;						// Spi clock line init high
	DR=1;						// Init DR for input
	AM=1;						// Init AM for input
	PWR_UP=1;					// nRF905 power on
	TRX_CE=0;					// Set nRF905 in standby mode
	TX_EN=0;					// set radio in Rx mode	

    TI = 0;//串行中断标志位清零
    TR1 = 1;//打开计数器1  
	//================
	Config905();				// Config nRF905 module
	SetTxMode();				// Set Tx Mode
	TxPacket();				    // Transmit Tx buffer data
	P0=0x00;                    // led open
	Delay(500);				    // delay for led light	
	P0=0xff;				    // led close
	SetRxMode();				// Set nRF905 in Rx mode

while(1)
{
	 //=====================从键盘获得数据
	  temp1=P0;
	  switch(P0)
	    {
	      case KEY1:P0=LED1;TxBuf[0]=LED1;tf=1;break;
	  	  case KEY2:P0=LED2;TxBuf[0]=LED2;tf=1;break;
		  case KEY3:P0=LED3;TxBuf[0]=LED3;tf=1;break;
		  case KEY4:P0=LED4;TxBuf[0]=LED4;tf=1;break;
	 	  //default:P0=0xff;TxBuf[0]=0xff; 
		 } 
	   if (tf==1)
	     {	
			//==============向串口发送相应的按键
			  switch(TxBuf[0])
			    {
			      case LED1:TxData('A');break;
			      case LED2:TxData('B');break;
			      case LED3:TxData('C');break;
			      case LED4:TxData('D');break;
			 	  //default:P0=0xff;TxBuf[0]=0xff; 
				 } 
		    //===============NRF905发送相应的按键	  
		    SetTxMode();			// Set Tx Mode
		    TxPacket();				// Transmit Tx buffer data
			TxBuf[0] = 0xff;
			SetRxMode();				// Set nRF905 in Rx mode
			tf=0;
			Delay(500);
			P0=0xff;
		 }

         //=====================等待NRF905接收
	   if (DR)			// If recive data ready... 
	   RxPacket();			// ... recive data
	 //=====================从串口接收到数据
	 if (rsbrf!=0)
	  {
		 switch(rsbrf)
		   {
		     case 'A':P0=LED1;TxBuf[0]=LED1;delayf=1;break;
			 case 'B':P0=LED2;TxBuf[0]=LED2;delayf=1;break;
			 case 'C':P0=LED3;TxBuf[0]=LED3;delayf=1;break;
			 case 'D':P0=LED4;TxBuf[0]=LED4;delayf=1;break;
		 	 //default:P0=0xff;TxBuf[0]=0xff; 
			}
	  //====================NRF905发送串口接收到的相应的字符 
	   if(TxBuf[0]!=0xff)
	     if(TxBuf[0]!=0x00)
		  {
		    SetTxMode();				// Set Tx Mode
		    TxPacket();					// Transmit Tx buffer data
			TxBuf[0] = 0xff;
			SetRxMode();				// Set nRF905 in Rx mode
		  }				            	
	 //=====================
		 if(delayf==1)
		   {
		     Delay(500);
			 delayf = 0 ;
			 P0 = 0xff ;
		   }
	   }
	 //=====================
	 if(rsbrf!=0)
	   {
	     TxData(rsbrf);
		 rsbrf = 0 ;
	   }
	 //=====================


}
}
/*******************************************************************************************/
/*******************************************************************************************/
//function Config905();
/*******************************************************************************************/
void Config905(void)
{
	unsigned char  i;					
	CSN=0;						// Spi enable for write a spi command
	SpiWrite(WC);				// Write config command写放配置命令
	for (i=0;i<RxTxConf.n;i++)	// Write configration words  写放配置字
	{
		SpiWrite(RxTxConf.buf[i]);
	}
	CSN=1;						// Disable Spi
}
/*******************************************************************************************/
//function SpiWrite();
/*******************************************************************************************/
void SpiWrite(unsigned char   byte)
{
	unsigned char  i;	
	DATA_BUF=byte;				// Put function's parameter into a bdata variable		
	for (i=0;i<8;i++)			// Setup byte circulation bits
	{	
		
		if (flag)				// Put DATA_BUF.7 on data line
			MOSI=1;
		else
			MOSI=0;
		SCK=1;					// Set clock line high
		DATA_BUF=DATA_BUF<<1;	// Shift DATA_BUF
		SCK=0;					// Set clock line low 
	}	
}
/*******************************************************************************************/
//function SpiRead();
/*******************************************************************************************/
unsigned char  SpiRead(void)
{
	unsigned char  i;	
	for (i=0;i<8;i++)			// Setup byte circulation bits
	{	
		DATA_BUF=DATA_BUF<<1;	// Right shift DATA_BUF
		SCK=1;					// Set clock line high
		if (MISO)
			flag1=1;			// Read data
		else
			flag1=0;
							
		
		SCK=0;					// Set clock line low
	}
	return DATA_BUF;			// Return function parameter
}
/*******************************************************************************************/
//function TxPacket();
/*******************************************************************************************/
void TxPacket(void)
{
	unsigned char  i;
	//Config905();				
	CSN=0;						// Spi enable for write a spi command	
	SpiWrite(WTP);				// Write payload command
	for (i=0;i<32;i++)
	{
		SpiWrite(TxBuf[i]);		// Write 32 bytes Tx data
	}
	CSN=1;						// Spi disable						
	Delay(1);
	CSN=0;						// Spi enable for write a spi command	
	SpiWrite(WTA);				// Write address command
	for (i=0;i<4;i++)			// Write 4 bytes address
	{
		SpiWrite(RxTxConf.buf[i+5]);
	}	
	CSN=1;						// Spi disable
	TRX_CE=1;					// Set TRX_CE high,start Tx data transmission
	Delay(1);					// while (DR!=1);
	TRX_CE=0;					// Set TRX_CE low
}
/*******************************************************************************************/
//function RxPacket();
/*******************************************************************************************/
void RxPacket(void)
{
	unsigned char  i;	
	TRX_CE=0;					// Set nRF905 in standby mode	
	CSN=0;						// Spi enable for write a spi command
	SpiWrite(RRP);				// Read payload command	
	for (i=0;i<32;i++)
	{
		RxBuf[i]=SpiRead();		// Read data and save to buffer		
	}
	CSN=1;						// Disable spi
	while(DR||AM);	
	P0=	RxBuf[0];				// Buffer[0] output from P0 port...
	TRX_CE=1;
	//xx=(RxBuf[0]>>4)&0x0f;
	//TxData(xx);
	 //=====================
	 //temp1=P0;
	switch(P0)
	    {
	      case LED1:TxData('A');;break;
	  	  case LED2:TxData('B');;break;
		  case LED3:TxData('C');;break;
		  case LED4:TxData('D');;break;
	 	  //default:P0=0xff;TxBuf[0]=0xff; 
		 } 
	 //=====================
	  
	Delay(500);					// ...light led
	P0=0xff;					// Close led								
}
/*******************************************************************************************/
//function SetTxMode();
/*******************************************************************************************/
void SetTxMode(void)				
{	
	TX_EN=1;
	TRX_CE=0;
	Delay(1); 					// delay for mode change(>=650us)
}				
/*******************************************************************************************/
//function SetRxMode();
/*******************************************************************************************/
void SetRxMode(void)
{
	TX_EN=0;                    //
	TRX_CE=1;                   //
	Delay(1); 					// delay for mode change(>=650us)				
}