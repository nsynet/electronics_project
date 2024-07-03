#ifndef __LOGGER_H
#define __LOGGER_H

#include "common.h"

/*=======================================================   
    �ռ�ʹ��: 
      0x000000 - 0x03FFFF :  256K �ֿ� 16*16
      0x040000 - 0x06FFFF :  192K �ֿ� 12*12
      0x070000 - 0x071FFF :    8K �洢�ռ�ӳ���
      0x072000 - 0x1FFFFF : 1592K ������, 256 Bytes/Page, 6368 Pages

    ���ݿ��ÿռ�: 2048K- 448K(�ֿ�) = 1600K
    ÿ��������� 32 Byte

    �洢�ռ�ӳ���:
    ÿ���ֽ�ӳ��������һҳ(256Byte)������

    ���� SPI Flash ֻ�ܽ� '1' �� '0', ͨ�����°취��һ���ֽ��ڲ�����
ʱҲ�ܶ�μ�¼.
    
=======================================================*/

#define DATA_MAP_BASE_ADDR      0x00070000  // ����ӳ�����ʼ��ַ
#define MAX_DATA_PAGES          6368        // 
#define DATA_MAP_PAGE_TRACK     0xFE        // b0000 1110
#define DATA_MAP_PAGE_USED      0xFD        // b0000 1101
#define DATA_MAP_PAGE_FULL      0xFB        // b0000 1011

#define DATA_BASE_ADDR          0x00072000  // ��������ʼ��ַ




/*=======================================================
GPS Data �洢:
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
   |00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F|
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
00h|$- YY MM DD hh mm ss dd mm mH mL NS dddmm mH mL|
01h|EW HD VD PD SH SL AH AM AL                   CS|
---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

0x00 $-: �ļ�ͷ $:��Ч��λ -:��Ч��λ
0x01 YY: ��
0x02 MM: ��
0x03 DD: ��
0x04 hh: Сʱ 24Сʱ��ʽ, UTC+8
0x05 mm: ����
0x06 ss: ��
0x07 dd: γ�� - dd
0x08 mm: γ�� - mm
0x09 mH: γ�� - mmmm �߰�λ, ��Χ 0-9999
0x0A mL: γ�� - mmmm �Ͱ�λ, ͬ��
0x0B NS: γ�ȱ�־ ��γ/��γ
0x0C ddd: ���� - ddd
0x0D mm: ���� - mm
0x0E mH: ���� - .mmmm �߰�λ, ͬ��
0x0F mL: ���� - .mmmm �Ͱ�λ, ͬ��
0x10 EW: γ�ȱ�־ ����/����
0x11 HD: HDOP ˮƽ�������� *10, ��Χ 0.0-25.5 ������Ϊ 0xFF
0x12 VD: VDOP ��ֱ�������� *10, ͬ��
0x13 PD: PDOP �ۺϾ������� *10, ͬ��
0x14 SH: Speed �ٶ�*10 �߰�λ, ��Χ 0-9999�� ��λ ��
0x15 SL: Speed �ٶ�*10 �Ͱ�λ, ͬ��

0x16 AH: ���� - Altitude �߰�λ

0x1F CS: Check Sum, 0x00 ������� 0x00 - 0x1E ��31���ֽ�
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

