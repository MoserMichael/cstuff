#ifndef _QUERY_STRING_H_
#define _QUERY_STRING_H

#include <cutils/bhash.h>
#include <hutils/uri.h>

typedef struct tagHASHNAMEVALUE {
  HASH_Entry entry;
  char *name;
  char *value;
} HASHNAMEVALUE;

typedef struct tagQUERYSTRING {
  HASH name_to_value;
} QUERYSTRING;

int QUERYSTRING_init(QUERYSTRING *query, URI *uri);
int QUERYSTRING_free(QUERYSTRING *query );

M_INLINE const char *QUERYSTRING_find(QUERYSTRING *query, const char *name )
{
   HASHNAMEVALUE *value;

   value = (HASHNAMEVALUE *) HASH_find( &query->name_to_value, (void *) name, -1 );
   if (value) {
     return value->value;
   }
   return 0;
}


#endif


