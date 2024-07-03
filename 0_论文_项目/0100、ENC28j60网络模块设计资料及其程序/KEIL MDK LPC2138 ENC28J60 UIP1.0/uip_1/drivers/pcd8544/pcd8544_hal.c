#include <lpc213x.h>
#include <mydef.h>
#include <lpc_reg.h>
//#define PCD8544_OUTPORT PORTB

//#define PCD8544_LIGHT PB0
#define PCD8544_CS 20
#define PCD8544_DC 23
#define PCD8544_RST 18

#define PCD8544_CMD 0
#define PCD8544_DATA 1

#define PCD8544_ENABLED set_bit(IO0CLR, PCD8544_CS)//CS = 0;
#define PCD8544_DISABLED set_bit(IO0SET, PCD8544_CS)//CS = 1;


#define PCD8544_CMD_MODE set_bit(IO0CLR, PCD8544_DC)//DC = 0;
#define PCD8544_DATA_MODE set_bit(IO0SET, PCD8544_DC)//DC = 1;


static void delay(uint32_t n)
{
    uint32_t d;
    while(n--) for(d=0; d<3000; d++);
}

/*===========================================================*/
void pcd8544_write_byte(uint8_t dc, uint8_t _data)
{
    
    if(dc == PCD8544_CMD) PCD8544_CMD_MODE;
    else PCD8544_DATA_MODE;

    PCD8544_ENABLED;

    SSPDR = _data;
    loop_until_bit_is_set(SSPSR, SSPTFE);

    delay(1);//SSP模式稍作延时，等待最后一帧数据移出

    PCD8544_DISABLED;

}

/*===========================================================*/
void pcd8544_write(uint8_t *src, uint32_t len)
{
    
    PCD8544_DATA_MODE;
    PCD8544_ENABLED;

    while(len)
    {
        len--;

        SSPDR = *src++;
        loop_until_bit_is_set(SSPSR, SSPTFE);
		delay(1);//SSP模式稍作延时，等待最后一帧数据移出

    }

    
    PCD8544_DISABLED;
}
