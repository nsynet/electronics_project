#include <lpc213x.h>
#include <mydef.h>
#include <lpc_reg.h>

#include "pcd8544_hal.h"
#include "_font_asc.h"

#define SET_X(x) x+0x80
#define SET_Y(y) y+0x40

void pcd8544_backlight(uint8_t sw)
{
    /*if(sw == 0) PCD8544_LIGHT_OFF;
    else PCD8544_LIGHT_ON;*/
}

/*----------------------------------------------------------*\
 | PCD5544 SSP Initialize                                       |
\*----------------------------------------------------------*/
void  ssp_init(void) 
{
    PINSEL1 |= 0x00000088;	// 设置SPI管脚连接

	IO0DIR |= (1<<20)|(1<<23)|(1<<18);	// CS/DC/RST
	IO0CLR = (1<<18);

	//SSPCCR = 0x08;		// 设置SPI时钟分频
	//SSPCR = 0x38;		// 设置SPI接口模式，MSTR=1，CPOL=1，CPHA=1，LSBF=0

	
    SSPCR0 = (0x01 << 8) |              // SCR  设置SPI时钟分频
             (0x00 << 7) |              // CPHA 时钟输出相位,仅SPI模式有效 
             (0x00 << 6) |              // CPOL 时钟输出极性,仅SPI模式有效
             (0x00 << 4) |              // FRF  帧格式 00=SPI,01=SSI,10=Microwire,11=保留
             (0x07 << 0);               // DSS  数据长度,0000-0010=保留,0011=4位,0111=8位,1111=16位

    SSPCR1 = (0x00 << 3) |              // SOD  从机输出禁能,1=禁止,0=允许
             (0x00 << 2) |              // MS   主从选择,0=主机,1=从机
             (0x01 << 1) |              // SSE  SSP使能,1=允许SSP与其它设备通信
             (0x00 << 0);               // LBM  回写模式
             
    SSPCPSR = 0x58;                     // PCLK分频值
    SSPIMSC = 0x07;                     // 中断屏蔽寄存器
    SSPICR  = 0x03;                     // 中断清除寄存器

    IO0SET=(1<<18);
}

/*===========================================================*/
static __inline pcd8544_set_xy(uint8_t x, uint8_t y)
{
    pcd8544_write_byte(PCD8544_CMD, SET_X(x));
    pcd8544_write_byte(PCD8544_CMD, SET_Y(y));
}


/*===========================================================*/
void pcd8544_cls(void)
{
    uint32_t i;
    pcd8544_set_xy(0, 0);
    for(i=0; i<512; i++) pcd8544_write_byte(PCD8544_DATA, 0);
    
}

/*===========================================================*/
void pcd8544_write_string(uint8_t x, uint8_t y, char *str, uint8_t reverse)
{
    uint8_t buf[6], c;
	uint32_t i;

    pcd8544_set_xy(x, y);

    while(*str != 0)
    {
        c = *str;
        if (c < ' ' || c > '~') c = '*';

        //c -= ' ';//字库从32开始
        for(i=0; i<6; i++) buf[i] = font6x8[c- ' '][i];
        str++;

        if(reverse == TRUE)
        {
            for(i=0; i<6; i++) buf[i] = ~buf[i];
        }

        pcd8544_write(buf, 6);
    }
}

/*===========================================================*/
/*void pcd8544_write_string_p(uint8_t x, uint8_t y, const char *str, uint8_t reverse)
{
    uint8_t buf[6], i, c;

    pcd8544_set_xy(x, y);

    while(*str != 0)
    {
        c = *str;
        if (c < ' ' || c > '~') c = '*';
        for(i=0; i<6; i++) buf[i] = font6x8[c - ' '][i];
        str++;

        if(reverse == TRUE)
        {
            for(i=0; i<6; i++) buf[i] = ~buf[i];
        }

        pcd8544_write(buf, 6);
    }
}*/

/*===========================================================*/
void pcd8544_cls_line(uint8_t l, uint8_t startp, uint8_t endp)
{
    uint32_t i;
    if(l>5) return;
    pcd8544_set_xy(startp, l);
    for(i = startp; i<=endp; i++) 
    {
        if(i == 84) break;
        pcd8544_write_byte(PCD8544_DATA, 0);
    }
}

/*===========================================================*/
void pcd8544_init(void)
{
    ssp_init();

    pcd8544_write_byte(PCD8544_CMD, 0x20|1);	//进入扩展指令   Activate Chip and H=1.
	pcd8544_write_byte(PCD8544_CMD, 0x80+38);	//设置Vop,相当于亮度       Set LCD Voltage to about 7V.
	pcd8544_write_byte(PCD8544_CMD, 0x04+2);	//设置温度系数,相当于对比度       
	pcd8544_write_byte(PCD8544_CMD, 0x13);	//设置偏置  Adjust voltage bias.
	pcd8544_write_byte(PCD8544_CMD, 0x20);	//进入基本指令    Horizontal addressing and H=0.
	pcd8544_write_byte(PCD8544_CMD, 0x0C);	//使能芯片活动/垂直寻址    Display Normal.

    
    pcd8544_cls();//清屏
}

