#include "ioutils.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <signal.h>

//static int has_disabled_sigpipe;

int disable_sigpipe()
{
  //if (!has_disabled_sigpipe) {
    if (signal(SIGPIPE,SIG_IGN) == SIG_ERR) {
      return -1;
    }
  //has_disabled_sigpipe = 1;
  //}

  return 0;
}

int  fd_set_blocking(int fd,  int is_blocking)
{
  unsigned long cmd = is_blocking ? 0 : 1;
  int res;

#ifdef _WIN32
  res = ioctlsocket( fd, FIONBIO, &cmd);
#else    
  res = ioctl( fd, FIONBIO, &cmd);
#endif
  return res;
}

int fd_get_bytes_available(int fd) 
{
  int ret = 0;

  ioctl( fd, FIONREAD,  &ret );
	
  return ret;
}

int fd_get_error(int fd) 
{
  int err = 0;

  socklen_t sz = sizeof(err);

  if (getsockopt( fd, SOL_SOCKET, SO_ERROR, (char *) &err, &sz ) != 0) {
    return -1;
  }
	
  return err;
}

int fd_set_buf_size( int fd, Buffsize_op op, int size)
{
  int rt;

  rt = setsockopt( fd,
		    SOL_SOCKET,
		    op == Send_buffer ? SO_SNDBUF : SO_RCVBUF,
		    (const char *) &size,
		    sizeof(int));
  if (rt != 0) {
     return rt;
  }
  return 0;
	
}

int fd_get_buf_size( int fd, Buffsize_op op )
{
  int size;
  socklen_t ln = sizeof(size);
	
  if (getsockopt(fd,
		 SOL_SOCKET,
		 op == Send_buffer ? SO_SNDBUF : SO_RCVBUF ,
		 (char *) &size,
		 &ln)  != 0) {
    return -1;
  }
  return size;
	
}

int fd_set_reuse_address(int fd, int reuse_on)
{
  int flag_on = reuse_on ? 1 : 0;
  int rt;

  rt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &flag_on, sizeof(flag_on)); 
  if (rt != 0) {
    return -1;
  }
  return 0;  
}

int fd_get_reuse_address(int fd) 
{
  int flag_on;
  socklen_t sz = sizeof(flag_on);

  if (getsockopt(fd, SOL_SOCKET, SO_REUSEADDR , (char *) &flag_on, &sz)  != 0) {

    return -1;
  }  
  return flag_on;
}


int fd_set_nagling(int fd, int on)
{
  if (on) {
    on = 1;
  }
  return setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on) );
}

int fd_set_linger_option(int fd, int on, int linger_value)
{
  struct linger l;
   
  l.l_onoff = on ? 1 : 0; 
  l.l_linger = linger_value;

  return setsockopt( fd, SOL_SOCKET, SO_LINGER, &l, sizeof(struct linger) );
}
 
int fd_close_by_RST( int fd )
{
  fd_set_linger_option( fd, 1, 0 );
  return close(fd);
}


int fd_make_tcp_listener(SOCKADDR *saddr, int backlog)
{
  int fd;
 
  fd = socket( SOCKADDR_family(saddr), SOCK_STREAM, IPPROTO_TCP );
  if (fd == -1) {
    return -1;
  }

  if (fd_set_reuse_address( fd, 1 )) {
    goto err;
  }

  if (bind( fd, SOCKADDR_saddr( saddr ), SOCKADDR_length( saddr )) ) {
    goto err;
  }

  if (listen( fd, backlog )) {
    goto err;
  }
  return fd;

err:
  close(fd);
  return -1;
}


