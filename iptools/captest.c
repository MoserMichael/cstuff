#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

#ifdef __linux__
#include <linux/net.h>
#endif

#include "loop_lib.h"


CAPTURECTX *capture_ctx;
const char *out_file;
const char *filter;
struct in_addr tx_interface;
const char *addr_name;
int capmode = 1;
extern void print_help();


void error_msg(const char *fmt, ... )
{
  va_list ap;
  char msg[ 1024 ];

  va_start( ap, fmt );
  vsnprintf( msg, sizeof(msg), fmt, ap );
  va_end( ap );

  fprintf( stderr, "ERROR: %s\n", msg ); 
  exit(1);   
} 

void parse_cmd_line(int argc, char *argv[]) {

  int c;
  int has_out_file = 0, has_tx_interface = 0;

  while ( (c = getopt(argc, argv, "hmw:f:i:")) != -1) {
     switch(c) {
	case 'h':
	   print_help();
	   break;
        case 'w':
	   out_file = strdup(optarg);	
	   has_out_file = 1;
	   break;
	case 'f':
	   filter = strdup(optarg);
	   break;
	case 'm':
	   capmode = 0;
	   break;
	
	case 'i':
	   if (inet_pton( AF_INET, optarg, &tx_interface) <= 0) {
	      error_msg("Illegal format of -i <tx_interface> parameter. %s is not an IPv4 address expected", optarg);
	   }

   	   addr_name = is_interface_address( &tx_interface );
	   if (!addr_name) {
     		error_msg("ip address %s is not an address of a interface\n", optarg);
   	   }
	   has_tx_interface = 1;
	   break;
       }
   }

   if (! has_tx_interface) {
     error_msg("-i parameter is missing");	
   }
   if (! has_out_file) {
     error_msg("-w parametter is missing");
   }
 
}

	
void sigint_handler(int sig)
{
  (void) sig;


  fprintf(stderr,"sigint\n");

  if (capture_ctx) {
    live_capture_stop( capture_ctx ); 
  }

  //(void) signal(SIGINT, SIG_DFL);

}


int main(int argc, char *argv[]) 
{

  parse_cmd_line( argc, argv );

#ifdef __linux__
  signal(SIGINT, sigint_handler);
#endif



  capture_ctx = live_capture_init( 
		    addr_name,
		    filter, 
		    out_file,
		    capmode );

  if (capture_ctx) {
    live_capture_start( capture_ctx ); 
  }


  fprintf(stderr,
#if __WORDSIZE == 64
	"Test run finished.\n\t%ld Packets where captured.\n\tOutput file %s\n", 
#else
	"Test run finished.\n\t%lld Packets where captured.\n\tOutput file %s\n",
#endif
	capture_ctx ? capture_ctx->packets_captured : 0,
	out_file);


  return 0;
}
