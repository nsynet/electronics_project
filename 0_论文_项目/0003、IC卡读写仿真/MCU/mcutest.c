#include "reg52.h"
#include "lcd.h"

unsigned char Rec_Buffer[4];
#define DELAY_IIC_TIME 60 
#define TRUE 1
#define FALSE 0

sbit SCL=P1^0;	//时钟线
sbit SDA=P1^1;	  //数据线

unsigned int money;

bit err_flag;


//PC发送的命令
#define CardID_Addr 	0x00	//卡号 地址	  00~02
#define Password_Addr   0x0a	//密码	  	   0a~0d
#define Money_Addr 	    0x0f	//金额		   0f~10

#define COM_RD_CARD 0x45   //上位机发的读卡命令
#define COM_WR_CARD 0x67	//上位机发的写卡命令
#define COM_ADD_MONEY 0xab	//上位机发的充钱命令
#define COM_DEC_MONEY 0x89	//上位机发的扣钱命令

#define COM_WR_ERROR  0xdd	//出错

#define FLAG_RD_OK	0x12		//上传的数据,表示读卡完毕
#define FLAG_WR_OK	0x34 		//上传的数据,表示写卡完毕
#define FLAG_RD_ER	0x56		//上传的数据,表示读卡错误
#define FLAG_WR_ER	0x78		//上传的数据,表示写卡错误
#define FLAG_NO_MONEY 0x9a	 	//卡上金额不足
#define FLAG_FULL_MONEY 0xbc	//卡上金额过多

#define adr_mon_h 0x0f 	//钱的高位地址
#define adr_mon_l 0x10

unsigned char Card_num[]={"ID:          "};
unsigned char Money_num[]={"MONEY:    .  "};

bit Check_Acknowledge(void);
void I2C_Start(void);
void I2C_Stop(void);
unsigned char Read_One_Byte(unsigned char);
unsigned char  ReadI2CByte(void);
void Send_0(void);
void Send_1(void);
void WriteI2CByte(unsigned char) ;
void Write_One_Byte(unsigned char,unsigned char);

/********** Function Definition 函数定义 ************/
void DELAY_IIC(unsigned int t)
{
	while(t!=0)
	t--;
}


void I2C_Start(void)
{
	SDA=1;
	SCL=1;
	DELAY_IIC(DELAY_IIC_TIME);
	SDA=0;
	DELAY_IIC(DELAY_IIC_TIME);
	SCL=0;
	DELAY_IIC(DELAY_IIC_TIME);
}


void I2C_Stop(void)

{
	SDA=0;
	SCL=1;
	DELAY_IIC(DELAY_IIC_TIME);
	SDA=1;
	DELAY_IIC(DELAY_IIC_TIME);
	SCL=0;
	DELAY_IIC(DELAY_IIC_TIME);
}


void Send_0(void)
{
	SDA=0;
	SCL=1;
	DELAY_IIC(DELAY_IIC_TIME);
	SCL=0;
	DELAY_IIC(DELAY_IIC_TIME);
}

void Send_1(void)
{
	SDA=1;
	SCL=1;
	DELAY_IIC(DELAY_IIC_TIME);
	SCL=0;
	DELAY_IIC(DELAY_IIC_TIME);
}


bit Check_Acknowledge(void)
{
	SDA=1;
	SCL=1;
	DELAY_IIC(DELAY_IIC_TIME/2);
	F0=SDA;
	DELAY_IIC(DELAY_IIC_TIME/2);
	SCL=0;
	DELAY_IIC(DELAY_IIC_TIME);
	if(F0==1)
		return FALSE;
	return TRUE;
}

void WriteI2CByte(unsigned char b)
{
	char i;
	for(i=0;i<8;i++)
		if((b<<i)&0x80)
			Send_1();
	else   	
		Send_0();
}

unsigned char ReadI2CByte(void)
{
	unsigned char b=0,i;
	for(i=0;i<8;i++)
	{
		SDA=1; 
		SCL=1; 
		DELAY_IIC(10);
		F0=SDA;
		DELAY_IIC(10);
		SCL=0;
		if(F0==1)
		{
			b=b<<1;
			b=b|0x01;
		}
		else
			b=b<<1;
	}
	return b;
}

/**********以下为读写24c02的函数**********/
void Write_One_Byte(unsigned char addr,unsigned char thedata)
{
	bit acktemp=1;
	I2C_Start();
	WriteI2CByte(0xa0);
	acktemp=Check_Acknowledge();
	WriteI2CByte(addr);
	acktemp=Check_Acknowledge();
	WriteI2CByte(thedata);
	acktemp=Check_Acknowledge();
	I2C_Stop();
}	

unsigned char Read_One_Byte(unsigned char addr) 
{ 
	bit acktemp=1;
	unsigned char mydata;
	I2C_Start();
	WriteI2CByte(0xa0);
	acktemp=Check_Acknowledge();
	WriteI2CByte(addr);
	acktemp=Check_Acknowledge();
	I2C_Start();
	WriteI2CByte(0xa1);
	acktemp=Check_Acknowledge();
	mydata=ReadI2CByte();
	acktemp=Check_Acknowledge();
	return mydata;
	I2C_Stop();
}

char Num2Char(unsigned char ch)
{
	if(ch<=9)
 		return (ch+'0');
 	else return (ch-10+'a');
}

void System_Init(void)
{
	TMOD=0x21;		//串行口初始化
 	TH1=0xfd;TL1=0xfd;	//设定波特率  9600  
 	PCON=0x00;
 	TR1=1;			//启动T1
 	SCON=0x50;	//串行口工作在方式1，并允许接收	
  	EA=1;	 	   	
	ES=1;	
}

void main(void)
{
	System_Init();	 	//初始化
	LCD_Init();

 	while(1)
	{
	//////////////此代码用来测试IIC读写/////////////////
#ifdef _IIC_DUBG		 
		 Write_One_Byte(0,2); //ADD0 写入数据2
		 Write_One_Byte(1,7); //ADD1 写入数据3
		 LCD_Wr_Char(0,0,Read_One_Byte(0)+'0');	 //读取24C02 并在LM016显示
		 LCD_Wr_Char(1,0,Read_One_Byte(1)+'0');		 
#endif		 
	//////////////////IIC测试结束//////////////////////////
	  	Money_num[6]=((money/10000)%10)+'0';
	  	Money_num[7]=((money/1000)%10)+'0';
	  	Money_num[8]=((money/100)%10)+'0';
	  	Money_num[9]=((money/10)%10)+'0';
	  	Money_num[11]=(money%10)+'0';

		LCD_Wr_String(0,0,Card_num);
		LCD_Wr_String(0,1,Money_num);
	}

}

//IC卡读和写子程序,中断	
void rxdata(void) interrupt 4 using 0
{
	unsigned char i,j;
	unsigned char rdbuf[20]; //读取卡中的数据缓冲
	unsigned char RxBuf[20];  //读取串口数据缓冲
	EA=0;
	for(i=0;i<2;i++) ////读取命令,第一个字节为命令字节,第二个字节为在第二个字节之后还有多少个字节	   
	{
		while(!RI);
		RI=0;
		RxBuf[i]=SBUF;  
	}

	if(RxBuf[1]!=0)	  //如果字节数不是0，则继续读取剩余的数据
	{
		for(i=0;i<RxBuf[1];i++)
		{
			while(!RI);
			RI=0;
			RxBuf[2+i]=SBUF; 
		}
	}

	if(RxBuf[0]==COM_RD_CARD) //读卡命令
	{
		err_flag=0;
		rdbuf[0]=Read_One_Byte(CardID_Addr);   //读卡中的ID号
		rdbuf[1]=Read_One_Byte(CardID_Addr+1);
		rdbuf[2]=Read_One_Byte(CardID_Addr+2);
		rdbuf[3]=Read_One_Byte(CardID_Addr+3);
		rdbuf[4]=Read_One_Byte(CardID_Addr+4);

		rdbuf[5]=Read_One_Byte(Money_Addr);
		rdbuf[6]=Read_One_Byte(Money_Addr+1);

		SBUF= COM_RD_CARD; //发送读卡命令返回PC

		while(!TI);
		TI=0;

		for(j=0;j<7;j++) //发送卡数据返回PC
		{
			SBUF=rdbuf[j];
			while(!TI);
			TI=0;
		}

    	  Card_num[3]=(rdbuf[0]>>4) +'0';	  
		  Card_num[4]=(rdbuf[0]&0x0f) +'0';  
		  Card_num[5]=(rdbuf[1]>>4) + '0';
		  Card_num[6]=(rdbuf[1]&0x0f) + '0';
		  Card_num[7]=(rdbuf[2]>>4) + '0';
		  Card_num[8]=(rdbuf[2]&0x0f) + '0';
		  Card_num[9]=(rdbuf[3]>>4) + '0';
		  Card_num[10]=(rdbuf[3]&0x0f) + '0';
		  Card_num[11]=(rdbuf[4]>>4) + '0';
		  Card_num[12]=(rdbuf[4]&0x0f) + '0';
			
		  money=rdbuf[5]*256+rdbuf[6];

	}
	else if(RxBuf[0]==COM_WR_CARD) //写卡命令
	{
		err_flag=0;
		Write_One_Byte(CardID_Addr,RxBuf[2]); //写卡号
		Write_One_Byte(CardID_Addr+1,RxBuf[3]);
		Write_One_Byte(CardID_Addr+2,RxBuf[4]);
		Write_One_Byte(CardID_Addr+3,RxBuf[5]);
		Write_One_Byte(CardID_Addr+4,RxBuf[6]);

		Write_One_Byte(Money_Addr,RxBuf[7]);
		Write_One_Byte(Money_Addr+1,RxBuf[8]);
		SBUF=COM_WR_CARD;while(!TI);TI=0;
		SBUF=COM_WR_CARD;while(!TI);TI=0;
		SBUF=COM_WR_CARD;while(!TI);TI=0;
		/*if((Read_One_Byte(CardID_Addr)!=RxBuf[2])
		 ||(Read_One_Byte(CardID_Addr+1)!=RxBuf[3])
		 ||(Read_One_Byte(CardID_Addr+2)!=RxBuf[4])
		 ||(Read_One_Byte(CardID_Addr+3)!=RxBuf[5])
		 ||(Read_One_Byte(CardID_Addr+4)!=RxBuf[6]))
		 err_flag=1;

		if(!err_flag)
		{
			SBUF=COM_WR_CARD;while(!TI);TI=0;
			SBUF=COM_WR_CARD;while(!TI);TI=0;
			SBUF=COM_WR_CARD;while(!TI);TI=0;
			SBUF=COM_WR_CARD;while(!TI);TI=0;
		}
		else
		{
			SBUF=COM_WR_ERROR;
			while(!TI);
			TI=0;
		} */
	}
	else if(RxBuf[0]==COM_ADD_MONEY) //充钱命令
	{
		err_flag=0;
		money=Read_One_Byte(Money_Addr)*256+Read_One_Byte(Money_Addr+1);
		money += (RxBuf[7]*256 + RxBuf[8]);

	 	if( Read_One_Byte(Money_Addr)!=(money/256)
		 || Read_One_Byte(Money_Addr+1)!=(money%256))
	 		err_flag=1;

	 	if(!err_flag)
	 	{
			SBUF=COM_ADD_MONEY;
			while(!TI);
			TI=0;
	 	}else 
	 	{
	 		SBUF=COM_WR_ERROR;
			while(!TI);
			TI=0;
	 	} 
	}
	else if(RxBuf[0]==COM_DEC_MONEY)
	{
			 err_flag=0;
			 money=Read_One_Byte(Money_Addr)*256+Read_One_Byte(Money_Addr+1);
			 money -= (RxBuf[7]*256 + RxBuf[8]); 

			 Write_One_Byte(Money_Addr,money/256);
			 Write_One_Byte(Money_Addr+1,money%256);	  

			 if( Read_One_Byte(Money_Addr)!=(money/256)
				 || Read_One_Byte(Money_Addr+1)!=(money%256))
			 err_flag=1;

			 if(!err_flag)
			 {
				SBUF=COM_DEC_MONEY;
				while(!TI);
				TI=0;
			 }else 
			 {
			 	SBUF=COM_WR_ERROR;
				while(!TI);
				TI=0;
			 } 		
	}
	EA=1;

}