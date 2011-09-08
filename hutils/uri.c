#include "uri.h"
#include "charclass.h"
#include <string.h>
#include <arpa/inet.h>


char *strdup_range(char *from, char *to) {
  char *r;

  r = malloc( to - from + 1 );
  if (!r) {
    return 0;
  }

  strncpy( r, from, to - from  );
  r[ to - from ] = '\0';
  
  return r;
}


//  mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" | "(" | ")"
M_INLINE int is_mark(int8_t ch) {
   return ch == '-' || ch == '_' || ch == '.' || ch == '!' || ch == '~' || ch == '*' || ch == '\'' || ch == '(' || ch == ')';
}

//  unreserved    = alphanum | mark
 M_INLINE int is_unreserved(int8_t ch) {
    return is_alphanum(ch) || is_mark(ch);
}

// reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |  "$" | ","
 M_INLINE int is_reserved(int8_t ch) {
    return ch == ';' || ch == '/' || ch == '?' || ch == ':' || ch == '@' || ch == '&' || ch == '=' || ch == '+' || ch == '$' || ch == ',';
}


typedef struct tagURIPARSECTX {
  URI *rep; 
  char *cdata_pos,*cdata_pos_start;
  char *cdata_raw_pos;
} URIPARSECTX;


char *ctx_copy_string_raw( URIPARSECTX *ctx, char *start, char *end )
{
  char *ret = ctx->cdata_raw_pos;

  strncpy( ctx->cdata_raw_pos, start, end - start );
  ctx->cdata_raw_pos += end - start;
  * ctx->cdata_raw_pos ++ ='\0';

  return ret;
}

void ctx_add_escaped_char( URIPARSECTX *ctx, char ch )
{
  * ctx->cdata_pos ++ = ch;
}

char *ctx_finish_escaped_string( URIPARSECTX *ctx ) 
{
  char *ret = ctx->cdata_pos_start;
 
  * ctx->cdata_pos ++ = '\0';
  ctx->cdata_pos_start = ctx->cdata_pos;
 
 return ret;
}

void ctx_undo_escaped_string( URIPARSECTX *ctx )
{
  ctx->cdata_pos = ctx->cdata_pos_start;
}
 
// escaped       = "%" hex hex

M_INLINE int parse_escaped( URIPARSECTX *ctx,  char *ptr, char ** next )
{  
  int high,low;
  int unescaped_char;

  if (*ptr != '%') {
    return 1;
  }

  high = is_hex_ext( *(ptr + 1) );
  if (! high ) {
    return -1;
  }

  low = is_hex_ext( *(ptr + 2) );  
  if (! low ) {
    return -1;
  }
  
  unescaped_char = (high << 4) | low;
  if (! (unescaped_char >=0 && unescaped_char <= 0x1F) ) {
    ctx_add_escaped_char( ctx, unescaped_char );
  }

  *next = ptr + 3;
  return 0;
} 

//      uric          = reserved | unreserved | escaped
M_INLINE int parse_uric( URIPARSECTX *ctx, char *ptr, char **next )
{
   if (is_reserved( *ptr ) || is_unreserved( *ptr ) ) {
     ctx_add_escaped_char( ctx, *ptr );
     *next = ptr +1;
     return 0;
   }	
   return parse_escaped( ctx, ptr, next );
}


M_INLINE int parse_uric_sequence( URIPARSECTX *ctx, char *ptr, char **next ) 
{
  int rt;

  while( (rt = parse_uric(  ctx, ptr, next )) == 0 ) {
    ptr = *next;
  }
  return rt;
}

//    pchar         = unreserved | escaped |
//                      ":" | "@" | "&" | "=" | "+" | "$" | ","
M_INLINE int parse_pchar( URIPARSECTX *ctx, char *ptr, char **next)
{
  char ch = *ptr;

  if (is_unreserved( ch ) || ch == ':' || ch == '@' || ch == '&' || ch == '=' || ch == '+' || ch == '$' || ch == ',') {
    ctx_add_escaped_char( ctx, ch );
    *next = ptr + 1;
    return 0;
  }
  return parse_escaped(  ctx, ptr, next );
}

M_INLINE int parse_pchar_sequence( URIPARSECTX *ctx, char *ptr, char **next ) 
{
  int rt;

  while( (rt = parse_pchar( ctx, ptr, next )) == 0 ) {
    ptr = *next;
  }
  return 0;
}

//      segment       = *pchar *( ";" param )
M_INLINE int parse_segment( URIPARSECTX *ctx, char *ptr, char **next ) 
{
  if (parse_pchar_sequence( ctx, ptr, next ) < 0) {
    return -1;
  }
  
  ptr = *next;
  if (*ptr == ';') {
    ctx_add_escaped_char( ctx, ';' );
    ptr = *next = ptr + 1;
    if (parse_pchar_sequence( ctx, ptr, next ) < 0) {
      return -1;
    }
  }
  return 0;
}

//path_segments = segment *( "/" segment )
M_INLINE int parse_path_segments( URIPARSECTX *ctx, char *ptr, char **next ) {
  while (*ptr == '/') {
    ctx_add_escaped_char( ctx, '/' );
    ptr = *next = ptr + 1;
    if (parse_segment( ctx, ptr, next ) < 0) {
      return -1;
    }
    ptr = *next;
  }
  return 0;
}

M_INLINE int parse_ipv4_address( URIPARSECTX *ctx,  char *ptr, char **next ) {
  int i;

  char *start = ptr;

  for (i=0; i < 4; i++) {
    if (! is_digit(*ptr)) {
      return -1;
    }
    for( ++ ptr; is_digit( *ptr ); ++ptr );
    if ( i == 3) {
      break;
    }
    if (*ptr != '.') {
      return -1;
    } 
    ++ ptr;
  }

  *next = ptr;
  ctx->rep->flags |= URI_FLAGS_HOST_IPv4;  
  ctx->rep->host = ctx_copy_string_raw(ctx, start, ptr );

  return 0;
}

//[ userinfo "@" ] 
//userinfo      = *( unreserved | escaped |
//                   ";" | ":" | "&" | "=" | "+" | "$" | "," )
M_INLINE int parse_userinfo( URIPARSECTX *ctx, char *ptr, char **next)
{
    char *start = ptr;
    int rt;

    while ( *ptr  != '@' ) {
       if ( is_unreserved( *ptr ) || *ptr == ';' || *ptr ==  ':' || *ptr ==  '&' 
		|| *ptr ==  '=' || *ptr ==  '+' || *ptr ==  '$' || *ptr ==  ',') {
         ctx_add_escaped_char( ctx, *ptr );
	 ++ ptr;
	 continue;
       } 
       if ((rt = parse_escaped( ctx, ptr, next )) != 0) {
         ctx_undo_escaped_string( ctx );  
	 *next = start;
	 return rt;
       }  
       ptr = *next;
    }

    ctx->rep->userinfo_raw = ctx_copy_string_raw(ctx, start, ptr );
    ctx->rep->userinfo =  ctx_finish_escaped_string(ctx); 
    *next = ptr + 1;

    return 0;
}


M_INLINE int parse_domainlabel(  char *ptr, char **next )
{
   if (!is_alphanum( *ptr) ) {
     return -1;
   }
   for(ptr += 1; is_alphanum( *ptr ) || *ptr == '-'; ++ptr);
   
   if (!is_alphanum( *(ptr-1) ) ) {
     return -1;
   }
   *next = ptr;

   return 0;
}

//hostname      = *( domainlabel "." ) toplabel [ "." ]
//domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
//toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
M_INLINE int parse_hostname(  URIPARSECTX *ctx, char *ptr, char **next )
{
   char *last_component;
   char *start = ptr;

   for ( ; ; ) {
       last_component = ptr;
       if (*ptr == '/') {
         goto ok;
       }
       if (parse_domainlabel( ptr, next) < 0) {
         return -1;
       }
       ptr = *next;
 
       if (*ptr != '.') { 
         break;
       }
       ++ptr;
    }
    // check that last component is top label
    if (is_digit( * last_component ) ) {
       return -1;
    }

ok:
    *next = ptr;
 
    ctx->rep->flags |= URI_FLAGS_HOST_HOSTNAME;   
    ctx->rep->host = ctx_copy_string_raw(ctx, start, ptr );
    
    return 0;
}


M_INLINE int parse_ipv6_address(  URIPARSECTX *ctx, char *ptr, char **next )
{ 
   struct in6_addr addr;
   char *dup, *start;
   int rt;


   if (*ptr != '[') {
     return 1;
   }

   start = ptr;
   for(;*ptr != ']' && *ptr != '\0'; ++ ptr);
   if (*ptr != ']') {
     return -1;
   }

   dup = strdup_range( start+1, ptr );
   if (!dup) {
     return -1;
   }
   rt = inet_pton( AF_INET6, dup, &addr); 
   free(dup);
   if (rt == 1) {
     ctx->rep->flags |= URI_FLAGS_HOST_IPv6;   
     ctx->rep->host = ctx_copy_string_raw(ctx, start, ptr );
     
     *next = ptr + 1;
     return 0;
   }
   return -1;
}



//hostport      = host [ ":" port ]
//host          = hostname | IPv4address | Ipv6address
M_INLINE int parse_hostport(  URIPARSECTX *ctx, char *ptr, char **next )
{
   char *start;

   if ( ! parse_ipv4_address( ctx, ptr, next ) ) {
     goto pport;
   }

   if ( ! parse_hostname( ctx, ptr, next) ) {
     goto pport;
   }

   if ( ! parse_ipv6_address( ctx, ptr, next ) ) {
     goto pport;
   }

   return -1;

pport:

   ptr = *next;

   if (*ptr != ':' ) {
      return 0;
   }
   
   for( start = ptr = ptr + 1; is_digit( *ptr ); ++ ptr );
   *next = ptr;

   ctx->rep->port = atoi( start );
   return 0;
}

M_INLINE int parse_server( URIPARSECTX *ctx, char *ptr ,char **next )
{
  int rt ;

  rt = parse_userinfo(ctx, ptr, next);
  if (rt < 0) {
    return -1;
  }
  ptr = *next;
  return parse_hostport( ctx, ptr, next);
}


M_INLINE int parse_authority(  URIPARSECTX *ctx, char *ptr, char **next )
{
  if (!parse_server( ctx, ptr, next)) {
    return 0;
  }
  return -1;
}


//  scheme        = alpha *( alpha | digit | "+" | "-" | "." ) ":"
M_INLINE int parse_scheme( URIPARSECTX *ctx, char *line, char **next )
{
    char *start = line;

    if (is_alpha( *line )) {
       ++line;
       while( is_alphanum( *line ) || *line == '+' || *line == '-' || *line == '.') {
	 ++line;
       }
       if ( *line == ':') {
	   ctx->rep->flags |=  URI_FLAGS_HAS_SCHEME; 
	   ctx->rep->scheme = ctx_copy_string_raw(ctx, start, line );
           * next = line + 1;
	   return 0;
       }
    } 

    return -1;
}

M_INLINE int parse_abs_path( URIPARSECTX *ctx, char *ptr, char **next )
{
    char *start;

    start = ptr;
    if (parse_path_segments( ctx, ptr, next ) < 0) {
      return -1;
    }
    if (*next != ptr) {
	   ctx->rep->path_raw = ctx_copy_string_raw(ctx, start, *next );
	   ctx->rep->path =  ctx_finish_escaped_string(ctx); 
    }
    return 0;
}

// authority [ abs_path ]
M_INLINE int parse_net_path( URIPARSECTX *ctx, char *ptr , char **next )
{
   if (! parse_authority( ctx, ptr, next )) {
      ptr = *next;
      if (*ptr == '/') {
	 return parse_abs_path( ctx, ptr, next );
      }
   }

   return 0;
}

M_INLINE int parse_uric_no_slash( URIPARSECTX *ctx, char *ptr, char **next )
{
   if (is_unreserved( *ptr ) || *ptr == ';' || *ptr == '?' || *ptr == ':' || *ptr == '@' || *ptr == '&'
	    || *ptr == '=' || *ptr == '+' || *ptr == '$' || *ptr == ',') {
     ctx_add_escaped_char( ctx, *ptr );
     *next = ptr +1;
     return 0;
   }	
   return parse_escaped( ctx, ptr, next );
}

 

int parse_opaque_part( URIPARSECTX *ctx, char *ptr, char **next )
{
   char *start = ptr;
   int rt;
   if ( (rt = parse_uric_no_slash( ctx, ptr, next )) != 0 ) {
     return rt;
   }
   if (parse_uric_sequence( ctx, ptr, next ) < 0) {
     return -1;
   }
   ctx->rep->flags |= URI_FLAGS_IS_OPAQUE;  
   ctx->rep->opaque_raw = ctx_copy_string_raw(ctx, start, ptr );
   ctx->rep->opaque =  ctx_finish_escaped_string(ctx); 


   return 0;
}


int parse_hier_part( URIPARSECTX *ctx, char *ptr , char **next, int parse_opaque)
{
  char *start;

  if (ptr[0] == '/') {
    if (ptr[1] == '/') {
      if (parse_net_path( ctx, ptr + 2, next )) {
        return -1;
      }
    } else {
      if (parse_abs_path( ctx, ptr + 1, next )) {
        return -1;
      }
    } 
  } else {
    if (parse_opaque) {
       return parse_opaque_part( ctx, ptr, next );
    }
    return -1;
  }

  ptr = *next;
  if (*ptr == '?') {
 
    ptr ++;
    start = ptr;
    if ( parse_uric_sequence( ctx, ptr, next ) == -1 ) {
      return -1;
    }
    ptr = *next;
    ctx->rep->query_raw = ctx_copy_string_raw(ctx, start, ptr );
    ctx->rep->query =  ctx_finish_escaped_string(ctx); 
  }

  if ( *ptr != '#' ) {
    return 0;
  }

  ++ ptr;
  start = ptr;
  if (parse_uric_sequence( ctx, ptr, next ) == -1) {
    return -1;
  }
  ptr = *next;
  ctx->rep->fragment_raw = ctx_copy_string_raw(ctx, start, ptr );
  ctx->rep->fragment =  ctx_finish_escaped_string(ctx); 
  
  return 0;
}

M_INLINE void URI_init( URI *url )
{
  memset( url, 0, sizeof( URI ) );
  url->port = -1;
}

int URI_parse( URI *url, char *line )
{
   URI_init( url );
   URIPARSECTX ctx;
   char *ptr,*next;

   ctx.rep = url;

   url->cdata = (char *) malloc( strlen( line ) );
   if (!url->cdata) {
     goto err;
   }
   ctx.cdata_pos_start = ctx.cdata_pos = url->cdata;

   url->cdata_raw = (char *) malloc( strlen( line ) );
   if (!url->cdata_raw) {
     goto err;
   }
   ctx.cdata_raw_pos = url->cdata_raw;


   ptr = line;
   if (ptr[0] == '/') {
        if ( parse_hier_part( &ctx, ptr, &next, 0) ) {
	  goto err;
	}
   } else {
	if (! parse_scheme( &ctx, ptr, &next )) {
	    ptr = next;
	    if ( parse_hier_part( &ctx, ptr, &next, 1 ) ) {
	      goto err;
	    } 	
	} else {
	    if ( parse_authority( &ctx, ptr, &next )) {
	       goto err;
	    }
	}   
   }

  if (*next == '\0') {
    return 0;
  }

err:
  URI_free( url );
  return -1;
}


