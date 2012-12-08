/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/dring.h>
#include <vtest/vtest.h>

typedef struct
{
  DRING base;
  int num;
}
  INT_DLIST_ENTRY;

static int compare_entry(INT_DLIST_ENTRY *a1, INT_DLIST_ENTRY *a2)
{
	
	if (a1->num < a2->num) {
		return -1;
	}

	if (a1->num > a2->num) {
		return 1;
	}
	return 0;
}

static int *shuffle(int n)
{
	int *arr,i,tmp,pos;
	
	arr = (int *) malloc(n * sizeof(int));
	for(i = 0; i < n; i++) {
		arr[i] = i;
	}

	for(i = 0; i < n; i++) {

		pos = (rand() % (n - 1)) + 1;
		
		tmp = arr[0];
		arr[0] = arr[pos];
		arr[pos] = tmp;

	}
	return arr;

}

void DRING_test()
{
	DRING list;
	INT_DLIST_ENTRY *tmp;
	DRING *pos, *next;
	int i, *arr;


	DRING_init( &list );

	for(i=0;i<10;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2);
		DRING_push_back(&list, (DRING *) tmp);
	}

	VASSERT( DRING_check( &list ) );

	i = 0;
	DRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20 );

	i = 18;
	DRING_FOREACH_REVERSE( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i -= 2;
	}
	VASSERT( i == -2 );

    pos = DRING_get_first( &list );
	for(i=0;i<10;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2) + 1;

  	    DRING_insert_after( pos, (DRING *) tmp);
		pos = DRING_get_next( &list, pos );
		pos = DRING_get_next( &list, pos );
	}

	VASSERT( DRING_check( &list ) );

	i = 0;
	DRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i++;
	}
	VASSERT( i == 20 );

	i = 19;
	DRING_FOREACH_REVERSE( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i--;
	}
	VASSERT( i == -1 );

	i = 0;
	DRING_FOREACH_SAVE( pos, next, &list ) {
		if (i & 1) {
			free( DRING_unlink( pos ) );
		}
		i+= 1;
	}
	VASSERT( i == 20 );
	
	VASSERT( DRING_check( &list ) );

	i = 0;
	DRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20);

	DRING_FOREACH_SAVE( pos, next,  &list ) {
			free( DRING_unlink( pos ));
	}

	VASSERT( DRING_check( &list ) );
	VASSERT( DRING_isempty( &list ) );


	arr = shuffle(100);
	for(i=0; i<100; i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = arr[i];

		DRING_insert_sorted(&list, (DRING_COMPARE) compare_entry,(DRING *) tmp);
	}
	free(arr);

	VASSERT( DRING_check( &list ) );

	i = 0;
	DRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);
		
	DRING_FOREACH_SAVE( pos, next,  &list ) {
			free( DRING_unlink(  pos ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		DRING_push_back(&list, (DRING *) tmp);
	}
	VASSERT( DRING_check( &list ) );

	i = 0;
	DRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);

	DRING_FOREACH_SAVE( pos, next,  &list ) {
			free( DRING_unlink(  pos ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		DRING_push_front(&list, (DRING *) tmp);
	}

	VASSERT( DRING_check( &list ) );

	i = 99;
	DRING_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i -= 1;
	}
	VASSERT( i == -1);

	DRING_FOREACH_SAVE( pos, next,  &list ) {
			free( DRING_unlink(  pos ));
	}

		
}
