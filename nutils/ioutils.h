#ifndef _IOUTILS_H_Y_Z_
#define _IOUTILS_H_Y_Z_

#include <nutils/addrutil.h>

/**
 * @defgroup IOUTILS
 * @brief functions for setting of common socket options.
 *
 * @{
 */


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
 * @brief get receive or send buffer size
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
 * @brief make socket for accepting connections (listening endpoint), with SO_REUSE_ADDRESS and given backlog length.
 */
int fd_make_tcp_listener(SOCKADDR *saddr, int backlog);



/* 
 * @}
 */
#endif


