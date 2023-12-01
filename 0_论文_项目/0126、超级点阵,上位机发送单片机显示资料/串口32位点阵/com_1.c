#include "reg51.h"
#include "string.h"
sbit P3_2=P3^2;
char receive_data[96];
char flag;
char converse=0;
unsigned char code table[]={
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//���ڳ�ʼ�� ����Ϊ11.0592M����ʽ1 ������300��57600
void InitCom(unsigned char BaudRate)
{
unsigned char THTL;
switch (BaudRate)
{
case 1: THTL = 64; break; //������300
case 2: THTL = 160; break; //600
case 3: THTL = 208; break; //1200
case 4: THTL = 232; break; //2400
case 5: THTL = 244; break; //4800
case 6: THTL = 250; break; //9600
case 7: THTL = 253; break; //19200
case 8: THTL = 255; break; //57600
default: THTL = 208;
}
SCON = 0x50; //���ڷ�ʽ1,�������
TMOD = 0x20; //��ʱ��1��ʱ��ʽ2
TCON = 0x40; //�趨ʱ��1��ʼ����
TH1 = THTL;
TL1 = THTL;
PCON = 0x80; //�����ʼӱ�����,SMODλ
RI = 0; //���շ���־
TI = 0;
TR1 = 1; //������ʱ��
}

//�Ӵ��ڽ���һ�������ַ���(���жϷ�ʽ,ֻ�����ַ�����һ����idata�ַ�������)
//��unsigned char *Str;
//unsigned char i[]="abcd";
//InHandStr = &i;
void ComInStr(unsigned char *Str)
{
while(*Str !='@') //�ж��Ƿ����ַ�����β��
{
while(!RI); //������ж��ַ��Ƿ�����
*Str = SBUF; //�����ַ�
RI = 0; //��RI
Str++; //�ַ���ָ���һ
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
    unsigned char i,j;    //i:ÿ���ֵ���ʾѭ����jÿ���ֵ���ʾ�����2�
    unsigned int b=0;      //��ʾƫ�ƿ��ƣ�char�������ֻ����ʾ14������+һ���հ�λ�ַ�
    unsigned char a;//�����ƶ����ʱ��
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
    if(a>3) //�ƶ����ʱ�䣻ȡֵ0--255
        {
        a=0;
        b+=2;
        if(b>=96)  //��ʾ�����һ���֣���ͷ��ʾ���ж�ֵ=����*32
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

//���ڽ����жϺ��� 
//void serial () interrupt 4 using 3
//{
   //ComInStr(receive_data);
   //flag=1;
//}
