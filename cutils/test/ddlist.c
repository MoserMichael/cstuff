/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */


#include <cutils/ddlist.h>
#include <vtest/vtest.h>

typedef struct
{
  DDLIST_entry base;
  int num;
}
  INT_DDLIST_ENTRY;

static int compare_entry(INT_DDLIST_ENTRY *a1, INT_DDLIST_ENTRY *a2)
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

void DDLIST_test()
{
	DDLIST list;
	INT_DDLIST_ENTRY *tmp;
	DDLIST_entry *pos, *next;
	int i, *arr;


	DDLIST_init( &list );

	for(i=0;i<10;i++) {
		tmp = (INT_DDLIST_ENTRY *) malloc(sizeof(INT_DDLIST_ENTRY));
		tmp->num = (i * 2);
		DDLIST_push_back(&list, (DDLIST_entry *) tmp);
	}

	VASSERT( DDLIST_check( &list ) );

	i = 0;
	DDLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20 );

	i = 18;
	DDLIST_FOREACH_REVERSE( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i -= 2;
	}
	VASSERT( i == -2 );

    pos = DDLIST_get_first( &list );
	for(i=0;i<10;i++) {
		tmp = (INT_DDLIST_ENTRY *) malloc(sizeof(INT_DDLIST_ENTRY));
		tmp->num = (i * 2) + 1;

  	    DDLIST_insert_after(&list, pos, (DDLIST_entry *) tmp);
		pos = DDLIST_get_next( pos );
		pos = DDLIST_get_next( pos );
	}

	VASSERT( DDLIST_check( &list ) );

	i = 0;
	DDLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i++;
	}
	VASSERT( i == 20 );

	i = 19;
	DDLIST_FOREACH_REVERSE( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i--;
	}
	VASSERT( i == -1 );

	i = 0;
	DDLIST_FOREACH_SAVE( pos, next, &list ) {
		if (i & 1) {
			free( DDLIST_unlink( &list, pos ) );
		}
		i+= 1;
	}
	VASSERT( i == 20 );
	VASSERT( DDLIST_size( &list ) == 10)

	VASSERT( DDLIST_check( &list ) );

	i = 0;
	DDLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20);

	DDLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DDLIST_unlink( &list, pos ));
	}

	VASSERT( DDLIST_check( &list ) );
	VASSERT( DDLIST_isempty( &list ) );


	arr = shuffle(100);
	for(i=0; i < 100; i++) {
		tmp = (INT_DDLIST_ENTRY *) malloc(sizeof(INT_DDLIST_ENTRY));
		tmp->num = arr[i];

		DDLIST_insert_sorted(&list, (DDLIST_COMPARE) compare_entry,(DDLIST_entry *) tmp);
	}
	free(arr);

	VASSERT( DDLIST_check( &list ) );

	i = 0;
	DDLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);
		
	DDLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DDLIST_unlink( &list, pos ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DDLIST_ENTRY *) malloc(sizeof(INT_DDLIST_ENTRY));
		tmp->num = i;
		DDLIST_push_back(&list, (DDLIST_entry *) tmp);
	}
	VASSERT( DDLIST_check( &list ) );

	i = 0;
	DDLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);

	DDLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DDLIST_unlink( &list, pos ));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DDLIST_ENTRY *) malloc(sizeof(INT_DDLIST_ENTRY));
		tmp->num = i;
		DDLIST_push_front(&list, (DDLIST_entry *) tmp);
	}

	VASSERT( DDLIST_check( &list ) );

	i = 99;
	DDLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DDLIST_ENTRY *) pos)->num == i );
		i -= 1;
	}
	VASSERT( i == -1);

	DDLIST_FOREACH_SAVE( pos, next,  &list ) {
			free( DDLIST_unlink( &list, pos ));
	}

		
}
