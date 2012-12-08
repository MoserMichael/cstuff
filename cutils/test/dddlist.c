/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */


#include <cutils/dddlist.h>
#include <vtest/vtest.h>

typedef struct
{
  DDDLIST base;
  int num;
}
  INT_DDDLIST_ENTRY;


void DDDLIST_test()
{
	DDDLIST list;
	INT_DDDLIST_ENTRY *tmp;
	DDDLIST *pos, *next;
	int i;


	DDDLIST_init( &list );

	pos = &list;
	for(i=0;i<10;i++) {
		tmp = (INT_DDDLIST_ENTRY *) malloc(sizeof(INT_DDDLIST_ENTRY));
		tmp->num = (i * 2);
		DDDLIST_insert_after(pos, (DDDLIST *) tmp);
		pos = &tmp->base;
	}

	VASSERT( DDDLIST_check( &list ) );

	i = 0;
	DDDLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DDDLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20 );


	i = 0;
	DDDLIST_FOREACH_SAVE( pos, next, &list ) {
		if (i & 1) {
			free( DDDLIST_unlink(  pos ) );
		}
		i+= 1;
	}
	VASSERT( i == 10 );
	VASSERT( DDDLIST_size( &list ) == 5)

	VASSERT( DDDLIST_check( &list ) );

	DDDLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DDDLIST_unlink( pos ));
	}

	VASSERT( DDDLIST_check( &list ) );
	VASSERT( DDDLIST_isempty( &list ) );

}
