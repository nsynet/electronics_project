
#include <stdio.h>
#include "uip_includes.h"
//#include <mydef.h>
#include "sys_clock.h"
#include "../drivers/pcd8544/pcd8544.h"

#include "../drivers/rtc/rtc.h"

//#pragma import(__use_no_semihosting_swi)

void uip_appcall(void)
{
//hello_world_appcall();
}
 

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
	

struct uip_eth_addr ethaddr;
uint8_t my_mac[6] = {'A', 'N', 'C', 'H', 'E', 'Y'};

int main(void)
{	  
	int i;
	uip_ipaddr_t ipaddr;
	struct timer periodic_timer, arp_timer;
    char buf[20];

	uint32_t errcount=0;

/* 
 * 定时器初始化 
*/ 
    pcd8544_init();

    sys_clock_init(); //初始化时钟
	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);

    //sprintf(buf,"%u %u", periodic_timer.start, periodic_timer.interval);
    //pcd8544_write_string(0,2,buf,0);
 
    
    //rtc_init();
/* 
 * 网络设备初始化 
*/ 
	tapdev_init(my_mac);

    pcd8544_write_string(0, 0, "UIP1.0..",0);
/* 
 * UIP初始化 
*/ 
	uip_init();
  
/* 
 * ARP初始化 
*/ 
	uip_arp_init();
	
/* 
 * 本机IP初始化 
*/ 

    
    uip_ipaddr(ipaddr, 192,168,1,200);
    uip_sethostaddr(ipaddr);
    uip_ipaddr(ipaddr, 192,168,1,1);
    uip_setdraddr(ipaddr);
    uip_ipaddr(ipaddr, 255,255,255,0);
    uip_setnetmask(ipaddr);

	

/* 
 * 本机MAC初始化 
*/ 
	for (i = 0; i < 6; i++) ethaddr.addr[i] = my_mac[i];
  	uip_setethaddr(ethaddr);//UIP MAC初始化

    telnetd_init();

/* 
 * 循环
*/ 


	while(1) 
	{
		uip_len = tapdev_read();

            //        rtc_read_time(&t);
            //sprintf(buf, "%02u:%02u:%02u", t.hour, t.minute, t.second);
            //pcd8544_write_string(0, 5,buf,0);
		
		if (uip_len > 0) 
		{
            //pcd8544_write_string(0, 1, "RCV PAT",0);
		   if(BUF->type == htons(UIP_ETHTYPE_IP)) 
		   {
               //pcd8544_write_string(0, 2, "pt IP",0);
				uip_arp_ipin();
				uip_input();
				if(uip_len > 0) 
				{
                    //pcd8544_write_string(0,3,"send req",0);
					uip_arp_out();
					tapdev_send();
				}
			} 
			else 
			if(BUF->type == htons(UIP_ETHTYPE_ARP)) 
			{
                //pcd8544_write_string(0, 2, "pt ARP",0);
				uip_arp_arpin();
				if(uip_len > 0) 
				{
                    //pcd8544_write_string(0,3,"send req",0);
					tapdev_send();
				}
			}
			else
			{
                sprintf(buf, "error %u", ++errcount);
				pcd8544_write_string(0, 2, buf,0);
			}

		} 
		else 
		if(timer_expired(&periodic_timer)) 
		{

			timer_reset(&periodic_timer);
			for(i = 0; i < UIP_CONNS; i++) 
			{
				uip_periodic(i);
				if(uip_len > 0) 
				{
					uip_arp_out();
                  
                    pcd8544_write_string(0, 4, "ARP SEND",0);
					tapdev_send();
				}
			}

			#if UIP_UDP
			for(i = 0; i < UIP_UDP_CONNS; i++) 
			{
				uip_udp_periodic(i);
				if(uip_len > 0) 
				{
					uip_arp_out();
					tapdev_send();
				}
			}
			#endif /* UIP_UDP */
      
			/* Call the ARP timer function every 10 seconds. */
			if(timer_expired(&arp_timer)) 
			{
				timer_reset(&arp_timer);
				uip_arp_timer();
			}
		}
	}
//	return 0;
}

/*
;*****************************************************************************************************
;* 函数名称 : uip_log
;* 描    述 : 辅助函数
;* 输　	 入 : 无
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
void uip_log(char *m)
{
	//RS232_Printf("uIP log message: %s\n", m);
}


