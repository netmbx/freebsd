/*
 * Copyright (c) 1990, 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static  char rcsid[] =
	"@(#)$Header: /home/ncvs/src/usr.sbin/tcpdump/tcpdump/print-ppp.c,v 1.2 1995/03/08 12:52:40 olah Exp $ (LBL)";
#endif

#ifdef PPP
#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/ioctl.h>

#include <net/if.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>

#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <pcap.h>
#include <signal.h>
#include <stdio.h>

#include <netinet/if_ether.h>
#include "ethertype.h"

#include <net/ppp_defs.h>
#include "interface.h"
#include "addrtoname.h"

struct protonames {
	u_short protocol;
	char *name;
};

static struct protonames protonames[] = {
	/*
	 * Protocol field values.
	 */
	PPP_IP,		"IP",		/* Internet Protocol */
	PPP_XNS,	"XNS",		/* Xerox NS */
	PPP_IPX,	"IPX",		/* IPX Datagram (RFC1552) */
	PPP_VJC_COMP,	"VJC_UNCOMP",	/* VJ compressed TCP */
	PPP_VJC_UNCOMP,	"VJC_UNCOMP",	/* VJ uncompressed TCP */
	PPP_COMP,	"COMP",		/* compressed packet */
	PPP_IPCP,	"IPCP",		/* IP Control Protocol */
	PPP_IPXCP,	"IPXCP",	/* IPX Control Protocol (RFC1552) */
	PPP_CCP,	"CCP",		/* Compression Control Protocol */
	PPP_LCP,	"LCP",		/* Link Control Protocol */
	PPP_PAP,	"PAP",		/* Password Authentication Protocol */
	PPP_LQR,	"LQR",		/* Link Quality Report protocol */
	PPP_CHAP,	"CHAP",		/* Cryptographic Handshake Auth. Proto*/
};

void
ppp_hdlc_print(const u_char *p, int length)
{
	int proto = PPP_PROTOCOL(p);
	int i;

	printf("%4d %02x ", length, PPP_CONTROL(p));

	for (i = (sizeof(protonames) / sizeof(protonames[0])) - 1; i >= 0; --i){
		if (proto == protonames[i].protocol) {
			printf("%s: ", protonames[i].name);
			break;
		}
	}
	if (i < 0)
		printf("%04x: ", proto);
}

void
ppp_if_print(u_char *user, const struct pcap_pkthdr *h,
	     register const u_char *p)
{
	register int length = h->len;
	register int caplen = h->caplen;

	ts_print(&h->ts);

	if (caplen < PPP_HDRLEN) {
		printf("[|ppp]");
		goto out;
	}

	/*
	 * Some printers want to get back at the link level addresses,
	 * and/or check that they're not walking off the end of the packet.
	 * Rather than pass them all the way down, we set these globals.
	 */
	packetp = p;
	snapend = p + caplen;

	if (eflag)
		ppp_hdlc_print(p, length);

	length -= PPP_HDRLEN;

	switch(PPP_PROTOCOL(p)) {
	case PPP_IP:
	case ETHERTYPE_IP:
		ip_print((const u_char *)(p + PPP_HDRLEN), length);
		break;
	case PPP_IPX:
	case ETHERTYPE_IPX:
		ipx_print((const u_char *)(p + PPP_HDRLEN), length);
		break;

	default:
		if(!eflag)
			ppp_hdlc_print(p, length);
		if(!xflag)
			default_print((const u_char *)(p + PPP_HDRLEN),
					caplen - PPP_HDRLEN);
	}

	if (xflag)
		default_print((const u_char *)(p + PPP_HDRLEN),
				caplen - PPP_HDRLEN);
out:
	putchar('\n');
}
#else
#include <sys/types.h>
#include <sys/time.h>

#include <stdio.h>

#include "interface.h"
void
ppp_if_print(u_char *user, const struct pcap_pkthdr *h, const u_char *p)
{
	error("not configured for ppp");
	/* NOTREACHED */
}
#endif
