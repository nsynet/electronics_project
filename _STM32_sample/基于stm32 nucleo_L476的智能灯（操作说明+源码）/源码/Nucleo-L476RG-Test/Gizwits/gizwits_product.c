/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits 控制协议处理,及平台相关的硬件初始化 
* @author       Gizwits
* @date         2016-09-05
* @version      V03010101
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "hal_key.h"

#include "gizwits_product.h"

/** 用户区设备状态变量 */
extern dataPoint_t currentDataPoint;

#ifndef GPIO_KEY_NUM                                   
#define GPIO_KEY_NUM 1///< 定义按键成员总数
#endif
extern keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< 定义单个按键成员数组指针
extern keysTypedef_t keys;                                     ///< 定义总的按键模块结构体指针
/**@name Gizwits 用户API接口
* @{
*/

/**
* @brief 事件处理接口

* 说明：

* 1.用户可以对WiFi模组状态的变化进行自定义的处理

* 2.用户可以在该函数内添加数据点事件处理逻辑，如调用相关硬件外设的操作接口

* @param[in] info : 事件队列
* @param[in] data : 协议数据
* @param[in] len : 协议数据长度
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)data;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)data;
  protocolTime_t *ptime = (protocolTime_t *)data;

  if((NULL == info) || (NULL == data))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_LD2_ONOFF:
        currentDataPoint.valueLD2_OnOff = dataPointPtr->valueLD2_OnOff;
        GIZWITS_LOG("Evt: EVENT_LD2_ONOFF %d \n", currentDataPoint.valueLD2_OnOff);
        if(0x01 == currentDataPoint.valueLD2_OnOff)
        {
          //user handle
          LD2_ON();
        }
        else
        {
          //user handle
          LD2_OFF();
        }
        break;




      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
        LD2_OFF();
        break;
      case WIFI_DISCON_ROUTER:
        break;
      case WIFI_CON_M2M:
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      default:
        break;
    }
  }

  return 0;
}

/**@} */

/**
* @brief MCU复位函数

* @param none
* @return none
*/
void mcuRestart(void)
{
    //__set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

/**@} */

/**
* @brief 定时器TIM中断处理函数

* @param none
* @return none
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance ==TIM6)//tim6 1ms中断，作为MCU和WIFI模组的心跳用
  {
    gizTimerMs();
  }
  if(htim->Instance ==TIM7)//tim7 1ms中断，按键检测逻辑
  {
    keyHandle((keysTypedef_t *)&keys);
  }
}

/**
* @brief 定时器TIM3定时器初始化

* @param none
* @return none
*/
void timerInit(void)
{
   HAL_TIM_Base_Start_IT(&htim6); 
   HAL_TIM_Base_Start_IT(&htim7); 
}

/**
* @brief USART5串口中断函数

* 接收功能，用于接收与WiFi模组间的串口协议数据
* @param none
* @return none
*/

extern uint8_t RxData;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(UartHandle->Instance ==USART1)
  {					
    gizPutData(&RxData, 1);
    HAL_UART_Receive_IT(&huart1, &RxData,1);
  } 
}

/**
* @brief 串口写操作，发送数据到WiFi模组
*
* @param buf      : 数据地址
* @param len       : 数据长度
*
* @return : 正确返回有效数据长度;-1，错误返回
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    static uint8_t aa=0x55;
    
    if(NULL == buf)
    {
        return -1;
    }
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
#endif
    
    for(i=0; i<len; i++)
    {
        HAL_UART_Transmit(&huart1,(uint8_t *)&buf[i],1,0XFFFF);	
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", buf[i]);
#endif
        if(i >=2 && buf[i] == 0xFF)
        {
          HAL_UART_Transmit(&huart1,(uint8_t *)&aa,1,0XFFFF);	
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", 0x55);
#endif
        }
    }
    
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("\n");
#endif
    
    return len;
}

/**
* @brief USART串口初始化

* 用于与WiFi模组间的串口通信
* @param none
* @return none
*/
void uartInit(void)
{
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&RxData,1); 
}

