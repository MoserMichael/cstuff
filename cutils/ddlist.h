/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VBASE_DDLIST_H_
#define _VBASE_DDLIST_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>



/**
 * @defgroup DDLIST_entry
 * @brief an entry in double linked list, add to structure as member in order to make structure storable in hash table.
 * If the user wants to link his struct(ure) into a DDLIST linked list, then he must embed a DDLIST_entry into his structure.
 * Access to user defined structure is via embedded SLIST_entry.
 * @{
 */
typedef struct tagDDLIST_entry 
{
  struct tagDDLIST_entry *next,*prev;
}  
  DDLIST_entry;

/*
 * @}
 */


/**
 * @defgroup DDLIST
 * @brief Double linked list data structure; where each list element can be of different length. Each element has a pointer to the next and previous element of the list.
 *
 * This double linked list has a list header (DDLIST).
 * The list header points to the first and last element in the list.
 * first elements previous pointer is NULL; last elements next pointer is NULL
 *
 * Usage: If the user wants to link his struct(ure) into a list, then he must embed a DDLIST_entry into his structure.
 * Access to user defined structure is via embedded DDLIST_entry.
 *
 * Note: identical to DLIST with the distinction that there is no counter of elements. DDLIST_size  traverses the whole list.
 * @{
 */
typedef struct {
	DDLIST_entry root; /** root.next - pointer to first element, root.last pointer to last element */
} DDLIST;


typedef void	 (*DDLIST_VISITOR_V)(DDLIST *list, DDLIST_entry *entry, void *context);
typedef int32_t  (*DDLIST_VISITOR)  (DDLIST *list, DDLIST_entry *entry, void *context);
typedef int32_t  (*DDLIST_COMPARE)  (DDLIST_entry *,  DDLIST_entry * );


/**
 * @brief initialises an empty list head
 * @param head (in) list head 
 */
M_INLINE void DDLIST_init( DDLIST *head ) 
{ 
  head->root.next = head->root.prev = 0; 
}


M_INLINE void DDLIST_entry_init( DDLIST_entry *head ) 
{ 
  head->next = head->prev = 0; 
}


/**
 * @brief checks if argument list is empty
 * @param list pointer to list.
 * @returns not zero for non empty list.
 */
M_INLINE int DDLIST_isempty( DDLIST *head ) 
{
  return head->root.next == 0;
}

/**
 * @brief insert new entry before a given entry.
 * @param list	   (in|out) pointer to list head
 * @param pos	   (in) current position (newentry inserted after this one).
 * @param newentry (in) pointer to new list entry (to be inserted).
 */
M_INLINE int DDLIST_insert_before( DDLIST *list, DDLIST_entry *pos, DDLIST_entry *newentry) 
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

	return 0;
}


/**
 * @brief insert new entry after a given entry.
 * @param list	   (in) pointer to list head
 * @param pos	   (in) current position (newentry inserted after this one).
 * @param newentry (in) pointer to new list entry (to be inserted).
 */
M_INLINE void DDLIST_insert_after( DDLIST *list, DDLIST_entry *pos, DDLIST_entry *newentry) 
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


}


/**
 * @brief delete an element from a list.
 * @param list (in|out) the object
 * @param link (in) deletes this list element
 */
M_INLINE DDLIST_entry *DDLIST_unlink( DDLIST *list, DDLIST_entry *link ) 
{
	DDLIST_entry *next,*prev;

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

	return link;
}


/**
 * @brief insert element as last in list (used to maintain queue)
 * @param list list head.
 * @param newentry entry to be inserted into list
 */
M_INLINE void DDLIST_push_back( DDLIST *list, DDLIST_entry *newentry)
{
  DDLIST_insert_after( list, list->root.next, newentry );
}

/**
 * @brief insert element as first in list (used to maintain queue)
 * @param list (in) list head.
 * @param newentry (in) entry to be inserted into list
 */
M_INLINE void DDLIST_push_front( DDLIST *list, DDLIST_entry *newentry)
{
  DDLIST_insert_before( list, list->root.prev, newentry );
}

/**
 * @brief remove the first element from list (used to maintain double ended queue)
 * @param list (in) the object
 * @return the first element of list, NULL if list empty
 */
M_INLINE DDLIST_entry * DDLIST_pop_front( DDLIST *list )
{
  DDLIST_entry *ret;

  if (DDLIST_isempty( list )) {
    return 0;
  }
  ret = list->root.prev;
  DDLIST_unlink(list, ret);
  return ret;  
}


/**
 * @brief remove the last element from list (used to maintain double ended queue)
 * @param list (in) the object
 * @return the first element of list, NULL if list empty
 */
M_INLINE DDLIST_entry * DDLIST_pop_back( DDLIST *list)
{ 
  DDLIST_entry *ret;

  if (DDLIST_isempty( list )) {
    return 0;
  }
  ret = list->root.next;
  DDLIST_unlink(list, ret);
  return ret;
}


M_INLINE DDLIST_entry *DDLIST_get_first(DDLIST *list)
{
  if (DDLIST_isempty(list)) {
    return 0;
  }  
  return list->root.prev;
}

M_INLINE DDLIST_entry *DDLIST_get_last(DDLIST *list)
{
  if (DDLIST_isempty(list)) {
    return 0;
  }
  return list->root.next;
}

M_INLINE DDLIST_entry *DDLIST_get_next( DDLIST_entry *cur) 
{
  return cur->next;
}


M_INLINE DDLIST_entry *DDLIST_get_prev( DDLIST_entry *cur ) 
{
  return cur->prev;
}

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DDLIST_entry *) pointer to the current element
 * @param list (type DDLIST *) pointer to the list that is traversed
 */
#define DDLIST_FOREACH( loopvarname, list )\
  for((loopvarname) = (list)->root.prev;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarname)->next )

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DDLIST_entry *) pointer to the current element
 * @param list (type DDLIST *) pointer to the list that is traversed
 */
#define DDLIST_FOREACH_REVERSE( loopvarname, list )\
  for((loopvarname) = (list)->root.next;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarname)->prev )

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DDLIST_entry *) pointer to the current element
 * @param loopvarnamenext (type DDLIST_entry *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DDLIST *) pointer to the list that is traversed
 */
#define DDLIST_FOREACH_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = (list)->root.prev, (loopvarnext) = (loopvarname) ? (loopvarname)->next : 0;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarnext), (loopvarnext) = (loopvarname) ? (loopvarname)->next : 0)

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DDLIST_entry *) pointer to the current element
 * @param loopvarnamenext (type DDLIST_entry *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DDLIST *) pointer to the list that is traversed
 */
#define DDLIST_FOREACH_REVERSE_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = (list)->root.next, (loopvarnext) = (loopvarname) ? (loopvarname)->prev : 0;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarnext), (loopvarnext) = (loopvarname) ? (loopvarname)->prev : 0)



/*
 * @brief: return number of elements in list; iterates over the whole list to get this number
 */
M_INLINE size_t DDLIST_size( DDLIST *list )
{
	DDLIST_entry *cur;
	size_t elmcount = 0;
	
	DDLIST_FOREACH(  cur, list ) {
	  elmcount++;
	}
	return elmcount;
}

/*
 * @brief get the nth element of a list as counted from the beginning of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE DDLIST_entry *DDLIST_get_nth( DDLIST *list, size_t nth)
{
	DDLIST_entry *elm;


	for(elm = DDLIST_get_first(list); elm && nth > 0; nth -= 1, elm = DDLIST_get_next(elm)); 

	return elm;
}


/**
 * @brief iterate over all elements of a list, callback is invoked for either element of the list. list is traversed from first element to the last element.
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 */
M_INLINE void DDLIST_foreach( DDLIST *lst, DDLIST_VISITOR_V eval, void *context, int save_from_del)
{
    DDLIST_entry *cur, *next;

	if (!eval) {
	  return;
	}

	if (save_from_del) {
		DDLIST_FOREACH_SAVE( cur, next, lst) {
	   		eval( lst, cur, context );
		}
	} else {
		DDLIST_FOREACH(  cur, lst ) {
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
M_INLINE void DDLIST_foreach_reverse( DDLIST *lst, DDLIST_VISITOR_V eval, void *context, int save_from_delete)
{
	DDLIST_entry *cur, *next;

	if (!eval) {
	  return ;
	}

	if ( save_from_delete ) {
		DDLIST_FOREACH_REVERSE_SAVE( cur, next, lst ) {
	   		eval( lst, cur, context );
		}
	} else {
		DDLIST_FOREACH_REVERSE( cur, lst ) {
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
M_INLINE DDLIST_entry * DDLIST_findif( DDLIST *lst, DDLIST_VISITOR eval, void *context, int32_t *retval, int save_from_del)
{
	int ret;
    DDLIST_entry *cur, *next;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
		return 0;
	}
	
	if (save_from_del) {
		DDLIST_FOREACH_SAVE( cur, next, lst) {
	   		ret = eval( lst, cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}
		 		return cur;
	   		}
		}
	} else {
		DDLIST_FOREACH(  cur, lst ) {
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
M_INLINE DDLIST_entry * DDLIST_findif_reverse( DDLIST *lst, DDLIST_VISITOR eval, void *context, int32_t *retval, int save_from_delete)
{
	int ret;
	DDLIST_entry *cur, *next;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
	  return 0;
	}

	if ( save_from_delete ) {
		DDLIST_FOREACH_REVERSE_SAVE( cur, next, lst ) {

	   		ret = eval( lst, cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}
		 		return cur;
	   		}
		}
	} else {
		DDLIST_FOREACH_REVERSE( cur, lst ) {

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
 * @param offset_of_link (in) offset of DDLIST_entry in embedded structure.
 */
M_INLINE void DDLIST_deleteif( DDLIST *list, DDLIST_VISITOR check_if, void *context,  int offset_of_link)
{
    DDLIST_entry *cur,*next,*del;

    DDLIST_FOREACH_SAVE(cur,next,list) {
		if (!check_if || check_if( list, cur, context))  {
			del = DDLIST_unlink( list, cur );
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
 * @param offset_of_link offset of DDLIST_entry in embedded structure.
 */
M_INLINE void DDLIST_deleteall( DDLIST *list, DDLIST_VISITOR_V on_delete, void *context, int offset_of_link)
{
    DDLIST_entry *cur,*next,*del;

    DDLIST_FOREACH_SAVE(cur,next,list) {
		
		if (on_delete)  {			
			on_delete( list, cur, context);
		}

		del = DDLIST_unlink( list, cur );
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
M_INLINE void DDLIST_insert_sorted( DDLIST *list, DDLIST_COMPARE compare, DDLIST_entry *newentry) 
{
	DDLIST_entry *cur;
	
	DDLIST_FOREACH(  cur, list ) {
		if (compare(cur,newentry) > 0) {

			DDLIST_insert_before(list, cur,newentry);
			return;
		}
	}

	DDLIST_push_back( list, newentry );
}

/**
 * @brief Reverse a list
 * This function turns the first element into the last element, the second element
 * into the one before the last, and so on and on.
 * @param header (in) the object
 */
M_INLINE void DDLIST_reverse( DDLIST *lst )
{
	DDLIST_entry *cur = lst->root.prev, *next;
	DDLIST_entry *tmp;
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
M_INLINE int DDLIST_check(DDLIST *header)
{
	DDLIST_entry *cur,*next,*prev = 0;
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

	DDLIST_FOREACH( cur, header ) {

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

	return 1;
}

/**
 @}
 */

#ifdef  __cplusplus
}
#endif

#endif
