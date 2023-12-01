#ifndef __KBD_H
#define __KBD_H

#include "common.h"

#define KEY_MENU    0x01
#define KEY_UP      0x02
#define KEY_DOWN    0x04
#define KEY_ENTER   0x08

void KBD_IO_Init(void);
u8 KBD_GetKey(void);

#endif
