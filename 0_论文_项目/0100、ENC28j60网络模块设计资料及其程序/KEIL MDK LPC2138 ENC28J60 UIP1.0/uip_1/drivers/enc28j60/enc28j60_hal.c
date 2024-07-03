#include <lpc213x.h>
#include <mydef.h>
#include <lpc_reg.h>

#include "enc28j60_hal.h"
#include "enc28j60.h"

uint8_t enc28j60_read_op(uint8_t op, uint8_t address)
{
        ENC28J60_ENABLED;
        // issue read command
        S0SPDR = op | (address & ADDR_MASK);
        wait_spi_finish();
        // read data
        S0SPDR = 0x00;
        wait_spi_finish();
        // do dummy read if needed (for mac and mii, see datasheet page 29)
        if(address & 0x80)
        {
                S0SPDR = 0x00;
                wait_spi_finish();
        }
        // release CS
        
        ENC28J60_DISABLED;
        return(S0SPDR);
}

void enc28j60_write_op(uint8_t op, uint8_t address, uint8_t __data)
{
        ENC28J60_ENABLED;
        // issue write command
        S0SPDR = op | (address & ADDR_MASK);
        wait_spi_finish();
        // write data
        S0SPDR = __data;
        wait_spi_finish();
        
        ENC28J60_DISABLED;
}

void enc28j60_read_buffer(uint8_t *dest, uint32_t len)
{
        ENC28J60_ENABLED;
        // issue read command
        S0SPDR = ENC28J60_READ_BUF_MEM;
        wait_spi_finish();
        while(len)
        {
                len--;
                // read data
                S0SPDR = 0x00;
                wait_spi_finish();
                *dest = S0SPDR;
                dest++;
        }
        //*data='\0';
        
        ENC28J60_DISABLED;
}

void enc28j60_write_buffer(uint8_t* data, uint32_t len)
{
        ENC28J60_ENABLED;
        // issue write command
        S0SPDR = ENC28J60_WRITE_BUF_MEM;
        wait_spi_finish();
        while(len)
        {
                len--;
                // write data
                S0SPDR = *data;
                data++;
                wait_spi_finish();
        }
        ENC28J60_DISABLED;
}
