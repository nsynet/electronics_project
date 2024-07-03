/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2006 Embedded Artists AB
 *
 * Description:
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <LPC2xxx.H>
#include "general.h"
#include "osapi.h"
#include "uip.h"
#include "uip_arp.h"
#include "ethapi.h"
#include <printf_P.h>
#include <ea_init.h>


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define UIP_STACK 1024
#define UIP_PRIO  2
static tU8 uipStack[UIP_STACK];

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])


/******************************************************************************
 * Local variables
 *****************************************************************************/
tU32 numMs = 0;
static tU8  mac[6] = {0x00, 0xea, 0x98, 0x01, 0xea, 0x55};


/******************************************************************************
 * Global variables
 *****************************************************************************/

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * P R O C E S S E S                                                          *
 *****************************************************************************/

/*****************************************************************************
 *
 * Description:
 *    Output function for ethernet driver. 
 *
 ****************************************************************************/
static void
send_to_eth(void)
{
  if(uip_len < 40 + UIP_LLH_LEN)
    ethIf_send(uip_buf, uip_len, NULL, 0);
  else
    ethIf_send(uip_buf, 40+UIP_LLH_LEN, uip_appdata, uip_len-(40+UIP_LLH_LEN));
}


/*****************************************************************************
 *
 * Description:
 *    Prints log-messages.
 *
 ****************************************************************************/
void
uip_log(char *m)
{
  printf("uIP log message: %s\n", m);
}


/*****************************************************************************
 *
 * Description:
 *    UIP process. 
 *
 ****************************************************************************/
static void
uipProcess(void* pArg)
{
  tU8 i;
  tU8 arptimer;
  struct uip_eth_addr ethAddr;

  ethAddr.addr[0] = mac[0];
  ethAddr.addr[1] = mac[1];
  ethAddr.addr[2] = mac[2];
  ethAddr.addr[3] = mac[3];
  ethAddr.addr[4] = mac[4];
  ethAddr.addr[5] = mac[5];

  uip_setethaddr(ethAddr);

  uip_arp_init();

  /* Initialize the device driver. */ 
  ethIf_init(mac);

  /* Initialize the uIP TCP/IP stack. */
  uip_init();

  /* Initialize the HTTP server. */
  httpd_init();

  arptimer = 0;

  while (1)
  {
    /* Let the tapdev network device driver read an entire IP packet
       into the uip_buf. If it must wait for more than 0.5 seconds, it
       will return with the return value 0. If so, we know that it is
       time to call upon the uip_periodic(). Otherwise, the tapdev has
       received an IP packet that is to be processed by uIP. */
    uip_len = ethIf_poll(uip_buf, UIP_BUFSIZE);

    if (uip_len == 0)
    {
      for (i = 0; i < UIP_CONNS; i++)
      {
        uip_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
          send_to_eth();
        }
      }

#if UIP_UDP
      for (i = 0; i < UIP_UDP_CONNS; i++)
      {
        uip_udp_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
          send_to_eth();
        }
      }
#endif /* UIP_UDP */

      /* Call the ARP timer function every 10 seconds. */
      if (++arptimer == 20)
      {
        uip_arp_timer();
        arptimer = 0;
      }
    }
    else
    {
      if (BUF->type == htons(UIP_ETHTYPE_IP))
      {
        uip_arp_ipin();
        uip_input();
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
          send_to_eth();
        }
      }
      else if (BUF->type == htons(UIP_ETHTYPE_ARP))
      {
        uip_arp_arpin();
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */    
        if (uip_len > 0)
        {
          send_to_eth();
        }
      }
    }
  }
}


/******************************************************************************
 *                        T I M E R  M A N A G E M E N T                      *
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Get clock
 *
 * Returns:
 *    The time
 *
 *****************************************************************************/
tU32
clockMs(void)
{
  return numMs;
}


/******************************************************************************
 *
 * Description:
 *    This function is called by the OS at 100 Hz.
 *
 *****************************************************************************/
void
appTick(tU32 ms)
{
  numMs += ms;
}


/******************************************************************************
 *                            I N I T I A L I Z I N G                         *
 *****************************************************************************/

/*****************************************************************************
 *
 * Description:
 *    Init process
 *
 ****************************************************************************/
static void
initProcess(void* pArg)
{
  tU8  pid    = 0;
  tU8  error  = 0;

  printf("\n\n\n\n*****************************************************************************");
  printf("\n*                                                                           *");
  printf("\n* Welcome to the 10M Ethernet expansion board test program...               *");
  printf("\n* that contains an implementation of the uIP TCP/IP stack.                  *");
  printf("\n* It is possible to PING the board as well as 'surf' to the board           *");
  printf("\n* via an Internet browser.                                                  *");
  printf("\n* The IP-address is: 192.168.1.230 (subnet mask: 255.255.255.0)             *");
  printf("\n* (make sure you are on the same subnet).                                   *");
  printf("\n*                                                                           *");
  printf("\n*****************************************************************************");

  /* uip process */
  osCreateProcess(&uipProcess, uipStack, UIP_STACK, &pid, UIP_PRIO, NULL, &error);
  osStartProcess(pid, &error);

  osDeleteProcess();
}


/*****************************************************************************
 *
 * Description:
 *    Main method 
 *
 * Returns:
 *    
 *
 ****************************************************************************/
int
main(void)
{
  static tU8 initStk[500];
  tU8 pid = 0;
  tU8 error = 0;

  eaInit();
  osInit();

  osCreateProcess(initProcess, initStk, 500, &pid, 0, NULL, &error);
  osStartProcess(pid, &error);

  osStart();

  return 0;
}

