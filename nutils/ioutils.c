#include "ioutils.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

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

  getsockopt( fd, SOL_SOCKET, SO_ERROR, (char *) &err, &sz );
	
  return err;
}

int fd_set_buf_size( int fd, Buffsize_op op, int size)
{
  if (setsockopt( fd,
		    SOL_SOCKET,
		    op == Send_buffer ? SO_SNDBUF : SO_RCVBUF,
		    (const char *) &size,
		    sizeof(int)) < 0) {
     return -1;
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
		 &ln) < 0) {
    return -1;
  }
  return size;
	
}

int fd_set_reuse_address(int fd, int reuse_on)
{
  int flag_on = reuse_on ? 1 : 0;

  if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &flag_on, sizeof(flag_on))) < 0) {
    return -1;
  }
  return 1;  
}

int fd_get_reuse_address(int fd) 
{
  int flag_on;
  socklen_t sz = sizeof(flag_on);

  if (getsockopt(fd, SOL_SOCKET, SO_REUSEADDR , (char *) &flag_on, &sz) < 0) {

    return -1;
  }  
  return flag_on;
}

