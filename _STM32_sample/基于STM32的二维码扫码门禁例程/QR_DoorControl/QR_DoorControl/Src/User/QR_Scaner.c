#include <string.h>
#include "QR_Scaner.h"
#include "usart.h"
#include "systick.h"

#include "LED.h"
#include "KEY.h"

uint8_t		TmpBuf[1];
uint8_t		QRScanerRxBuf[1024];
__IO uint32_t	QR_Scaner_RxIdx = 0;

uint8_t complete_flag = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		if(TmpBuf[0] != 0x00)
			QRScanerRxBuf[QR_Scaner_RxIdx++] = TmpBuf[0];
		
		//HAL_UART_Transmit(&huart1, (uint8_t *)TmpBuf, 1, 0xffff);
		
		if(QR_Scaner_RxIdx >= sizeof(QRScanerRxBuf))
		{
			memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
			QR_Scaner_RxIdx = 0;
			printf("QRScanerRxBuf ����\r\n");
		}
		
		HAL_UART_Receive_IT(&huart2, TmpBuf, 1);
	}
}

char multip_scaner_cmd[] = "AT+SCAN=0\r\n";	//ȫ��ɨ��
char single_scaner_cmd[] = "AT+SCAN=1\r\n";	//����ɨ��

int scan_mode_set(uint8_t mode)
{
	memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
	QR_Scaner_RxIdx = 0;	
	
	switch(mode)
	{
		case 0:
			HAL_UART_Transmit(&huart2, (uint8_t *)multip_scaner_cmd, strlen(multip_scaner_cmd), 100);
			break;
		case 1:
			HAL_UART_Transmit(&huart2, (uint8_t *)single_scaner_cmd, strlen(single_scaner_cmd), 100);
			break;
		default:
			return -1;
	}	
		
	delayms(20);

	if(strstr((char *)QRScanerRxBuf, "[AT]OK\r\n"))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

#define QR_SCANER_RESET_RETURN		"OK! - [QBoard][A very outstanding Bar Code Scanner]\r\n"

#define PASS_WORD	"1234567890"			// Ĭ�ϵ��Ž����룬�޸ĳ��Լ������룬Ҳ���Ƕ�ά������ݡ�

void QR_Door_Control(void)
{	
	uint32_t tick_start = 0;
	
	printf("---��ӭʹ�ö�ά���Ž�ϵͳ---\r\n");
	
	memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
	QR_Scaner_RxIdx = 0;
	
	LED_Control(0);		// ��LED��
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	delayms(50);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

	// �������ڽ���
	HAL_UART_Receive_IT(&huart2, TmpBuf, 1);
	
	// �ȴ���ά��ɨ��ģ������
	tick_start = get_systicks();
	while(1)
	{
		if(strstr((char *)QRScanerRxBuf , QR_SCANER_RESET_RETURN))
		{
			printf("\r\n .��ά��ɨ��ģ��������...\r\n");
			break;
		}
		
		if(query_time(tick_start, 5000))
		{
			goto exit;
		}
	}
	printf("\r\n .�ѽ���ɨ��ʶ��ģʽ����ɨ�뿪��--->\r\n");

	memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
	QR_Scaner_RxIdx = 0;	
	while(1)
	{
		if(QR_Scaner_RxIdx)
		{		
			/* �ȴ��������� */
			delayms(100);
			
			/* �Ա��յ������ݺ����� */
			if(strcmp((char *)QRScanerRxBuf, PASS_WORD"\r\n") == 0)
			{
				//printf("������:%s", (char *)QRScanerRxBuf);
				printf("\r\n .�Ž�������ȷ�����Ѵ򿪡�(��K1���ɹ���) <---\r\n");
				
				LED_Control(1);			// ��LED
				//
				/* �������ö������*/
				//				
				while(Get_KEY_Status()) // �ȴ��� K1 ������
				{
					delayms(20);
				}
				//
				/* �������ö������*/
				//
				LED_Control(0);			// �ر�LED
				printf("\r\n .���ѹرգ���ɨ�뿪��\r\n");
			}
			else
			{
				printf("\r\n .�Ž��������!\r\n");
				//delayms(200);
			}
			
			// ���ѹرգ����¿�ʼɨ�뿪��
			if(scan_mode_set(1) != 0)
			{
				printf("���õ���ɨ��ģʽʧ��2\r\n");
				break;
			}	
			
			memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
			QR_Scaner_RxIdx = 0;				
		}	
	}
	
exit:
	
	printf("����ɨ��ʶ��ģʽʧ�ܣ�����Ӳ�����ӡ�\r\n");
	printf("QRScanerRxBuf:%s", QRScanerRxBuf);
}
