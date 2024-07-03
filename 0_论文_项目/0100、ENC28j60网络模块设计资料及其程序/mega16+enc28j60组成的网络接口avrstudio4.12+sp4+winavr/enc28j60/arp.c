/*==============================================*/
//	ARPЭ�����
//	FOR ARM DEV 1.0
//	�汾:		1.0
//	����:		HAN		3366524@QQ.COM
//	����:		2005��1��5��
//	test ok, 2005/1/8
/*==============================================*/

#include	"j60.h"


extern union netcard rxdnetbuf;
extern union netcard txdnetbuf;
extern union NetNode myNode;

union arp_table_type arp_tab[MaxLenARPtable];
static unsigned char	arpindex = 0;			//ARP table ѭ�������

/*==============================================*/
//	��������:	arp��̬��������ʼ��
/*==============================================*/
void arptab_init(void)
{
	unsigned char	i,j;
	for(i = 0; i < MaxLenARPtable; i++)
	{
		for(j = 0; j < 12; j++)
		{
			arp_tab[i].bytes[j] = 0;
		}
	}
}

/*==============================================*/
//	��������:	arp����
/*==============================================*/
void arp_request(union ip_address_type *ip_address)
{
	unsigned char	i;
	union ethernet_address_type castaddress;

	for(i = 0; i < 6; i++)
	{
		castaddress.bytes[i] = 0xFF;
		txdnetbuf.arpframe.sourcenodeid[i] = myNode.node.mac[i];
	}

	for(i = 0; i < 4; i++)
	{
		txdnetbuf.arpframe.sourceip[i] = myNode.nodebytes.ipbytes[i];
		txdnetbuf.arpframe.destip[i] = ip_address->bytes[i];
	}
	txdnetbuf.arpframe.harewaretype=0x0100;
	txdnetbuf.arpframe.protocaltype=0x0008;
	txdnetbuf.arpframe.halength=0x06;
	txdnetbuf.arpframe.palength=0x04;
 	txdnetbuf.arpframe.operation=0x0100;//Ӧ�����
 	for(i=46;i<64;i++)
		txdnetbuf.bytedata.bytebuf[i]=0x00;
	encPacketSend(&castaddress,&txdnetbuf,60,ARP_PACKET);
}

/*==============================================*/
//	��������:	arpӦ��
/*==============================================*/
void arp_answer(void)
{
	unsigned char	i;
	union ethernet_address_type answeraddress;

	if( (rxdnetbuf.arpframe.destip[0] == myNode.nodebytes.ipbytes[0])
		&& (rxdnetbuf.arpframe.destip[1] == myNode.nodebytes.ipbytes[1])
		&& (rxdnetbuf.arpframe.destip[2] == myNode.nodebytes.ipbytes[2])
		&& (rxdnetbuf.arpframe.destip[3] == myNode.nodebytes.ipbytes[3]))
	{								//��ʾ���������ip��ַ������
	    	for(i = 16; i < 64; i++)
		{							//����arp�����ͻ�����
	    		txdnetbuf.bytedata.bytebuf[i]=rxdnetbuf.bytedata.bytebuf[i];
	    	}
	    	for(i = 0; i < 6; i++)
		{							//���ƶԷ�������ַ�����ص�ַ   
			answeraddress.bytes[i]=rxdnetbuf.etherframe.sourcenodeid[i];
			txdnetbuf.arpframe.sourcenodeid[i]=myNode.node.mac[i];
			txdnetbuf.arpframe.destnodeid[i]=rxdnetbuf.arpframe.sourcenodeid[i];
	    	}
		for(i = 0; i < 4; i++)
		{
			txdnetbuf.arpframe.destip[i]=rxdnetbuf.arpframe.sourceip[i];
			txdnetbuf.arpframe.sourceip[i]=rxdnetbuf.arpframe.destip[i];
		}
		txdnetbuf.arpframe.operation=0x0200;//��Ӧ����
		//Lib_Uart_SendString("arp answer tx\n");
		encPacketSend(&answeraddress,&txdnetbuf,60,ARP_PACKET);
	}
	else
	{
	 	//	Lib_Uart_SendString("arp request ip error\n");
	}
}

/*==============================================*/
//	��������:	arpӦ����
/*==============================================*/
void arp_process(void)
{	
	unsigned char	i,j;

	for(i = 0; i < MaxLenARPtable; i++)
	{
		if(arp_tab[i].arp.status == 1)
		{
			if((arp_tab[i].arp.ip_address.bytes[0] == rxdnetbuf.arpframe.sourceip[0])
				&& (arp_tab[i].arp.ip_address.bytes[1] == rxdnetbuf.arpframe.sourceip[1])
				&& (arp_tab[i].arp.ip_address.bytes[2] == rxdnetbuf.arpframe.sourceip[2])
				&& (arp_tab[i].arp.ip_address.bytes[3] == rxdnetbuf.arpframe.sourceip[3]))
			{
				arp_tab[i].arp.ttl=0x80;
				for(j = 0; j < 4; j++)
					arp_tab[i].arp.ip_address.bytes[j]=rxdnetbuf.arpframe.sourceip[j];
				for(j = 0; j < 6; j++)
					arp_tab[i].arp.ethernet_address.bytes[j]=rxdnetbuf.arpframe.sourcenodeid[j];
				return;
			}
		}
		
	}

	for(i = 0;i < MaxLenARPtable; i++)
	{
		if(arp_tab[i].arp.status == 0)
		{
			arp_tab[i].arp.status = 1;
			arp_tab[i].arp.ttl = 0x80;
			for(j = 0; j < 4; j++)
				arp_tab[i].arp.ip_address.bytes[j] = rxdnetbuf.arpframe.sourceip[j];
			for(j = 0;j < 6; j++)
				arp_tab[i].arp.ethernet_address.bytes[j] = rxdnetbuf.arpframe.sourcenodeid[j];
			return;
		}
	}

	arp_tab[arpindex].arp.status = 1;		//write arp package to some location.
	arp_tab[arpindex].arp.ttl = 0x80;
	for(j = 0; j < 4; j++)
		arp_tab[arpindex].arp.ip_address.bytes[j] = rxdnetbuf.arpframe.sourceip[j];
	for(j = 0; j < 6; j++)
		arp_tab[arpindex].arp.ethernet_address.bytes[j] = rxdnetbuf.arpframe.sourcenodeid[j];
	arpindex++;
	if(arpindex == MaxLenARPtable)
		arpindex = 0;
}

/*==============================================*/
//	��������:	����ARP����
/*==============================================*/
void updatearptab(void)
{
	unsigned char	i;

	for(i = 0; i < MaxLenARPtable; i++)
	{
		if(arp_tab[i].arp.status == 1)
		{
			if(arp_tab[i].arp.ttl == 0)
			{
				arp_tab[i].arp.status=0;	
			}
			else
			{
				arp_tab[i].arp.ttl--;
			}
		}
	}
}

/*==============================================*/
//	��������:	��ARP�����в���ָ��IP/MACӳ���
/*==============================================*/
unsigned char arp_find_mac(union ip_address_type ip,union ethernet_address_type *macadr)
{
	unsigned char	i,j;

	for(i = 0; i < MaxLenARPtable; i++)
	{
		if(arp_tab[i].arp.status == 1)
			if(arp_tab[i].arp.ip_address.dwords == ip.dwords)
			{
				for(j = 0; j < 6; j++)
					macadr->bytes[j] = arp_tab[i].arp.ethernet_address.bytes[j];
				return 1;
		    }
	}
	return 0;
}

/*==============================================*/
//	��������:	��ARP������ֱ�Ӽ���IP/MACӳ���
/*==============================================*/
void arp_ip_mac(void)
{
	unsigned char	i,j;

	for(i = 0; i < MaxLenARPtable; i++)
	{
		if(arp_tab[i].arp.status == 1)
		{
			if((arp_tab[i].arp.ip_address.bytes[0] == rxdnetbuf.ipframe.sourceip[0])
				&& (arp_tab[i].arp.ip_address.bytes[1] == rxdnetbuf.ipframe.sourceip[1])
				&& (arp_tab[i].arp.ip_address.bytes[2] == rxdnetbuf.ipframe.sourceip[2])
				&& (arp_tab[i].arp.ip_address.bytes[3] == rxdnetbuf.ipframe.sourceip[3]))
			{
				arp_tab[i].arp.ttl=0x80;
				for(j = 0; j < 4; j++)
					arp_tab[i].arp.ip_address.bytes[j] = rxdnetbuf.ipframe.sourceip[j];
				for(j = 0; j < 6; j++)
					arp_tab[i].arp.ethernet_address.bytes[j] = rxdnetbuf.etherframe.sourcenodeid[j];
				return;
			}
		}
		
	}

	for(i = 0;i < MaxLenARPtable; i++)
	{
		if(arp_tab[i].arp.status == 0)
		{
			arp_tab[i].arp.status = 1;
			arp_tab[i].arp.ttl = 0x80;
			for(j = 0; j < 4; j++)
				arp_tab[i].arp.ip_address.bytes[j] = rxdnetbuf.ipframe.sourceip[j];
			for(j = 0;j < 6; j++)
				arp_tab[i].arp.ethernet_address.bytes[j] = rxdnetbuf.etherframe.sourcenodeid[j];
			return;
		}
	}

	arp_tab[arpindex].arp.status = 1;		//write arp package to some location.
	arp_tab[arpindex].arp.ttl = 0x80;
	for(j = 0; j < 4; j++)
		arp_tab[arpindex].arp.ip_address.bytes[j] = rxdnetbuf.ipframe.sourceip[j];
	for(j = 0; j < 6; j++)
		arp_tab[arpindex].arp.ethernet_address.bytes[j] = rxdnetbuf.etherframe.sourcenodeid[j];
	arpindex++;
	if(arpindex == MaxLenARPtable)
		arpindex = 0;
}
