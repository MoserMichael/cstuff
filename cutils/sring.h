/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VBASE_SRING_H_
#define _VBASE_SRING_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>

/**
 * @defgroup SRING
 * @brief Single linked circular list data structure; where each list element can be of different length. Each element has a pointer to the next element of the list.
 *
 * In a circularly-linked list, the first and final nodes are linked together. 
 * To traverse a circular linked list, you begin at any node and follow the list 
 * in either direction until you return to the original node. Viewed another way, 
 * circularly-linked lists can be seen as having no beginning or end. This type of 
 * list is most useful for managing buffers for data ingest, and in cases where you 
 * have one object in a list and wish to see all other objects in the list.
 * The pointer pointing to the whole list is usually called the end pointer.
 * (quoted from http://en.wikipedia.org/wiki/Linked_list )
 *
 * Usage: If the user wants to link his struct(ure) into a list, then he must embed a SLIST_entry into his structure.
 * Access to user defined structure is via embedded DRING.
 *
 * Note: unlike SLIST it is not easy to get the last element in the list; in SLIST we have a pointer to the last element
 * int the list header; with SRING we have to traverse the whole list in order to get to the last element.
 * @{
 */
typedef struct tagSRING 
{
  struct tagSRING *next;
}  
  SRING;

typedef void	 (*SRING_VISITOR_V)(SRING *entry, void *context);
typedef int32_t  (*SRING_VISITOR)	(SRING *entry, void *context);
typedef int32_t  (*SRING_COMPARE)	(SRING *, SRING *);

/**
 * @brief initialises an empty list head
 * @param head list head 
 */
M_INLINE void SRING_init( SRING *list ) 
{ 
  list->next = list; 
}

/**
 * @brief checks if argument list is empty
 * @param list pointer to list.
 * @returns not zero for non empty list.
 */
M_INLINE int SRING_isempty( SRING *list ) 
{
  return list->next == list;
}

/**
 * @brief insert new entry after a given entry.
 * @param newentry pointer to new list entry (to be inserted).
 * @param list current position (newentry inserted after this one).
 */
M_INLINE void SRING_insert_after( SRING *list, SRING *newentry) 
{
   newentry->next = list->next;
   list->next = newentry;
}


M_INLINE SRING * SRING_unlink_after( SRING *list ) 
{
   SRING *ret;
   
   if (SRING_isempty(list)) {
		return 0;
   }

   ret = list->next;
   list->next = list->next->next;
   return ret;
}


M_INLINE void SRING_push_front( SRING *list, SRING *newentry)
{
  SRING_insert_after( list, newentry );
}

/**
 * @brief remove the first element from list (used to maintain double ended queue)
 * @param elem (in) list head
 * @return the first element of list, NULL if list empty
 */
M_INLINE SRING * SRING_pop_front( SRING *elem )
{
  SRING *ret;
  if (SRING_isempty( elem )) {
    return 0;
  }
  ret = elem->next;
  SRING_unlink_after(elem);
  return ret;  
}


M_INLINE SRING *SRING_get_first(SRING *list)
{

  if (SRING_isempty(list)) {
    return 0;
  }  
  return list->next;
}


M_INLINE SRING *SRING_get_last(SRING *list)
{
  SRING *cur;

  if (SRING_isempty(list)) {
    return 0;
  }  
  
  for(cur = list;cur->next != list; cur = cur->next);

  return cur;
}

M_INLINE SRING *SRING_get_next( SRING *end, SRING *cur ) 
{
  if (cur->next == end) { 
    return 0;
  }
  return cur->next;

}

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type SRING *) pointer to the current element
 * @param list (type SRING *) pointer to the list that is traversed
 */
#define SRING_FOREACH( loopvarname, list )\
  for((loopvarname) = (list)->next;\
      (loopvarname) != (list);\
      (loopvarname) = (loopvarname )->next )



/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DRING *) pointer to the current element
 * @param loopvarnamenext (type DRING *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DRING *) pointer to the list that is traversed
 */
#define SRING_FOREACH_SAVE( loopvarname, loopvarnext, list ) \
  (loopvarname) = (list); (loopvarnext) = (loopvarname)->next; \
  do {


#define SRING_FOREACH_SAVE_END( loopvarname, loopvarnext, list )\
         (loopvarname) = (loopvarnext); \
	 (loopvarnext) = (loopvarnext)->next; \
   } while ((loopvarname) != (list)->next );


/*
 * @brief: return number of elements in list
 * the whole list structure is traversed.
 */
M_INLINE size_t SRING_size( SRING *list )
{
	size_t sz;
	SRING * cur;
	
	SRING_FOREACH( cur, list ) {
		sz += 1;
	}
	return sz;
}

/*
 * @brief get the nth element of a list as counted from the beginning of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE SRING *SRING_get_nth( SRING *list, size_t nth)
{

	SRING *elm;

	for(elm = SRING_get_first(list); elm != 0 && nth > 0; nth -= 1, elm = SRING_get_next(list, elm)); 

	return elm;
}

/*
 * @brief get the nth element of a list as counted from the end of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 * traverses the list twice: once to get size of list, then to get the element.
 */
M_INLINE SRING *SRING_get_nth_reverse( SRING *list, size_t nth)
{	
	return SRING_get_nth( list, SRING_size( list ) - nth );
}


/**
 * @brief iterate over all elements of a list, callback is invoked for either element of the list.
 *
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 */
M_INLINE void SRING_foreach( SRING *lst, SRING_VISITOR_V eval, void *context)
{
	SRING *cur;

	if (!eval) {
	  return ;
	}

	SRING_FOREACH(  cur, lst ) {
		eval( cur, context );

	}		
}

/**
 * @brief find an element within the linked list. callback is invoked for each element of the list, in forward direction from first element to last element; when the callback returns non zero value the iteration stops as we have found what we searched for.
 * 
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param retval (out) optional - the first non zero value returned by eval callback, if NULL then ignored.
 * @return the list element that has matched (i.e. element that has been found).
 */
M_INLINE SRING *SRING_findif( SRING *lst, SRING_VISITOR eval, void *context, int32_t *retval)
{
	int ret;
	SRING *cur;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
	  return 0;
	}

	SRING_FOREACH(  cur, lst ) {
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

/** 
 * @brief iterate over all entries of the list and delete entries that match predicate from the list, and frees the memory (optionally)
 * @param list (in) the object.
 * @param check_if (in) predicate function; the function returns 1 then inspected argument element will be deleted; if argument pointer is NULL then all entries will be deleted. 
 * @param context (in) data pointer passed to every invocation of check_if
 * @param free_ctx (in) interface used to free data of entry, is argument pointer is NULL then nothing is freed.
 * @param offset_of_link (in) offset of SRING in embedded structure.
 */
M_INLINE size_t SRING_deleteif( SRING *list, SRING_VISITOR check_if, void *context,  int offset_of_link)
{
    SRING *cur,*prev,*del;
	size_t  ret;

	prev = list;
	ret = 0;

	for(cur = prev->next; cur != list;) {
    
		if (!check_if || check_if( cur, context))  {

			cur = cur->next;
			del = SRING_unlink_after( prev );
			if (offset_of_link != -1) {
				free( _MEMBEROF(del, offset_of_link) );
			}
			ret++;

		} else {

			prev = cur;
			cur = cur->next;
		}
	}
	return ret;
}

/** 
 * @brief iterate over all entries of the list and delete them.
 * @param list (in) the object

 * @param on_delete(in) if not NULL then this callback is called prior to deleting each list element.
 * @param context (in) if on_delete is not NULL then this pointer is passed as argument to on_delete.

 * @param free_ctx (in) allocation interface used to free data of entry.
 * @param offset_of_link (in) offset of SRING in embedded structure.
 * @return nunmber of elements deleted.
 */
M_INLINE void SRING_deleteall( SRING *list, SRING_VISITOR_V on_delete, void *context, int offset_of_link)
{
    SRING *cur,*prev,*del;

	prev = list;

	for(cur = prev->next; cur != list;) {
    
		if (on_delete) {
			on_delete( cur, context);
		}

		cur = cur->next;
		del = SRING_unlink_after( prev );
		if (offset_of_link != -1) {
			free( _MEMBEROF(del, offset_of_link) );
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
M_INLINE void SRING_insert_sorted( SRING *list, SRING_COMPARE compare, SRING *newentry) 
{
	SRING *cur,*prev;
	
	prev = list;
	SRING_FOREACH(  cur, list ) {
		if (compare(cur,newentry) > 0) {

			SRING_insert_after(prev,newentry);
			return;
		}
		prev = cur;
	}

	SRING_insert_after(prev,newentry);
}

/**
 * @brief Reverse a list
 * This function turns the first element into the last element, the second element
 * into the one before the last, and so on and on.
 * @param header (in) the object
 */
M_INLINE void SRING_reverse( SRING *lst )
{
	SRING *cur = lst->next, *next, *tmp;

	if (cur) {
		next = cur->next;
		cur->next = lst;

		while(next != lst) {
			tmp = next->next;
			
			next->next = cur;
			cur = next;
			next = tmp;
		}
		lst->next = cur;
	}
}


/**
 * verify list for consistency, i.e. that it does not have loops.
 * Uses Floyds cycle finding algorithm.
 * http://en.literateprograms.org/Floyd%27s_cycle-finding_algorithm_%28C%29
 * @return 0 if list is inconsistent.
 */
M_INLINE size_t SRING_check( SRING *head )
{
	SRING *slow, *fast = head->next;

	SRING_FOREACH( slow, head ) {
		if (!slow) {
			return 0;
		}
		fast = fast->next;
		if (!fast) {
			return 0;
		}
		if (fast == head) {
			return 1;
		}
		if (fast == slow) {
			return 0;
		}

		fast = fast->next;
		if (!fast) {
			return 0;
		}
		if (fast == head) {
			return 1;
		}
		if (fast == slow) {
			return 0;
		}
	}
	return 1;
}

/**
 * @}
 */
 

#ifdef  __cplusplus
}
#endif

#endif
