#ifndef __ADDRUTIL_H__
#define __ADDRUTIL_H__

#include <netinet/in.h>
#include <cutils/base.h>

/** 
 * @defgroup IPADDRESS
 * @brief  class for transparent treatment of ip addresses. encapsulates either ipv4 or ipv6 address.
 * @{
 */
typedef struct  {
  int family;
  union {
    struct in_addr  ipv4;
    struct in6_addr ipv6;
  } addr;
}
  IPADDRESS;

int  IPADDRESS_any( IPADDRESS *addr, int family );

int  IPADDRESS_loopback( IPADDRESS *addr, int family );

void IPADDRESS_broadcast_ipv4( IPADDRESS *addr );

int  IPADDRESS_parse_string( IPADDRESS *addr, const char *str );

char * IPADDRESS_to_string( IPADDRESS *addr );

M_INLINE int IPADDRESS_family( IPADDRESS *addr) {
  return addr->family;
}


/**
 * @}
 */

/**
 * @defgroup SOCKADDR
 * @brief class for transparent treatment of port / ip address pairs. enacapsulates either sockaddr_in or sockaddr_in6
 *
 * @{ 
 */
typedef union {
  struct sockaddr_in6 addr_ipv6; 
  struct sockaddr_in  addr_ipv4; 
} SOCKADDR;


/** 
 * @brief initialise ip address / port pair.
 */
int SOCKADDR_init( SOCKADDR *saddr, IPADDRESS *addr, uint16_t  port );

/**
 * @brief return addressing family of object (AF_INET or AF_INET6)
 */
int SOCKADDR_family( SOCKADDR *addr );

/**
 * @brief return address of ip address portion of address / port pair.
 */
void * SOCKADDR_ipaddr( SOCKADDR * addr );


/*
 * @brief friendly cast to struct sockaddr
 */
M_INLINE struct sockaddr *SOCKADDR_saddr( SOCKADDR *addr )
{
  return (struct sockaddr *) addr;
}

/**
 * @brief return address of address / port pair (depends on addressing family)
 */
socklen_t SOCKADDR_length( SOCKADDR * addr );

/**
 * @brief formats objec as string
 */
char * SOCKADDR_to_string( SOCKADDR *saddr );

/**
 * @}
 */

#endif



