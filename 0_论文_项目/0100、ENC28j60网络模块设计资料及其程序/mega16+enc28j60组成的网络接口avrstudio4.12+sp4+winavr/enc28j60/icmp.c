/*==============================================*/
//	ICMPЭ�����
//	FOR AVR DEV 1.0
//	�汾:		1.0
//	����:		HAN		3366524@QQ.COM
//	����:		2005��1��6��
//	test ok, 2005/1/8
/*==============================================*/

#include	"j60.h"

extern union netcard rxdnetbuf;
extern union netcard txdnetbuf;
extern union NetNode myNode;

extern unsigned int chksum(unsigned char *check,unsigned int size);
extern unsigned int Swap16U(unsigned int data16u);

/*==============================================*/
//	��������:	PINGӦ��
/*==============================================*/
void ping_answer(void)
{
	unsigned int	i;
	unsigned int	size;
	unsigned int	tosize;

	union ethernet_address_type answeraddress;
	
//	size = rxdnetbuf.etherframe.length;
//	if(size == 60)
	{
		tosize = Swap16U(rxdnetbuf.ipframe.totallength) - 28;	//	real icmp data
		size = tosize + 42;
	}

	if(rxdnetbuf.icmpframe.type == 0x08)
	{				//	��ʾ��ping����
	    	for (i = 16; i < (size+4); i++)	//	ADD form protocal
	    	{			//	�����ݸ��Ƶ����ͻ�����	   
	 	    	txdnetbuf.bytedata.bytebuf[i] = rxdnetbuf.bytedata.bytebuf[i];
	    	}
	 	for(i = 0; i < 6; i++)
	 	{			//	Դ��ַΪĿ���ַ
	    		answeraddress.bytes[i] = rxdnetbuf.etherframe.sourcenodeid[i];
	 	}
		txdnetbuf.ipframe.ttl = txdnetbuf.ipframe.ttl - 1;
	    	txdnetbuf.ipframe.crc = 0;
		for(i = 0; i < 4; i++)
		{
			txdnetbuf.ipframe.destip[i] = rxdnetbuf.ipframe.sourceip[i];
			txdnetbuf.ipframe.sourceip[i] = myNode.nodebytes.ipbytes[i];
		}
		txdnetbuf.ipframe.crc = 0xFFFF - chksum((txdnetbuf.bytedata.bytebuf + 18),20);  
	    txdnetbuf.icmpframe.type = 0x00;		// is icmp answer;
		txdnetbuf.icmpframe.crc = 0;
		txdnetbuf.icmpframe.crc = 0xFFFF - chksum((txdnetbuf.bytedata.bytebuf + 38),(size -34)); 
	    encPacketSend(&answeraddress,&txdnetbuf,size,IP_PACKET);		
	}  
}
