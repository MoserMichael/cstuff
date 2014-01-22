#include <webby/webby.h>
#include <butils/logg.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

static WEBBY_CONFIG webby_cfg;
static WEBBY *webby;


typedef HTTP_SERVLET * (* SERVLET_FACTORY) ();
typedef HTTP_FILTER * (* FILTER_FACTORY) ();



void print_help();
int  add_servlet( const char *lib_name, const char *fact_function);
int  add_filter( size_t vhost_id, const char *lib_name, const char *fact_function );
int  parse_servlet_definition( char *arg,  char **lib_name, char **fact_function);
int  parse_filter_definition( char *arg, size_t *vhost_id, char **lib_name, char **fact_function);


void parse_cmd_line(int argc, char *argv[]) {

  int c;
  char *tmp, *host_name, *cfg_file = 0, *lib_name, *fact_function;
  int port_num;
  size_t vhost_id;
  char stmp[ 20 ];

  while ( (c = getopt(argc, argv, "c:v:s:f:")) != -1) {
     switch(c) {
	case 'c': // server configuration
	  cfg_file = strdup( optarg );
	  break;

       case 'v':
       case 's':
       case 'f':
	  break;

       default:
	  print_help();
	  break;
     }
  }

  if (cfg_file) {
    if (WEBBY_CONFIG_load( &webby_cfg, cfg_file )) {
	     fprintf(stderr,"ERROR: invalid configuration file %s\n", cfg_file );
	     exit(1);
     }
     webby = WEBBY_init( &webby_cfg );
	  if (!webby) {
	     fprintf(stderr,"ERROR: Can't initialise web server.\n");
	     exit(1);
     }
  } else {
     fprintf(stderr,"ERROR: missing -c option\n");
     exit(1);
  }

  for( optind = 0;  (c = getopt(argc, argv, "hc:v:s:f:")) != -1 ; ) {
     if (c == 'v') {
	  tmp = strtok(optarg, ",");
	  if (tmp) {
	    host_name = strdup( optarg );	    
	  } else {
	    host_name = 0;
	  }
	  
	  if (!host_name == 0 || strcmp( host_name, "" ) == 0) {
	     fprintf(stderr,"ERROR: -v option does not specify a host name " );
	     exit(1);
	  }
	  tmp = strtok( 0, ",");
	  if (tmp) {
	    port_num = atoi( tmp );
	    strncpy( stmp, tmp, sizeof(stmp) );
	  } else {
	    port_num = -1;
	    strcpy(stmp, "any");
	  }
	  

	  if (WEBBY_add_vhost( webby, host_name, port_num, &vhost_id )) {
	    fprintf(stdout,"Virtual host id %d with host %s port %s has been added\n", vhost_id, host_name, stmp );
	  }
     }
  }

  for( optind = 0;  (c = getopt(argc, argv, "hc:v:s:f:")) != -1 ; ) {
       if (c == 'f') { 	  
	  if (parse_filter_definition( optarg, &vhost_id, &lib_name, &fact_function)) {
            exit(1);
	  }
	  if (add_filter( vhost_id, lib_name, fact_function ) ) {
	    exit(1);
	  }
	  if (vhost_id != (size_t)  -1) {
	    fprintf(stdout,"Adding filter %s to virtual host id: %d", lib_name, vhost_id);
	  } else {
	    fprintf(stdout,"Adding filter %s to default virtual host\n", lib_name );
	  }
        }

	if (c == 's') {
	  if (parse_servlet_definition( optarg,  &lib_name, &fact_function)) {
            exit(1);
	  }
	  if (add_servlet( lib_name, fact_function ) ) {
	    exit(1);
	  }
	  fprintf(stdout,"Adding servlet %s\n", lib_name );
        }
      }
}


void *load_factory( const char *lib_name, const char *fact_function, const char *default_factory_function )
{
  void *tmp;
  void *ret;
 
  tmp = dlopen(lib_name, RTLD_NOW ); //RTLD_LAZY );
  if (!tmp) {
     fprintf(stderr,"ERROR: Can't load shared library %s, error %s\n", lib_name, dlerror() );
     exit(1);
  }
  if (!fact_function) {
    fact_function = default_factory_function;  
  }
  ret =  dlsym(tmp, fact_function );
  if (!ret) {
    fprintf(stderr,"ERROR: Can't find method %s in shared library %s, error %s\n", fact_function, lib_name, dlerror() );
    exit(1);
  }
  return ret;   
}

//servlet definition: <shared library name>[,<servlet factory function>]
int  parse_servlet_definition( char *arg,  char **lib_name, char **fact_function)
{ 
  *lib_name = strtok( strdup(arg), "," );
  if (!lib_name) {
    exit(1);
  }
  *fact_function = strtok( 0, "," );
  if (! *fact_function) {
    *fact_function = "load_servlet";
  }
  return 0;
}

int is_digits( char *str )
{
  for(;*str != '\0'; ++str) {
    if (! isdigit( (unsigned char) *str) ) {
      return 0;
    }
  }
  return 1;
}

//filter definition: [<virtual host id>],<shared library name>[,<filter factory function>]
int  parse_filter_definition( char *arg, size_t *vhost_id, char **lib_name, char **fact_function)
{
  char *tok[3];
  int i;

  for(i = 0; i < 3 ; i++ ) {
    tok[i] = 0;
  }

  for(i = 0; i < 3 ; i++ ) {
    tok[i] = strtok( i == 0 ? strdup(arg) : 0, ",");
    if (tok[i] == 0) {
      break;
    }
  }

  if (i == 3) {
   *vhost_id = atoi( tok[0] );
   *lib_name = strdup( tok[1] );
   *fact_function = strdup(tok[2]);
   return 0;
  }

  if (i == 2) {
    i = 0;
    if (is_digits( tok[0] ) ) {
      *vhost_id = atoi( tok[0] );
      i = 1;
    } else {
     *vhost_id = -1;
    }

    *lib_name = strdup( tok[ i ] );
    i += 1;

    if (tok[i] != 0) {
      *fact_function = strdup( tok[ i ] );
    } else {
      *fact_function = "load_filter";
    }
  }
  return 0;
}



int add_servlet( const char *lib_name, const char *fact_function)
{
  HTTP_SERVLET *servlet;
  SERVLET_FACTORY servlet_factory;
  
  servlet_factory = (SERVLET_FACTORY) load_factory( lib_name, fact_function, "load_servlet" );

  servlet = servlet_factory();
  if (!servlet) {
    fprintf(stderr,"ERROR: shared library %s, factory method %s did not create a servlet\n", lib_name, fact_function );
    exit(1);
  }

  WEBBY_add_servlet( webby, servlet );
  return 0;
}   

int  add_filter( size_t vhost_id, const char *lib_name, const char *fact_function )
{
  FILTER_FACTORY filter_factory;
  HTTP_FILTER *filter;

  filter_factory = (FILTER_FACTORY) load_factory( lib_name, fact_function, "load_filter" );

  filter = filter_factory();
  if (!filter) {
    fprintf(stderr,"ERROR: shared library %s, factory method %s did not create a filter\n", lib_name, fact_function );
    exit(1);
  }

  WEBBY_add_filter(  webby, vhost_id, filter );
  return 0;
}



void start_server()
{
  if (WEBBY_run( webby )) {
    fprintf(stderr,"ERROR: failed to start webby the web server\n" );
    exit(1);
  }
}


int main(int argc, char *argv[]) 
{
  MLOG_init( MLOG_LEVEL_INFO, MLOG_ACTION_CONSOLE, 0);
  parse_cmd_line( argc, argv );
  start_server();
  return 0;
}
