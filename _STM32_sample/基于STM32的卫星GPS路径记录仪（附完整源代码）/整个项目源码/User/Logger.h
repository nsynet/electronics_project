#ifndef __LOGGER_H
#define __LOGGER_H

#include "common.h"

/*=======================================================   
    空间使用: 
      0x000000 - 0x03FFFF :  256K 字库 16*16
      0x040000 - 0x06FFFF :  192K 字库 12*12
      0x070000 - 0x071FFF :    8K 存储空间映射表
      0x072000 - 0x1FFFFF : 1592K 数据区, 256 Bytes/Page, 6368 Pages

    数据可用空间: 2048K- 448K(字库) = 1600K
    每个点的数据 32 Byte

    存储空间映射表:
    每个字节映射数据区一页(256Byte)的数据

    根据 SPI Flash 只能将 '1' 置 '0', 通过以下办法是一个字节在不擦除
时也能多次记录.
    
=======================================================*/

#define DATA_MAP_BASE_ADDR      0x00070000  // 数据映射表起始地址
#define MAX_DATA_PAGES          6368        // 
#define DATA_MAP_PAGE_TRACK     0xFE        // b0000 1110
#define DATA_MAP_PAGE_USED      0xFD        // b0000 1101
#define DATA_MAP_PAGE_FULL      0xFB        // b0000 1011

#define DATA_BASE_ADDR          0x00072000  // 数据区起始地址




/*=======================================================
GPS Data 存储:
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
   |00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F|
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
00h|$- YY MM DD hh mm ss dd mm mH mL NS dddmm mH mL|
01h|EW HD VD PD SH SL AH AM AL                   CS|
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

0x00 $-: 文件头 $:有效定位 -:无效定位
0x01 YY: 年
0x02 MM: 月
0x03 DD: 日
0x04 hh: 小时 24小时格式, UTC+8
0x05 mm: 分钟
0x06 ss: 秒
0x07 dd: 纬度 - dd
0x08 mm: 纬度 - mm
0x09 mH: 纬度 - mmmm 高八位, 范围 0-9999
0x0A mL: 纬度 - mmmm 低八位, 同上
0x0B NS: 纬度标志 北纬/南纬
0x0C ddd: 经度 - ddd
0x0D mm: 经度 - mm
0x0E mH: 经度 - .mmmm 高八位, 同上
0x0F mL: 经度 - .mmmm 低八位, 同上
0x10 EW: 纬度标志 东经/西经
0x11 HD: HDOP 水平精度因子 *10, 范围 0.0-25.5 超过则为 0xFF
0x12 VD: VDOP 垂直精度因子 *10, 同上
0x13 PD: PDOP 综合精度因子 *10, 同上
0x14 SH: Speed 速度*10 高八位, 范围 0-9999， 单位 节
0x15 SL: Speed 速度*10 低八位, 同上

0x16 AH: 海拔 - Altitude 高八位

0x1F CS: Check Sum, 0x00 依次异或 0x00 - 0x1E 这31个字节
=======================================================*/

#define FLASH_STATUS_UNKNOW 0x00
#define FLASH_STATUS_MOUNTED 0x01
#define FLASH_STATUS_DISKFULL 0x02
#define FLASH_STATUS_NEEDSTART 0x02
#define NO_DATA_SPACE 0x02


void BrowseRecord(void);
void LOG_Point(void);
u8   LOG_MountFlash(void);
u16  LOG_GetDataMapIndex(void);
u8   LOG_GetDataIndexInPage(void);
void LOG_GetLastRecord(char* pStr1, char* pStr2);
void LOG_DumpFlashToTF(void);
void LOG_KMLToTF(void);

#endif

