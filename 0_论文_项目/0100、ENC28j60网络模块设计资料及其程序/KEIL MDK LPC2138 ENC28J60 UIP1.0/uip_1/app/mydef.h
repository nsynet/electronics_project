/***************************************************************
*ARM»ù±¾²Ù×÷*
*(c)Ancheel*
*2008-9-6
************************************************************/

#ifndef __MYDEF_H__
#define __MYDEF_H__

#include <stdint.h>


#define TRUE   1
#define FALSE  0

#ifndef __stdint_h
//#define __stdint_h
typedef unsigned char     uint8;
typedef signed char         int8;
typedef unsigned short    uint16;
typedef signed short        int16;
typedef unsigned int        uint32;
typedef signed int            int32;
typedef unsigned long     uint64;
typedef signed long         int64;

typedef unsigned char     uint8_t;
typedef signed char         int8_t;
typedef unsigned short    uint16_t;
typedef signed short        int16_t;
typedef unsigned int        uint32_t;
typedef signed int            int32_t;
typedef unsigned long     uint64_t;
typedef signed long         int64_t; 

#endif

#define _BV(_b)   (1<<(_b))

#define set_bit(_r, _b)  _r |= _BV(_b)
#define clr_bit(_r, _b)   _r &= ~_BV(_b)

#define bit_is_set(_r,_b)   (_r & _BV(_b))
#define bit_is_clr(_r,_b) (!(_r & _BV(_b)))

#define loop_until_bit_is_set(_r,_b) while (bit_is_clr(_r, _b))
#define loop_until_bit_is_clr(_r,_b)  while (bit_is_set(_r, _b)) 




#endif
