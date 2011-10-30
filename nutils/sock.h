#ifndef __SOCKSOCK_H_
#define __SOCKSOCK_H_


/**
  @defgroup SOCKET
  @brief a usable socket abstraction

  When you create a socket(2) then it is blocking by default.
  This means that you can't specify a timeout while using the socket - that makes it useless for any practical purpose.

  This abstraction creates a non blocking socket, and you can use it in a blocking manner from a thread, alas you can specify timeouts.
  This way you can have 'one thread per connection' in a way that allows you to  set timeouts for read, write and connect primitives.

  In a way this wrapper replace one set of utterly incomprehensible default behavior (the Berkley socket api) with another set of behaviour (which maybe makes more sense, maybe it doesn't - a matter of taste, as many things are with computers)

  Still, the Berkley socket interface is supposed to create the illusion, that one can do TCP networking without understanding the protocols. That is an illusion, and one simply has to read 'TCP IP Illustrated' by R. Stevens in order to correct it.

* @{
*/
typedef struct tagSOCKCTX {

  int verbose;
  int connected;
  int close_on_peer_close;
  int fd;

  void *addr; 
  int addr_size;
  
} SOCKCTX;

#ifndef LINGER_OPTION_VALUE
#define LINGER_OPTION_VALUE 300
#endif

/**
  if flag is set then nagle algorithm is enabled - this is the opposite behaviour of normal sockets where nagle is enabled by default.

 The Nagle algorithm makes sense for high latency networks such as the internet; it does not make sense for low latency LAN networks. Here application performance is typically faster without the nagle algorithm. - this stuff therefore assumes
 that is more likely used in a local network.
 
 The Nagle algorithm http://en.wikipedia.org/wiki/Nagle%27s_algorithm tries to combine network writes into larger packets, If the application calls send then the data is not Immediately written out; instead it is gathered with the hope of accumulating data into larger packets, these packets are then sent on the Nagle algorithm's timer.

*/
#define SOCKCTX_FLAGS_NAGLE_ON 2

/**
  In 99% of all application protocols you will want to close the connection,
  once the peer closes it's side, and recv returns 0.

  Set this option if you do not want the socket closed, once the peer closes it's side of the connection, and recv returns 0;

  Setting this option means the following. Once the peer closes its connection, it is no longer able to
  send data over the connection, howeve the peer still expects to read outstanding data that will be send by the application,
  and the application will send this data after it has received the information that the peer closed its side.

  Also, if this options is set, then SO_LINGER option is enabled to the value of LINGER_OPTION_VALUE
  This means that when the socket is closed, and there is data that has to be written still, then close will block
  until the outstanding data has been acknowledged by the peer.
*/
#define SOCKTCX_FLAGS_DONT_CLOSE_ON_PEER_CLOSE 4


/**
@brief creates a socket and sets some options
@param ctx - pointer to socket object.
@param verbose
@param flags  - bitmask of SOCKTX_FLAGS_xxx values
*/
int SOCK_init( SOCKCTX *ctx , int verbose, int flags);

int SOCK_attach( SOCKCTX *ctx , int fd, int verbose, int flags);


/**
@brief creates a socket and sets some options
@param ctx - pointer to socket object.
@param verbose
@param flags  - bitmask of SOCKTX_FLAGS_xxx values
*/
int SOCK_init( SOCKCTX *ctx , int verbose, int flags);


/**
 @brief set read and write buffer sizes; a value of -1 for buffer size is ignored.
 @param ctx - pointer to socket object.
 @param read_buffer_size
 @param write_buffer_size
 */
int SOCK_send_buffer_sizes( SOCKCTX *ctx, int read_buffer_size, int write_buffer_size);

/**
 @brief connects a sockets with timeout (in seconds)
 @param ctx - pointer to socket object.
 @param addr
 @param addr_size
 @param connect_timeout
 */
int SOCK_connect( SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout);

/**
 @brief read some data from a socket with timeout (in seconds)
 @param ctx - pointer to socket object.
 @return either one of: -1 on error, 0 if socket has been closed, number of bytes read.
 */
int SOCK_recv( SOCKCTX *ctx, void *msg, size_t length, int read_timeout );

/**
 * @brief wait for a read event to occur, if one occurs until timeout, then 0 is returned, if no event occured then 1 is returned, -1 is returned on socket error.
 */
int SOCK_wait_for_read_event( SOCKCTX *ctx, int read_timeout );

/**
 @brief read whe whole buffer from a socket with timeout (in seconds)
 @param ctx - pointer to socket object.
 @return either one of: -1 on error, 0 if socket has been closed during receiving buffer, length on success.
*/
int SOCK_recv_all( SOCKCTX *ctx, void *msg, size_t length, int read_timeout );

/**
 @brief write whe whole buffer from a socket with timeout (in seconds)
 @param ctx - pointer to socket object.
 */
int SOCK_send( SOCKCTX *ctx, void *msg, size_t length, int write_timeout );

/**
 @brief close the 
 @param ctx - pointer to socket object.
socket
 */
int SOCK_close( SOCKCTX *ctx );

/**
@brief ungraceful connection termination.

This function avoids TIME_WAIT when initiating closing of connection.

TIME_WAIT occurs, when the socket has been shut down properly; the application has sent FIN to the peer, and received an ACK for this message; the application has now sent the final last ACK to the peer; the connection is now in TIME_WAIT state and remains so for quite some time. Why? The last ACK might get lost, so TCP wants to make sure that no retransmissions of the last ACK are required.

For high performance servers this can mean that we will run out of socket handles.

The solution is for the client to terminate the connection ungracefully; instead of sending a FIN packet, the application can send out a RST (reset) packet, this will avoid the whole protocol of closing connections.

Here the SO_LINGER option is set with timeout of 0 prior to calling close; this results in sending out the RST packet / terminates the connection ungracefully.

Note that there is still a chance that the RST packet will get lost; in this we can only hope that the server will close it's side of the connection due to some connection idle timeout.
 @param ctx - pointer to socket object.
*/

int SOCK_close_with_reset( SOCKCTX *ctx );

/**
 * @}
 */


#endif

