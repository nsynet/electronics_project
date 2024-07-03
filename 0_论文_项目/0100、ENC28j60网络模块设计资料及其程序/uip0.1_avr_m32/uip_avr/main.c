/*
;*****************************************************************************************************
;*    							Copyright (c) 2006 by JiaoJinXing. 
;*									All rights reserved.
;*
;*---- �ļ���Ϣ --------------------------------------------------------------------------------------
;*��   ��   �� : main.c
;*��   ��   �� : ������
;*��        �� : Ӧ���������ļ�
;*
;*---- ��ʷ�汾��Ϣ ----------------------------------------------------------------------------------
;* �ա�		�� : 2008�� 6 �� 27 ��
;* ��	��	�� : ������
;* �衡		�� : �����汾 V1.0.0
;*
;*---- ��ϵ��ʽ --------------------------------------------------------------------------------------
;* Email 	   : wyoujtg@163.com
;* Web 		   :
;*		    
;*****************************************************************************************************
;*/

	#include "Config.h"

/*
;*****************************************************************************************************
;* �������� : Target_Init
;* ��    �� : Ŀ����ʼ��
;* �䡡	 �� : ��
;*        
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void Target_Init(void)
{
	RS232_Init(9600);

	RS232_Printf("hello world!\n");	
	
	UIP_Clock_Init(); 
	
    sei();
}


void uip_appcall(void)
{

}
 

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
	
/*
;*****************************************************************************************************
;* �������� : main
;* ��    �� : ������
;* �䡡	 �� : ��
;*        
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
#if UIP_FIXEDETHADDR == 0
struct uip_eth_addr ethaddr;
char avr_mac[] = {'A', 'V', 'R', 'N', 'E', 'T'};
#endif

int main(void)
{	  
	int i;
	uip_ipaddr_t ipaddr;
	struct timer periodic_timer, arp_timer;

/* 
 * оƬ��ʼ�� 
*/ 
	Target_Init();
	
/* 
 * ��ʱ����ʼ�� 
*/ 
	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);
 
/* 
 * �����豸��ʼ�� 
*/ 
	tapdev_init();

/* 
 * UIP��ʼ�� 
*/ 
	uip_init();
  
/* 
 * ARP��ʼ�� 
*/ 
	uip_arp_init();
	
/* 
 * ����IP��ʼ�� 
*/ 
#if UIP_FIXEDADDR == 0
	uip_ipaddr(ipaddr, UIP_IPADDR0,UIP_IPADDR1,UIP_IPADDR2,UIP_IPADDR3);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, UIP_DRIPADDR0,UIP_DRIPADDR1,UIP_DRIPADDR2,UIP_DRIPADDR3);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, UIP_NETMASK0,UIP_NETMASK1,UIP_NETMASK2,UIP_NETMASK3);
	uip_setnetmask(ipaddr);
#endif 

/* 
 * ����MAC��ʼ�� 
*/ 
#if UIP_FIXEDETHADDR == 0
	for (i = 0; i < 6; i++)
  		ethaddr.addr[i] = avr_mac[i];
  	uip_setethaddr(ethaddr);
#endif

telnetd_init();

/* 
 * ѭ��
*/ 
	while(1) 
	{
		uip_len = tapdev_read();
		
		if (uip_len > 0) 
		{
		   if(BUF->type == htons(UIP_ETHTYPE_IP)) 
		   {
				uip_arp_ipin();
				uip_input();
				if(uip_len > 0) 
				{
					uip_arp_out();
					tapdev_send();
				}
			} 
			else 
			if(BUF->type == htons(UIP_ETHTYPE_ARP)) 
			{
				uip_arp_arpin();
				if(uip_len > 0) 
				{
					tapdev_send();
				}
			}
			else
			{
				RS232_Printf("packet type error\n");
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
	return 0;
}

/*
;*****************************************************************************************************
;* �������� : uip_log
;* ��    �� : ��������
;* �䡡	 �� : ��
;*        
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void uip_log(char *m)
{
	RS232_Printf("uIP log message: %s\n", m);
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

