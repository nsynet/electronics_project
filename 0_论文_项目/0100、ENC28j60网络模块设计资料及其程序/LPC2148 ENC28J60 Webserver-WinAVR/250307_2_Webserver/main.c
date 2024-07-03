/*
		Andreas Weschenfelder				24.03.07
		
viele Fehler beim kompilieren, aber es geht!!!

dynamische HP:
	- Variablen befinden sich in dyn.js
	- Veraenderung durch Austausch im Array
	- nach Ausführen von make_fs_data muss das const entfernt werden!!!
	- Buttons werden automatisch veraendert.
	- Temperatur-Sensor eingefuegt				24.03.07
	- Temperatur wird automatisch veraendert.	24.03.07
	- Datenempfang realisiert					25.03.07
		->link: /value_xxx xxx=wert, umleitung auf index.html
*/


//WinArm-Inlcudes
#include "Common_WinARM/inc/LPC214x.h"
#include "Common_WinARM/inc/type.h"
#include "Common_WinARM/inc/irq.h"
//#include "Common_WinARM/inc/timer.h"
#include "Common_WinARM/inc/interrupt_utils.h"
#include "Common_WinARM/inc/swi.h"
#include "Common_WinARM/inc/target.h"

#include <stdlib.h>
#include <string.h>
#include "LPC_INC\ITOA.c"

#include "Common_WinARM/src/irq.c"
//#include "Common_WinARM/src/interrupt_utils.c"
#include "Common_WinARM/src/target.c"
//#include "Common_WinARM/src/timer.c"



//Olimex-Board-Defines
#include "LPC_OLIMEX_BOARD\olimex_lpc2148.h"		//Board-incs
#include "LPC_OLIMEX_BOARD\typedefs_2.h"			//U16 etc.
#include "LPC_OLIMEX_BOARD\my_delay.h"				//warteschleifen
#include "LPC_OLIMEX_BOARD\my_delay.c"

//System-Clock / PLL -> entstehende CLK für MAIN_CLK definieren!
#include "LPC_SYSTEM_CLOCK\system_clock.c"


//Uart 0+1
#include "LPC_UART0\uart.c"
#define BAUD_RATE	115200
#define MAIN_CLK	12000000*5

//SPI0
#include "LPC_SPI/spi_polling.c"

//ENC28J60
#include "LPC_ENC28J60\ENC28J60.c"
//Webserver
char get_value(char *s);
#include "LPC_webserver\my_webserver_incs.h"
//Webserver-Defines
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
#define BUFFER_SIZE 1500

//Dyn_js
#include "LPC_webserver_dyn/webserver_dyn.h"

//I2C0
//#include "LPC_I2C/I2C.c"
//Temperatur-Sensor
#include "LPC_DS1621/DS1621.c"
		

U32 timer_value, periodic_timer_value;

char get_value(char *s)
{
if ((s[1]=='v')&&(s[2]=='a')&&(s[3]=='l')&&(s[4]=='u')&&(s[5]=='e'))
	{
	//Value wird eingelesen. Max=000...999
	int value = (s[7]-48)*100 + (s[8]-48)*10 + (s[9]-48);		//In Value steht der Gesendete Wert
	UART1Send_Int(value);
	UART1Send( "\n\r", strlen("\n\r") );
	//HP manipulieren...
	//Emfpangen
	//0=LED1 ausschalten
	//1=LED1 anschalten
	//2=LED2 ausschalten
	//3=LED2 anschalten
	//in HP
	//0=beide LEDs aus
	//1=LED1 an, LED2 aus
	//2=LED2 an, LED2 aus
	//3=LED1,LED2 an
	if (value==0)
		{LED_IOCLR = (1<<LED1);}
	if (value==1)
		{LED_IOSET = (1<<LED1);}
	if (value==2)
		{LED_IOCLR = (1<<LED2);}
	if (value==3)
		{LED_IOSET = (1<<LED2);}
	
	char tmp_help=0;
	if (LED_IOPIN&(1<<LED1))
		{tmp_help = tmp_help + 1;}
	if (LED_IOPIN&(1<<LED2))
		{tmp_help = tmp_help + 2;}
	data_dyn_js[DYN_JS_LED] = tmp_help+0x30;
	return 1;
	}
return 0;
}


/*************************************************************************
	main
	====
**************************************************************************/
int main(void)
{
char tmp;

	//************************************************************************************************
	// PLL and MAM
	//************************************************************************************************
	Init_PLL_USB();		//60 Mhz
	
	//************************************************************************************************
	//Interrupt-Controller initialisieren - Vor Interruptabhaengigen funktionen
	//************************************************************************************************
	init_VIC();
	
	//************************************************************************************************
	//UART - Init
	//************************************************************************************************
	//Uart0-Init
	UART0Init(BAUD_RATE,MAIN_CLK);	/* baud rate setting */
	UART1Init(BAUD_RATE,MAIN_CLK);	/* baud rate setting */
	
	U1IER = IER_THRE | IER_RLS;			/* Disable RBR */
	UART1Send( "\n\r** UART1 **", strlen("\n\r** UART1 **") );
	//UART1Send_Int(10);
	UART1Count = 0;
	U1IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	
	U0IER = IER_THRE | IER_RLS;			/* Disable RBR */
	UART0Send( "\n\r**---------------------------------------------------------------**", strlen("\n\r**-----------------------------------------------------------------") );
	UART0Send( "\n\r**                        Webserver_Start                        **", strlen("\n\r**                        Webserver_Start                        **") );
	UART0Send( "\n\r**---------------------------------------------------------------**\n\r", strlen("\n\r**-----------------------------------------------------------------\n\r") );
	//UART0Send_Int(10);
	UART0Count = 0;
	U0IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	
	//************************************************************************************************
	//Temperatur-Sensor init
	//************************************************************************************************
	init_ds1621();

	
	//************************************************************************************************
	//Weberver - Initialisieren
	//************************************************************************************************
	//U32 a;
	
    uip_ipaddr_t ipaddr;
    
    //set Mac Addr.
    //(0xc0,0xa8,0x00,0x40,0x03,0x11); 
    uip_ethaddr.addr[0] = 0xc0;
    uip_ethaddr.addr[1] = 0xa8;
    uip_ethaddr.addr[2] = 0x00;
    uip_ethaddr.addr[3] = 0x40;
    uip_ethaddr.addr[4] = 0x03;
    uip_ethaddr.addr[5] = 0x11;
	
 	/*ENC Initialisieren*/
    for(int a=0;a<1000000;a++);
    for(int a=0;a<1000000;a++);
	enc28j60Init();
   	nicSetMacAddress(uip_ethaddr.addr);

    uip_init();


    //uip_ipaddr(ipaddr, 192,168,1,199);
	uip_ipaddr(ipaddr, 169,254,101,200);	
    uip_sethostaddr(ipaddr);
    uip_ipaddr(ipaddr, 192,168,1,198);
    uip_setdraddr(ipaddr);
    //uip_ipaddr(ipaddr, 255,255,255,0);
    uip_ipaddr(ipaddr, 255,255,0,0);
	uip_setnetmask(ipaddr);	
	
    uip_arp_init();
	httpd_init();
	enc28j60PhyWrite(0x14,0x0AA0); //0420 0AA0		//LED_A und LED_B blinkend
	//enc28j60PhyWrite(0x14,0x0740); //LEDB=linkstatus - LEDA=transmit an receive
	//enc28j60PhyWrite(0x14,0x0350); //LEDB=duplex status - LEDA=Collision
	//enc28j60PhyWrite(0x14,0x04A0); //linkstatus+blink
	enc28j60PhyWrite(0x14,0x0120); 	//LEDB=rx - LEDA=tx


	//Timer funktioniert
    init_timer();
    timer_value = SetDelay10ms(10000);
    periodic_timer_value    = SetDelay10ms(2);
	
	//Debug-LED - Timer alle 10ms
	//bzw. Benutzer-Interface
	LED_IODIR |= (1<<LED1)|(1<<LED2);
	LED_IOCLR = (1<<LED2);					//Status-Variable in dyn.js beachten...
	LED_IOSET = (1<<LED1);
	tmp=0;
	if (LED_IOPIN&(1<<LED1))
		{tmp = tmp + 1;}
	if (LED_IOPIN&(1<<LED2))
		{tmp = tmp + 2;}
	data_dyn_js[DYN_JS_LED] = tmp+0x30;
	B_IODIR &= ~((1<<B1)|(1<<B2));
	

	//Main-Loop
	while (1) 
		{


//**************************************************************************************************************
//Homepage manipulieren
//**************************************************************************************************************
tmp=0x30;
//HP manipulieren
//Buttons
if (B_IOPIN & (1<<B1))
	{tmp=tmp|0x01;}
if (B_IOPIN & (1<<B2))
	{tmp=tmp|0x02;}
data_dyn_js[DYN_JS_BUTTON] = tmp;

//Temperatur-Sensor - Annahme: Temperatur: -99...+99C
//read_ds1621();		//-> nicht an dieser Stelle, dauert zu lange -> server haengt sich auf
//mindestens 4 Werte schreiben!!!
//data_dyn_js[105...]
if ((temperatur_high&0x7F)>9)		//MSB ignorieren
	{
	if (temperatur_high&0x80)
		{data_dyn_js[DYN_JS_TEMPERATUR_START]=ASCII_MINUS;}
		else
		{data_dyn_js[DYN_JS_TEMPERATUR_START]=ASCII_PLUS;}
	
	data_dyn_js[DYN_JS_TEMPERATUR_START+1] = ((char)temperatur_high/10) + 0x30;
	data_dyn_js[DYN_JS_TEMPERATUR_START+2] = ((char)temperatur_high%10) + 0x30;
	data_dyn_js[DYN_JS_TEMPERATUR_START+3] = ASCII_PKT;
	
	if (temperatur_low & 0x80)
		{data_dyn_js[DYN_JS_TEMPERATUR_START+4] = 0x35;}
		else
		{data_dyn_js[DYN_JS_TEMPERATUR_START+4] = 0x30;}

	}
	else
	{
	if (temperatur_high&0x80)
		{data_dyn_js[DYN_JS_TEMPERATUR_START]=ASCII_MINUS;}
		else
		{data_dyn_js[DYN_JS_TEMPERATUR_START]=ASCII_PLUS;}
	
	data_dyn_js[DYN_JS_TEMPERATUR_START+1] = temperatur_high + 0x30;
	data_dyn_js[DYN_JS_TEMPERATUR_START+2] = ASCII_PKT;

	if (temperatur_low & 0x80)
		{data_dyn_js[DYN_JS_TEMPERATUR_START+3] = 0x35;}
		else
		{data_dyn_js[DYN_JS_TEMPERATUR_START+3] = 0x30;}
		
	data_dyn_js[DYN_JS_TEMPERATUR_START+4] = 0x30;
	}
	
//**************************************************************************************************************
//Webserver-Loop
//**************************************************************************************************************

//Puffer des ENC einlesen
uip_len = enc28j60PacketReceive(BUFFER_SIZE, uip_buf);

/*
if(uip_len > 0) 
{
UART0Send( "\n\r uIP_Len: ", strlen("\n\r uIP_Len: ") );
UART0Send_Int(uip_len);
}
*/
 
if(uip_len > 0) 
	{
    if(BUF->type == htons(UIP_ETHTYPE_IP)) 		
		{
		//UART0Send( " UIP_ETHTYPE_IP", strlen(" UIP_ETHTYPE_IP") );	
		uip_arp_ipin();
		uip_input();
		/* If the above function invocation resulted in data that
		   should be sent out on the network, the global variable
		   uip_len is set to a value > 0. */
		if(uip_len > 0) 
			{
			uip_arp_out();
			//UART0Send( " UIP_ETHTYPE_IP-Send", strlen(" UIP_ETHTYPE_IP-Send") );	
			enc28j60PacketSend(uip_len, uip_buf);
			}
		} 
		else 
		{
		if(BUF->type == htons(UIP_ETHTYPE_ARP)) 		
			{
			//UART0Send( " UIP_ETHTYPE_ARP", strlen(" UIP_ETHTYPE_ARP") );
			uip_arp_arpin();
			/* If the above function invocation resulted in data that
			   should be sent out on the network, the global variable
			   uip_len is set to a value > 0. */
			//UART0Send_Int(uip_len);->uip-len wird 0 gesetzt, keine antwort
			if(uip_len > 0) 
				{
				//UART0Send( " UIP_ETHTYPE_ARP-Send", strlen(" UIP_ETHTYPE_ARP-Send") );
				enc28j60PacketSend(uip_len, uip_buf);}
			}
			else
			{
			UART0Send( " Packet nicht erkannt!", strlen(" Packet nicht erkannt!") );
			}
		}
    } 
	else
	//Es ist kein uIP-Packet da...
	//Periodischen Timer abarbeiten
		
	if(CheckDelay10ms(periodic_timer_value)) 
		{
		/*
		//Debug-Ausgabe, Server läuft...
		if (LED_IOPIN&(1<<LED1))
			{LED_IOCLR = (1<<LED1);}
			else
			{LED_IOSET = (1<<LED1);}
		*/
		
		//Temperatur bitte hier stehen lassen!!!
		read_ds1621();		//temperatur_high und temperatur_low

		periodic_timer_value = SetDelay10ms(50);
		
		for(int i_tmp = 0; i_tmp < UIP_CONNS; i_tmp++) 
			{
			uip_periodic(i_tmp);
			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if(uip_len > 0) 
				{
				uip_arp_out();
				enc28j60PacketSend(uip_len, uip_buf);
				}
			}

		#if UIP_UDP
			for(int i_tmp = 0; i_tmp < UIP_UDP_CONNS; i_tmp++) 
				{
				uip_udp_periodic(i_tmp);
				/* If the above function invocation resulted in data that
				   should be sent out on the network, the global variable
				   uip_len is set to a value > 0. */
				if(uip_len > 0) 
					{
					uip_arp_out();
					enc28j60PacketSend(uip_len, uip_buf);
					}
				}
		#endif /* UIP_UDP */
  
		/* Call the ARP timer function every 10 seconds. */
		if(CheckDelay10ms(timer_value)) 
			{
			timer_value = SetDelay10ms(10000);
			uip_arp_timer();
			}
		}
//**************************************************************************************************************




	
		}
	
	return 0;
}

