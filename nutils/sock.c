
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "sock.h"
#include "ioutils.h"

int SOCK_attach( SOCKCTX *ctx , int fd, int verbose, int flags)
{
  ctx->verbose = verbose;
  ctx->connected = 0;

  ctx->addr = 0;
  ctx->addr_size = 0;

  disable_sigpipe();
 
  ctx->fd = fd;
 
 if ( fd_set_blocking( ctx->fd, 0 ) ) {
     if (ctx->verbose) {
       fprintf(stderr, "Failed to make socket non blocking errno %d\n", errno);
     }
  }

  if ((flags & SOCKCTX_FLAGS_NAGLE_ON) == 0) {  
    if ( fd_set_nagling( ctx->fd, 1 ) ) {
       if (ctx->verbose) {
         fprintf(stderr,"Failed to set nodelay option (set nodelay option). errno %d\n",errno);
       }    
    }
  }

  ctx->close_on_peer_close = 1; 
  if (flags & SOCKTCX_FLAGS_DONT_CLOSE_ON_PEER_CLOSE) {
    if ( fd_set_linger_option( ctx->fd, 1, LINGER_OPTION_VALUE) ) {
      if (ctx->verbose) {
       fprintf(stderr,"Failed to set linger option. %d\n", errno );
      }
    }
    ctx->close_on_peer_close = 0; 
  }

  return 0;
}


int SOCK_init( SOCKCTX *ctx , int verbose, int flags)
{
  int fd;
 
  fd = socket( PF_INET, SOCK_STREAM, 0 );
  if (fd == -1) {
     if (verbose) {
       fprintf(stderr, "Failed to create socket %d\n", errno);
     }
     return -1;
  }

  return SOCK_attach( ctx , fd, verbose, flags);
}
 

int SOCK_send_buffer_sizes( SOCKCTX *ctx, int read_buffer_size, int write_buffer_size)
{
  if (read_buffer_size > 0) {
    if (fd_set_buf_size( ctx->fd, Receive_buffer, read_buffer_size)) {
      fprintf(stderr,"Failed to set receive buffer size to %d\n",read_buffer_size);
      return -1;
    }
  }
  
  if (write_buffer_size > 0) {
    if (fd_set_buf_size( ctx->fd, Send_buffer, write_buffer_size)) {
      fprintf(stderr,"Failed to set send buffer size to %d\n",write_buffer_size);
      return -1;
    }
  }
  return 0;  

}


int SOCK_connect( SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout)
{
  int rt;
  fd_set wset,eset;
  struct timeval tv;
  int error = 0;
  int len;
    
  if (ctx->connected) {
    return 0;
  }

  if (addr != 0) {
     ctx->addr = malloc( addr_size );
     if (!ctx->addr) {
       goto err;
     }
     memcpy( ctx->addr, addr, addr_size );
     ctx->addr_size = addr_size;
  }

  if (!ctx->addr) {
    goto err;
  }

  do {
    rt = connect( ctx->fd, (struct sockaddr *) ctx->addr, ctx->addr_size );
  } while( rt == -1 && errno == EINTR);

  if (rt == -1) {


#if 0
     if (errno != EINPROGRESS) {
        if (ctx->verbose) {
          fprintf(stderr,"Connect syscall failed. %d\n", errno );
        }
 	goto err;
     }
#endif

     FD_ZERO(&wset);
     FD_SET(ctx->fd,&wset);
     eset = wset;

     tv.tv_sec = connect_timeout;
     tv.tv_usec = 0;

     do {
       rt = select( ctx->fd + 1, 0, &wset, &eset, &tv ); 
     } while( rt == -1 && errno == EINTR);
     
     if (rt <= 0) {
        if (ctx->verbose) {
          fprintf(stderr,"Connect timed out. %d\n", errno );
        }
        goto err;
     }

     if (FD_ISSET( ctx->fd, &wset) ||  FD_ISSET( ctx->fd, &eset)) {
       len = sizeof(error);
       error = 0;
       if (getsockopt( ctx->fd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *) &len ) || error) {
         if (ctx->verbose) {
           fprintf(stderr,"Connect socket error. %d errno %d\n", error, errno );
         }
 	 goto err;
       }
     }

     if (! FD_ISSET( ctx->fd, &wset)) {
         if (ctx->verbose) {
           fprintf(stderr,"Still not connected. errno %d\n", errno );
         }
 	 goto err;
     }
  }


  ctx->connected = 1;
  return 0;

err:
  SOCK_close(ctx);
  return -1;
}

int SOCK_wait_for_read_event( SOCKCTX *ctx, int read_timeout )
{
  int rt;
  fd_set rset;
  struct timeval tv;

  FD_ZERO(&rset);
  FD_SET(ctx->fd,&rset);

  tv.tv_sec = read_timeout;
  tv.tv_usec = 0;

   do {
     rt = select( ctx->fd + 1, &rset, 0, 0, &tv ); 
   } while( rt == -1 && errno == EINTR);
	

   if (rt == -1) {
     if (ctx->verbose) {
       fprintf(stderr, "select failed. %d\n", errno );
     }
     return -1;
   }
     
   if (rt == 0) {
     if (ctx->verbose) {
        fprintf(stderr,"read timed out\n" );
     }
     return 1;
   }

   return 0;
}


int SOCK_recv( SOCKCTX *ctx, void *msg, size_t length, int read_timeout )
{
  int rt;
     
retry:
     do {
       rt = recv( ctx->fd, msg, length, 0 );
     } while( rt == -1 && errno == EINTR );

     if (rt == -1) {
          
        if (errno != EAGAIN) {
           if (ctx->verbose) {
             fprintf(stderr,"recv failed errno %d\n", errno );
           }
           goto err;
        }

	rt = SOCK_wait_for_read_event( ctx, read_timeout ); 
	if (rt == 0) {
	   goto retry;
	}
	return -1;

     }

   if (rt == 0 && ctx->close_on_peer_close  ) {
      SOCK_close( ctx );
   }
   return rt;

err:
   return -1;
}	 

int SOCK_recv_all( SOCKCTX *ctx, void *msg, size_t length, int read_timeout )
{
  size_t pos;
  int rt;


  for(pos = 0; pos < length; ) {
      rt = SOCK_recv( ctx, msg, length, read_timeout );
      if (rt <= 0) {
	return rt;
      }
      pos += rt;
  }
  return pos;
}


int SOCK_send( SOCKCTX *ctx, void *bmsg, size_t length, int write_timeout )
{
  int rt;
  size_t pos;
  fd_set wset;
  struct timeval tv;
  char *msg = bmsg;

  for( pos = 0; pos < length; ) {

     do {
       rt = send( ctx->fd, msg + pos, length - pos, 0 );
     } while( rt == -1 && errno == EINTR );

     if (rt == -1) {
          
        if (errno != EAGAIN) {
           if (ctx->verbose) {
             fprintf(stderr,"send failed errno %d\n", errno );
           }
           goto err;
        }

 	FD_ZERO(&wset);
        FD_SET(ctx->fd,&wset);

        tv.tv_sec = write_timeout;
        tv.tv_usec = 0;

        do {
          rt = select( ctx->fd + 1, 0, &wset, 0, &tv ); 
        } while( rt == -1 && errno == EINTR);
	

	if (rt == -1) {
	  if (ctx->verbose) {
	    fprintf(stderr, "select failed. %d\n", errno );
	  }
	}
     
        if (rt == 0) {
           if (ctx->verbose) {
             fprintf(stderr,"send timed out\n" );
           }
       	   goto err;
        }
     }
     pos += rt;
   }
   return pos;

err:
   return -1;
}	 

int SOCK_close( SOCKCTX *ctx )
{
  if (ctx->fd != -1) {
    if (close(ctx->fd)) {
      if (ctx->verbose) {
	 fprintf(stderr,"Close failed errno %d\n", errno);
      }
    }
    ctx->fd = -1;
    ctx->connected = 0;
  }
  return 0;
}

int SOCK_close_with_reset( SOCKCTX *ctx )
{
  if (ctx->fd != -1) {
    if (fd_close_by_RST( ctx->fd )) {
      if (ctx->verbose) {
        fprintf(stderr,"Failed to set linger option. %d\n", errno );
      }
    }
  }
  ctx->fd = -1;
  ctx->connected = 0;
  return 0;
}
  

