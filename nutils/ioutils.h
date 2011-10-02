#ifndef _IOUTILS_H_Y_Z_
#define _IOUTILS_H_Y_Z_

#include <nutils/addrutil.h>

/**
 * @defgroup IOUTILS
 * @brief functions for setting of common socket options and doing stuff with sockets.
 *
 * @{
 */


/** 
 * @brief disable SIG_PIPE signal, 
 * checks for static flag so as not to call system call again and again.
 */
int disable_sigpipe();

/**
 * @brief set mode to blocking / non blocking.
 */
int fd_set_blocking(int fd,  int is_blocking);

/**
 * @brief return bytes available for reading
 */
int fd_get_bytes_available(int fd) ;

/**
 * @brief return socket error.
 */
int fd_get_error(int fd);

typedef enum _Buffsize_op {
	Receive_buffer,
	Send_buffer,
} Buffsize_op;

/**
 * @brief set receive or send buffer size
 */
int fd_set_buf_size( int fd, Buffsize_op op, int size);

/**
 * @brief return the receive or send buffer size
 */
int fd_get_buf_size( int fd, Buffsize_op op );

/**
 * @brief set reuse address option
 */
int fd_set_reuse_address(int fd, int reuse_on);

/**
 * @brief get reuse address option
 */
int fd_get_reuse_address(int fd);


/**
 * @brief set naggle algorithm (tcp no delay option)
 */
int fd_set_nagling(int fd, int on);

/**
 * @brief set linger option
 */
int fd_set_linger_option(int fd, int on, int linger_value);

/**
 * @brief ungracefull connection termination - send RST to peer;
 */
int fd_close_by_RST( int fd );

 
/**
 * @brief make socket for accepting connections (listening endpoint), with SO_REUSE_ADDRESS and given backlog length.
 */
int fd_make_tcp_listener(SOCKADDR *saddr, int backlog);



/* 
 * @}
 */
#endif


