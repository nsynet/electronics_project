#include "reg51.h"
#include "string.h"
sbit P3_2=P3^2;
char receive_data[96];
char flag;
char converse=0;
unsigned char code table[]={
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//´®¿Ú³õÊ¼»¯ ¾§ÕñÎª11.0592M¡¡·½Ê½1 ²¨ÌØÂÊ300£­57600
void InitCom(unsigned char BaudRate)
{
unsigned char THTL;
switch (BaudRate)
{
case 1: THTL = 64; break; //²¨ÌØÂÊ300
case 2: THTL = 160; break; //600
case 3: THTL = 208; break; //1200
case 4: THTL = 232; break; //2400
case 5: THTL = 244; break; //4800
case 6: THTL = 250; break; //9600
case 7: THTL = 253; break; //19200
case 8: THTL = 255; break; //57600
default: THTL = 208;
}
SCON = 0x50; //´®¿Ú·½Ê½1,ÔÊĞí½ÓÊÕ
TMOD = 0x20; //¶¨Ê±Æ÷1¶¨Ê±·½Ê½2
TCON = 0x40; //Éè¶¨Ê±Æ÷1¿ªÊ¼¼ÆÊı
TH1 = THTL;
TL1 = THTL;
PCON = 0x80; //²¨ÌØÂÊ¼Ó±¶¿ØÖÆ,SMODÎ»
RI = 0; //ÇåÊÕ·¢±êÖ¾
TI = 0;
TR1 = 1; //Æô¶¯¶¨Ê±Æ÷
}

//´Ó´®¿Ú½ÓÊÕÒ»¸ö¶¨³¤×Ö·û´®(·ÇÖĞ¶Ï·½Ê½,Ö»ÓÃÓÚ×Ö·ûĞèÓĞÒ»¸öµÄidata×Ö·û´®Êı×é)
//Èçunsigned char *Str;
//unsigned char i[]="abcd";
//InHandStr = &i;
void ComInStr(unsigned char *Str)
{
while(*Str !='@') //ÅĞ¶ÏÊÇ·ñµ½ÁË×Ö·û´®µÄÎ²²¿
{
while(!RI); //¿ÕÓï¾äÅĞ¶Ï×Ö·ûÊÇ·ñÊÕÍê
*Str = SBUF; //±£´æ×Ö·û
RI = 0; //ÇåRI
Str++; //×Ö·û´®Ö¸Õë¼ÓÒ»
converse=1;
}
}

void delay(int c)
    {
    int i,j;
    for(i=0;i<c;i++)
        for(j=0;j<10;j++)
            ;
     }
void delay1s(void) 

{ 

  unsigned char i,j,k; 

  for(k=60;k>0;k--) 

  for(i=20;i>0;i--) 

  for(j=248;j>0;j--); 

} 
void main()
{
    unsigned char i,j;    //i:Ã¿¸ö×ÖµÄÏÔÊ¾Ñ­»·£»jÃ¿¸ö×ÖµÄÏÔÊ¾Âë³ıÒÔ2£
    unsigned int b=0;      //ÏÔÊ¾Æ«ÒÆ¿ØÖÆ£¬charÀàĞÍ×î¶àÖ»ÄÜÏÔÊ¾14¸öºº×Ö+Ò»¸ö¿Õ°×Î»×Ö·û
    unsigned char a;//¿ØÖÆÒÆ¶¯¼ä¸ôÊ±¼ä
   InitCom(6);
      EA=1;
      ES=1;
      ET1=0;
   while(1)
   {  char i;
      ComInStr(receive_data);
while(converse)
    {
    j=0;
    if(a>3) //ÒÆ¶¯¼ä¸ôÊ±¼ä£»È¡Öµ0--255
        {
        a=0;
        b+=2;
        if(b>=96)  //ÏÔÊ¾µ½×îºóÒ»¸ö×Ö£¬»ØÍ·ÏÔÊ¾£¬ÅĞ¶ÏÖµ=×ÖÊı*32
            {
            b=0;
            }   
        }
     P3_2=1;
     for(i=0;i<16;i++)
        {
        P1=i;
        P0=receive_data[j+b];
        P2=receive_data[j+b+1];
        delay(5);
        P0=0x00;
        P2=0x00;
        j+=2;
        }

     P3_2=0;
     for(i=0;i<16;i++)
        {
        P1=i;
        P0=receive_data[j+b];
        P2=receive_data[j+b+1];
        delay(5);
        P0=0x00;
        P2=0x00;
        j+=2;
        }
        a++;

    }
	  
  }


}

//´®¿Ú½ÓÊÕÖĞ¶Ïº¯Êı 
//void serial () interrupt 4 using 3
//{
   //ComInStr(receive_data);
   //flag=1;
//}
