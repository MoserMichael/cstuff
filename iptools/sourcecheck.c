#define _GNU_SOURCE
#include <cutils/bhash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "sourcecheck.h"

#ifdef __linux__
#define N_COMMAND "netstat -anu"
#else
#define N_COMMAND "netstat -a -p udp"
#endif


typedef struct tagSOURCEPORTCHECK
{
	HASH_Entry link;
	int source_port;
}
	SOURCEPORTCHECK;

static int hash_compare(HASH_Entry *entry, void * key, ssize_t key_length)
{
	SOURCEPORTCHECK *lhs;

        (void) key_length;

	lhs = (SOURCEPORTCHECK *) entry;

	if (lhs->source_port == * ((int *) key) ) {
		return 0;
	}
	return 1;
}

static HASH source_check_hash;

int init_source_port_check(struct in_addr capture_host)
{
  FILE *in;
  char *line = 0,*sport,*sip,*tok;
  size_t len;
  struct in_addr src_addr;
  int   src_port;
  SOURCEPORTCHECK *entry;

  in = popen( N_COMMAND, "r");
  if (in == 0) {
    return -1;
  }


  if (HASH_init( &source_check_hash, 100, 0, hash_compare, 0) ) {
    return -1;
  }

  while( getline( &line, &len, in) != -1) {

     tok = strtok(line," \t");
     if (strcmp(tok,"udp") != 0 && strcmp(tok,"UDP") != 0) {
       continue;
     }

#ifdef __linux__
     tok = strtok( 0, " \t");
     if (tok == 0) {
       continue;
     }

     tok = strtok( 0, " \t");
     if (tok == 0) {
       continue;
     }

#endif


     tok = strtok( 0, " \t");
     if (tok == 0) {
       continue;
     }

     sip = strtok(tok,": ");	
     sport = strtok( 0, " "); 

     if (inet_pton( AF_INET, sip, &src_addr) <= 0) {  
       continue;
     }
     src_port = atoi(sport);

     if (capture_host.s_addr != src_addr.s_addr && src_addr.s_addr != INADDR_ANY) {
       continue;
     }
 
     entry = (SOURCEPORTCHECK *) malloc( sizeof(SOURCEPORTCHECK) );
     entry->source_port = src_port;

     HASH_insert( &source_check_hash , &entry->link, &src_port, sizeof(src_port) );

  }

  free(line);
  fclose(in);

  return 0;
}

int check_source_port(int src_port)
{
   if (HASH_find( &source_check_hash, &src_port, sizeof(src_port) ) != 0) {
     return -1;
   }
   return 0;
}
