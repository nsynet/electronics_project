#include<reg52.h>

#define WC		0x00
#define RC		0x10
#define WTP		0x20
#define RTP		0x21
#define WTA		0x22
#define RTA		0x23
#define RRP		0x24
//#define BYTE_BIT0	0x01
#define uchar unsigned char
#define uint  unsigned int
unsigned char idata RFConf[11]=
{
  0x00,                             //配置命令//
  0x4c,                             //CH_NO,配置频段在423MHZ
  0x0c,                             //输出功率为10db,不重发，节电为正常模式
  0x44,                             //地址宽度设置，为4字节
  0x03,0x03,                        //接收发送有效数据长度为3字节
  0xCC,0xCC,0xCC,0xCC,              //接收地址
  0x58,                             //CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
};

unsigned char led[]={0X01,0X37,0X42,0X22,0X34,0X28,0X08,0X33,0X00,0X20};//预发送数据码
 unsigned char send_bruff[3]={0X01,0X01,0X01};
unsigned char TxAddress[4]={0xcc,0xcc,0xcc,0xcc};       //地址码
//unsigned char rx_bruff;    
bdata unsigned  char DATA_BUF;             
sbit flag=DATA_BUF^7;

sbit AM=P3^7;	 //	 8
sbit CD=P3^6;	 //	 7
sbit CSN=P1^0;	 //13
sbit SCK=P1^1;	 //	12
sbit MISO=P1^2;	 //	10
sbit MOSI=P1^3;	 //	11
sbit DR=P1^4;	 //	9
sbit PWR=P1^5;	 //	5
sbit TRX_CE=P1^6;//	4
sbit TXEN=P1^7;	 //	3

sbit add_n=P2^7;        
sbit send=P2^6;
sbit led0 =P3^0;
sbit led1 =P3^1;
sbit led2 =P3^2;

//---------------延时--------------//
void Delay(unsigned char n)
{
	unsigned int i;
	while(n--)
	for(i=0;i<80;i++);

}
//--------------------------------11.0592M晶振  n=100 868us 左右-------------------------------
//------------------------------------------  毫秒延时         -----------------------------------------
void Delay_ms(uchar n)
{ 
	unsigned int i,j,k;

	for(i=0;i<n;i++)
	{
		for(k=0;k<5;k++)
		{
			for(j=0;j<200;j++);	
		}
	}
}
//------------SPI写操作代码---------//

void SpiWrite(unsigned char send)
{
	unsigned char i;
	DATA_BUF=send;
	for (i=0;i<8;i++)
	{
		if (flag)	//总是发送最高位
		{
			MOSI=1;
		}
		else
		{
			MOSI=0;
		}
		SCK=1;
		DATA_BUF=DATA_BUF<<1;
		SCK=0;
	}
}

//-------------初始化设置---------------//
void nRF905Init(void)
{
    CSN=1;						// Spi 	disable
	SCK=0;						// Spi clock line init low
	DR=0;						//  DR=1  接收准备。DR＝0 发送准备  
	AM=0;						// Init AM for input
	CD=0;						// Init CD for input
	PWR=1;					    // nRF905 power on
	TRX_CE=0;					// Set nRF905 in standby mode
	TXEN=0;					    // set radio in Rx mode
}


//----------设置配置寄存器-------//
void Config905(void)
{
	unsigned char i;
	CSN=0;						
	//SpiWrite(WC);				
	for (i=0;i<11;i++)	
	{
	   SpiWrite(RFConf[i]);
	}
	CSN=1;					
}


//------------发送数据-------------//
void TxPacket()
{
   unsigned char i,j;
   CSN=0;
   
   SpiWrite(WTP);
		for(j=0;j<3;j++)
		  {
             SpiWrite(send_bruff[j]); //循环3次发送3个字节
      	   }
  CSN=1;
  Delay(1);						
  CSN=0;	
  SpiWrite(WTA);				        // Write address command
  for (i=0;i<4;i++)			// Write 4 bytes address
	   {
		 SpiWrite(TxAddress[i]);
	    }
  CSN=1;
  Delay(1);
  TRX_CE=1;
  Delay(1);
  TRX_CE=0;
}
//-----------------set send  mode-----------------//
void SetTxMode(void)
{
	
    TRX_CE=0;
	TXEN=1;
	Delay(1); 					// delay for mode change(>=650us)
}

/*
//------------SPI读操作代码---------//
unsigned char SpiRead(void)
{
	unsigned char j;
	for (j=0;j<8;j++)
	{
        DATA_BUF=DATA_BUF<<1;
		SCK=1;
		if (MISO)	//读取最高位，保存至最末尾，通过左移位完成整个字节
		{
			DATA_BUF|=BYTE_BIT0;
		}
		else
		{
			DATA_BUF&=~BYTE_BIT0;
		}
		SCK=0;
	 }
	 return DATA_BUF;
}
//-----------   resive  mode  ------------------//
void SetRxMode(void)
{
	TXEN=0;
	TRX_CE=1;
	Delay(1); 					// delay for mode change(>=650us)
}
//--------------读取RF905接收数据------------//
void RxPacket(void)						
{
	unsigned char i;
    Delay(1);
//	TRX_CE=0;					// Set nRF905 in standby mode
    Delay(100);
    TRX_CE=0;
	CSN=0;						// Spi enable for write a spi command
    Delay(1);
	SpiWrite(RRP);
	for (i = 0 ;i < 1 ;i++)
	{ 
		rx_bruff=SpiRead();		// Read data and save to buffer       
	}
	CSN=1;
     
 Delay(10);
	TRX_CE=1;							
}
//-------检查是否有新数据传入-----------//
unsigned char CheckDR(void)		//检查是否有新数据传入 Data Ready
{
	if (DR)                    //当数据完全接收DR为1
	{
         ;
		return 1;
	}
	else
	{
		return 0;
	}
}
//----------数据接收------------------//
void  RX(void)
{
          SetRxMode();			      // Set nRF905 in Rx mode
          while (CheckDR()==0);       //RX wait
		  Delay(10);
	  	  RxPacket();
		  Delay(10);
}
*/ 

//-----------------按键发送函数---------------------------------
void sender_bruff() 
{
                            
                  SetTxMode();
				  Delay_ms(10);
                  TxPacket();    //发送数据
              	  Delay_ms(100);    
}
/*
//----------------接收数据语音芯片----------------//
void led_ISD()
     {
	    led0=1;
	    led1=1;
	    led2=1;	
        // rx_Buffer=0x00;
        P0=0xff;
		P3=0xff;             
        Delay(10);
		P3=0x00; 

       }
  */
//---------------按键加一函数--------------------------//
void add_number(void) 
{
  int i,j;
  
    if(!add_n)
     {
	   Delay_ms(100);
   	    while(add_n)	     
            send_bruff[2]=led[i++];
			send_bruff[1]=led[j++];
	        Delay(100);
			if(++i<3) i=0;
	    
      }
	 
  
}
//************************main function/***********************// 
void main()
 {
   
   nRF905Init();
   Config905();
   while(1)
         {	     
		 
			  led0=1;
			  led1=1;
			  led2=1;
			//  DR=1;   RX();   led1=0;

			  //if(rx_bruff==0x12)  led_ISD(); 
			    if(!add_n)  
				         add_number();
			  while(!send);
			       Delay_ms(10);
			        if(!send)
			          {
					   	led0=0;
			           // DR=0;
					
				 sender_bruff();
				  led0=0;
			  led1=0;
			  led2=0;
				 	  Delay_ms(100);
			           }		
				  
			                    
         }

			
}
