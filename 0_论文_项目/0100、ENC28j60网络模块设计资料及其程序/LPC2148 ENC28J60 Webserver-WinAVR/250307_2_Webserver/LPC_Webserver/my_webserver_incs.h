/*
Andreas Weschenfelder

include-files fuer den Webserver
 */
#ifndef __WEBSERVER_INC_H__
#define __WEBSERVER_INC_H__

#include "LPC_webserver\clock-arch.h"

#include "LPC_webserver\uip.h"
#include "LPC_webserver\uip_arp.h" 

#include "LPC_webserver\uip.c"
#include "LPC_webserver\memb.c"
#include "LPC_webserver\uip_arp.c"
#include "LPC_webserver\httpd.c"
#include "LPC_webserver\httpd-fs.c"
#include "LPC_webserver\http-strings.c"
#include "LPC_webserver\psock.c"
#include "LPC_webserver\syscalls.c"
#include "LPC_webserver\httpd-cgi.c"
#include "LPC_webserver\clock-arch.c"

#endif /* __WEBSERVER_INC_H__ */
