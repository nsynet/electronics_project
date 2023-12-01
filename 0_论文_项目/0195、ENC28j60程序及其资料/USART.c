#include "USART.h"
#include "STC12C5A.h"
#define BAUD 9600
//���ڳ�ʼ��
void USART_Init()
{
	PCON &= 0x7f;		//�����ʲ����� SMOD=0
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xfe;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0f;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xB2;			//�趨��ʱ��ֵ
	TH1 = 0xB2;			//�趨��ʱ����װֵ
	ET1 = 0;			//��ֹ��ʱ��1�ж�
	TR1 = 1;			//������ʱ��1
	ES = 1;				//�������ж�
	EA = 1;				//�����ж�
}	  

//����һ���ַ�,ASCII��
void SendASC(unsigned char d)
{
	
	SBUF=d;
	while(!TI);	//�ȴ��������,TI��λ
	TI=0;
}

//�����ַ���
void SendString(unsigned char *str)	
{
	while(*str)
	{
		SendASC(*str) ;
		str++;
	}
}


//�����жϷ������
void USART_Interrupt(void)  interrupt 4	  //�������� �ͷ���main����������һ����
{
	unsigned char RxData=0;
	EA=0; 
   	if(RI) 			//�����ж�RI(�����жϱ�־)�Ƿ�Ϊ1
    {
    	RI=0;
    	RxData = SBUF;	//�����������Ľ��մ���
		SendString("The word you send is ");
		SendASC(RxData);
		SendASC('\n');
    }
	EA=1;
}


