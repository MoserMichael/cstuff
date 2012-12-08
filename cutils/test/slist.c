/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */


#include <cutils/slist.h>
#include <vtest/vtest.h>

typedef struct
{
  SLIST_entry base;
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

void SLIST_test()
{
	SLIST list;
	INT_DLIST_ENTRY *tmp;
	SLIST_entry *prev, *pos;
	int i, *arr;

	SLIST_init( &list );

	for(i=0;i<10;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2);
		SLIST_push_back(&list, (SLIST_entry *) tmp);
	}

	VASSERT( SLIST_check( &list ) );

	i = 0;
	SLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20 );

    pos = SLIST_get_first( &list );
	for(i=0;i<10;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = (i * 2) + 1;

  	    SLIST_insert_after(&list, pos, (SLIST_entry *) tmp);
		pos = SLIST_get_next( pos );
		pos = SLIST_get_next( pos );
	}

	VASSERT( SLIST_check( &list ) );

	i = 0;
	SLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i++;
	}
	VASSERT( i == 20 );
	VASSERT( list.elmcount == 20)

	i = 0;
	prev = 0;
	SLIST_FOREACH(  pos, &list ) {
		if (i) {

			free( SLIST_unlink_after( &list, prev ) );
			prev = pos = prev->next;
			if (!pos) {
				break;
			}

		} else {
			prev = pos;
		}
		
		i+= 1;
	}

	VASSERT( i == 10 );
	VASSERT( list.elmcount == 10)

	VASSERT( SLIST_check( &list ) );

	i = 0;
	SLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 2;
	}
	VASSERT( i == 20);

	while(!SLIST_isempty( &list)) {
			free( SLIST_unlink_after( &list, 0));
	}

	VASSERT( SLIST_check( &list ) );
	VASSERT( SLIST_isempty( &list ) );


	arr = shuffle(100);
	for(i=0; i<100; i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = arr[i];

		SLIST_insert_sorted(&list, (SLIST_COMPARE) compare_entry,(SLIST_entry *) tmp);
	}
	free(arr);

	VASSERT( SLIST_check( &list ) );

	i = 0;
	SLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);
		
	while(!SLIST_isempty( &list)) {
			free( SLIST_unlink_after( &list, 0));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		SLIST_push_back(&list, (SLIST_entry *) tmp);
	}
	VASSERT( SLIST_check( &list ) );

	i = 0;
	SLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i += 1;
	}
	VASSERT( i == 100);

	while(!SLIST_isempty( &list)) {
			free( SLIST_unlink_after( &list, 0));
	}

	for(i=0;i<100;i++) {
		tmp = (INT_DLIST_ENTRY *) malloc(sizeof(INT_DLIST_ENTRY));
		tmp->num = i;
		SLIST_push_front(&list, (SLIST_entry *) tmp);
	}

	VASSERT( SLIST_check( &list ) );

	i = 99;
	SLIST_FOREACH( pos, &list ) {

		VASSERT( ((INT_DLIST_ENTRY *) pos)->num == i );
		i -= 1;
	}
	VASSERT( i == -1);

	while(!SLIST_isempty( &list)) {
			free( SLIST_unlink_after( &list, 0));
	}

	VASSERT( SLIST_check( &list ) );
		
}
