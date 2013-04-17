/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VBASE_DRING_H_
#define _VBASE_DRING_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>


/**
 * @defgroup DRING
 * @brief Double linked circular list data structure; where each list element can be of different length. Each element has a pointer to the next and previous element of the list.
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
 * @{
 */
typedef struct tagDRING 
{
  struct tagDRING *next,*prev;
}  
  DRING;


typedef void	 (*DRING_VISITOR_V) (DRING *entry, void *context);
typedef int32_t  (*DRING_VISITOR) (DRING *entry, void *context);
typedef int32_t  (*DRING_COMPARE) (DRING *, DRING *);


/**
 * @brief initialises an empty list head
 * @param head list head 
 */
M_INLINE void DRING_init( DRING *head ) 
{ 
  head->next = head->prev = head; 
}


/**
 * @brief checks if argument list is empty
 * @param list pointer to list.
 * @returns not zero for non empty list.
 */
M_INLINE int DRING_isempty( DRING *list ) 
{
  return list->next == list;
}

/**
 * @brief insert new entry before a given entry.
 * @param newentry pointer to new list entry (to be inserted).
 * @param list current position (newentry inserted before this one).
 */
M_INLINE void DRING_insert_before( DRING *list, DRING *newentry) 
{
   DRING *prev = list->prev;

   prev->next = newentry;    
   newentry->next = list;

   newentry->prev = prev; 
   list->prev = newentry; 
}

/**
 * @brief insert new entry after a given entry.
 * @param newentry pointer to new list entry (to be inserted).
 * @param list current posision (newentry inserted after this one).
 */
M_INLINE void DRING_insert_after( DRING *list, DRING *newentry) 
{
   DRING_insert_before( list->next, newentry );
}

/**
 * @brief insert new element into sorted list; Maintains ordering relationship between elements of linked list (sort by ascending order)
 * A sorting algorithm based on this function is not very efficient; it is of complexity O(n^2); nevertheless usefull if a list is modified and has to be always maintained in sorted order.
 * @param list (in) the object
 * @param compare (in) comparison function.
 * @param newentry (in) element that is to be inserted into list.
 */
 void DRING_insert_sorted( DRING *list, DRING_COMPARE compare, DRING *newentry); 

/**
 * @brief delete an element from a list.
 * @param list deletes this list element
 */
M_INLINE DRING *DRING_unlink( DRING *list ) 
{
   list->next->prev = list->prev;
   list->prev->next = list->next;
   return list;
}

/**
 * @brief insert element as last in list (used to maintain double ended queue)
 * @param list list head.
 * @param newentry entry to be inserted into list
 */
M_INLINE void DRING_push_back( DRING *list, DRING *newentry)
{
  DRING_insert_after( list->prev, newentry );
}

/**
 * @brief insert element as first in list (used to maintain double ended queue)
 * @param list list head.s
 * @param newentry entry to be inserted into list
 */
M_INLINE void DRING_push_front( DRING *list, DRING *newentry)
{
  DRING_insert_after( list, newentry );
}

/**
 * @brief remove first element from list (used to maintain double ended queue)
 * @param elem (in) list head
 * @return the first element of list, NULL if list empty
 */
M_INLINE DRING * DRING_pop_front( DRING *elem )
{
  DRING *ret;
  if (DRING_isempty( elem )) {
    return 0;
  }
  ret = elem->next;
  DRING_unlink(elem->next);
  return ret;  
}


/**
 * @brief remove last element from list (used to maintain double ended queue)
 * @param elem (in) list head.
 * @return the last element of list, NULL if list empty
 */
M_INLINE DRING * DRING_pop_back( DRING *elem )
{ 
  DRING *ret;

  if (DRING_isempty( elem )) {
    return 0;
  }
  ret = elem->prev;
  DRING_unlink(elem->prev);
  return ret;
}


M_INLINE DRING *DRING_get_first(DRING *list)
{
  if (DRING_isempty(list)) {
    return 0;
  }  
  return list->next;
}

M_INLINE DRING *DRING_get_last(DRING *list)
{
  if (DRING_isempty(list)) {
    return 0;
  }
  return list->prev;
}


M_INLINE DRING *DRING_get_next( DRING *end, DRING *cur ) 
{
  if (cur->next == end) { 
    return 0;
  }
  return cur->next;

}


M_INLINE DRING *DRING_get_prev( DRING *end, DRING *cur ) 
{
  if (cur->prev == end) { 
    return 0;
  }
  return cur->prev;

}

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DRING *) pointer to the current element
 * @param list (type DRING *) pointer to the list that is traversed
 */
#define DRING_FOREACH( loopvarname, list )\
  for((loopvarname) = (list)->next;\
      (loopvarname) != (list);\
      (loopvarname) = (loopvarname )->next )

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DRING *) pointer to the current element
 * @param list (type DRING *) pointer to the list that is traversed
 */
#define DRING_FOREACH_REVERSE( loopvarname, list )\
  for((loopvarname) = (list)->prev;\
      (loopvarname) != (list);\
      (loopvarname) = (loopvarname)->prev )

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DRING *) pointer to the current element
 * @param loopvarnamenext (type DRING *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DRING *) pointer to the list that is traversed
 */
#define DRING_FOREACH_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = (list)->next, (loopvarnext) = (loopvarname)->next;\
      (loopvarname) != (list);\
      (loopvarname) = (loopvarnext), (loopvarnext) = (loopvarname)->next )

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DRING *) pointer to the current element
 * @param loopvarnamenext (type DRING *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DRING *) pointer to the list that is traversed
 */
#define DRING_FOREACH_REVERSE_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = (list)->prev, (loopvarnext) = (loopvarname)->prev;\
      (loopvarname) != (list);\
      (loopvarname) = (loopvarnext), (loopvarnext) = (loopvarname)->prev )

/*
 * @brief: return number of elements in list
 * the whole list structure is traversed.
 */
M_INLINE size_t DRING_size( DRING *list )
{
	size_t sz = 0;
	DRING * cur;
	
	DRING_FOREACH( cur, list ) {
		sz += 1;
	}
	return sz;
}

/*
 * @brief get the nth element of a list as counted from the beginning of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE DRING *DRING_get_nth( DRING *list, size_t nth)
{

	DRING *elm;

	for(elm = DRING_get_first(list); elm != 0 && nth > 0; nth -= 1, elm = DRING_get_next(list, elm)); 

	return elm;
}

/*
 * @brief get the nth element of a list as counted from the end of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 * traverses the list twice: once to get size of list, then to get the element.
 */
M_INLINE DRING *DRING_get_nth_reverse( DRING *list, size_t nth)
{	
	return DRING_get_nth( list, DRING_size( list ) - nth );
}

/**
 * @brief iterate over all elements of a list, callback is invoked for either element of the list. list is traversed forward from first element to the last element.
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 */
 void DRING_foreach( DRING *lst, DRING_VISITOR_V eval, void *context, int save_from_del);

/**
 * @brief iterate over all elements of a list, callback is invoked for each element of the list. list is traversed backword from last element to the first element.
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 */
 void DRING_foreach_reverse( DRING *lst, DRING_VISITOR_V eval, void *context, int save_from_delete);

/**
 * @brief find an element within the linked list. callback is invoked for each element of the list, in forward direction from first element to last element; when the callback returns non zero value the iteration stops as we have found what we searched for.
 * 
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param retval (out) optional - the first non zero value returned by eval callback, if NULL then ignored.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 * @return the list element that has matched (i.e. element that has been found).
 */
M_INLINE DRING *DRING_findif( DRING *lst, DRING_VISITOR eval, void *context, int32_t *retval, int save_from_del)
{
	int ret;
    DRING *cur, *next;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
		return 0;
	}

	if (save_from_del) {
		DRING_FOREACH_SAVE( cur, next, lst) {
	   		ret = eval( cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}
		 		return cur;
	   		}
		}
	} else {
		DRING_FOREACH(  cur, lst ) {
			ret = eval( cur, context );
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
 * @param lastpos (out) position in the list that has been found.
 * @param save_from_del (in) if non zero then callback can remove the current link from the list (a bit slower); if null then not; if zero then can't remove the current link from the list.
 * @return the list element that has matched (i.e. element that has been found).
 */
M_INLINE DRING *DRING_find_reverse( DRING *lst, DRING_VISITOR eval, void *context, int32_t *retval, int save_from_delete)
{
	int ret;
	DRING *cur, *next;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
	  return 0;
	}

	if ( save_from_delete ) {
		DRING_FOREACH_REVERSE_SAVE( cur, next, lst ) {

	   		ret = eval( cur, context );
	   		if (ret) {
				if (retval) {
					*retval = ret;
				}
		 		return cur;
	   		}
		}
	} else {
		DRING_FOREACH_REVERSE( cur, lst ) {

	   		ret = eval( cur, context );
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
 * @param offset_of_link (in) offset of DRING in embedded structure.
 * @return nunmber of elements deleted.
 */
M_INLINE size_t DRING_deleteif( DRING *list, DRING_VISITOR check_if, void *context, int offset_of_link)
{
    DRING *cur,*next,*del;
	size_t  ret = 0;

    DRING_FOREACH_SAVE(cur,next,list) {
		if (!check_if || check_if( cur, context))  {
			del = DRING_unlink( cur );
 			if (offset_of_link != -1) {
				free( _MEMBEROF(del,offset_of_link) );
 			}
			ret ++;
		}
	}
	return ret;
}

/** 
 * @brief iterate over all entries of the list and delete them.
 * @param list (in) the object

 * @param on_delete(in) if not NULL then this callback is called prior to deleting each list element.
 * @param context (in) if on_delete is not NULL then this pointer is passed as argument to on_delete.

 * @param free_ctx allocation interface used to free data of entry.
 * @param offset_of_link offset of DRING in embedded structure.
 */
M_INLINE void DRING_deleteall( DRING *list, DRING_VISITOR_V on_delete, void *context, int offset_of_link)
{
    DRING *cur,*next,*del;

    DRING_FOREACH_SAVE(cur,next,list) {
		if (on_delete) {
			on_delete( cur, context);
		}
		del = DRING_unlink( cur );
 		if (offset_of_link != -1) {
			free( _MEMBEROF(del,offset_of_link) );
		}
	}
}

/**
 * @brief Reverse a list
 * This function turns the first element into the last element, the second element
 * into the one before the last, and so on and on.
 * @param header (in) the object
 */
M_INLINE void DRING_reverse( DRING *lst )
{
	DRING *cur = lst->prev, *next;
	DRING *tmp;

	if (cur) {
		next = cur->next;
		cur->next = lst;

		while(next) {
			tmp = next->next;
			
			next->next = cur;
			cur->prev = next;

			cur = next;
			next = tmp;
		}

		tmp = lst->prev;
		lst->prev = lst->next;
		lst->next = tmp;
	}
}

/**
 * @brief check consistency of list
 * @param lst list header.
 * @return zero if list contains errors.
 */
M_INLINE int DRING_check(DRING *header)
{
	DRING *cur,*next;
        size_t sz = 0;

	DRING_FOREACH( cur, header ) {
		if (!cur) {
		    return 0;
		}
		next = cur->next;
		if (!next || next->prev != cur) {		
			return 0;
		}
		sz += 1;
	}

	if (sz == 0) {
	    if (header->next != header->prev || header->next != header) {
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
