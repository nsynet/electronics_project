#ifndef __S1D15E06_H
#define __S1D15E06_H

#include "common.h"

//           *--------------------> Column 0,1,2 ..., 159
// Page 0    |                       
// Page 1    |
//  ...      |                      <- Start Line = COM0
// Page 25   |
//           V

#define CMD_DISPLAY_ON          0xAF    //1 显示开/关
#define CMD_DISPLAY_OFF         0xAE    //1
#define CMD_DISPLAY_OFF_VSS     0xBE    //2 显示关或节电模式输出电平
#define CMD_DISPLAY_OFF_VC      0xBF    //2
#define CMD_DISPLAY_NORMAL      0xA6    //3 正常显示/反色显示
#define CMD_DISPLAY_REVERSE     0xA7    //3
#define CMD_DISPLAY_ALL_ON      0xA5    //4 全部显示/关闭
#define CMD_DISPLAY_ALL_OFF     0xA4    //4
#define CMD_COM_OUTPUT_NORMAL   0xC4    //5 COM 扫描方向 COM0->COM131
#define CMD_COM_OUTPUT_REVERSE  0xC5    //5              COM131->COM0
#define CMD_SET_START_LINE      0x8A    //6 开始显示的行号 用于滚动屏幕
#define CMD_SET_PAGE_ADDR       0xB1    //7 设置页地址
                                        //      **XX XXXX : **00 0000(0) -> **10 0000 (32)
#define CMD_SET_COLUMN_ADDR     0x13    //8 设置纵行地址
                                        //      XXXX XXXX : 0000 0000(0) -> 1001 1111 (159)
#define CMD_DATA_WRITE          0x1D    //9
#define CMD_DATA_READ           0x1C    //10
#define CMD_DATA_IN_DIR_COLUMN  0x84    //11 设置地址指针自增方向 纵列/页
#define CMD_DATA_IN_DIR_PAGE    0x85    //11
#define CMD_COLUM_ADDR_NORMAL   0xA0    //12 列地址自增 正常(左->右)/反向(右->左)
#define CMD_COLUM_ADDR_REVERSE  0xA1    //12
#define CMD_SET_LINE_INVERSION  0x36    //13 N-行反转设置
#define CMD_LINE_INVERSION_ON   0xE5    //14
#define CMD_LINE_INVERSION_OFF  0xE4    //14
#define CMD_DISPLAY_MODE        0x66    //15 显示模式 单色/灰阶   **** **XX
#define  DAT_DISPLAY_MODE_BIN   0x01
#define  DAT_DISPLAY_MODE_4GRAY 0x00
#define CMD_SET_GRAY_PATTERN    0x39    //16 设置灰阶颜色的对比度 *XXX *XXX (1,0)(0,1) : *001 (淡) -> *110 (深)  
#define CMD_AREA_SCROLL         0x10    //17 设置区域卷动  4Gray:
                                        //      **** **XX 卷动模式: 00 全屏; 01 以上; 10 以下; 11 中间
                                        //      XXXX XXXX 起始行: 0000 0000 (00H) -> 1000 0011 (83H)
                                        //      XXXX XXXX 终止行: 0000 0000 (00H) -> 1000 0011 (83H)
                                        //      XXXX XXXX 显示行数: 0000 0000 (1) -> 1000 0011 (132)
#define CMD_SET_DUTY            0x6D    //18 设置占空比
                                        //      **XX XXXX 占空比设置: **00 0000 (1/4), **00 0001 (1/8) -> **01 1111 (1/128), **10 0000 (1/132)
                                        //      **XX XXXX 起始点设置
#define CMD_PARTIAL_DISPLAY_ON  0x97    //19 局部显示开/关
#define CMD_PARTIAL_DISPLAY_OFF 0x96    //19
#define CMD_SET_PARTIAL_DISPLAY 0x32    //20 设置局部显示
                                        //      XXXX XXXX 起始行: 0000 0000 (0) -> 1000 0011 (132)
                                        //      XXXX XXXX 行数: 0000 0001 (1) -> 1000 0011 (132)
#define CMD_READ_MODIFY_MODE    0xE0    //21 修改模式, 读取不增加AC, 写入后AC++,
#define CMD_READ_MODIFY_END     0xEE    //22 修改模式结束
#define CMD_BUILT_IN_OSC_ON     0xAB    //23 内置晶振开/关
#define CMD_BUILT_IN_OSC_OFF    0xAA    //23
#define CMD_SET_BUILT_IN_OSC_F  0x5F    //24 设置内置晶振频率(kHz)
                                        //      **** XXXX | fosc  | fcl
                                        //      ----------+-------+------
                                        //           0000 | 120.0 | fosc=120.0  *AFTER RESET
                                        //           0001 | 100.0 | fosc=100.0
                                        //           0010 |  88.0 | fosc=88.0
                                        //           0011 |  76.0 | fosc=76.0
                                        //           0100 | 120.0 | fosc/2=60.0
                                        //           0101 | 100.0 | fosc/2=50.0
                                        //           0110 |  88.0 | fosc/2=44.0
                                        //           0111 |  76.0 | fosc/2=38.0
                                        //           1000 | 120.0 | fosc/4=30.0
                                        //           1001 | 100.0 | fosc/4=25.0
                                        //           1010 |  88.0 | fosc/4=22.0
                                        //           1011 |  76.0 | fosc/4=19.0
                                        //           1100 | 120.0 | fosc/8=15.0
                                        //           1101 | 100.0 | fosc/8=12.5
                                        //           1110 |  88.0 | fosc/8=11.0
                                        //           1111 |  76.0 | fosc/8=9.5
#define CMD_POWER_CONTROL       0x25    //25 电源控制
                                        //      ***X XXXX
                                        //      ---X X--- : 11:三倍;        10:两倍;        01 Vout=VDD
                                        //      ------X-- : 1: Set-up ON;   0:Set-up OFF
                                        //      -------X- : 1: VC ON;       0:VC OFF
                                        //      --------X : 1: LCD Voltage ON               0: LCD Voltage OFF                              
#define CMD_SETUP_CK_FREQUENCY  0x41    //26
                                        //      *XXX *XXX (Set-up CK)(Set-down CK) :
                                        //      *011 (fosc/8)
                                        //      *100 (fosc/16)
                                        //      *101 (fosc/32)
                                        //      *110 (fosc/64)
                                        //      *111 (fosc/128)
#define CMD_CRYSTAL_VOL         0x2B    //27 晶振电压范围
                                        //      **** **XX : 00 (1.77-3.50V) *AFTER RESET, 10 (2.53-5.00V), 11 (3.54-7.00V)
#define CMD_SET_ELECTRONIC      0x81    //28
                                        //      *XXX XXXX : 000 0000 (小) -> 111 1111(大)
#define CMD_DISCHARGE_ON        0xEB    //29
#define CMD_DISCHARGE_OFF       0xEA    //29          
#define CMD_POWER_SAVING_ON     0xA9    //30
#define CMD_POWER_SAVING_OFF    0xA8    //30
#define CMD_TEMP_GRAD_SEL       0x4E    //31
#define CMD_STATUS_READ         0x8E    //32
#define CMD_RESET               0xE2    //33
#define CMD_MLS_DRV_DISPERSIVE  0x9C    //34
#define CMD_MLS_DRV_NONDISPERS  0x9D    //34
#define CMD_NOP                 0xE3    //35

#define GRAY_WHITE              0x00
#define GRAY_GW                 0x01
#define GRAY_GB                 0x02
#define GRAY_BLACK              0x03


#define ICON_INDEX_BAT0         0
#define ICON_INDEX_BAT25        17
#define ICON_INDEX_BAT50        34
#define ICON_INDEX_BAT75        51
#define ICON_INDEX_BAT100       68
#define ICON_INDEX_FIX_NULL     85
#define ICON_INDEX_FIX_N        100
#define ICON_INDEX_FIX_2D       115
#define ICON_INDEX_FIX_3D       130
#define ICON_INDEX_FIX_G        145
#define ICON_INDEX_FIX_DG       160
#define ICON_INDEX_FIX_DR       175
#define ICON_INDEX_DOP_DOP      190
#define ICON_INDEX_DOP_P        208
#define ICON_INDEX_DOP_H        216
#define ICON_INDEX_DOP_V        225
#define ICON_INDEX_TAB_SAT      234
#define ICON_INDEX_TAB_SPD      269
#define ICON_INDEX_TAB_LOG      299
#define ICON_INDEX_LOG_START    318

#define GUI_BT_NORMAL    0x00
#define GUI_BT_DOWN      0x01
#define GUI_BT_SELECT    0x02





void LCD_IO_Config(void);
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetGray(u8 ucText, u8 ucBack);
void LCD_SetPos(u8 ucX, u8 ucPage);

void LCD_PutChar05(char ucDAT);
void LCD_PutChar08(char ucDAT);
void LCD_PutChar12(char ucDAT);
void LCD_PutHz12(u16 Hz);
void LCD_PutChar16(char ucDAT);
void LCD_PutHz16(u16 Hz);
void LCD_PutStr05(u8 ucX, u8 ucY, char* cStr);
void LCD_PutStr08(u8 ucX, u8 ucY, char* cStr);
void LCD_PutStr16(u8 ucX, u8 ucY, char* cStr);
void LCD_PutStr12(u8 ucX, u8 ucY, char* cStr);


void LCD_GPS_SNR(u8 ucX, u8 ucY, u8 ID, u8 ucSNR, u8 SatUsed);
//void LCD_GPS_SatInfo(struct_GPSGSV* pGSV, u8* SatUsed);
void LCD_GPS_Course(u8 ucX, u8 ucY, u16 deg);


void LCD_GUI_Rect(u8 ucXS, u8 ucYS, u8 ucXE, u8 ucYE, u8 uRectStyle);
void LCD_GUI_Button(u8 ucX, u8 ucY, char * btStr, u8 btStyle);
void LCD_GUI_ProgressBar(u8 ucX, u8 ucY, u8 ucLen, u8 ucPerc, u8 ucStyle);
void LCD_GUI_ScrollBarV(u8 ucX, u8 ucPageTop, u8 ucPageBottom, u8 ucRange, u8 ucPos);
void LCD_GUI_HR(u8 ucXS, u8 ucXE, u8 ucY);

void LCD_GUI_BG1(void);
void LCD_GUI_FlashMap(u8 * ucFlashMap, u16 PageID);

void LCD_GPS_Tab(u8 TabIndex);

void LCD_FillBase00(void);
void LCD_Icon(u8 ucX, u8 ucY, u16 StartIndex, u8 TextColor, u8 BackColor);
void LCD_IconSave(u8 ucX, u8 ucPage);   
#endif
