#ifndef __SOURCE_CHECK__H_
#define __SOURCE_CHECK__H_


#include <netinet/in.h>



int init_source_port_check(struct in_addr capture_host);
int check_source_port(int src_port);

#endif

