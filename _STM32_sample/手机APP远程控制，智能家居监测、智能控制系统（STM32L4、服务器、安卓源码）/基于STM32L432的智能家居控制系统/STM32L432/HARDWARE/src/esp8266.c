#include "esp8266.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="CKJ";			//·����SSID��
const u8* wifista_password="15219712622"; 	//��������

const u8* server_ip="192.168.1.1";			//������IP
const u8* server_port="9999"; 	//�������˿�

//ESP8266״̬��λ7��1Ϊ������WIFI
//λ4-5��00ΪID��Ϊ0��Զ��������Ҫ������01��10��11�Դ�����
//λ2-0��000Ϊ��λ״̬��001Ϊ���յ�'+'�ţ�����һ�����ַ�Ϊ'I'ʱ��ʼ������010Ϊ���յ�"+I"��׼�����նԷ�ID��
//011Ϊ���յ�ID�ţ�׼���������100Ϊ���������У�׼����ɽ��գ�101Ϊ��ǰ���������ϣ�
u8 ESP8266_STA;
//���Զ��������ַ���
u8 ESP8266_Cmd[50];


void esp8266_init()
{
	while(esp8266_send_cmd("ATE0","OK",20));  //�ػ���
	esp8266_send_cmd("AT+CWMODE=1","OK",20);  //STAģʽ
	esp8266_send_cmd("AT+CIPMUX=1","OK",20);  //������ģʽ
	sprintf((char*)ESP8266_TX_BUF,"AT+CWJAP=\"%s\",\"%s\"\r\n",wifista_ssid,wifista_password);
	esp8266_send_data(ESP8266_TX_BUF,"OK",8000);  //����wifi
	while(Check_Connections() != 2 && Check_Connections() != 4);  //�ȴ�WIFI�������
	sprintf((char*)ESP8266_TX_BUF,"AT+CIPSTART=0,\"TCP\",\"%s\",%s\r\n",server_ip,server_port);
	esp8266_send_data(ESP8266_TX_BUF,"OK",50);  //���ӷ�����
}


//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(ESP8266_RX_STA&0X8000)		//���յ�һ��������
	{ 
		ESP8266_RX_BUF[ESP8266_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)ESP8266_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	ESP8266_RX_STA=0;
	esp8266_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(ESP8266_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp8266_check_cmd(ack))
				{
//					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					ESP8266_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 esp8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	ESP8266_RX_STA=0;
	esp8266_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(ESP8266_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp8266_check_cmd(ack))break;//�õ���Ч���� 
				ESP8266_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

//�ȴ�ESP8266Ӧ��
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 Wait_Reply(u8 *ack,u16 waittime)
{
	ESP8266_RX_STA=0;
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(ESP8266_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp8266_check_cmd(ack))break;//�õ���Ч���� 
				ESP8266_RX_STA=0;
			} 
		}
		if(waittime==0) return 1; 
	}
	return 0;
}

//�Զ���ǰID��������
//���ͳɹ�����1��ʧ�ܷ���0
u8 Send_NetworkData(u8 ID,u8* data)
{
	u8 data_buf[22];
	sprintf((char*)data_buf,"AT+CIPSENDEX=%1d,%d\r\n",ID,strlen((const char*)data));
	WIFI_SendData(data_buf);
//	printf("AT+CIPSENDEX=%1d,%d",ID,strlen((const char*)data));
	if(!Wait_Reply(">",10) && !esp8266_send_data(data,"SEND OK",30))
	{
		return 1;
	}
	return 0;
}

//���esp8266�������������������״̬
//����ֵ��0:״̬��ȡʧ��
//2 ������AP�����IP��ַ
//3���ѽ���TCP��UDP����
//4���Ͽ���������
//5��δ����AP
u8 Check_Connections()
{
	if(!esp8266_send_cmd("AT+CIPSTATUS","STATUS:",10))
		return ESP8266_RX_BUF[7]-0x30;
	return 0;
}

//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 esp8266_quit_trans(void)
{
	while((USART1->ISR&0X40)==0);	//�ȴ����Ϳ�
	USART1->TDR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART1->ISR&0X40)==0);	//�ȴ����Ϳ�
	USART1->TDR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART1->ISR&0X40)==0);	//�ȴ����Ϳ�
	USART1->TDR='+';      
	delay_ms(500);					//�ȴ�500ms
	return esp8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}

//��ȡATK-ESP8266ģ���AP+STA����״̬
//����ֵ:0��δ����;1,���ӳɹ�
u8 esp8266_apsta_check(void)
{
	if(esp8266_quit_trans())return 0;			//�˳�͸�� 
	esp8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	if(esp8266_check_cmd("+CIPSTATUS:0")&&
		 esp8266_check_cmd("+CIPSTATUS:1")&&
		 esp8266_check_cmd("+CIPSTATUS:2")&&
		 esp8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 esp8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(esp8266_quit_trans())return 0;			//�˳�͸�� 
	esp8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	p=esp8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//�õ�����״̬	
	return res;
}

//��ȡClient ip��ַ
//ipbuf:ip��ַ���������
void esp8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(esp8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=esp8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

//����ɨ��
void Cmd_Scan()
{
	u8 num=0,i=1,j=0;
	if((ESP8266_STA&7)==5)
	{
		if(ESP8266_Cmd[0]=='{')
		{
			for(;i<50;i++)
			{
				if(ESP8266_Cmd[i]==',') num++;
				if(num==7)
				{
					j=++i;
					while(ESP8266_Cmd[j++]!=',' && j-i<4);
					switch(j-i-1)
					{
						case 1:Manual_STA=ESP8266_Cmd[i]-0x30;
										break;
						case 2:Manual_STA=(ESP8266_Cmd[i]-0x30)*10 + (ESP8266_Cmd[i+1]-0x30);
										break;
						case 3:Manual_STA=(ESP8266_Cmd[i]-0x30)*100 + (ESP8266_Cmd[i+1]-0x30)*10 + (ESP8266_Cmd[i+2]-0x30);
										break;
					}
//					printf("len:%d  %s\r\n",j-i,&ESP8266_Cmd[i]);
					i=j;
					while(ESP8266_Cmd[j++]!='}' && j-i<4);
					switch(j-i-1)
					{
						case 1:AUTO_STA=ESP8266_Cmd[i]-0x30;
										break;
						case 2:AUTO_STA=(ESP8266_Cmd[i]-0x30)*10 + (ESP8266_Cmd[i+1]-0x30);
										break;
						case 3:AUTO_STA=(ESP8266_Cmd[i]-0x30)*100 + (ESP8266_Cmd[i+1]-0x30)*10 + (ESP8266_Cmd[i+2]-0x30);
										break;
					}
//					printf("len:%d  %s\r\n",j-i,&ESP8266_Cmd[i]);
//					printf("%d  %d\r\n",Manual_STA,AUTO_STA);
					break;
				}
			}
		}
	}
}





