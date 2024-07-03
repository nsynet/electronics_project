#include <mydef.h>
#ifndef __PCD8544_H__
#define __PCD8544_H__

#define PCD8544_BACKLIGHT_ON 1
#define PCD8544_BACKLIGHT_OFF 0

#define PCD8544_MAX_Y 83

extern void pcd8544_init(void);
extern void pcd8544_backlight(uint8_t sw);
extern void pcd8544_cls(void);

extern void pcd8544_cls_line(uint8_t l, uint8_t startp, uint8_t endp);

extern void pcd8544_write_string(uint8_t x, uint8_t y, char *str, uint8_t reverse);

//extern void pcd8544_write_string_p(uint8_t x, uint8_t y, const char *str, uint8_t reverse);

#endif
