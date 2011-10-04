#include "val.h"
#include <ctype.h>

static int parse_spec( const char **pos, VAL_TYPE *type)
{
  const char *cur = *pos;
  int length = 0;

  if (*cur == '\0') {
    return 1;
  }

  for( ; isspace(*cur) ; ++cur);

  if (*cur != '%') {
    return -1;
  }
  ++cur;

  if (*cur == 's') {
    *type = VAL_TYPE_STRING;
    goto ret;
  }

  if (*cur == 'p') {
    *type = VAL_TYPE_PTR;
    goto ret;
  }

  for( length = 0; *cur == 'h'; ++cur,++length );
   
  if ( *cur == 'q') { 
     ++cur;
     length = 4;
  }

  if (*cur != 'd' && *cur != 'u') {
    return -1;
  }

  switch( length ) {
      case 2:
        *type = *cur == 'd' ?  VAL_TYPE_INT8 : VAL_TYPE_UINT8;  
	goto ret;
      case 1:
        *type = *cur == 'd' ?  VAL_TYPE_INT16 : VAL_TYPE_UINT16;  
	goto ret;
      case 0:
        *type = *cur == 'd' ?  VAL_TYPE_INT32 : VAL_TYPE_UINT32;  
	goto ret;
      case 4:
        *type = *cur == 'd' ?  VAL_TYPE_INT64 : VAL_TYPE_UINT64;  
	goto ret;
  }
  return -1;

ret:
  *pos = cur + 1;
  return 0;
}

int VALUES_printv( VALUES *values, const char *format, va_list ap )
{
  const char *pos;
  VAL_TYPE type;
  VAL val;
  int rt;
  ARRAY_reset( &values->values );
  
  pos = format;
  while( (rt = parse_spec( &pos, &type ) ) == 0 ) {
    val.type = type;
    switch( type ) {
       case VAL_TYPE_UINT8:
         val.u.u8 = (uint8_t) va_arg( ap, int32_t );
	 break;
      case VAL_TYPE_INT8:
         val.u.n8 = (int8_t) va_arg( ap, int32_t );
	 break;
       case VAL_TYPE_UINT16:
         val.u.u16 = (uint16_t) va_arg( ap, uint32_t );
	 break;
       case VAL_TYPE_INT16:
         val.u.n16 = (int16_t) va_arg( ap, int32_t );
	 break;
       case VAL_TYPE_UINT32:
         val.u.u32 = va_arg( ap, uint32_t );
	 break;
      case VAL_TYPE_INT32:
         val.u.n32 = va_arg( ap, int32_t );
	 break;
      case VAL_TYPE_UINT64:
         val.u.u64 = va_arg( ap, uint64_t );
	 break;
      case VAL_TYPE_INT64:
         val.u.n64 = va_arg( ap, int64_t );
	 break;
      case VAL_TYPE_STRING:
      case VAL_TYPE_PTR:
         val.u.pval = va_arg( ap, void *  );
	 break;
     }
     ARRAY_push_back( &values->values, &val, sizeof(val) );
  }
  va_end( ap );
  
  if (rt == -1) {
    return -1;
  }
  return 0; 
}

int VALUES_scanv( VALUES *values, const char *format, va_list ap )
{
  VAL_TYPE type;
  VAL *val;
  size_t i;
  const char *pos;
  void *ptr;
  int rt;

  pos = format;
  for(i = 0; i < ARRAY_size(&values->values) && (rt = parse_spec( &pos, &type )) == 0; i++ ) {
     val = VALUES_at( values, i );

     if (val->type != type) {
       va_end(ap);
       return -1;
     }

     ptr = va_arg( ap, void *);

     switch( type ) {
       case VAL_TYPE_UINT8: 
	 * ((uint8_t *) ptr) = val->u.u8;
	 break;
       case VAL_TYPE_INT8:
	 * ((int8_t *) ptr) = val->u.n8;
	 break;
       case VAL_TYPE_UINT16:
	 * ((uint16_t *) ptr) = val->u.u16;
	 break;
       case VAL_TYPE_INT16:
	 * ((int16_t *) ptr) = val->u.n16;
	 break;
      case VAL_TYPE_UINT32:
	 * ((uint32_t *) ptr) = val->u.u32;
	 break;
      case VAL_TYPE_INT32:
	 * ((int32_t *) ptr) = val->u.n32;
	 break;
      case VAL_TYPE_UINT64:
	 * ((uint64_t *) ptr) = val->u.u64;
	 break;
      case VAL_TYPE_INT64:
	 * ((int64_t *) ptr) = val->u.n64;
	 break;
      case VAL_TYPE_STRING:
      case VAL_TYPE_PTR:
	 * ((char **) ptr) = val->u.sval;
	 break;
     }
  }
  va_end( ap );

  if (rt == -1) {
    return -1;
  }
  if (*pos != '\0') {
    return -1;
  }
  return 0;
}


