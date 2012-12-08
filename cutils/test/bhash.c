/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/bhash.h>
#include <string.h>
#include <stdio.h>
#include "vtest/vtest.h"

typedef struct tagSTR2INT
{
	HASH_Entry entry;
	char name[6];
	int  value;
}
	STR2INT;

static int hash_compare(HASH_Entry *entry, const void * key, ssize_t key_length)
{
	STR2INT *lhs;

        (void) key_length;

	lhs = (STR2INT *) entry;

	if (strcmp(lhs->name,key) == 0) {
		return 0;
	}
	return 1;
}

static int hash_visitor (HASH_Entry *entry, void *data)
{
	STR2INT *lhs = (STR2INT *) entry;
	char tmp[6];
	int *tmpn;

	sprintf(tmp,"N:%03d",lhs->value);

	VASSERT_RET( strcmp(tmp,lhs->name) == 0, -1);

	tmpn = (int *) data;
	*tmpn += 1;
	return 0;
}



#define FIRST_S "first"
#define SECOND_S "second"
#define THIRD_S "third"

#define TST_NUM 100


void HASH_test()
{
	HASH hash;
	STR2INT *tmp;
	int i,count;
	char stmp[6];
	HASH_Entry *entry;

	VASSERT( !HASH_init( &hash, 10, 0, hash_compare, 0) );
	VASSERT(  HASH_check(&hash) );
	
	for(i=0;i<TST_NUM;i++) {

		tmp = malloc( sizeof(STR2INT) );
		sprintf(tmp->name,"N:%03d",i);
		tmp->value = i;

		VASSERT( !HASH_insert( &hash, &tmp->entry, tmp->name, -1 ) );
	}

	VASSERT( HASH_check(&hash) );
	VASSERT( HASH_size(&hash) == TST_NUM );

	count = 0;
	HASH_foreach( &hash, hash_visitor, &count);
	VASSERT( count == TST_NUM );

	for(i=0;i<TST_NUM;i++) {
		sprintf(stmp,"N:%03d",i);
		entry = HASH_find(&hash, &stmp, -1 );
		VASSERT(entry);
		VASSERT( ((STR2INT *) entry)->value == i );
	}

	VASSERT( HASH_check(&hash) );
	
	HASH_deleteall( &hash, 0, 0, 0);
	HASH_free( &hash );

}

