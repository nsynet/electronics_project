#include <mydef.h>
#include <lpc_reg.h>
#include <stdio.h>

#include "../drivers/pcd8544/pcd8544.h"
#include "../drivers/rtc/rtc.h"

int main(void)
{
    rtc_time_t t;
    char buf[20]= " 12455";

    pcd8544_init();
    rtc_init();

    t.year = 2009;
	t.month = 5;
	t.day = 17;

    rtc_set_time(&t);

    pcd8544_write_string(0, 0, "UIP 1.0..", 0);

    while(1)
    {
        rtc_read_time(&t);
        sprintf(buf, "%u-%02u-%02u", t.year, t.month, t.day);
        pcd8544_write_string(0, 2, buf , 0);
        sprintf(buf, "%02u:%02u:%02u", t.hour, t.minute, t.second);
        pcd8544_write_string(0, 3, buf, 0);
    }

}
