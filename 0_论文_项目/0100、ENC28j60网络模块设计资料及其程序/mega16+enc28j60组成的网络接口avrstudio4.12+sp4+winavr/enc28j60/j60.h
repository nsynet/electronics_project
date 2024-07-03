#ifndef	_j60_h_
#define	_j60_h_

#include "ne2000.h"

void nicSetMacAddress(void);
void encInit(void);
void encPhyWrite(unsigned char add, unsigned int data);
void encPacketSend(union ethernet_address_type *pDestAddr, union netcard *txdnet, unsigned int buffer_length, unsigned int packet_type);
unsigned int encPacketReceive(union netcard *rxdnet);

#endif
