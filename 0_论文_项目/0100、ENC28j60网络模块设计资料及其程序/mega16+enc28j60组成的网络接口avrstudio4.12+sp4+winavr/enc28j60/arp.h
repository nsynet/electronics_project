#ifndef _arp_h_
#define	_arp_h_

void arptab_init(void);
void arp_request(union ip_address_type *ip_address);
void arp_answer(void);
void arp_process(void);
void updatearptab(void);
void arp_ip_mac(void);

#endif
