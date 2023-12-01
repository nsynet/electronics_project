#include <iom16v.h>			//�����ͺ�ͷ�ļ�
#include <macros.h>			//����"λ"����ͷ�ļ�
#include <stdio.h>
#include <DS18B20.h>
#define uchar unsigned char	
#define uint  unsigned int
#define SEGLK PA3	//����ܶ�ѡ���������ƶ�
#define BITLK PA4	//�����λѡ���������ƶ�
#define LEDLK PA2	//LED���������ƶ�

const SEGMENT[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d, 0x07,
		           0x7f,0x6f};
/*******************************************
��������: Delayms
��    ��: ��ʱָ�����루8M����
��    ��: MS--��ʱ�ĺ�����
����ֵ  : ��
********************************************/
void Delayms(uint MS)		  
{
 uint i,j;
 for( i=0;i<MS;i++)
 for(j=0;j<1141;j++);	//1141����8MHz�����£�ͨ��������淴��ʵ��õ�����ֵ
}
/*******************************************
��������: Board_init
��    ��: ��ʼ��ʵ��壬�ر�ȫ������
��    ��: ��
����ֵ  : ��
/********************************************/
void Board_init(void)
{
	DDRA=0xFF;	//���ĸ�IO��ȫ������Ϊ���
	DDRB=0xFF;
	DDRC=0xFF;
	DDRD=0xFF;

	SPCR=0x00;	//��ԭSPI�ڣ�ʹ��Ϊ�����˿�	

	PORTA|=BIT(PA6);//(BEEP)�رշ�����
	PORTC&=~BIT(PC6);	  //(RELAY)��ʼ���̵���Ϊ��������״̬	
	PORTA&=~BIT(PA1);	//�ر�SPI����DA
	PORTB=0xFF;	//�ر�LED
	PORTA|=BIT(LEDLK);	//�������ݣ�ʹLEDϨ��
 	Delayus(5);		
 	PORTA&=~BIT(LEDLK);
	
	PORTB=0x00;	   //�����ѡ
 	PORTA|=BIT(SEGLK);		  
 	Delayus(50);			  
 	PORTA&=~BIT(SEGLK);	    
 	
 	PORTB=0xFF;	   //���λѡ
 	PORTA|=BIT(BITLK);		   
 	Delayus(50);				   
 	PORTA&=~BIT(BITLK);	   
	
	PORTD=0xFF;
	PORTA|=BIT(PA7);
	Delayus(50);
	PORTA&=~BIT(PA7);

	
}

/*******************************************
��������: One_smg_display
��    ��: ָ�����������ʾָ��������
��    ��: data--��ʾ������(0-15)
	  	  number--ָ���������(1-6)
����ֵ  : ��
/********************************************/
void One_smg_display(uchar data,uchar number)
{ 
 PORTB|=0x3F;			   //���λѡ
 PORTA|=BIT(BITLK);		   //����λѡ
 Delayus(50);			   //����ʱ��
 PORTA&=~BIT(BITLK);	   //����λѡ

 PORTB=SEGMENT[data];	   //�����ѡ
 PORTA|=BIT(SEGLK);		   //���¶�ѡ
 Delayus(50);			   //����ʱ��
 PORTA&=~BIT(SEGLK);	   //�����ѡ
 
 number=~BIT(number-1);	   //����λѡ��ָ��ʱ��1~6����������ʱ����0~5��
 PORTB=number;			   //���λѡ
 PORTA|=BIT(BITLK);		   //����λѡ
 Delayus(50);				   //����ʱ��
 PORTA&=~BIT(BITLK);	   //����λѡ
}

/********************************************/
void uart0_init(void) 
{
  UCSRB = 0x00;  //��ֹ����
  UCSRA = 0x02;  //����
  UCSRC = 0x06;  //8λ����λ��1��ֹͣλ
  UBRRL = 0x67;  //������Ϊ9600
  UBRRH = 0x00; 
  UCSRB = 0x18;  //������
}
/********************************************************/
void uart0_send(uchar i)  //����һ���ַ�
{
  while(!(UCSRA&(1<<UDRE))); //�ȴ����ͻ�����Ϊ��
    UDR=i;                    //��������
  while(!(UCSRA&(1<<TXC)));		 //�ȴ��������
    UCSRA|=1<<TXC;	           //����������״̬λ
}
/*******************************************/
void str_send(char *s) //����һ���ַ�
{
 while(*s)
 {
  uart0_send(*s); //�ַ���δ�������������
  s++;
 }
}
/*********************************************/
uchar uart0_receive(void) //����һ���ַ�
{
  while(!(UCSRA&(1<<RXC)));//�ȴ���������
  return UDR;               //��������
}

/*******************************************
��������: main
��    ��: 6�������ѭ��������ʾָ��������
��    ��: ��
����ֵ  : ��
/********************************************/
void main(void)
{	 uint t=0;
     uchar temh_d,teml_d,temm_d;
	 //uchar RDATA[3];
	 uart0_init();
	 Board_init();	   //��ʼ��������
	 while(1)
	 {
	 
	  t=Read_temp()*0.625;//�õ�ת���¶�ֵ
      Delayms(2);
	  temh_d=t/100;
	  temm_d=(t/10)%10;
	  teml_d=t%10;
	 
	   
		//uart0_send(temh_d+48);
		//Delayms(1);
	   // uart0_send(temm_d+48);
		//Delayms(1);
	    uart0_send(temh_d+48);
		uart0_send(temm_d+48);
		str_send(".");
		uart0_send(teml_d+48);
		uart0_send(32);
		uart0_send(32);
		uart0_send(32);
		//Delayms(1);
		//str_send(" ");
		
	  One_smg_display(teml_d,6);
	  Delayms(2);
	  One_smg_display(temm_d,5);
	  Delayms(2);
	  One_smg_display(temh_d,4);
	  Delayms(2);
	
	 }
}