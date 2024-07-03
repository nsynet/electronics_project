/*==============================================*/
//	RTL8019AS��������
//	FOR AVR DEV 1.0
//	�汾:		1.0
//	����:		HAN		3366524@QQ.COM
//	����:		2005��1��3��
/*==============================================*/

/*==============================================*/
//	CP220X net packet define
//	FOR AVR Mega16
//	for little ram, max packet size = all - 1200
//	�汾:		1.0
//	����:		HAN		3366524@QQ.COM
//	����:		2006��11��10��
/*==============================================*/

#ifndef	_ne2000_h_
#define	_ne2000_h_

typedef unsigned char  INT8U;
typedef unsigned int	INT16U;
typedef unsigned long   INT32U;

struct net_node						//	�����ڵ���Ϣ
{
	INT32U	ip;						//	IP��ַ
	INT32U	mask;					//	��������
	INT32U	gate;					//	����
	INT16U	port;					//	�˿ں�
	INT8U	mac[6];   				//	��̫����ַ
};

struct net_nodebytes
{
	INT8U	ipbytes[4];
	INT8U	maskbytes[4];
	INT8U	gatebytes[4];
	INT8U	portbytes[2];
	INT16U	macwords[3];
};

union NetNode{
	struct net_node node;
	struct net_nodebytes nodebytes;
};
	

#define IP4_ADDR(a,b,c,d) ((INT32U)(a & 0xff) << 24) | ((INT32U)(b & 0xff) << 16) | \
                          ((INT32U)(c & 0xff) << 8) | (INT32U)(d & 0xff)

union ethernet_address_type{
	INT16U	words[3];
	INT8U	bytes[6];
};

union ip_address_type{
	INT32U	dwords;
	INT16U	words[2];
	INT8U	bytes[4];
};

union arp_table_type{
	INT8U	bytes[12];
	INT16U	words[6];
	struct{
		INT8U	status;
		INT8U	ttl;
		union ip_address_type ip_address;
		union ethernet_address_type ethernet_address;
	}arp;							//״̬,����ʱ��,ip��ַ,��̫����ַ
};

//��̫��֡                  
struct ethernet{
	INT8U	status;				//����״̬
	INT8U	nextpage;			//��һ��ҳ
	INT16U	length;				//��̫�����ȣ����ֽ�Ϊ��λ
	INT8U	destnodeid[6];		//Ŀ��������ַ
	INT8U	sourcenodeid[6];		//Դ������ַ
	INT16U	protocal;			//��һ��Э��
	INT8U	packet[300];		//��������
};

//IP��   ����IHL=5ʱ�� ����=5ʱ��ת��
struct ip{
	INT16U	head[9];			//��̫����ͷ
	INT8U	verandihl;			//�汾��ͷ����
	INT8U	typeofserver;		//��������
	INT16U	totallength;			//�ܳ���
	INT16U	frameindex;		//IP֡���
	INT16U	segment;			//�ֶα�־
	INT8U	ttl;					//����ʱ��
	INT8U	protocal;			//��һ��Э��
	INT16U	crc;				//У���
	INT8U	sourceip[4];			//ԴIP
	INT8U	destip[4];			//Ŀ��IP
	INT8U	packet[280];		//IP��������
};

struct ippacket{
	INT16U	head[9];			//��̫����ͷ
	INT8U	ippacket[300];		//IP��������
};
                
struct arp{
	INT16U	head[9];  			//��̫��ͷ
//arp���ĵ������ܳ�28�ֽ�
	INT16U	harewaretype;		//��̫��Ϊ0x0001
	INT16U	protocaltype;		//ip Ϊ0X0800
	INT8U	halength;			//=0X06
	INT8U	palength;			//=0X04
	INT16U	operation;			//����  0X0001Ϊ���� 0X0002ΪӦ�� 0X0003Ϊ�����ַ���� 0X0004Ϊ�����ַӦ��
	INT8U	sourcenodeid[6];		//Դ������ַ
	INT8U	sourceip[4];			//ԴIP��ַ
	INT8U	destnodeid[6];		//Ŀ��������ַ
	INT8U	destip[4];			//Ŀ��IP��ַ
}; 

struct icmp{						//������IP���У���IP���ϲ�Ϊ0X01��Ӧ��
	INT16U	head[9];			//��̫��ͷ
	INT16U	iphead[10];			//IPͷ
	INT8U	type;				//0X08 PING���� 0X00 PINGӦ��
	INT8U	option;				//0X00 PING
	INT16U	crc;      
	INT16U	id;
	INT16U	seq;
	INT8U	icmpdata[272];
};

struct tcp{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	sourceport;			//Դ�˿�
	INT16U	destport;			//Ŀ�Ķ˿�
	INT16U	seqnumberH;		//˳���
	INT16U	seqnumberL;
	INT16U	acknumberH;
	INT16U	acknumberL;		//ȷ�Ϻ�
	INT8U	offset;				//����ƫ����
	INT8U	control;				//���ӿ���
	INT16U	window;			//����
	INT16U	crc;				//У��� ������αͷ����TCPͷ��������
	INT16U	urg;				//����ָ��
	INT8U	tcpdata[260];		//TCP����
};

struct udp{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	sourceport;		//Դ�˿�
	INT16U	destport;			//Ŀ�Ķ˿�
	INT16U	length;    
	INT16U	crc;				//У��� ������αͷ����udpͷ��������
	INT8U	udpdata[272];	//udp����
};

struct tftprw{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	udphead[4];
	INT16U	opencode;		//	��д�����־
	INT8U	fileandmode[270];	//	�ļ����Ͳ�������
};

struct tftpd{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	udphead[4];
	INT16U	opencode;		//	��д�����־
	INT16U	number;			//	���Ż��߲����
	INT8U	tftpdata[268];	//	���ݻ��߲����Ϣ
};

struct bytes{
	INT8U	bytebuf[336];
};
             
//����Э��Ĺ�����
union netcard{
	struct bytes bytedata;
	struct ethernet etherframe;	
	struct arp arpframe;
	struct icmp icmpframe;
	struct tcp tcpframe;  
	struct ip ipframe;
	struct udp udpframe;
	struct ippacket ippacket;
	struct tftprw tftprwframe;
	struct tftpd tftpdataframe;
};

typedef struct
{
	union ip_address_type	ipaddr;
	union ip_address_type	his_sequence;
	union ip_address_type	my_sequence;
	union ip_address_type	old_sequence;
	union ip_address_type	his_ack;
	INT16U	port;
	INT8U	timer;
	INT8U	inactivity;	 
	INT8U	state;
	INT8U	query[23];
}tcpconnection;

#define IP_PACKET           0x0008
#define ARP_PACKET          0x0608
#define RARP_PACKET			0x3580

#define	MaxLenARPtable		0x02

#endif
