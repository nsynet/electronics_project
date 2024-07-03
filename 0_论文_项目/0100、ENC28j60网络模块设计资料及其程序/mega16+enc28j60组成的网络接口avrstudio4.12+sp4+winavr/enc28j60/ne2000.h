/*==============================================*/
//	RTL8019AS基本定义
//	FOR AVR DEV 1.0
//	版本:		1.0
//	作者:		HAN		3366524@QQ.COM
//	日期:		2005年1月3日
/*==============================================*/

/*==============================================*/
//	CP220X net packet define
//	FOR AVR Mega16
//	for little ram, max packet size = all - 1200
//	版本:		1.0
//	作者:		HAN		3366524@QQ.COM
//	日期:		2006年11月10日
/*==============================================*/

#ifndef	_ne2000_h_
#define	_ne2000_h_

typedef unsigned char  INT8U;
typedef unsigned int	INT16U;
typedef unsigned long   INT32U;

struct net_node						//	本机节点信息
{
	INT32U	ip;						//	IP地址
	INT32U	mask;					//	子网掩码
	INT32U	gate;					//	网关
	INT16U	port;					//	端口号
	INT8U	mac[6];   				//	以太网地址
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
	}arp;							//状态,生存时间,ip地址,以太网地址
};

//以太网帧                  
struct ethernet{
	INT8U	status;				//接收状态
	INT8U	nextpage;			//下一个页
	INT16U	length;				//以太网长度，以字节为单位
	INT8U	destnodeid[6];		//目的网卡地址
	INT8U	sourcenodeid[6];		//源网卡地址
	INT16U	protocal;			//下一层协议
	INT8U	packet[300];		//包的内容
};

//IP包   仅当IHL=5时用 当不=5时作转换
struct ip{
	INT16U	head[9];			//以太网包头
	INT8U	verandihl;			//版本与头长度
	INT8U	typeofserver;		//服务类型
	INT16U	totallength;			//总长度
	INT16U	frameindex;		//IP帧序号
	INT16U	segment;			//分段标志
	INT8U	ttl;					//生存时间
	INT8U	protocal;			//下一层协议
	INT16U	crc;				//校验和
	INT8U	sourceip[4];			//源IP
	INT8U	destip[4];			//目的IP
	INT8U	packet[280];		//IP包的内容
};

struct ippacket{
	INT16U	head[9];			//以太网包头
	INT8U	ippacket[300];		//IP包的内容
};
                
struct arp{
	INT16U	head[9];  			//以太网头
//arp报文的内容总长28字节
	INT16U	harewaretype;		//以太网为0x0001
	INT16U	protocaltype;		//ip 为0X0800
	INT8U	halength;			//=0X06
	INT8U	palength;			//=0X04
	INT16U	operation;			//操作  0X0001为请求 0X0002为应答 0X0003为反向地址请求 0X0004为反向地址应答
	INT8U	sourcenodeid[6];		//源网卡地址
	INT8U	sourceip[4];			//源IP地址
	INT8U	destnodeid[6];		//目的网卡地址
	INT8U	destip[4];			//目的IP地址
}; 

struct icmp{						//包含在IP包中，是IP的上层为0X01的应用
	INT16U	head[9];			//以太网头
	INT16U	iphead[10];			//IP头
	INT8U	type;				//0X08 PING请求 0X00 PING应答
	INT8U	option;				//0X00 PING
	INT16U	crc;      
	INT16U	id;
	INT16U	seq;
	INT8U	icmpdata[272];
};

struct tcp{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	sourceport;			//源端口
	INT16U	destport;			//目的端口
	INT16U	seqnumberH;		//顺序号
	INT16U	seqnumberL;
	INT16U	acknumberH;
	INT16U	acknumberL;		//确认号
	INT8U	offset;				//数据偏移量
	INT8U	control;				//连接控制
	INT16U	window;			//流控
	INT16U	crc;				//校验和 ，包括伪头部，TCP头部，数据
	INT16U	urg;				//紧急指针
	INT8U	tcpdata[260];		//TCP数据
};

struct udp{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	sourceport;		//源端口
	INT16U	destport;			//目的端口
	INT16U	length;    
	INT16U	crc;				//校验和 ，包括伪头部，udp头部，数据
	INT8U	udpdata[272];	//udp数据
};

struct tftprw{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	udphead[4];
	INT16U	opencode;		//	读写代码标志
	INT8U	fileandmode[270];	//	文件名和操作类型
};

struct tftpd{
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	udphead[4];
	INT16U	opencode;		//	读写代码标志
	INT16U	number;			//	块编号或者差错码
	INT8U	tftpdata[268];	//	数据或者差错信息
};

struct bytes{
	INT8U	bytebuf[336];
};
             
//所有协议的共用体
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
