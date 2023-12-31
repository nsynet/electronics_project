//********************************************************************************************
//
// File : ip.h implement for Internet Protocol
//
//********************************************************************************************
#define IP_HEADER_LEN		20

#define IP_PROTO_ICMP_V		0x01
#define IP_PROTO_TCP_V		0x06
#define IP_PROTO_UDP_V		0x11
#define IP_V4_V				0x40
#define IP_HEADER_LENGTH_V	0x05

#define IP_P				0x0E
#define IP_HEADER_VER_LEN_P	0x0E
#define IP_TOS_P			0x0F
#define IP_TOTLEN_H_P		0x10
#define IP_TOTLEN_L_P		0x11
#define IP_ID_H_P			0x12
#define IP_ID_L_P			0x13
#define IP_FLAGS_H_P		0x14
#define IP_FLAGS_L_P		0x15
#define IP_TTL_P			0x16
#define IP_PROTO_P			0x17
#define IP_CHECKSUM_H_P		0x18
#define IP_CHECKSUM_L_P		0x19
#define IP_SRC_IP_P			0x1A
#define IP_DST_IP_P			0x1E


//********************************************************************************************
//
// Prototype function
//
//********************************************************************************************
//void ip_fill_ip_address( unsigned char *buf, unsigned char *avr_ip, unsigned char *dest_ip );
//void ip_fill_hdr_checksum( unsigned char *buf );

 BYTE ip_packet_is_ip ( BYTE *rxtx_buffer );
 void ip_generate_header ( BYTE *rxtx_buffer, WORD total_length, BYTE protocol, BYTE *dest_ip );

