#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <pthread.h>


#ifdef __linux__
#include <linux/net.h>
#endif

#include "subst.h"
#include "loop_lib.h"
#include "hex.h"
#include "sourcecheck.h"


extern void print_help();

#define MAX_RULES 20

#define UNUSED_ARGS(x) (void) (x);

typedef struct tagPARAMS {

  struct in_addr capture_host;

  struct in_addr tx_interface;
  const char *tx_interface_name;
  int    tx_interface_mtu;

  SUBST_RULE subst_rules[ MAX_RULES ];
  int subst_rules_offset;
  int subst_rules_count;

  const char *capture_file;
  const char *filter_expression;

  const char *output_capture_file;
  const char *output_capture_filter;


  int is_dry_run;
  int verbose;
  int no_capture;
  IP_FRAGMENT_ACTION fragment_action;

} PARAMS;

                    

PARAMS global_params;
int raw_sock;


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
  int has_capture_host = 0;
  int capture_file = 0, has_tx_interface = 0;


  memset( &global_params, 0, sizeof(PARAMS) );
  global_params.subst_rules_offset = global_params.subst_rules_count = 1;
  global_params.fragment_action = IP_FRAGMENT_REASSEMBLE;  
  
  while ( (c = getopt(argc, argv, "vnmhdx:r:c:i:f:y:o:")) != -1) {
     switch(c) {
	case 'h':
	  print_help();
	  break;

        case 'v':
	  global_params.verbose++;
 	  break;

	case 'd':
	  global_params.is_dry_run = 1;
	  break;
	
	case 'n':
	  global_params.fragment_action = IP_FRAMGENT_PASS_AS_IS;
	  break;

	case 'm':
	  global_params.no_capture = 1;
	  break;
	
	case 'x':	   
	  global_params.capture_file = strdup( optarg );
          capture_file = 1;
	  break;
	
	case 'o':
	  global_params.output_capture_file = strdup( optarg );
	  break;
  
	case 'y':
	  global_params.output_capture_filter = strdup( optarg );
	  break;

	case 'r':
	   if (global_params.subst_rules_count < MAX_RULES) {
	      compile_substitution_rule( &global_params.subst_rules[ global_params.subst_rules_count ], optarg );
              global_params.subst_rules_count ++;
	   } 
	   break;
	
	case 'c':
           has_capture_host = 1;
	   if (inet_pton( AF_INET, optarg, &global_params.capture_host) <= 0) {
	      error_msg("Illegal format of -c <capture_host> parameter. IPv4 address expected");
	   };
	   break;

	case 'i':
           has_tx_interface = 1;
	   if (inet_pton( AF_INET, optarg, &global_params.tx_interface) <= 0) {
	      error_msg("Illegal format of -i <tx_interface> parameter. IPv4 address expected");
	   }
	   break;

	case 'f': 
	   global_params.filter_expression = strdup( optarg );
	   break;
      }
  }

  if (!capture_file) {
     error_msg("Capture file missing. please specify -c <file name> parameter.\n");
  }
	
  if (!has_capture_host) {
     error_msg("Capture host missing. Please specify -c <ipv4 address> parameter.\n");
  }

  if (!has_tx_interface) {
     if ( is_interface_address( &global_params.capture_host ) == 0) {
       error_msg("tx interface missing and capture host is not an interface on local machine. Please specify -i <ipv4 address> parameter.\n");                                                                                                                                            
     } else {
       printf("Capture host is equal to tx_interface\n");
       global_params.tx_interface = global_params.capture_host;      	
       global_params.subst_rules_offset = 1;
     }
   } else {
     MAKE_SOURCE_RULE( &global_params.subst_rules[0], global_params.capture_host, global_params.tx_interface );
     global_params.subst_rules_offset = 0;
   }



  if ( ( global_params.tx_interface_name = is_interface_address( &global_params.tx_interface ) ) == 0 ) {
     error_msg("tx interface value (-i parameter) is not a network interface on this host\n");
  }
  global_params.tx_interface_mtu  =  get_interface_mtu( &global_params.tx_interface );
  if (global_params.tx_interface_mtu == -1) {
    error_msg( "Can't get MTU size of outgoing interface %s\n", 
	    global_params.tx_interface_name );
  }
}



void dump_packet(void *data, size_t n_bytes)
{
  char *packet_dump;
	   
  packet_dump = make_hex_dump( (const char *) data, n_bytes );	 
  if (packet_dump) {
      fprintf(stderr,"Send Packet\n%s\n", 
	    packet_dump);
      free(packet_dump);
  }
}


#if 0
static void sigchld_hdl (int sig)
{
  (void) sig;
  while (waitpid(-1, NULL, WNOHANG) > 0) {
  }
}
 
void do_exec(const char *cmd_line) 
{
   char *args[ 200 ];
   char *path, *cpy, *tok;
   int i;

   path = strdup( cmd_line );
   tok = strtok( cpy, " \t" );
      
   for(i = 0, tok = strtok( 0, " \t" ); tok ; i++, tok = strtok( 0, " \t" ) ) {
     args[ i] = tok;
   } 
   args[i] = 0;
   execv( path, args ); 
   
   fprintf(stderr,"execv of %s failed. errno=%d\n", path, errno);
   exit(1);   
}


static pid_t tcpdump_pid;

void install_capture_filter() 
{
  char cmd_line[ 4096 ];
  char filter_cmd[ 1024 ];

  sprintf(filter_cmd, "udp and dst %s", inet_ntoa( global_params.tx_interface ) );

  sprintf(cmd_line, "tcpdump -i %s -w 65535 -w %s.in.cap %s", 
		global_params.tx_interface_name,
		global_params.capture_file, 
		filter_cmd );

  

  signal(SIGCHLD, sigchld_hdl ); 
  if ((tcpdump_pid = fork()) == 0) { 
     do_exec( cmd_line );
  } 

}

void stop_capture_filter()
{
   kill(tcpdump_pid,SIGTERM);
}

#endif


#define CAPTURE_SUFFIX ".in.cap"

static CAPTURECTX *capture_ctx;
static pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void * capture_filter_thread( void * arg )
{

  char filter_cmd[ 1024 ];
  char *file_path;

  UNUSED_ARGS(arg);



  if (global_params.output_capture_filter)  {
     strcpy(filter_cmd,"udp and ");
     strcat(filter_cmd, global_params.output_capture_filter );
  } else {
     sprintf(filter_cmd, "udp and dst %s", 
     inet_ntoa( global_params.tx_interface ) );
  } 
  
  if (global_params.output_capture_file == 0) {
    file_path = (char *) malloc( 
      strlen( global_params.capture_file )  + 1 + strlen(CAPTURE_SUFFIX) );
    strcpy( file_path, global_params.capture_file );
    strcat( file_path, CAPTURE_SUFFIX );
  } else {
    file_path = (char *) global_params.output_capture_file;
  } 



  capture_ctx = live_capture_init( 
		    global_params.tx_interface_name,
		    filter_cmd, 
		    file_path,
		    1 );



  if (global_params.verbose) {
    fprintf(stderr,"Capture started.\n\tDevice:\t%s\n\tDevice MTU:\t%d\n\tFilter:\t%s\n\tOutput:\t%s\n", global_params.tx_interface_name, global_params.tx_interface_mtu, filter_cmd, file_path );
  }

  pthread_mutex_lock( &count_mutex );
  pthread_cond_signal( &condition_var );
  pthread_mutex_unlock( &count_mutex );

  if (capture_ctx) {
    live_capture_start( capture_ctx ); 
  }

  return 0;
}


void install_capture_filter()
{
  pthread_t th;

  pthread_create( &th, 0, capture_filter_thread, 0 );

  pthread_mutex_lock( &count_mutex );
  pthread_cond_wait( &condition_var, &count_mutex );
  pthread_mutex_unlock( &count_mutex );

  if (capture_ctx == 0) {
    error_msg("Could not start capture on device %s", global_params.tx_interface_name);
  }
 
}

void stop_capture_filter()
{
  if (capture_ctx) {
    live_capture_stop( capture_ctx ); 
  }

}



#define IP_LEN(x) ((int)  (x)->ip_hl)


static struct timeval first_time, first_time_current;
static int is_first_packet;
static uint64_t sent_packet_count;

int my_nanosleep(const struct timespec *req, struct timespec *rem);


void delay_packet( struct timeval tv )
{
   struct timeval packet_due, now_time, wait_time, tmp_tm;  
   struct timespec wait_time_spec, rem_time;
   

   timersub( &tv, &first_time, &tmp_tm );
   timeradd( &tmp_tm, &first_time_current, &packet_due);

   gettimeofday( & now_time, 0);

   if (timercmp( &packet_due, &now_time, > ) ) {
      timersub( &packet_due, &now_time,  &wait_time );

      wait_time_spec.tv_sec = wait_time.tv_sec;
      wait_time_spec.tv_nsec = 1000 * wait_time.tv_usec;


      //fprintf( stderr, "%ld.%06ld \n",  wait_time_spec.tv_sec, wait_time_spec.tv_nsec );

      do {
       if (my_nanosleep( &wait_time_spec, &rem_time ) != -1 || errno != EINTR) {
         break;
       }
       wait_time_spec = rem_time;

      } while(1);
   }
}

int transform_packet( int64_t count, const struct pcap_pkthdr *pkthdr, struct ip *ip, struct udphdr *udp, const uint8_t *data, size_t size )
{
  int i;
  int modify_flag = 0;

  if (!is_first_packet) {
     gettimeofday( & first_time_current, 0);
     first_time = pkthdr->ts; 
     is_first_packet = 1;
  } 


  if (ip->ip_src.s_addr != global_params.capture_host.s_addr) {
    return -1;
  }

  for(i = global_params.subst_rules_offset; i < global_params.subst_rules_count; i++) {
    modify_flag = apply_substituion_rule( &global_params.subst_rules[ i ], ip, udp, modify_flag );
  }

  if (check_source_port( ntohl( UDP_SPORT(udp) ) ) ) {
    show_packet(stderr, count, &pkthdr->ts, ip, udp, data, size);
    error_msg("Can't send packet with source port %d, this udp port is already in use on this host.", ntohl( UDP_SPORT(udp) ) );
  }

  //if (modify_flag & MODIFY_FLAG_IP_MODIFIED) {
    ip->ip_sum = ip_cksum( ip, IP_LEN(ip) * 4 );
  //}
 
  //if (modify_flag & MODIFY_FLAG_UDP_MODIFIED) {
    if ( UDP_CKSM(udp) != 0) {
      UDP_CKSM(udp) = udp_cksum( ip, udp ); //udp, udp->len );
    }
  //}



  delay_packet( pkthdr->ts );

  if (global_params.verbose > 1) {
    dump_packet( (void *) ip, ntohs( ip->ip_len)  );
  }
  return 0;
}
 
void dump_udp_on_ipv4_replay_mode (PACKET_DATA *data) //int64_t count, const struct pcap_pkthdr *pkthdr, struct ip *ip, struct udphdr *udp, const uint8_t *data, size_t size)
{
  struct sockaddr_in sin;


  if (transform_packet( data->count, data->pkthdr, data->ip, data->udp,  data->udp_data, data->udp_data_size )) {
    return;
  }

  memset(&sin, 0, sizeof(sin) );
  sin.sin_family = AF_INET;
  sin.sin_port = UDP_DPORT( data->udp );
  sin.sin_addr.s_addr = data->ip->ip_dst.s_addr;

 
  if ( sendto( raw_sock, data->ip, ntohs( data->ip->ip_len),  0, (const struct sockaddr *) &sin, sizeof(sin) ) < 0) {
    show_packet(stderr, data->count, &data->pkthdr->ts, data->ip, data->udp,  data->ip, ntohs( data->ip->ip_len) );
    error_msg( "Failed to send packet. errno=%d\n", errno );
  } else {
    sent_packet_count ++;
  }
}

void stop_capture_filter();


void do_replay_mode()
{
  struct timeval start_time, finish_time, diff_time;
  

  if (! global_params.no_capture) { 
    install_capture_filter(); 
  } else {
    if (global_params.verbose) {
      fprintf(stderr, "No capture is performed during this run\n");
    }

  }

  if (global_params.verbose) {
    fprintf(stderr, "Starting simulation run\n");
  }

  raw_sock = socket (PF_INET, SOCK_RAW, IPPROTO_RAW);
  if (raw_sock == -1) {
     error_msg("Failed to create raw socket. errno=%d\n", errno );
  }


  {
    int one = 1;
    const int *val = &one;
    if (setsockopt (raw_sock, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
      error_msg("Warning: Cannot set HDRINCL! errno=%d\n", errno );
    }
  }

#if 0
#ifdef __linux__
  // how do you do that on cygwin ???
  if (setsockopt( raw_sock, SOL_SOCKET, SO_BINDTODEVICE, global_params.tx_interface_name, 4) < 0) {
    error_msg("Can't bind raw socket to interface %s. erno=%d", global_params.tx_interface_name, errno );
  }

#else
  { 
     struct sockaddr_in bind_addr;

     memset( &bind_addr, 0, sizeof( bind_addr ) );

     bind_addr.sin_family = AF_INET;
     bind_addr.sin_addr.s_addr = global_params.tx_interface.s_addr;

     if (bind( raw_sock, (struct sockaddr *) &bind_addr, sizeof( bind_addr ) ) ) {
        error_msg("Can't bind raw interface, errno=%d", errno );
     }
  }
#endif
#endif

  gettimeofday( &start_time, 0);
  inspect_udp_on_ipv4( global_params.capture_file, global_params.filter_expression, global_params.fragment_action , dump_udp_on_ipv4_replay_mode );
  gettimeofday( &finish_time, 0);
  timersub( &finish_time, &start_time, &diff_time);
  close( raw_sock );

  if (global_params.verbose) {
    fprintf(stderr,
#if __WORDSIZE == 64
		   "Finished sending of packets.\n"
		   "\tTotal of %ld packets have been sent\n"
		   "\tTest run took %ld.%06ld seconds\n" 
		   "Waiting for last packets to arrive...\n",
#else 
		   "Finished sending of packets.\n"
		   "\tTotal of %lld packets have been sent\n"
		   "\tTest run took %ld.%06ld seconds\n" 
		   "Waiting for last packets to arrive...\n",

#endif
		sent_packet_count++,
		diff_time.tv_sec,
		diff_time.tv_usec
	       );
  }

  sleep(5);
  
  if (global_params.verbose) {
    fprintf(stderr,
#if __WORDSIZE == 64
	"Test run finished.\n\t%ld Packets where captured.\n", 
#else
	"Test run finished.\n\t%lld Packets where captured.\n", 
#endif
	capture_ctx ? capture_ctx->packets_captured : 0);
  }
  stop_capture_filter();

}


void dump_udp_on_ipv4_dry_run (PACKET_DATA *data) //int64_t count, const struct pcap_pkthdr *pkthdr, struct ip *ip, struct udphdr *udp, const uint8_t *data, size_t size)
{
  if (transform_packet( data->count, data->pkthdr, data->ip, data->udp,  data->udp_data, data->udp_data_size )) {
    return;
  }
}

void do_dry_run_mode()
{
  inspect_udp_on_ipv4( global_params.capture_file, global_params.filter_expression,  global_params.fragment_action, dump_udp_on_ipv4_dry_run );
}



int main(int argc, char *argv[])
{
  parse_cmd_line( argc, argv );            

  if (init_source_port_check(global_params.capture_host)) {
    error_msg("Failed to get list of source ports. errno=%d",errno);
  }
 
  if (global_params.is_dry_run) {
    do_dry_run_mode();
  } else {
    do_replay_mode();
    sleep(5);
    stop_capture_filter();
  }

  return 0;  
}
