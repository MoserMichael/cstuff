#ifndef __WEBBY_IMPL_
#define __WEBBY_IMPL_

#include <stddef.h>

/**
 * @defgroup WEBBYIMPL
 * 
 * Functions that must be supplied by server implementaton at link time
 * 
 * @{
 */

struct tagWEBBY;
struct tagWEBBY_CONFIG;

typedef int (*PFN_WEBBY_impl_new)( struct tagWEBBY *srv, struct tagWEBBY_CONFIG *cfg, void **impl );
typedef int (*PFN_WEBBY_impl_shutdown)( void *impl );
typedef int (*PFN_WEBBY_impl_run)( void *impl );
typedef int (*PFN_WEBBY_impl_send_data)( void *conndata, void *data, size_t size );
typedef int (*PFN_WEBBY_impl_response_completed)(  void *conndata, int connection_close );


typedef struct tagWEBBY_impl_interface {
  PFN_WEBBY_impl_new impl_new;
  PFN_WEBBY_impl_shutdown impl_shutdown;
  PFN_WEBBY_impl_run impl_run;
  PFN_WEBBY_impl_send_data impl_send_data;
  PFN_WEBBY_impl_response_completed impl_response_completed;

} WEBBY_impl_interface;

/**
 * @}
 */

#endif

