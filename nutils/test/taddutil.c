#include <nutils/addrutil.h>
#include "vtest/vtest.h"
#include <string.h>


void ADDRUTIL_test()
{
  IPADDRESS addr;
  SOCKADDR saddr;
  char *tmp;

  VASSERT( ! IPADDRESS_any( &addr, AF_INET6 ) );

  VASSERT( ! SOCKADDR_init( &saddr, &addr, 51000  ) );

  tmp = SOCKADDR_to_string( &saddr );
  VASSERT( ! strcmp("[::]:51000" , tmp ) );
  free(tmp);

  //---
  VASSERT( ! IPADDRESS_any( &addr, AF_INET ) );

  VASSERT( ! SOCKADDR_init( &saddr, &addr, 51000 ) );

  tmp = SOCKADDR_to_string( &saddr );
  VASSERT( ! strcmp("0.0.0.0:51000" , tmp ) );
  free(tmp);
  
  //---
  VASSERT( ! IPADDRESS_parse_string( &addr, "2001:db8::1" ) );

  VASSERT( ! SOCKADDR_init( &saddr, &addr, 51000  ) );

  tmp = SOCKADDR_to_string( &saddr );
  VASSERT( ! strcmp("[2001:db8::1]:51000" , tmp ) );
  free(tmp);

  //---
  VASSERT( ! IPADDRESS_parse_string( &addr, "138.0.0.1" ) );

  VASSERT( ! SOCKADDR_init( &saddr, &addr, 51000  ) );

  tmp = SOCKADDR_to_string( &saddr );
  VASSERT( ! strcmp("138.0.0.1:51000" , tmp ) );
  free(tmp);


}
