/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */


#include <cutils/dlist.h>
#include <vtest/vtest.h>

typedef struct
{
  DLIST_entry base;
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

void DLIST_test()
{
	DLIST list;
	INT_DLIST_ENTRY *tmp;
	DLIST_entry *pos, *next;
	int i, *arr;


	DLIST_init( &list );

	for(i=0;i<10;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2);
		DLIST_push_back(&list, (DLIST_entry *) tmp);
	}

	VASSERT( DLIST_check( &list ) );

	i = 0;
	DLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20 );

	i = 18;
	DLIST_FOREACH_REVERSE( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i -= 2;
	}
	VASSERT( i == -2 );

    pos = DLIST_get_first( &list );
	for(i=0;i<10;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2) + 1;

  	    DLIST_insert_after(&list, pos, (DLIST_entry *) tmp);
		pos = DLIST_get_next( pos );
		pos = DLIST_get_next( pos );
	}

	VASSERT( DLIST_check( &list ) );

	i = 0;
	DLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i++;
	}
	VASSERT( i == 20 );

	i = 19;
	DLIST_FOREACH_REVERSE( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i--;
	}
	VASSERT( i == -1 );

	i = 0;
	DLIST_FOREACH_SAVE( pos, next, &list ) {
		if (i & 1) {
			free( DLIST_unlink( &list, pos ) );
		}
		i+= 1;
	}
	VASSERT( i == 20 );
	VASSERT( list.elmcount == 10)

	VASSERT( DLIST_check( &list ) );

	i = 0;
	DLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20);

	DLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DLIST_unlink( &list, pos ));
	}

	VASSERT( DLIST_check( &list ) );
	VASSERT( DLIST_isempty( &list ) );


	arr = shuffle(100);
	for(i=0; i < 100; i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = arr[i];

		DLIST_insert_sorted(&list, (DLIST_COMPARE) compare_entry,(DLIST_entry *) tmp);
	}
	free(arr);

	VASSERT( DLIST_check( &list ) );

	i = 0;
	DLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);
		
	DLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DLIST_unlink( &list, pos ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		DLIST_push_back(&list, (DLIST_entry *) tmp);
	}
	VASSERT( DLIST_check( &list ) );

	i = 0;
	DLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);

	DLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DLIST_unlink( &list, pos ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		DLIST_push_front(&list, (DLIST_entry *) tmp);
	}

	VASSERT( DLIST_check( &list ) );

	i = 99;
	DLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i -= 1;
	}
	VASSERT( i == -1);

	DLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DLIST_unlink( &list, pos ));
	}

		
}
