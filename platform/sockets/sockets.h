/*
 * Copyright (c) 1982, 1985, 1986, 1988, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)socket.h	8.4 (Berkeley) 2/21/94
 * $FreeBSD: src/sys/sys/socket.h,v 1.60 2002/04/20 02:24:35 mike Exp $
 */

#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/types.h>
/*
 * Types
 */
#define SOCK_STREAM 1    /* stream socket */
#define SOCK_DGRAM 2     /* datagram socket */
#define SOCK_RAW 3       /* raw-protocol interface */
#define SOCK_RDM 4       /* reliably-delivered message */
#define SOCK_SEQPACKET 5 /* sequenced packet stream */

/*
 * Option flags per-socket.
 */
#define SO_DEBUG 0x0001        /* turn on debugging info recording */
#define SO_ACCEPTCONN 0x0002   /* socket has had listen() */
#define SO_REUSEADDR 0x0004    /* allow local address reuse */
#define SO_KEEPALIVE 0x0008    /* keep connections alive */
#define SO_DONTROUTE 0x0010    /* just use interface addresses */
#define SO_BROADCAST 0x0020    /* permit sending of broadcast msgs */
#define SO_USELOOPBACK 0x0040  /* bypass hardware when possible */
#define SO_LINGER 0x0080       /* linger on close if data present */
#define SO_OOBINLINE 0x0100    /* leave received OOB data in line */
#define SO_REUSEPORT 0x0200    /* allow local address & port reuse */
#define SO_TIMESTAMP 0x0400    /* timestamp received dgram traffic */
#define SO_ACCEPTFILTER 0x1000 /* there is an accept filter */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF 0x1001   /* send buffer size */
#define SO_RCVBUF 0x1002   /* receive buffer size */
#define SO_SNDLOWAT 0x1003 /* send low-water mark */
#define SO_RCVLOWAT 0x1004 /* receive low-water mark */
#define SO_SNDTIMEO 0x1005 /* send timeout */
#define SO_RCVTIMEO 0x1006 /* receive timeout */
#define SO_ERROR 0x1007    /* get error status and clear */
#define SO_TYPE 0x1008     /* get socket type */
/*efine	SO_PRIVSTATE	0x1009		   get/deny privileged state */
/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define SOL_SOCKET 0xffff /* options for socket level */

/* Protocol families.  */
#define PF_UNSPEC 0      /* Unspecified.  */
#define PF_LOCAL 1       /* Local to host (pipes and file-domain).  */
#define PF_UNIX PF_LOCAL /* Old BSD name for PF_LOCAL.  */
#define PF_FILE PF_LOCAL /* Another non-standard name for PF_LOCAL.  */
#define PF_INET 2        /* IP protocol family.  */
#define PF_AX25 3        /* Amateur Radio AX.25.  */
#define PF_IPX 4         /* Novell Internet Protocol.  */
#define PF_APPLETALK 5   /* Appletalk DDP.  */
#define PF_NETROM 6      /* Amateur radio NetROM.  */
#define PF_BRIDGE 7      /* Multiprotocol bridge.  */
#define PF_ATMPVC 8      /* ATM PVCs.  */
#define PF_X25 9         /* Reserved for X.25 project.  */
#define PF_INET6 10      /* IP version 6.  */
#define PF_ROSE 11       /* Amateur Radio X.25 PLP.  */
#define PF_DECnet 12     /* Reserved for DECnet project.  */
#define PF_NETBEUI 13    /* Reserved for 802.2LLC project.  */
#define PF_SECURITY 14   /* Security callback pseudo AF.  */
#define PF_KEY 15        /* PF_KEY key management API.  */
#define PF_NETLINK 16
#define PF_ROUTE PF_NETLINK /* Alias to emulate 4.4BSD.  */
#define PF_PACKET 17        /* Packet family.  */
#define PF_ASH 18           /* Ash.  */
#define PF_ECONET 19        /* Acorn Econet.  */
#define PF_ATMSVC 20        /* ATM SVCs.  */
#define PF_SNA 22           /* Linux SNA Project */
#define PF_IRDA 23          /* IRDA sockets.  */
#define PF_PPPOX 24         /* PPPoX sockets.  */
#define PF_WANPIPE 25       /* Wanpipe API sockets.  */
#define PF_BLUETOOTH 31     /* Bluetooth sockets.  */
#define PF_MAX 32           /* For now..  */

/* Address families.  */
#define AF_UNSPEC PF_UNSPEC
#define AF_LOCAL PF_LOCAL
#define AF_UNIX PF_UNIX
#define AF_FILE PF_FILE
#define AF_INET PF_INET
#define AF_AX25 PF_AX25
#define AF_IPX PF_IPX
#define AF_APPLETALK PF_APPLETALK
#define AF_NETROM PF_NETROM
#define AF_BRIDGE PF_BRIDGE
#define AF_ATMPVC PF_ATMPVC
#define AF_X25 PF_X25
#define AF_INET6 PF_INET6
#define AF_ROSE PF_ROSE
#define AF_DECnet PF_DECnet
#define AF_NETBEUI PF_NETBEUI
#define AF_SECURITY PF_SECURITY
#define AF_KEY PF_KEY
#define AF_NETLINK PF_NETLINK
#define AF_ROUTE PF_ROUTE
#define AF_PACKET PF_PACKET
#define AF_ASH PF_ASH
#define AF_ECONET PF_ECONET
#define AF_ATMSVC PF_ATMSVC
#define AF_SNA PF_SNA
#define AF_IRDA PF_IRDA
#define AF_PPPOX PF_PPPOX
#define AF_WANPIPE PF_WANPIPE
#define AF_BLUETOOTH PF_BLUETOOTH
#define AF_MAX PF_MAX
#define SOCK_MAXADDRLEN 255 /* longest possible addresses */
#define NET_RT_DUMP 1       /* dump; may limit to a.f. */
#define NET_RT_FLAGS 2      /* by flags, e.g. RESOLVING */
#define NET_RT_IFLIST 3     /* survey interface list */
#define NET_RT_MAXID 4
#define MSG_OOB 0x1       /* process out-of-band data */
#define MSG_PEEK 0x2      /* peek at incoming message */
#define MSG_DONTROUTE 0x4 /* send without using routing tables */
#define MSG_EOR 0x8       /* data completes record */
#define MSG_TRUNC 0x10    /* data discarded before delivery */
#define MSG_CTRUNC 0x20   /* control data lost before delivery */
#define MSG_WAITALL 0x40  /* wait for full request or error */
#define MSG_DONTWAIT 0x80 /* this message should be nonblocking */
#define MSG_EOF 0x100     /* data completes connection */
#define MSG_COMPAT 0x8000 /* used in sendit() */

/* "Socket"-level control message types: */
#define SCM_RIGHTS 0x01    /* access rights (array of int) */
#define SCM_TIMESTAMP 0x02 /* timestamp (struct timeval) */ 
#define SCM_CREDS 0x03     /* process creds (struct cmsgcred) */
/* Bits in the FLAGS argument to `send', `recv', et al.  */
/* Bits in the FLAGS argument to `send', `recv', et al.  */
/* Structure used to manipulate the SO_LINGER option.  */


#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2

#define MSG_NOSIGNAL 0
#define IPPROTO_TCP 6
#define	TCP_NODELAY		 1  /* Don't delay send to coalesce packets  */


struct linger
{
  int l_onoff;  /* Nonzero to linger on close.  */
  int l_linger; /* Time to linger.  */
};

struct sockaddr
{
  unsigned short sa_family; /* address family, AF_xxx.  */
  char sa_data[14];         /* 14 bytes of protocol address.  */
};

typedef unsigned int socklen_t;

struct in_addr
{
  unsigned int s_addr; /* that's a 32-bit int */
};

#define	INADDR_ANY		((in_addr_t) 0x00000000)

struct sockaddr_in
{
  short sin_family; /* e.g. AF_INET, AF_INET6 */
  unsigned short sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};
typedef struct sockaddr_in sockaddr_in;

static inline uint16_t htons(uint16_t port){
  return port;
}

struct addrinfo {
    int              ai_flags;      
    int              ai_family;     
    int              ai_socktype;   
    int              ai_protocol;  
    socklen_t        ai_addrlen;   
    struct sockaddr *ai_addr;      
    char            *ai_canonname;  
    struct addrinfo *ai_next;
};

struct hostent
{
  char *h_name;			/* Official name of host.  */
  char **h_aliases;		/* Alias list.  */
  int h_addrtype;		/* Host address type.  */
  int h_length;			/* Length of address.  */
  struct in_addr * h_addr;   /*NOT COMPLAINT, THIS IS NOT A LIST!*/
};

#endif /* SOCKETS_H */
