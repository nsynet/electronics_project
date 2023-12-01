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
			printf("QRScanerRxBuf 已满\r\n");
		}
		
		HAL_UART_Receive_IT(&huart2, TmpBuf, 1);
	}
}

char multip_scaner_cmd[] = "AT+SCAN=0\r\n";	//全速扫码
char single_scaner_cmd[] = "AT+SCAN=1\r\n";	//单次扫码

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

#define PASS_WORD	"1234567890"			// 默认的门禁密码，修改成自己的密码，也就是二维码的内容。

void QR_Door_Control(void)
{	
	uint32_t tick_start = 0;
	
	printf("---欢迎使用二维码门禁系统---\r\n");
	
	memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
	QR_Scaner_RxIdx = 0;
	
	LED_Control(0);		// 关LED灯
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	delayms(50);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

	// 启动串口接收
	HAL_UART_Receive_IT(&huart2, TmpBuf, 1);
	
	// 等待二维码扫描模块启动
	tick_start = get_systicks();
	while(1)
	{
		if(strstr((char *)QRScanerRxBuf , QR_SCANER_RESET_RETURN))
		{
			printf("\r\n .二维码扫描模块已启动...\r\n");
			break;
		}
		
		if(query_time(tick_start, 5000))
		{
			goto exit;
		}
	}
	printf("\r\n .已进入扫码识别模式，请扫码开门--->\r\n");

	memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
	QR_Scaner_RxIdx = 0;	
	while(1)
	{
		if(QR_Scaner_RxIdx)
		{		
			/* 等待数据收完 */
			delayms(100);
			
			/* 对比收到的数据和密码 */
			if(strcmp((char *)QRScanerRxBuf, PASS_WORD"\r\n") == 0)
			{
				//printf("密码是:%s", (char *)QRScanerRxBuf);
				printf("\r\n .门禁密码正确，门已打开。(按K1键可关门) <---\r\n");
				
				LED_Control(1);			// 打开LED
				//
				/* 在这里用舵机开门*/
				//				
				while(Get_KEY_Status()) // 等待按 K1 键关门
				{
					delayms(20);
				}
				//
				/* 在这里用舵机关门*/
				//
				LED_Control(0);			// 关闭LED
				printf("\r\n .门已关闭，请扫码开门\r\n");
			}
			else
			{
				printf("\r\n .门禁密码错误!\r\n");
				//delayms(200);
			}
			
			// 门已关闭，重新开始扫码开门
			if(scan_mode_set(1) != 0)
			{
				printf("设置单次扫码模式失败2\r\n");
				break;
			}	
			
			memset(QRScanerRxBuf, 0x00, sizeof(QRScanerRxBuf));
			QR_Scaner_RxIdx = 0;				
		}	
	}
	
exit:
	
	printf("进入扫码识别模式失败，请检查硬件连接。\r\n");
	printf("QRScanerRxBuf:%s", QRScanerRxBuf);
}
