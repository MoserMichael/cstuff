#ifndef _IOUTILS_H_Y_Z_
#define _IOUTILS_H_Y_Z_

/**
 * @defgroup IOUTILS
 * @brief functions for setting of common socket options.
 *
 * @{
 */


int fd_set_blocking(int fd,  int is_blocking);
int fd_get_bytes_available(int fd) ;
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

/* 
 * @}
 */
#endif


