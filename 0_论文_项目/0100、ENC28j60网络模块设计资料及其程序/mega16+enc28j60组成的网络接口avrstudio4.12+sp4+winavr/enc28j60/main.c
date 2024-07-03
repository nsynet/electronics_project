#include "board.h"
#include "j60.h"
#include "arp.h"
#include "icmp.h"

extern union NetNode myNode;
extern union netcard rxdnetbuf;
extern union netcard txdnetbuf;

int main(void)
{
	unsigned int num_bytes;
	OSCCAL = 0x9A;

	myNode.node.ip = IP4_ADDR(218,1,168,192);
	myNode.node.mask = IP4_ADDR(0,255,255,255);
	myNode.node.gate = IP4_ADDR(1,1,168,192);
	myNode.node.mac[0] = 0x00;
	myNode.node.mac[1] = 0x60;
	myNode.node.mac[2] = 0x97;
	myNode.node.mac[3] = 0x48;
	myNode.node.mac[4] = 0xF4;
	myNode.node.mac[5] = 0xAB;

	/*ENC Initialisieren*/
	encInit();
	//Mac Adresse setzen(stack.h, dort wird auch die Ip festgelegt)
	nicSetMacAddress();
	
	/*	Leds konfigurieren
		LEDA : Link status
		LEDB : Receive & Send activity
		Led Settings : enc28j60 datasheet, page 11*/
	
	encPhyWrite(0x14,0b0000010001110000);
	
	arptab_init();

	while(1)
	{
        // Unload packet from the receive buffer and store in <RX_BUFF>   
        num_bytes = encPacketReceive(&rxdnetbuf);
		if(num_bytes > 0)
		{
			if(rxdnetbuf.etherframe.protocal==ARP_PACKET)
			{			//表示收到一个arp请求包
				if(rxdnetbuf.arpframe.operation==0x0100)
				{
					arp_answer();//ARP request,处理arp数据包
				}
				else if(rxdnetbuf.arpframe.operation==0x0200)
				{
					arp_process();//ARP answer
				}
			}
			else if((rxdnetbuf.etherframe.protocal == IP_PACKET)		//	收到一个IP包
				&& ((rxdnetbuf.ipframe.verandihl&0xf0) == 0x40)	//	IP V4
				&& (rxdnetbuf.ipframe.destip[0] == myNode.nodebytes.ipbytes[0])
				&& (rxdnetbuf.ipframe.destip[1] == myNode.nodebytes.ipbytes[1])
				&& (rxdnetbuf.ipframe.destip[2] == myNode.nodebytes.ipbytes[2])
				&& (rxdnetbuf.ipframe.destip[3] == myNode.nodebytes.ipbytes[3]))	//	my IP
			{
				arp_ip_mac();
				if(rxdnetbuf.ipframe.protocal == 1)			//	ICMP
				{
					if(rxdnetbuf.icmpframe.type == 8)		//	echo
					{
						ping_answer();
					}
				}
				else if(rxdnetbuf.ipframe.protocal == 17)	//	udp
				{
					//	net_udp_rcv();
				}
				else if(rxdnetbuf.ipframe.protocal == 6)		//	tcp
				{
					//	tcp_rcve(&rxdnetbuf);;
				}
			}
			else
			{
				//Lib_Uart_SendString("\nIEEE 802 packet rejected!\n");		//	IEEE 802 pkt rejected
			}
			//updatearptab();
		}

	}
}
