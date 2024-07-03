#include <lpc213x.h>
#include <mydef.h>
#include <lpc_reg.h>

//#define ENC28J60_CONTROL_PORT   PORTB
#define ENC28J60_CONTROL_DDR    IO0DIR
#define ENC28J60_RESET  24
#define ENC28J60_CS     7
// set CS to 0 = active
#define ENC28J60_ENABLED       set_bit(IO0CLR, ENC28J60_CS);
// set CS to 1 = passive
#define ENC28J60_DISABLED       set_bit(IO0SET, ENC28J60_CS);
//

#define wait_spi_finish() loop_until_bit_is_set(S0SPSR, SPIF)

extern uint8_t enc28j60_read_op(uint8_t op, uint8_t address);
extern void enc28j60_write_op(uint8_t op, uint8_t address, uint8_t __data);

extern void enc28j60_read_buffer(uint8_t *dest, uint32_t len);
extern void enc28j60_write_buffer(uint8_t* data, uint32_t len);

#define clr_ENC28J60_RESET set_bit(IO1CLR, ENC28J60_RESET)
#define set_ENC28J60_RESET set_bit(IO1SET, ENC28J60_RESET)

#define enc28j60_IO_init() \
{\
    set_bit(IO0DIR, ENC28J60_CS);\
    set_bit(IO1DIR, ENC28J60_RESET);\
}

