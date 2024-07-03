#include "uip.h"
#include "uip_arch.h"

#define BUF ((uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])
#define IP_PROTO_TCP	6

void uip_add32(u8_t *op32, u16_t op16)
{
	uip_acc32[3] = op32[3] + (op16 & 0xff);
	uip_acc32[2] = op32[2] + (op16 >> 8);
	uip_acc32[1] = op32[1];
	uip_acc32[0] = op32[0];
  
	if(uip_acc32[2] < (op16 >> 8)) 
	{
		++uip_acc32[1];
		if(uip_acc32[1] == 0) 
		{
			++uip_acc32[0];
		}
	}

	if(uip_acc32[3] < (op16 & 0xff)) 
	{
		++uip_acc32[2];  
		if(uip_acc32[2] == 0) 
		{
			++uip_acc32[1];    
			if(uip_acc32[1] == 0) 
			{
				++uip_acc32[0];
			}
		}
	}
}

u16_t uip_chksum(u16_t *sdata, u16_t len)
{
	u16_t acc;
	for(acc = 0; len > 1; len -= 2) 
	{
		acc += *sdata;
		if(acc < *sdata) 
		{
			/* Overflow, so we add the carry to acc (i.e., increase by one). */
			++acc;
		}
		++sdata;
	}

	/* add up any odd byte */
	if(len == 1) 
	{
		acc += htons(((u16_t)(*(u8_t *)sdata)) << 8);
		if(acc < htons(((u16_t)(*(u8_t *)sdata)) << 8)) 
		{
			++acc;
		}
	}
	return acc;
}

u16_t uip_ipchksum(void)
{
	return uip_chksum((u16_t *)&uip_buf[UIP_LLH_LEN], 20);
}

u16_t uip_tcpchksum(void)
{
	u16_t hsum, sum;  
	/* Compute the checksum of the TCP header. */
	hsum = uip_chksum((u16_t *)&uip_buf[20 + UIP_LLH_LEN], 20);

	/* Compute the checksum of the data in the TCP packet and add it to the TCP header checksum. */
	sum = uip_chksum((u16_t *)uip_appdata,(u16_t)(((((u16_t)(BUF->len[0]) << 8) + BUF->len[1]) - 40)));
	if((sum += hsum) < hsum) 
	{
		++sum;
	}
  
	if((sum += BUF->srcipaddr[0]) < BUF->srcipaddr[0]) 
	{
		++sum;
	}
	if((sum += BUF->srcipaddr[1]) < BUF->srcipaddr[1]) 
		{
		++sum;
	}
	if((sum += BUF->destipaddr[0]) < BUF->destipaddr[0]) 
	{
		++sum;
	}
	if((sum += BUF->destipaddr[1]) < BUF->destipaddr[1]) 
	{
		++sum;
	}
	if((sum += (u16_t)htons((u16_t)IP_PROTO_TCP)) < (u16_t)htons((u16_t)IP_PROTO_TCP)) 
	{
		++sum;
	}

	hsum = (u16_t)htons((((u16_t)(BUF->len[0]) << 8) + BUF->len[1]) - 20);
  
	if((sum += hsum) < hsum) 
	{
		++sum;
	}
	return sum;
}
