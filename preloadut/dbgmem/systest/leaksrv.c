#include <nutils/ioutils.h>
#include <tutils/tstart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char  *str_dup_and_rev(char *str, size_t *len );

void *handle_conn(void *arg)
{
    char buf[ 4096 + 1 ];
    int fd = * (int*) arg;
    ssize_t ret;

    while( (ret = recv( fd, buf, sizeof(buf) - 1, 0)) != 0 ) {
	char *resp;
	size_t len;
	
	buf[ ret ] = '\0';
	resp = str_dup_and_rev( buf, &len );

	if (!resp) {
	  exit(1);
	}

	
	if (strcmp( resp, "exit") == 0) {
	  printf("Leak server receives exit message\n");
	  exit(1);
	}
	
	if (send( fd, resp, strlen(resp), 0 ) == -1) {
	  break;
	}
    }
    close(fd);
    return 0;
}

int main( int argc, char *argv[] )
{
    int fd;
    IPADDRESS   naddr;
    SOCKADDR addr;
    char *p;
    FILE *fp;

    if (argc < 3) {
      printf("Usage: <ip address> <port> - echo server for testing\n");
      exit(1);
    }

    disable_sigpipe();

    // make leak on startup.
    p = malloc( atoi( argv[ 2 ] ) );
    printf("Leak %p\n", p );

    if (IPADDRESS_parse_string( &naddr, argv[1] )) {
      printf("can't parse address %s\n", argv[1]);
      exit(1);
    }      
    
    SOCKADDR_init( &addr, &naddr, atoi( argv[2] ));

    fd = fd_make_tcp_listener( &addr, 50 );


    fp = fopen("mdbgleaksrv.pid","w");
    if (fp) {
      fprintf(fp,"%u",getpid());
      fclose(fp);
    }


    while( 1 ) {
	struct sockaddr_in in_addr;
	socklen_t raddrlen;
	pthread_t conn_thread;
	int fd_conn;

	raddrlen = sizeof( in_addr );
	fd_conn = accept(fd, (struct sockaddr *) &in_addr, &raddrlen);

	pthread_create_detached( &conn_thread, 0, handle_conn, &fd_conn );
    }
    return 0;
}
