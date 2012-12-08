/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/tree.h>
#include <vtest/vtest.h>

typedef struct {
	TREENODE node;
	int value;
} TREENODE_INT;


static int preorder_1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
static int postorder_1[] = { 2, 3, 4, 1, 7, 6, 5, 8, 10, 9 };

static int preorder_2[] = { 1, 2, 3, 4, 8, 9, 10 };
static int postorder_2[] = { 2, 3, 4, 1, 8, 10, 9 };


static int preorder_3[] = { 1, 3,  8, 9, 10 };
static int postorder_3[] = {  3, 1, 8, 10, 9 };

/*
 *	TODO:
	 - more thorough test of unlink
	 - test iteration over root (zero case)
	 - castings to iteration type in FOREACH macros (global thing!).
 */

void TREE_test()
{
/* test tree
	r
	|
	1-5-8-9
	| |   |  
	| 6   10
	| |
	| 7
	|
	2-3-4

*/
	TREENODE_INT root,*tmp1,*tmp2,*tmp3,*tmp4,  *tmp_next, *tmp_nexta;
	TREENODE *cur;
	int n;

	
	TREE_init_root(&root.node);

	TREE_FOREACH_PREORDER(cur,&root.node)
		VASSERT( 0 );
	TREE_FOREACH_PREORDER_END

	TREE_FOREACH_POSTORDER(cur,&root.node)
		VASSERT( 0 );
	TREE_FOREACH_POSTORDER_END

    VASSERT( TREE_check_tree(&root.node) );
   
    /*first level */
	tmp1 = malloc( sizeof(TREENODE_INT));
	tmp1->value = 5;
	TREE_insert_child(&root.node, (TREENODE *) tmp1, TREE_INSERT_FIRST,  1 );

	tmp2 = malloc( sizeof(TREENODE_INT));
	tmp2->value = 8;
	TREE_insert_child(&root.node, (TREENODE *) tmp2, TREE_INSERT_LAST,  1 );

	tmp3 = malloc( sizeof(TREENODE_INT));
	tmp3->value = 1;
	TREE_insert_child(&root.node, (TREENODE *) tmp3, TREE_INSERT_FIRST,  1 );

	tmp4 = malloc( sizeof(TREENODE_INT));
	tmp4->value = 9;
	TREE_insert_child(&root.node, (TREENODE *) tmp4, TREE_INSERT_LAST,  1 );

	/*level 2 from tmp1*/
	tmp_next = malloc( sizeof(TREENODE_INT));
	tmp_next->value = 3;
	TREE_insert_child((TREENODE *) tmp3, (TREENODE *) tmp_next, TREE_INSERT_LAST,  1 );

	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 4;
	TREE_insert_right_sibling((TREENODE *) tmp_next, (TREENODE *) tmp_nexta,  1 );

	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 2;
	TREE_insert_left_sibling((TREENODE *) tmp_next, (TREENODE *) tmp_nexta,  1 );

	/*level 2 from tmp2 */
	tmp_next = malloc( sizeof(TREENODE_INT));
	tmp_next->value = 6;
	TREE_insert_child((TREENODE *) tmp1, (TREENODE *) tmp_next, TREE_INSERT_FIRST,  1 );

	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 7;
	TREE_insert_child((TREENODE *)tmp_next, (TREENODE *) tmp_nexta, TREE_INSERT_LAST,  1 );

	/* level 2 from tmp4 */
	tmp_next = malloc( sizeof(TREENODE_INT));
	tmp_next->value = 10;
	TREE_insert_child((TREENODE *) tmp4, (TREENODE *) tmp_next, TREE_INSERT_FIRST,  1 );

    VASSERT( TREE_check_tree(&root.node) );

	/* test iteration */
	n = 0;
	TREE_FOREACH_PREORDER(cur,&root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == preorder_1[ n++ ] );

	TREE_FOREACH_PREORDER_END
	VASSERT( n == 10 );

	/* test iteration */
	n = 0;
	TREE_FOREACH_POSTORDER(cur, &root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == postorder_1[ n++ ] );

	TREE_FOREACH_POSTORDER_END
	VASSERT( n == 10 );

	/* delete some nodes */


	TREE_unlink_node((TREENODE *) tmp1);

    VASSERT( TREE_check_tree(&root.node) );

/* test tree
	r
	|
	1-8-9
	|   |  
	|   10
	| 
	|
	2-3-4

*/

	n = 0;
	TREE_FOREACH_PREORDER(cur,&root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == preorder_2[ n++ ] );

	TREE_FOREACH_PREORDER_END
	VASSERT( n == 7 );

	/* test iteration */
	n = 0;
	TREE_FOREACH_POSTORDER(cur,&root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == postorder_2[ n ++ ] );

	TREE_FOREACH_POSTORDER_END
	VASSERT( n == 7 );


	/* delete some nodes */
	TREE_unlink_node( (TREENODE *) TREE_rightmost_sibling( (TREENODE *) TREE_first_child( (TREENODE *) tmp3) ) );
	TREE_unlink_node( (TREENODE *) TREE_first_child( (TREENODE *) tmp3) );

    VASSERT( TREE_check_tree(&root.node) );

/* test tree
	r
	|
	1-8-9
	|   |  
	|   10
	| 
	|
	3

*/

	n = 0;
	TREE_FOREACH_PREORDER(cur,&root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == preorder_3[ n++ ] );

	TREE_FOREACH_PREORDER_END
	VASSERT( n == 5 );

	/* test iteration */
	n = 0;
	TREE_FOREACH_POSTORDER(cur,&root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == postorder_3[ n++ ] );

	TREE_FOREACH_POSTORDER_END
	VASSERT( n == 5 );

}

static int preorder_u1[] = { 1, 2, 5, 3, 4, 8, 9, 10, 11, 12, 13, 14 };
static int postorder_u1[] = { 5, 2, 3, 4, 1, 8, 10, 12, 13, 14, 11, 9   };

static int preorder_u2[] = { 2, 5, 3, 4, 8, 9, 10, 12, 13, 14 };
static int postorder_u2[] = { 5, 2, 3, 4, 8, 10, 12, 13, 14, 9   };


void TREE_test_unlink()
{
/* test tree
	r
	|
	1-8-9
	|   |  
	|   10--11
	|	     |
	|	     12-13-14
	| 
	|
	2-3-4
	|
	5

*/

	TREENODE_INT root,*tmp2,*tmp3,*tmp4,  *tmp_next, *tmp_nexta, *tmp_next2;
	TREENODE *cur;
	int n;

	
	TREE_init_root(&root.node);

    VASSERT( TREE_check_tree(&root.node) );
   
    /*first level */

	tmp2 = malloc( sizeof(TREENODE_INT));
	tmp2->value = 8;
	TREE_insert_child(&root.node, (TREENODE *) tmp2, TREE_INSERT_LAST,  1 );

	tmp3 = malloc( sizeof(TREENODE_INT));
	tmp3->value = 1;
	TREE_insert_child(&root.node, (TREENODE *) tmp3, TREE_INSERT_FIRST,  1 );

	tmp4 = malloc( sizeof(TREENODE_INT));
	tmp4->value = 9;
	TREE_insert_child(&root.node, (TREENODE *) tmp4, TREE_INSERT_LAST,  1 );

	/*level 2 from tmp1*/
	tmp_next = malloc( sizeof(TREENODE_INT));
	tmp_next->value = 3;
	TREE_insert_child((TREENODE *) tmp3, (TREENODE *) tmp_next, TREE_INSERT_LAST,  1 );


	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 4;
	TREE_insert_right_sibling((TREENODE *) tmp_next, (TREENODE *) tmp_nexta,  1 );

	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 2;
	TREE_insert_left_sibling((TREENODE *) tmp_next, (TREENODE *) tmp_nexta,  1 );

		tmp_next2 = malloc( sizeof(TREENODE_INT));
		tmp_next2->value = 5;
		TREE_insert_child((TREENODE *) tmp_nexta, (TREENODE *) tmp_next2, TREE_INSERT_LAST,  1 );

	/* level 2 from tmp4 */
	tmp_next = malloc( sizeof(TREENODE_INT));
	tmp_next->value = 10;
	TREE_insert_child((TREENODE *) tmp4, (TREENODE *) tmp_next, TREE_INSERT_LAST,  1 );

	tmp_next = malloc( sizeof(TREENODE_INT));
	tmp_next->value = 11;
	TREE_insert_child((TREENODE *) tmp4, (TREENODE *) tmp_next, TREE_INSERT_LAST,  1 );

	/* level 3 from tmp_next */
	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 12;
	TREE_insert_child((TREENODE *) tmp_next, (TREENODE *) tmp_nexta, TREE_INSERT_LAST,  1 );

	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 13;
	TREE_insert_child((TREENODE *) tmp_next, (TREENODE *) tmp_nexta, TREE_INSERT_LAST,  1 );

	tmp_nexta = malloc( sizeof(TREENODE_INT));
	tmp_nexta->value = 14;
	TREE_insert_child((TREENODE *) tmp_next, (TREENODE *) tmp_nexta, TREE_INSERT_LAST,  1 );

    VASSERT( TREE_check_tree(&root.node) );


	/* test iteration */
	n = 0;
	TREE_FOREACH_PREORDER(cur,&root.node)

		VASSERT( n < 12 && ((TREENODE_INT *)cur)->value == preorder_u1[ n++ ] );

	TREE_FOREACH_PREORDER_END
	VASSERT( n == 12 );

	/* test iteration */
	n = 0;
	TREE_FOREACH_POSTORDER(cur, &root.node)

		VASSERT( n < 12 && ((TREENODE_INT *)cur)->value == postorder_u1[ n++ ] );

	TREE_FOREACH_POSTORDER_END
	VASSERT( n == 12 );

	TREE_unlink_node( (TREENODE *) tmp3 );
	TREE_unlink_node( (TREENODE *) tmp_next );

    VASSERT( TREE_check_tree(&root.node) );


/* test tree
	r
	|
	2-3-4-8-9
	|	    |
	5		10-12-13-14

*/

	n = 0;
	TREE_FOREACH_PREORDER(cur,&root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == preorder_u2[ n++ ] );

	TREE_FOREACH_PREORDER_END
	VASSERT( n == 10 );

	/* test iteration */
	n = 0;
	TREE_FOREACH_POSTORDER(cur, &root.node)

		VASSERT( n < 10 && ((TREENODE_INT *)cur)->value == postorder_u2[ n++ ] );

	TREE_FOREACH_POSTORDER_END
	VASSERT( n == 10 );

}
