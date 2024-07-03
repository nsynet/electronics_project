

//定时器1初始化,串口波特率初始化
//自动ISP功能模块
sfr ISP_CONTR = 0xe7;
void timer1_init(void)
{
   TMOD = (TMOD & 0X0F) | 0X20;  
   SCON = 0x50;
   //PCON |= 0x80;//波特率翻倍
   TH1 = 0xff;  /* 22.1184MHz晶振, 115200bps */
	TR1 = 1;
	//TI = 1 ; //使用keil中的串口函数时需要置位
   ES = 1;
   EA = 1; 
}

/*  delay(unsigned int time)
{
	while(--time);
} */

//自动ISP功能函数
/*
 ISP_Check(unsigned char tmp)
{
    static isp_counter=0;
//	bit flag;
    unsigned char code isp_comm[16]={0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef,0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef};
    if(tmp != isp_comm[isp_counter++]) {
    	isp_counter = 0;
	//	flag=1;  
    	return ;//flag;
    }
    else if (isp_counter > 15) {
                                EA = 0;
	                            delay(20000);
                                ISP_CONTR = 0x60; //软复位进入ISP下载模式   
                                }
	//return flag=0;	     
}

*/
//串口中断
void Serial_int(void) interrupt 4 
{ 
   unsigned int j = 0;
	unsigned char i ;
	ES = 0;  //关闭中断
	if (RI) 
   {
      for(i=0;i<21;i++)
		{
	      while(!RI)
			{
			   if(j++ >60000) goto back ;
			} 
			RI = 0 ;
			j = 0 ;
			set_time_buf[i] = SBUF ; //read
		}    
   }
	Serial_SetTime();
	back :
	ES = 1;
}

