/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VBASE_DLIST_H_
#define _VBASE_DLIST_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>



/**
 * @brief an entry in double linked list; 
 * If the user wants to link his struct(ure) into a DLIST linked list, then he must embed a DLIST_entry into his structure.
 * Access to user defined structure is via embedded SLIST_entry.
 */
typedef struct tagDLIST_entry 
{
  struct tagDLIST_entry *next,*prev;
}  
  DLIST_entry;


/**
 * @brief Double linked list data structure; where each list element can be of different length. Each element has a pointer to the next and previous element of the list.
 *
 * This double linked list has a list header (DLIST).
 * The list header points to the first and last element in the list.
 * first elements previous pointer is NULL; last elements next pointer is NULL
 *
 * Usage: If the user wants to link his struct(ure) into a list, then he must embed a DLIST_entry into his structure.
 * Access to user defined structure is via embedded DLIST_entry.
 *
 * The list header contains a counter of elements (feature can be surpressed by defining #define DLIST_NO_ELMCOUNT
 */
typedef struct {
	size_t elmcount;   /** number of elements in list */ 
	DLIST_entry root; /** root.next - pointer to first element, root.last pointer to last element */
} DLIST;


typedef void	 (*DLIST_VISITOR_V)(DLIST *list, DLIST_entry *entry, void *context);
typedef int32_t  (*DLIST_VISITOR)  (DLIST *list, DLIST_entry *entry, void *context);
typedef int32_t  (*DLIST_COMPARE)  (DLIST_entry *,  DLIST_entry * );


/**
 * @brief initialises an empty list head
 * @param head (in) list head 
 */
M_INLINE void DLIST_init( DLIST *head ) 
{ 
  head->root.next = head->root.prev = 0; 
  head->elmcount = 0;
}

/**
 * @brief checks if argument list is empty
 * @param list pointer to list.
 * @returns not zero for non empty list.
 */
M_INLINE int DLIST_isempty( DLIST *head ) 
{
  return head->root.next == 0;
}

/**
 * @brief insert new entry before a given entry.
 * @param list	   (in|out) pointer to list head
 * @param pos	   (in) current position (newentry inserted after this one).
 * @param newentry (in) pointer to new list entry (to be inserted).
 */
M_INLINE int DLIST_insert_before( DLIST *list, DLIST_entry *pos, DLIST_entry *newentry) 
{
	if (list->root.next) {
	
		if (!pos) {
			return -1;
		}

		newentry->prev = pos->prev;
		pos->prev = newentry;
		newentry->next = pos;

		if (newentry->prev) {
			newentry->prev->next = newentry;
		}

		if (list->root.prev == pos) {
			list->root.prev = newentry;
		}


	} else {

		list->root.next = list->root.prev = newentry;
		newentry->next = newentry->prev = 0;

	}	

	list->elmcount ++;
	return 0;
}


/**
 * @brief insert new entry after a given entry.
 * @param list	   (in) pointer to list head
 * @param pos	   (in) current position (newentry inserted after this one).
 * @param newentry (in) pointer to new list entry (to be inserted).
 */
M_INLINE void DLIST_insert_after( DLIST *list, DLIST_entry *pos, DLIST_entry *newentry) 
{
	if (list->root.next) {
		newentry->next = pos->next;
		pos->next = newentry;
		newentry->prev = pos;

		if (newentry->next) {
			newentry->next->prev = newentry;
		}

		if (list->root.next == pos) {
			list->root.next = newentry;
		}


	} else {

		list->root.next = list->root.prev = newentry;
		newentry->next = newentry->prev = 0;		

	}


	list->elmcount ++;
}


/**
 * @brief delete an element from a list.
 * @param list (in|out) the object
 * @param link (in) deletes this list element
 */
M_INLINE DLIST_entry *DLIST_unlink( DLIST *list, DLIST_entry *link ) 
{
	DLIST_entry *next,*prev;

	next = link->next;
	prev = link->prev;

	if (next) {
		next->prev = link->prev;
	}
	if (prev) {
		prev->next = link->next;
	}

	if (list->root.next == link) {
		list->root.next = prev;	
	}
	
	if (list->root.prev == link) {
		list->root.prev = next;
	}

        link->prev = link->next = 0;

	list->elmcount --;
	return link;
}


/**
 * @brief insert element as last in list (used to maintain queue)
 * @param list list head.
 * @param newentry entry to be inserted into list
 */
M_INLINE void DLIST_push_back( DLIST *list, DLIST_entry *newentry)
{
  DLIST_insert_after( list, list->root.next, newentry );
}

/**
 * @brief insert element as first in list (used to maintain queue)
 * @param list (in) list head.
 * @param newentry (in) entry to be inserted into list
 */
M_INLINE void DLIST_push_front( DLIST *list, DLIST_entry *newentry)
{
  DLIST_insert_before( list, list->root.prev, newentry );
}

/**
 * @brief remove the first element from list (used to maintain double ended queue)
 * @param list (in) the object
 * @return the first element of list, NULL if list empty
 */
M_INLINE DLIST_entry * DLIST_pop_front( DLIST *list )
{
  DLIST_entry *ret;

  if (DLIST_isempty( list )) {
    return 0;
  }
  ret = list->root.prev;
  DLIST_unlink(list, ret);
  return ret;  
}


/**
 * @brief remove the last element from list (used to maintain double ended queue)
 * @param list (in) the object
 * @return the first element of list, NULL if list empty
 */
M_INLINE DLIST_entry * DLIST_pop_back( DLIST *list)
{ 
  DLIST_entry *ret;

  if (DLIST_isempty( list )) {
    return 0;
  }
  ret = list->root.next;
  DLIST_unlink(list, ret);
  return ret;
}


M_INLINE DLIST_entry *DLIST_get_first(DLIST *list)
{
  if (DLIST_isempty(list)) {
    return 0;
  }  
  return list->root.prev;
}

M_INLINE DLIST_entry *DLIST_get_last(DLIST *list)
{
  if (DLIST_isempty(list)) {
    return 0;
  }
  return list->root.next;
}

M_INLINE DLIST_entry *DLIST_get_next( DLIST_entry *cur) 
{
  return cur->next;
}


M_INLINE DLIST_entry *DLIST_get_prev( DLIST_entry *cur ) 
{
  return cur->prev;
}

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DLIST_entry *) pointer to the current element
 * @param list (type DLIST *) pointer to the list that is traversed
 */
#define DLIST_FOREACH( loopvarname, list )\
  for((loopvarname) = (list)->root.prev;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarname)->next )

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DLIST_entry *) pointer to the current element
 * @param list (type DLIST *) pointer to the list that is traversed
 */
#define DLIST_FOREACH_REVERSE( loopvarname, list )\
  for((loopvarname) = (list)->root.next;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarname)->prev )

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DLIST_entry *) pointer to the current element
 * @param loopvarnamenext (type DLIST_entry *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DLIST *) pointer to the list that is traversed
 */
#define DLIST_FOREACH_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = (list)->root.prev, (loopvarnext) = (loopvarname) ? (loopvarname)->next : 0;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarnext), (loopvarnext) = (loopvarname) ? (loopvarname)->next : 0)

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DLIST_entry *) pointer to the current element
 * @param loopvarnamenext (type DLIST_entry *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DLIST *) pointer to the list that is traversed
 */
#define DLIST_FOREACH_REVERSE_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = (list)->root.next, (loopvarnext) = (loopvarname) ? (loopvarname)->prev : 0;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarnext), (loopvarnext) = (loopvarname) ? (loopvarname)->prev : 0)



/*
 * @brief: return number of elements in list
 * if we don't have element count in list (DLIST_NO_ELMCOUNT defined), then the whole list structure is traversed.
 */
M_INLINE size_t DLIST_size( DLIST *list )
{
	return list->elmcount;
}

/*
 * @brief get the nth element of a list as counted from the beginning of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE DLIST_entry *DLIST_get_nth( DLIST *list, size_t nth)
{
	DLIST_entry *elm;

	if (nth >= list->elmcount) {
		return 0;
	}

	/* if less then half of count - iterate from start of list, otherwise from tail. */
	if (nth < (list->elmcount >> 1)) {
		for(elm = DLIST_get_first(list); nth > 0; nth -= 1, elm = DLIST_get_next(elm)); 
	} else {
		nth = list->elmcount - nth - 1;
		for(elm = DLIST_get_last(list);  nth > 0; nth -= 1, elm = DLIST_get_prev(elm)); 
	}

	return elm;
}

/*
 * @brief get the nth element of a list as counted from the end of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE DLIST_entry *DLIST_get_nth_reverse( DLIST *list, size_t nth)
{	
	return DLIST_get_nth( list, DLIST_size( list ) - nth );
}

/**
 * @brief iterate over all elements of a list, callback is invoked for either element of the list. list is traversed from first element to the last element.
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 */
M_INLINE void DLIST_foreach( DLIST *lst, DLIST_VISITOR_V eval, void *context, int save_from_del)
{
    DLIST_entry *cur, *next;

	if (!eval) {
	  return;
	}

	if (save_from_del) {
		DLIST_FOREACH_SAVE( cur, next, lst) {
	   		eval( lst, cur, context );
		}
	} else {
		DLIST_FOREACH(  cur, lst ) {
			eval( lst, cur, context );
		}
	}		
}

/**
 * @brief iterate over all elements of a list, callback is invoked for either element of the list. list is traversed backword from last element to the first element.
 *
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 */
M_INLINE void DLIST_foreach_reverse( DLIST *lst, DLIST_VISITOR_V eval, void *context, int save_from_delete)
{
	DLIST_entry *cur, *next;

	if (!eval) {
	  return ;
	}

	if ( save_from_delete ) {
		DLIST_FOREACH_REVERSE_SAVE( cur, next, lst ) {
	   		eval( lst, cur, context );
		}
	} else {
		DLIST_FOREACH_REVERSE( cur, lst ) {
	   		eval( lst, cur, context );
		}
	}
}



/**
 * @brief find an element within the linked list. callback is invoked for each element of the list, in forward direction from first element to last element; when the callback returns non zero value the iteration stops as we have found what we searched for.
 * 
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param retval (out) optional - the first non zero value returned by eval callback, if NULL then ignored.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 * @return the list element that has matched (i.e. element that has been found).
 *
 */
M_INLINE DLIST_entry * DLIST_findif( DLIST *lst, DLIST_VISITOR eval, void *context, int32_t *retval, int save_from_del)
{
	int ret;
    DLIST_entry *cur, *next;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
		return 0;
	}
	
	if (save_from_del) {
		DLIST_FOREACH_SAVE( cur, next, lst) {
	   		ret = eval( lst, cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}
		 		return cur;
	   		}
		}
	} else {
		DLIST_FOREACH(  cur, lst ) {
			ret = eval( lst, cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}
		 		return cur;
	   		}
		}
	}		

	return 0;
}



/**
 * @brief find an element within the linked list. callback is invoked for each element of the list, in reverse direction from last element to first element; when the callback returns non zero value the iteration stops as we have found what we searched for. 
 * 
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param retval (out) optional - the first non zero value returned by eval callback, if NULL then ignored.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 * @return the list element that has matched (i.e. element that has been found).
 *
 */
M_INLINE DLIST_entry * DLIST_findif_reverse( DLIST *lst, DLIST_VISITOR eval, void *context, int32_t *retval, int save_from_delete)
{
	int ret;
	DLIST_entry *cur, *next;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
	  return 0;
	}

	if ( save_from_delete ) {
		DLIST_FOREACH_REVERSE_SAVE( cur, next, lst ) {

	   		ret = eval( lst, cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}
		 		return cur;
	   		}
		}
	} else {
		DLIST_FOREACH_REVERSE( cur, lst ) {

	   		ret = eval( lst, cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}		 		
				return cur;
	   		}
		}
	}

	return 0;
}


/** 
 * @brief iterate over all entries of the list and delete entries that match predicate from the list, and frees the memory (optionally)
 * @param list (in) the object.
 * @param check_if (in) predicate function; the function returns 1 then inspected argument element will be deleted; if argument pointer is NULL then all entries will be deleted. 
 * @param context (in) data pointer passed to every invocation of check_if
 * @param free_ctx (in) interface used to free data of entry, is argument pointer is NULL then nothing is freed.
 * @param offset_of_link (in) offset of DLIST_entry in embedded structure.
 */
M_INLINE void DLIST_deleteif( DLIST *list, DLIST_VISITOR check_if, void *context,  int offset_of_link)
{
    DLIST_entry *cur,*next,*del;

    DLIST_FOREACH_SAVE(cur,next,list) {
		if (!check_if || check_if( list, cur, context))  {
			del = DLIST_unlink( list, cur );
			if (offset_of_link != -1) {
  			  free(  _MEMBEROF( del, offset_of_link ) );
 			}
		}
	}
}

/** 
 * @brief iterate over all entries of the list and delete them.
 * @param list (in) the object

 * @param on_delete(in) if not NULL then this callback is called prior to deleting each list element.
 * @param context (in) if on_delete is not NULL then this pointer is passed as argument to on_delete.
 
 * @param free_ctx allocation interface used to free data of entry.
 * @param offset_of_link offset of DLIST_entry in embedded structure.
 */
M_INLINE void DLIST_deleteall( DLIST *list, DLIST_VISITOR_V on_delete, void *context, int offset_of_link)
{
    DLIST_entry *cur,*next,*del;

    DLIST_FOREACH_SAVE(cur,next,list) {
		
		if (on_delete)  {			
			on_delete( list, cur, context);
		}

		del = DLIST_unlink( list, cur );
		if (offset_of_link != -1) {
		  free( _MEMBEROF(del,offset_of_link) );
		}
	}
}


/**
 * @brief insert new element into sorted list; Maintains ordering relationship between elements of linked list (sort by ascending order)
 * A sorting algorithm based on this function is not very efficient; it is of complexity O(n^2); nevertheless usefull if a list is modified and has to be always maintained in sorted order.
 * @param list (in) the object
 * @param compare (in) comparison function.
 * @param newentry (in) element that is to be inserted into list.
 */
M_INLINE void DLIST_insert_sorted( DLIST *list, DLIST_COMPARE compare, DLIST_entry *newentry) 
{
	DLIST_entry *cur;
	
	DLIST_FOREACH(  cur, list ) {
		if (compare(cur,newentry) > 0) {

			DLIST_insert_before(list, cur,newentry);
			return;
		}
	}

	DLIST_push_back( list, newentry );
}

/**
 * @brief Reverse a list
 * This function turns the first element into the last element, the second element
 * into the one before the last, and so on and on.
 * @param header (in) the object
 */
M_INLINE void DLIST_reverse( DLIST *lst )
{
	DLIST_entry *cur = lst->root.prev, *next;
	DLIST_entry *tmp;
	if (cur) {
		next = cur->next;
		cur->next = 0;

		while(next) {
			tmp = next->next;
			
			next->next = cur;
			cur->prev = next;

			cur = next;
			next = tmp;
		}

		tmp = lst->root.prev;
		lst->root.prev = lst->root.next;
		lst->root.next = tmp;
	}
}

/**
 * @brief check consistency of list
 * @param lst list header.
 * @return zero if list contains errors.
 */
M_INLINE int DLIST_check(DLIST *header)
{
	DLIST_entry *cur,*next,*prev = 0;
	size_t count = 0;

	if (header->root.prev && !header->root.next) {
		return 0;
	}

	if (header->root.prev && header->root.prev->prev) {
		return 0;
	}

	if (header->root.next && header->root.next->next) {
		return 0;
	}

	DLIST_FOREACH( cur, header ) {

		next = cur->next;
		if (next && next->prev != cur) {		
			return 0;
		}
		if (cur->prev != prev) {
			return 0;
		}
		count += 1;
		prev = cur;
	}

	if (header->root.next != prev) {
		return 0;
	}

	if (header->elmcount != count) {
		return 0;
	}
	return 1;
}


#ifdef  __cplusplus
}
#endif

#endif
