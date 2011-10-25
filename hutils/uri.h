#ifndef __URI_X_Y_Z_
#define __URI_X_Y_Z_

#include <cutils/base.h>
#include <string.h>

// ===============================================================

/**
 * @defgroup URI
 * @brief uri parser
 *
 * @{
 */

#define URI_FLAGS_PARSE_PARTS     1
#define URI_FLAGS_IS_ABSOLUTE_URI 2
#define URI_FLAGS_IS_OPAQUE       4
#define URI_FLAGS_HOST_IPv4       8
#define URI_FLAGS_HOST_IPv6       16
#define URI_FLAGS_HOST_HOSTNAME   32
#define URI_FLAGS_HAS_SCHEME      64



typedef struct tagURI {
  int  flags;

  char *cdata_raw;
  char *cdata;
  char *cdata_is_escaped; // each bytes says if corresponding byte has been escaped. could do this as a bit array, alas.

  char *scheme;

  char *userinfo_raw;
  char *userinfo;
  
  char *host;
  int   port;
  
  char *path_raw;
  char *path;

  char *query_raw;
  char *query;
  
  char *fragment_raw;
  char *fragment;

  char *opaque_raw;
  char *opaque;

} URI;

M_INLINE void URI_init( URI *url )
{
  memset( url, 0, sizeof( URI ) );
  url->port = -1;
}



int URI_parse( URI *url, char *line);

M_INLINE void URI_free( URI *url) {
  if (url->cdata_raw) {
    free( url->cdata_raw );
  }

  if (url->cdata) {
    free(url->cdata);
  }

  if ( url->cdata_is_escaped ) {
    free( url->cdata_is_escaped );
  }
  URI_init( url );
}

M_INLINE const char *URI_scheme( URI *url )
{
  return url->scheme;
}

M_INLINE const char *URI_userinfo_raw( URI *url )
{
  return url->userinfo_raw;
}

M_INLINE const char *URI_userinfo( URI *url )
{
  return url->userinfo;
}

M_INLINE const char *URI_host( URI *url )
{
  return url->host;
}

M_INLINE int URI_port( URI *url )
{
  return url->port;
}


M_INLINE const char *URI_path_raw( URI *url )
{
  return url->path_raw;
}

M_INLINE const char *URI_path( URI *url )
{
  return url->path;
}

M_INLINE const char *URI_query_raw( URI *url )
{
  return url->query_raw;
}

M_INLINE const char *URI_query( URI *url )
{
  return url->query;
}

M_INLINE const char *URI_fragment_raw( URI *url )
{
  return url->fragment_raw;
}

M_INLINE const char *URI_fragment( URI *url )
{
  return url->fragment;
}

M_INLINE const char *URI_opaque_raw( URI *url )
{
  return url->opaque_raw;
}

M_INLINE const char *URI_opaque( URI *url )
{
  return url->opaque;
}

/**
 * @}
 */

#endif

