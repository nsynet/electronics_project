#include <lpc213x.h>
#include <mydef.h>


//
//#define PCD8544_LIGHT_ON clr_bit(PCD8544_OUTPORT, PCD8544_LIGHT)
//#define PCD8544_LIGHT_OFF set_bit(PCD8544_OUTPORT, PCD8544_LIGHT)

#define PCD8544_CMD 0
#define PCD8544_DATA 1

extern void pcd8544_write_byte(uint8_t dc, uint8_t _data);

extern void pcd8544_write(uint8_t *src, uint32_t len);
