/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VTREE_H_
#define _VTREE_H_

#ifdef  __cplusplus
extern "C" {
#endif


#include <cutils/base.h>


#define TREE_NEXTLEVEL_LAST 
#define TREE_NEXTLEVEL_LEFT

/**
 * tree. Each node has variable number of child nodes, the node is embeddable (like ring list).
 * This tree is especially suited for parse trees.
 */
typedef struct tagTREENODE {
	struct tagTREENODE *parent;
	
	struct tagTREENODE *nextlevel_first;
#ifdef TREE_NEXTLEVEL_LAST
	struct tagTREENODE *nextlevel_last;
#endif
	struct tagTREENODE *right;
#ifdef TREE_NEXTLEVEL_LEFT	
	struct tagTREENODE *left;
#endif
} TREENODE;

M_INLINE void TREE_init_root(TREENODE *tree)
{
	tree->right = tree->parent = tree->nextlevel_first = 0;
#ifdef TREE_NEXTLEVEL_LEFT
	tree->left = 0;
#endif
#ifdef TREE_NEXTLEVEL_LAST
	tree->nextlevel_last = 0;
#endif
}

M_INLINE TREENODE *TREE_parent(TREENODE *node)
{
	return node->parent;
}


M_INLINE TREENODE *TREE_left_sibling(TREENODE *node)
{
#ifdef TREE_NEXTLEVEL_LEFT
	return node->left;
#else
	TREENODE *tmp;

	if (!node->parent) {
		return 0;
	}
	tmp = node->parent->nextlevel_first;
	if (tmp == node) {
		return 0;
	}
	while(tmp && tmp->right != node) {
		tmp = tmp->right;
	}
	return tmp;
#endif
}

M_INLINE TREENODE *TREE_right_sibling(TREENODE *node)
{
	return node->right;
}

M_INLINE TREENODE *TREE_leftmost_sibling(TREENODE *node)
{
 	TREENODE *parent = node->parent;
	if (parent) {
		return parent->nextlevel_first;
	}
	return 0;
}

M_INLINE TREENODE *TREE_rightmost_sibling(TREENODE *node)
{
#ifdef TREE_NEXTLEVEL_LAST
	TREENODE *parent = node->parent;
	if (parent) {
		return parent->nextlevel_last;
	}
	return 0;
#else
	while(node->right) {
		node = node->right;
	}
#endif
	return node;
}

M_INLINE TREENODE *TREE_first_child(TREENODE *node)
{
	return node->nextlevel_first;
}

M_INLINE TREENODE *TREE_last_child(TREENODE *node)
{
#ifdef TREE_NEXTLEVEL_LAST
	return node->nextlevel_last;
#else
	return TREE_rightmost_sibling(node->nextlevel_first);
#endif
}



/* insert function -> new element next level is initialised hereby to zero ? option? new function? */

M_INLINE int TREE_insert_right_sibling(TREENODE *current, TREENODE *newnode, int node_is_leaf)
{
	TREENODE *parent = current->parent;

	if (!parent) {
		return -1;
	}
	if (node_is_leaf) {
		TREE_init_root(newnode);
	}

#ifdef TREE_NEXTLEVEL_LAST
	if (parent->nextlevel_last == current) {
		parent->nextlevel_last = newnode;
	}
#endif

	newnode->parent = parent;
	
#ifdef TREE_NEXTLEVEL_LEFT
	newnode->left  = current;
	if (current->right) {
		current->right->left = newnode;
	}
#endif
	newnode->right = current->right;
	current->right = newnode;



	return 0;
}


M_INLINE int TREE_insert_left_sibling(TREENODE *current, TREENODE *newnode, int node_is_leaf)
{
	TREENODE *parent = current->parent;
	if (!parent) {
		return -1;
	}

	if (node_is_leaf) {
		TREE_init_root(newnode);
	}

	if (parent->nextlevel_first == current) {
		parent->nextlevel_first = newnode;
	}

	newnode->parent = parent;

#ifdef TREE_NEXTLEVEL_LEFT
	if (current->left) {
		current->left->right = newnode;
	}
#endif
	newnode->right = current;
#ifdef TREE_NEXTLEVEL_LEFT	
	newnode->left = current->left;
	current->left = newnode;
#endif



	return 0;
}

typedef enum {
  TREE_INSERT_FIRST,
  TREE_INSERT_LAST,

} TREE_INSERT_MODE;

M_INLINE void TREE_insert_child(TREENODE *parent, TREENODE *newnode, TREE_INSERT_MODE mode, int node_is_leaf )
{	
	/* init new node */
	if (node_is_leaf) {
		newnode->nextlevel_first = 0;
#ifdef TREE_NEXTLEVEL_LAST
		newnode->nextlevel_last = 0;
#endif
	}
	newnode->parent = parent;
	newnode->right = 0;
#ifdef TREE_NEXTLEVEL_LEFT
	newnode->left = 0;
#endif	

	
	if (!parent->nextlevel_first) {		

		/* insert as first child */
		//newnode->nextlevel_first = parent->nextlevel_first;
		parent->nextlevel_first = newnode;
#ifdef TREE_NEXTLEVEL_LAST
		//newnode->nextlevel_last = parent->nextlevel_last;
		parent->nextlevel_last = newnode;
#endif
		
	} else {

		/* insert as sibling */
		switch(mode) {
			case TREE_INSERT_FIRST: {
					TREE_insert_left_sibling(parent->nextlevel_first,newnode, 0);
				}
				break;
			case TREE_INSERT_LAST:
#ifdef TREE_NEXTLEVEL_LAST
				TREE_insert_right_sibling( parent->nextlevel_last,newnode, 0);
#else
				TREE_insert_right_sibling( TREE_rightmost_sibling(parent->nextlevel_first), newnode, 0);
#endif

				break;
		}
	}

}


M_INLINE void TREE_merge_childs(TREENODE *parent, TREE_INSERT_MODE mode, TREENODE *newnode)
{	
	TREENODE *ch;

	switch(mode) {

	case TREE_INSERT_LAST:	
		ch = TREE_first_child(newnode);
		if (!ch) {
			return;
		}

		while(ch) {
			TREE_insert_child(parent, ch, TREE_INSERT_LAST, 0 );
			ch = TREE_right_sibling(ch);
		}
		break;

	case TREE_INSERT_FIRST:	
		ch = TREE_last_child(newnode);
		if (!ch) {
			return;
		}

		while(ch) {
			TREE_insert_child(parent, ch, TREE_INSERT_FIRST, 0 );
			ch = TREE_left_sibling(ch);
		}
		break;
	}
}


M_INLINE TREENODE * TREE_unlink_node(TREENODE *node)
{
	TREENODE *tmp,*tmp_next, *tmp_last;

	if (!node->parent) {
		return 0;
	}

	/* wrong */
	tmp = node->parent;
	if (tmp && tmp->nextlevel_first == node) {

		if (!node->nextlevel_first) {
			tmp->nextlevel_first = node->right;
			goto sdel;
		}

		tmp->nextlevel_first = node->nextlevel_first;				
		tmp_next = tmp->nextlevel_first;
		tmp_last = TREE_rightmost_sibling(tmp_next);

		tmp_last->right = node->right;
#ifdef TREE_NEXTLEVEL_LEFT
		if (node->right) {
			node->right->left = tmp_last;
		}
#endif
		do {
			tmp_next->parent = tmp;
			if (tmp_next == tmp_last) {
				break;
			}
			tmp_next = tmp_next->right;
		} while(tmp_next);

		return node;
	}

#ifdef TREE_NEXTLEVEL_LAST
	if (tmp && tmp->nextlevel_last == node) {

		if (!node->nextlevel_last) {
			tmp->nextlevel_last = TREE_left_sibling(node);
			goto sdel;
		}

		tmp->nextlevel_last = node->nextlevel_last;	
		tmp_next = tmp->nextlevel_last;
		tmp_last = TREE_leftmost_sibling(tmp_next);

#ifdef TREE_NEXTLEVEL_LEFT
		tmp_last->left = node->left;
#endif
		TREE_left_sibling(node)->right = tmp_last;

		do {
			tmp_last->parent = tmp;
			if (tmp_next == tmp_last) {
				break;
			}
			tmp_last = tmp_last->right;
		} while(tmp_last);					

		return node;
	}
#endif

sdel:

#ifdef TREE_NEXTLEVEL_LEFT
	tmp = node->right;
	if (tmp) {
		tmp->left = node->left;
	}
#endif

	tmp = TREE_left_sibling(node);
	if (tmp) {
		tmp->right = node->right;
	}

	return node;
}


typedef int  (*TREE_VISITOR) (TREENODE *entry, void *context);
typedef void  (*TREE_VISITOR_V) (TREENODE *entry, void *context);

typedef enum {
  TREE_PREORDER,
  TREE_POSTORDER,

} TREE_VISIT_EVENT;

typedef int  (*TREE_VISITOR_EXT_V) (TREENODE *entry, TREE_VISIT_EVENT visit, void *context);


#define TREE_FOREACH_CHILD(current, node) \
	for((current) = TREE_first_child(node); \
		(current); \
		(current) = TREE_right_sibling(current)) 

#define TREE_FOREACH_CHILD_REVERSE(current, node) \
	for((current) = TREE_last_child(node); \
		(current); \
		(current) = TREE_left_sibling(current)) 


M_INLINE size_t TREE_count_child_nodes( TREENODE *current)
{
	size_t ret = 0;
	TREENODE *c;

	TREE_FOREACH_CHILD(c,current) {
		ret++;
	}
	return ret;
}

/*
 a
 |
 b-d-g
 | |
 c e-f
*/
M_INLINE TREENODE *TREE_preorder_next(TREENODE *current)
{
	if (current->nextlevel_first) {
		current = current->nextlevel_first;
	} else {
		if (current->right) {
			current = current->right;
		} else {
			while(current->parent) {
				current = current->parent;
				if (current->right) {
					current = current->right;
					break;
				}
			}
		}
	}

	return current;
}

#if 0
M_INLINE TREENODE *TREE_preorder_next_ext(TREENODE *current, TREE_VISITOR_EXT_V ext, void *context)
{
	if (current->nextlevel_first) {
		current = current->nextlevel_first;
	} else {
		if (current->right) {
			current = current->right;
		} else {
			while(current->parent) {

				ext( context, TREE_POSTORDER, context);

				current = current->parent;
				if (current->right) {
					current = current->right;
					break;
				}
			}
		}
	}

	return current;
}
#endif


/**
 * @brief iterate over all elements of a list of child nodes, callback is invoked for either element of the list. list is traversed from first element to the last element.
 * @param lst (in) node in a tree (parent of enumerated childs).
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 */
M_INLINE void TREE_foreach_child( TREENODE *lst, TREE_VISITOR_V eval, void *context)
{
    TREENODE *cur;

	if (!eval) {
	  return;
	}

	TREE_FOREACH_CHILD( cur, lst) {
	   	eval( cur, context );
	}
	
}



M_INLINE void TREE_foreach_child_reverse( TREENODE *lst, TREE_VISITOR_V eval, void *context)
{
    TREENODE *cur;

	if (!eval) {
	  return;
	}


	TREE_FOREACH_CHILD_REVERSE( cur,lst) {
	   	eval( cur, context );
	}
	
}

/**
 * @brief find an element within the list of child nodes. callback is invoked for each element of the list, in forward direction from first element to last element; when the callback returns non zero value the iteration stops as we have found what we searched for.
 * 
 * @param tree (in) node in a tree (parent of enumerated childs).
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param retval (out) optional - the first non zero value returned by eval callback, if NULL then ignored.
 * @return the list element that has matched (i.e. element that has been found).
 *
 */
M_INLINE TREENODE * TREE_findif_child( TREENODE *tree, TREE_VISITOR eval, void *context, int32_t *retval)
{
	int ret;
    TREENODE *cur;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
		return 0;
	}
	

	TREE_FOREACH_CHILD( cur, tree) {
	   	ret = eval( cur, context );
	   	if (ret) {
			if (retval) {
				*retval = ret;
			}
		 	return cur;
	   	}
	}

	return 0;
}

M_INLINE TREENODE * TREE_findif_child_reverse( TREENODE *tree, TREE_VISITOR eval, void *context, int32_t *retval)
{
	int ret;
    TREENODE *cur;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
		return 0;
	}
	

	TREE_FOREACH_CHILD_REVERSE( cur, tree) {
	   	ret = eval( cur, context );
	   	if (ret) {
			if (retval) {
				*retval = ret;
			}
		 	return cur;
	   	}
	}

	return 0;
}


#define TREE_FOREACH_PREORDER(current, tree)\
{\
	TREENODE *TREE_nextnode##next;\
	current = TREE_preorder_next((tree));\
	if (current)\
		for( TREE_nextnode##next =  TREE_preorder_next(current);\
			    (current) && current != (tree); \
				(current) = TREE_nextnode##next, TREE_nextnode##next =  TREE_preorder_next(current)) {

		
#define	TREE_FOREACH_PREORDER_END\
	}\
}




/*
 f
 |
 b-d-e
 | |
 a b-c
*/
M_INLINE TREENODE *TREE_postorder_next(TREENODE *current, TREENODE *prev)
{

	if (current->nextlevel_first && (!prev || current->parent == prev)) {
next_level:
		do {		
			current = current->nextlevel_first;
		}
		while(current->nextlevel_first);
	} else {
		if (current->right) {
			current = current->right;
			if (current->nextlevel_first) {
			  goto next_level;
			}
		} else {
			if (current->parent) {
				current = current->parent;
			}
		}
	}
	return current;
}


/* wrong loop condition */
#define TREE_FOREACH_POSTORDER(current, tree)\
{\
	TREENODE *TREE_nextnode##next,*TREE_nextnode##prev;\
	TREE_nextnode##prev = 0; \
	current = TREE_postorder_next((tree),TREE_nextnode##prev);\
	if (current)\
		for( TREE_nextnode##next =  TREE_postorder_next(current,TREE_nextnode##prev);\
			    (current) && current != (tree); \
				TREE_nextnode##prev = (current), \
				(current) = TREE_nextnode##next, \
				TREE_nextnode##next =  TREE_postorder_next(current,TREE_nextnode##prev)) {




#define TREE_FOREACH_POSTORDER_END\
	}\
}\

/*
 
 preorder(tree)
 begin
     if tree is null, return;

     print(tree.root);
     preorder(tree.left_subtree);
     preorder(tree.right_subtree);
 end 
 */
M_INLINE void TREE_foreach_preorder(TREENODE *node, TREE_VISITOR_V visit, void *context)
{
	TREENODE *current;

	TREE_FOREACH_PREORDER(current,node)
		visit(node,context);
	TREE_FOREACH_POSTORDER_END
}

M_INLINE TREENODE * TREE_find_preorder(TREENODE *node, TREE_VISITOR visit, void *context)
{
	TREENODE *current;

	TREE_FOREACH_PREORDER(current,node)
		if (visit(node,context)) {
			return current;
		}
	TREE_FOREACH_POSTORDER_END
	return 0;
}



/*

 postorder(tree)
 begin
     if tree is null, return;

     postorder(tree.left_subtree);
     postorder(tree.right_subtree);
     print(tree.root);
 end 
 */
M_INLINE void TREE_foreach_postorder(TREENODE *node, TREE_VISITOR_V visit, void *context)
{
	TREENODE *current;

	TREE_FOREACH_POSTORDER(current,node)
		visit(node,context);
	TREE_FOREACH_POSTORDER_END
}

#if 0
M_INLINE void TREE_foreach_preorder_ext(TREENODE *tree, TREE_VISITOR_EXT_V visit, void *context)
{
	TREENODE *current;
	
	current = TREE_preorder_next(tree);
	if (current) {
		for( ;
			 (current) && current != tree;
			 (current) = TREE_preorder_next_ext(current, visit, context)) {
				 
			visit( context, TREE_PREORDER, context );
		}
	}

}
#endif

M_INLINE TREENODE * TREE_find_postorder(TREENODE *node, TREE_VISITOR visit,void *context)
{
	TREENODE *current;

	TREE_FOREACH_POSTORDER(current,node)
		if (visit(node,context)) {
			return current;
		}
	TREE_FOREACH_POSTORDER_END
	return 0;
}





/**
 * @brief check tree for consistency errors
 * @return 0 if the tree is inconsistent
 */
M_INLINE int TREE_check_tree(TREENODE *root)
{
	TREENODE *firstch = root->nextlevel_first;
#ifdef TREE_NEXTLEVEL_LAST
	TREENODE *lastch   = root->nextlevel_last;
#endif
#ifndef TREE_NEXTLEVEL_LEFT	
	TREENODE *slow,*fast, *next;
#endif

	if (!firstch 
#ifdef TREE_NEXTLEVEL_LAST
			|| !lastch
#endif
		) {

#ifdef TREE_NEXTLEVEL_LAST
		if (firstch || lastch) {
			return 0;
		}
#endif
		return 1;		
	}	
	
	if (firstch->parent != root) {
		return 0;
	}

#ifdef TREE_NEXTLEVEL_LEFT	
	for(;;firstch = firstch->right) {
		TREENODE *next;
		
		if (!firstch) {
			return 0;
		}

		if (!TREE_check_tree(firstch)) {
			return 0;
		}

		next = firstch->right;
		if (!next) {
#ifdef TREE_NEXTLEVEL_LAST
			if (lastch != firstch) {
				return 0;
			}
#endif
			break;
		}

		if (next->left != firstch) {
			return 0;
		}
	}
#else
	slow=fast=firstch;
	while(1) {
		if (!TREE_check_tree(slow)) {
			return 0;
		}
		next = fast->right;
		if (!next) {
			break;			
		}
		fast = next;
		if (fast == slow) {
			return 0;
		}
		next = fast->right;
		if (!next) {
			break;			
		}
		fast = next;
		if (fast == slow) {
			return 0;
		}
		slow = slow->right;
	}
#ifdef TREE_NEXTLEVEL_LAST
	if (lastch != fast) {
		return 0;
	}
#endif

#endif
	return 1;
}

#ifdef  __cplusplus
}
#endif

#endif
