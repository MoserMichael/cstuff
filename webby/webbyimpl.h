#ifndef __WEBBY_IMPL_
#define __WEBBY_IMPL_

#include <webby/webby.h>

/**
 * @defgroup WEBBYIMPL
 * 
 * Functions that must be supplied by server implementaton at link time
 * 
 * @{
 */

int WEBBY_impl_new( WEBBY *srv, void **impl );
int WEBBY_impl_shutdown( void *impl );
int WEBBY_impl_run( void *impl );

int WEBBY_impl_send_data( void *conndata, void *data, size_t size );
int WEBBY_impl_response_completed(  void *conndata, int connection_close );

/**
 * @}
 */

#endif

