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
    PINSEL1 |= 0x00000088;	// ����SPI�ܽ�����

	IO0DIR |= (1<<20)|(1<<23)|(1<<18);	// CS/DC/RST
	IO0CLR = (1<<18);

	//SSPCCR = 0x08;		// ����SPIʱ�ӷ�Ƶ
	//SSPCR = 0x38;		// ����SPI�ӿ�ģʽ��MSTR=1��CPOL=1��CPHA=1��LSBF=0

	
    SSPCR0 = (0x01 << 8) |              // SCR  ����SPIʱ�ӷ�Ƶ
             (0x00 << 7) |              // CPHA ʱ�������λ,��SPIģʽ��Ч 
             (0x00 << 6) |              // CPOL ʱ���������,��SPIģʽ��Ч
             (0x00 << 4) |              // FRF  ֡��ʽ 00=SPI,01=SSI,10=Microwire,11=����
             (0x07 << 0);               // DSS  ���ݳ���,0000-0010=����,0011=4λ,0111=8λ,1111=16λ

    SSPCR1 = (0x00 << 3) |              // SOD  �ӻ��������,1=��ֹ,0=����
             (0x00 << 2) |              // MS   ����ѡ��,0=����,1=�ӻ�
             (0x01 << 1) |              // SSE  SSPʹ��,1=����SSP�������豸ͨ��
             (0x00 << 0);               // LBM  ��дģʽ
             
    SSPCPSR = 0x58;                     // PCLK��Ƶֵ
    SSPIMSC = 0x07;                     // �ж����μĴ���
    SSPICR  = 0x03;                     // �ж�����Ĵ���

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

        //c -= ' ';//�ֿ��32��ʼ
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

    pcd8544_write_byte(PCD8544_CMD, 0x20|1);	//������չָ��   Activate Chip and H=1.
	pcd8544_write_byte(PCD8544_CMD, 0x80+38);	//����Vop,�൱������       Set LCD Voltage to about 7V.
	pcd8544_write_byte(PCD8544_CMD, 0x04+2);	//�����¶�ϵ��,�൱�ڶԱȶ�       
	pcd8544_write_byte(PCD8544_CMD, 0x13);	//����ƫ��  Adjust voltage bias.
	pcd8544_write_byte(PCD8544_CMD, 0x20);	//�������ָ��    Horizontal addressing and H=0.
	pcd8544_write_byte(PCD8544_CMD, 0x0C);	//ʹ��оƬ�/��ֱѰַ    Display Normal.

    
    pcd8544_cls();//����
}

