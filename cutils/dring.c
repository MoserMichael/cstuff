/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/dring.h>



 void DRING_foreach( DRING *lst, DRING_VISITOR_V eval, void *data, int save_from_del)
{
    DRING *cur, *next;

	if (!eval) {
	  return;
	}

	if (save_from_del) {
		DRING_FOREACH_SAVE( cur, next, lst) {
	   		eval( cur, data );
		}
	} else {
		DRING_FOREACH(  cur, lst ) {
			eval( cur, data );
		}
	}
		
}


 void DRING_foreach_reverse( DRING *lst, DRING_VISITOR_V eval, void *data, int save_from_delete)
{
	DRING *cur, *next;

	if (!eval) {
	  return ;
	}

	if ( save_from_delete ) {
		DRING_FOREACH_REVERSE_SAVE( cur, next, lst ) {

	   		eval( cur, data );
		}
	} else {
		DRING_FOREACH_REVERSE( cur, lst ) {

	   		eval( cur, data );
		}
	}
}




 void DRING_insert_sorted( DRING *list, DRING_COMPARE compare, DRING *newentry) 
{
	DRING *cur;
	
	DRING_FOREACH(  cur, list ) {
		if (compare(cur,newentry) > 0) {
			DRING_insert_before(cur,newentry);
			return;
		}
	}

	DRING_push_back( list, newentry );
}

