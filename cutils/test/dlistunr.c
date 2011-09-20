#include <cutils/dlistunr.h>
#include "vtest/vtest.h"


void DLISTUNR_test()
{
	DLISTUNR list;
	int i;
	DLISTUNR_position idx;
	int *ptr, idn;
	
	VASSERT( !DLISTUNR_init( &list, sizeof(int), 10) );
	VASSERT( DLISTUNR_check(&list) );

	for(i=50;i<100;i++) {
		DLISTUNR_push_back(&list, &i, sizeof(int) );
	}
	VASSERT( DLISTUNR_check(&list) );

	idn = 50;
	DLISTUNR_FOREACH( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == idn);
		idn ++;
	}
	VASSERT( idn == 100 );

	for(i = 49; i >= 0; i--) {
		DLISTUNR_push_front(&list, &i, sizeof(int) );
	}

	VASSERT( DLISTUNR_check(&list) );

	/* iterate over list */
	idn = 0;
	DLISTUNR_FOREACH( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == idn);
		idn ++;
	}
	VASSERT( idn == 100 );
	
	/* iterate over list */
	idn = 99;
	DLISTUNR_FOREACH_REVERSE( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == idn);
		idn --;
	}
	VASSERT( idn == -1 );

	/* checks */
	VASSERT( DLISTUNR_check(&list) );
	VASSERT( list.elmcount == 100)
	VASSERT( list.entrycount == 10);

	DLISTUNR_free(&list, 0, 0);

	VASSERT( list.elmcount == 0)
	VASSERT( list.entrycount == 0);

	VASSERT( DLISTUNR_check(&list) );

}


#define NTRY 40

void DLISTUNR_test_insert()
{

	DLISTUNR list;
	int i,tmp;
	DLISTUNR_position idx;
	int *ptr, idn;
	

	VASSERT( !DLISTUNR_init( &list, sizeof(int), 10) );
	VASSERT( DLISTUNR_check(&list) );

	for(i=0;i< NTRY;i++) {
		tmp = i * 2;
		DLISTUNR_push_back(&list, &tmp, sizeof(int) );
	}

	VASSERT( DLISTUNR_check(&list) );


	for( i=0, idx = DLISTUNR_get_first( &list );i<NTRY;i++) {
		 tmp = (i * 2) + 1;
		 DLISTUNR_insert_after( &list, idx, &tmp, sizeof(tmp) );
		 idx = DLISTUNR_next( idx );
		 idx = DLISTUNR_next( idx );
	}

	VASSERT( DLISTUNR_check(&list) );

	/* iterate over list */
	idn = 0;
	DLISTUNR_FOREACH( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == idn);
		idn ++;
	}
	VASSERT( idn == (2 * NTRY) );

	/* iterate over list */
	idn = (2 * NTRY) - 1;
	DLISTUNR_FOREACH_REVERSE( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == idn);
		idn --;
	}
	VASSERT( idn == -1 );

	VASSERT( DLISTUNR_check(&list) );

	VASSERT( list.elmcount == (2 * NTRY))
	VASSERT( list.entrycount == ((2 * NTRY) / 10) );

	for( idx = DLISTUNR_get_first( &list );
		 idx.entry != (DLISTUNR_entry *) &list.root;) {

		 ptr = (int *) DLISTUNR_at( &list, idx );

		 DLISTUNR_unlink( &list, idx );

		 idx = DLISTUNR_next( idx );
	}

	VASSERT( DLISTUNR_check(&list) );

	idn = 1;
	DLISTUNR_FOREACH( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == idn);
		idn += 2;
	}
	VASSERT( idn == (2 * NTRY) + 1 );


	DLISTUNR_free(&list, 0, 0);

	VASSERT( list.elmcount == 0)
	VASSERT( list.entrycount == 0);

	VASSERT( DLISTUNR_check(&list) );


	/*--------*/
	for(i=0;i<100;i++) {
		DLISTUNR_push_back(&list, &i, sizeof(int));
	}
	VASSERT( DLISTUNR_check( &list ) );

	i = 0;
	DLISTUNR_FOREACH( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == i );
		i += 1;
	}
	VASSERT( i == 100);

	DLISTUNR_free(&list, 0, 0);

	for(i=0;i<100;i++) {
		DLISTUNR_push_front(&list, &i, sizeof(int));
	}

	VASSERT( DLISTUNR_check( &list ) );

	i = 99;
	DLISTUNR_FOREACH( idx, &list ) {
		ptr = (int *) DLISTUNR_at( &list, idx );
		VASSERT( *ptr == i );
		i -= 1;
	}
	VASSERT( i == -1);

	DLISTUNR_free(&list, 0, 0);

}

