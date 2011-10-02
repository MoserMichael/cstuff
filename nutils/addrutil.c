#include "addrutil.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

// --------------------------------------------------------------------
int IPADDRESS_any( IPADDRESS *addr, int family ) 
{
  switch(family) {
    case AF_INET:
      addr->family = AF_INET;
      addr->addr.ipv4.s_addr = INADDR_ANY;
      break;
    case AF_INET6:
      addr->family = AF_INET6;
      addr->addr.ipv6 = in6addr_any; 
      break;
    default:
      return -1;
  }
  return 0;
}

void IPADDRESS_broadcast_ipv4( IPADDRESS *addr )
{
  addr->family = AF_INET;
  addr->addr.ipv4.s_addr = htonl(INADDR_BROADCAST);
}


int IPADDRESS_loopback( IPADDRESS *addr, int family )
{
  switch(family) {
    case AF_INET:
      addr->family = AF_INET;
      addr->addr.ipv4.s_addr = htonl(INADDR_LOOPBACK);
      break;
    case AF_INET6:
      addr->family = AF_INET6;
      addr->addr.ipv6 = in6addr_loopback;
      break;
    default:
      return -1;
  }
  return 0;
}



int IPADDRESS_parse_string( IPADDRESS *addr, const char *str )
{
  if (inet_pton( AF_INET, str, &addr->addr.ipv4 )) {
     addr->family = AF_INET;
     return 0;
  } else if (inet_pton( AF_INET6, str, &addr->addr.ipv6 )) { 
     addr->family = AF_INET6;
     return 0;
  } 
  return -1;
}

char * IPADDRESS_to_string( IPADDRESS *addr )
{
  char sbuf[100];

  if (! inet_ntop( addr->family, (const void *) &addr->addr, sbuf, sizeof(sbuf) ) ) {
    return 0; 
  }
  return strdup( sbuf );
}

// --------------------------------------------------------------------
int SOCKADDR_init( SOCKADDR *saddr, IPADDRESS *addr, uint16_t  port ) 
{
  memset( saddr, 0, sizeof( SOCKADDR ) );
  saddr->addr_ipv4.sin_port =  htons( port ); 
  saddr->addr_ipv4.sin_family = addr->family; 

  if (addr->family == AF_INET) {
    saddr->addr_ipv4.sin_addr = addr->addr.ipv4;
  } else if (addr->family == AF_INET6) {
    saddr->addr_ipv6.sin6_addr = addr->addr.ipv6;
  } else {
    return -1;
  }
  return 0;
}

socklen_t SOCKADDR_length( SOCKADDR * addr )
{
  if (addr->addr_ipv4.sin_family  == AF_INET) {
    return sizeof(struct sockaddr_in);
  } else if (addr->addr_ipv4.sin_family == AF_INET6) {
    return sizeof(struct sockaddr_in6);
  }
  return -1;
}

int SOCKADDR_family( SOCKADDR *addr )
{
  return addr->addr_ipv4.sin_family;
}

void * SOCKADDR_ipaddr( SOCKADDR * addr )
{
  if (addr->addr_ipv4.sin_family  == AF_INET) {
    return &addr->addr_ipv4.sin_addr;
  } else if (addr->addr_ipv4.sin_family == AF_INET6) {
    return &addr->addr_ipv6.sin6_addr;
  }
  return 0;
}

char * SOCKADDR_to_string( SOCKADDR *saddr )
{
  char sbuf[100];
  int nlen, remain;

  if (SOCKADDR_family(saddr) == AF_INET6) {
    sbuf[0] = '[';

    if (! inet_ntop( SOCKADDR_family( saddr ), (const void *) SOCKADDR_ipaddr( saddr ), sbuf + 1, sizeof(sbuf) - 1 ) ) {
      return 0; 
    }

    nlen = strlen(sbuf);
    remain = sizeof(sbuf) - nlen; 

    if (remain <= 0) {
      return 0;
    }

    snprintf(sbuf+strlen(sbuf),remain-1,"]:%hu", ntohs( saddr->addr_ipv4.sin_port ) ); 
  } else {
    if (! inet_ntop( SOCKADDR_family( saddr ), (const void *) SOCKADDR_ipaddr( saddr ), sbuf , sizeof(sbuf) ) ) {
      return 0; 
    }

    nlen = strlen(sbuf);
    remain = sizeof(sbuf) - nlen; 

    if (remain <= 0) {
      return 0;
    }

    snprintf(sbuf+strlen(sbuf),remain-1,":%hu", ntohs( saddr->addr_ipv4.sin_port ) ); 
  }
  return strdup(sbuf);
}


