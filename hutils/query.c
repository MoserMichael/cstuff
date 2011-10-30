#include "query.h"
#include <string.h>
#include "sutils.h"


static int QUERYSTRING_add(QUERYSTRING *query, char *name, char *value )
{
  HASHNAMEVALUE *entry; 

  if (!name || !value) {
    return -1;
  }

  entry = (HASHNAMEVALUE *) malloc( sizeof( HASHNAMEVALUE ) );
  if (!entry) {
    return -1;
  }
  entry->name = name; //strdup( name );
  entry->value = value; //strdup( value );

  return HASH_insert( &query->name_to_value, &entry->entry, entry->name, -1 );
}

static int compare_entry(HASH_Entry *entry, void *key, ssize_t key_size)
{
  M_UNUSED(key_size);
  return strcmp( ((HASHNAMEVALUE *) entry)->name, key);
}

int QUERYSTRING_init(QUERYSTRING *query, URI *uri)
{
   char *start, *pos, *name;    
   int state = 0;
   size_t off;  
   char is_escaped_char;

   pos = uri->query;

   if (HASH_init( &query->name_to_value, 32, 1, compare_entry, 0 ) ) {
     return -1;
   }

   for( ; *pos != 0 ; ++pos ) {
      off = pos - uri->cdata;
      is_escaped_char = uri->cdata_is_escaped[ off ];

      switch(state) {
	 case 0: // start parsing of name component.
	   start = pos;
	   state = 1;
	 //break;

	 case 1:
	   if (is_escaped_char != 0 || *pos != '=') {
	      break;
	   }
	   state = 2;
	 //break;

	 case 2: // end of name
	   name = strdup_range(start, pos);
	   state = 3;
	   start = pos + 1;
	   break;

	case 3:	 // parsing the value component.
	   if (is_escaped_char == 0 && *pos == '&') {
	      state = 0;
	      if (QUERYSTRING_add( query, name,  strdup_range(start, pos) ) ) {
                return -1;
	      }
	   }
	   break;
      }
   }
   if (state <= 2) {
	 if (QUERYSTRING_add( query,  strdup_range(start, pos), strdup("") ) ) {
                return -1;
	 }

   } else {
	 if (QUERYSTRING_add( query, name, strdup_range(start, pos) ) ) {
             return -1;
	 }
   }
   return 0;
}

int QUERYSTRING_free(QUERYSTRING *query )
{
  HASH_Entry *cur;
  HASHNAMEVALUE *nv;

  HASH_DELETEALL( cur, &query->name_to_value )
    nv = (HASHNAMEVALUE *) cur;
    free(nv->name);
    free(nv->value);
    free(nv);
  HASH_DELETEALL_END
  HASH_free( &query->name_to_value );
  return 0;
}


