#include "p24HJ64GP504.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"
#include <string.h>

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

struct uip_udp_conn *myudp_conn;

/****************************************TCP************************************************/


void example1_init(void) 
{
	uip_listen(HTONS(1234));
}


void example1_app(void) 
{
	if(uip_newdata() || uip_rexmit()) 
	{
		uip_send("I can fly!\n", 11);
	}
}

struct example2_state 
{
	enum {WELCOME_SENT=0x0001, WELCOME_ACKED=0x0002} state;
};

void example2_init(void) 
{
	uip_listen(HTONS(2345));
}


void example2_app(void)
{
	struct example2_state *s;

	s = (struct example2_state *)(uip_conn->appstate);
	if(uip_connected()) 
	{
		s->state = WELCOME_SENT;
		uip_send("Welcome!\n", 9);
		return;
	}
	if(uip_acked() && s->state == WELCOME_SENT)
	{
		s->state = WELCOME_ACKED;
	}
	if(uip_newdata()) 
	{
		uip_send("ok\n", 3);
	}
	if(uip_rexmit()) 
	{
		switch(s->state) 
		{
			case WELCOME_SENT:
								uip_send("Welcome!\n", 9);
								break;
			case WELCOME_ACKED:
								uip_send("ok\n", 3);
								break;
		}
	}
}


void uip_appinit(void) 
{
	example1_init();
	example2_init();
}

void uip_appcall(void) 
{
	switch(uip_conn->lport) 
	{
		case HTONS(1234):
				example1_app();
				break;
		case HTONS(2345):
				example2_app();
				break;
	}
}
/***************************************ENDTCP**********************************************/	
/*
;*****************************************************************************************************
;* 函数名称 : main
;* 描    述 : 主函数
;* 输　	 入 : 无
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"

#include "timer.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

/*---------------------------------------------------------------------------*/
int
main(void)
{
	int i;
	uip_ipaddr_t ipaddr;
	struct timer periodic_timer, arp_timer;
	
	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);
	
	tapdev_init();
	
	uip_init();
	
	uip_ipaddr(ipaddr, 192,168,0,80);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192,168,0,1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);
	
	uip_appinit();
	
	while(1) 
	{
		uip_len = tapdev_read();
		if(uip_len > 0) 
		{
			if(BUF->type == htons(UIP_ETHTYPE_IP)) 
			{
				uip_arp_ipin();
				uip_input();
				/* If the above function invocation resulted in data that
				should be sent out on the network, the global variable
				uip_len is set to a value > 0. */
				if(uip_len > 0)
				{
					uip_arp_out();
					tapdev_send();
				}
			} 
			else if(BUF->type == htons(UIP_ETHTYPE_ARP)) 
			{
				uip_arp_arpin();
				/* If the above function invocation resulted in data that
				should be sent out on the network, the global variable
				uip_len is set to a value > 0. */
				if(uip_len > 0) 
				{
					tapdev_send();
				}
			}
		
		} 
		else if(timer_expired(&periodic_timer)) 
		{
			timer_reset(&periodic_timer);
			for(i = 0; i < UIP_CONNS; i++) 
			{
				uip_periodic(i);
				/* If the above function invocation resulted in data that
				should be sent out on the network, the global variable
				uip_len is set to a value > 0. */
				if(uip_len > 0) 
				{
					uip_arp_out();
					tapdev_send();
				}
			}
			
			#if UIP_UDP
			for(i = 0; i < UIP_UDP_CONNS; i++) 
			{
				uip_udp_periodic(i);
				/* If the above function invocation resulted in data that
				should be sent out on the network, the global variable
				uip_len is set to a value > 0. */
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
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
void uip_log(char *m)
{
	//RS232_Printf("uIP log message: %s\n", m);
}

/*
void resolv_found(char *name, u16_t *ipaddr)
{
	if(ipaddr == NULL) 
	{
		RS232_Printf("Host '%s' not found.\n", name);
	} 
	else 
	{
		RS232_Printf("Found name '%s' = %d.%d.%d.%d\n", name,
		htons(ipaddr[0]) >> 8,
		htons(ipaddr[0]) & 0xff,
		htons(ipaddr[1]) >> 8,
		htons(ipaddr[1]) & 0xff);
	}
}

#ifdef __DHCPC_H__
void dhcpc_configured(const struct dhcpc_state *s)
{
	uip_sethostaddr(s->ipaddr);
	uip_setnetmask(s->netmask);
	uip_setdraddr(s->default_router);
	resolv_conf(s->dnsaddr);
}
#endif

void smtp_done(unsigned char code)
{
	RS232_Printf("SMTP done with code %d\n", code);
}

void webclient_closed(void)
{
	RS232_Printf("Webclient: connection closed\n");
}

void webclient_aborted(void)
{
	RS232_Printf("Webclient: connection aborted\n");
}

void webclient_timedout(void)
{
	RS232_Printf("Webclient: connection timed out\n");
}

void webclient_connected(void)
{
	RS232_Printf("Webclient: connected, waiting for data...\n");
}

void webclient_datahandler(char *data, u16_t len)
{
	RS232_Printf("Webclient: got %d bytes of data.\n", len);
}
*/
/*
;*****************************************************************************************************
;*                            			End Of File
;*****************************************************************************************************
;*/

