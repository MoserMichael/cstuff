/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VBASE_SLIST_H_
#define _VBASE_SLIST_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>


/**
 * @defgroup SLIST_entry
 * @brief an entry in single linked list, add to structure as member in order to make structure storable in hash table.
 * If the user wants to link his struct(ure) into a SLIST linked list, then he must embed a DLIST_entry into his structure.
 * Access to user defined structure is via embedded SLIST_entry.
 * @{
 */
typedef struct tagSLIST_entry 
{
  struct tagSLIST_entry *next;
}  
  SLIST_entry;

/**
 * @}
 */


/**
 * @defgroup SLIST
 * @brief Single linked list data structure; where each list element can be of different length.
 *
 * This single linked list has a list header (SLIST).
 * The list header points to the first and last element in the list.
 * last elements next pointer is NULL.
 *
 * Usage: If the user wants to link his struct(ure) into a list, then he must embed a SLIST_entry into his structure.
 * Access to user defined structure is via embedded SLIST_entry.
 *
 * The list header contains a counter of elements (feature can be surpressed by defining #define SLIST_NO_ELMCOUNT
 * @{
 */
typedef struct {
#ifndef SLIST_NO_ELMCOUNT
	size_t elmcount;   /** number of elements in list */ 
#endif
	SLIST_entry *prev, *next; /** root.prev - pointer to first element,  root.next - pointer to last element */
} SLIST;



typedef void	(*SLIST_VISITOR_V)(SLIST *list, SLIST_entry *entry, void *context);
typedef int32_t (*SLIST_VISITOR)  (SLIST *list, SLIST_entry *entry, void *context);
typedef int32_t (*SLIST_COMPARE)  (SLIST_entry *,  SLIST_entry * );


/**
 * @brief initialises an empty list head
 * @param head list head 
 */
M_INLINE void SLIST_init( SLIST *head ) 
{ 
  head->next = head->prev = 0; 
#ifndef SLIST_NO_ELMCOUNT
  head->elmcount = 0;
#endif
}

/**
 * @brief checks if argument list is empty
 * @param list pointer to list.
 * @returns not zero for non empty list.
 */
M_INLINE int SLIST_isempty( SLIST *head ) 
{
  return head->next == 0;
}


/**
 * @brief insert new entry after a given entry.
 * @param list pointer to list head
 * @param pos current posision (newentry inserted after this one).
 * @param newentry pointer to new list entry (to be inserted).
 */
M_INLINE void SLIST_insert_after( SLIST *list, SLIST_entry *pos, SLIST_entry *newentry) 
{
	if (list->next) {

		if (pos){
		
			newentry->next = pos->next;
			pos->next = newentry;

			if (!newentry->next) {
				list->next = newentry;
			} 

		} else  {
			newentry->next = list->prev;
			list->prev = newentry;
		} 


	} else {

		list->next = list->prev = newentry;
		newentry->next =  0;		

	}


#ifndef SLIST_NO_ELMCOUNT
	list->elmcount ++;
#endif
}


/**
 * @brief delete an element from a list.
 * @param list deletes this list element
 */
M_INLINE SLIST_entry *SLIST_unlink_after( SLIST *list, SLIST_entry *link ) 
{
	SLIST_entry *ret;

	if (!link) {
		ret = list->prev;
		list->prev = ret->next;
		if (!list->prev) {
			list->next = 0;
		}
		
	} else {
		ret = link->next;	
		if (!ret) {
			return 0;
		}

		link->next = ret ? ret->next : 0;

		if (link->next == 0) {
			list->next = link;
		}

	}
#ifndef SLIST_NO_ELMCOUNT
		list->elmcount --;
#endif

	return ret;
}


/**
 * @brief insert element as last in list.
 * @param list list head.
 * @param newentry entry to be inserted into list
 */
M_INLINE void SLIST_push_back( SLIST *list, SLIST_entry *newentry)
{
  SLIST_insert_after( list, list->next, newentry );
}

/**
 * @brief insert element as first in list (used to maintain queue)
 * @param list list head
 * @param newentry entry to be inserted into list
 */
M_INLINE void SLIST_push_front( SLIST *list, SLIST_entry *newentry)
{
  SLIST_insert_after( list, 0, newentry );
}

/**
 * @brief append contents of one list onto the other
 * @param dest 
 * @param src  content of this list is appended onto dest list. postcondition: src list is empty
 */

M_INLINE void SLIST_append( SLIST *dest, SLIST *src) 
{
	if (SLIST_isempty(dest)) {
		
		if (!SLIST_isempty(src)) {

			dest->prev = src->prev;
			dest->next = src->next;
#ifndef SLIST_NO_ELMCOUNT
			dest->elmcount = src->elmcount;
#endif	

			//SLIST_init(src);
		} 

	} else {

		if (!SLIST_isempty(src)) {
			dest->next->next = src->prev;
			dest->next = src->next;
#ifndef SLIST_NO_ELMCOUNT
			dest->elmcount += src->elmcount;
#endif	
			//SLIST_init(src);

		}
	}


}


/**
 * @brief remove the first element from list (used to maintain queue)
 * @param list (in) the object
 * @return the first element of list, NULL if list empty
 */
M_INLINE SLIST_entry * SLIST_pop_front( SLIST *list )
{
  return SLIST_unlink_after(list, 0);
}


M_INLINE SLIST_entry *SLIST_get_first(SLIST *list)
{
  return list->prev;
}

M_INLINE SLIST_entry *SLIST_get_last(SLIST *list)
{
  return list->next;
}


M_INLINE SLIST_entry *SLIST_get_next( SLIST_entry *cur) 
{
  return cur->next;
}




/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type SLIST_entry *) pointer to the current element
 * @param list (type SLIST *) pointer to the list that is traversed
 */
#define SLIST_FOREACH( loopvarname, list )\
  for((loopvarname) = (list)->prev;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarname)->next )

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvsarname (type SLIST_entry *) pointer to the current element
 * @param list (type SLIST *) pointer to the list that is traversed
 */
#define SLIST_FOREACH_SAVE( loopvarname, loopvarnamenext, list )\
  for((loopvarname) = (list)->prev, (loopvarnamenext) = (loopvarname) ? (loopvarname)->next : 0;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarnamenext),  (loopvarnamenext) = (loopvarname) ? (loopvarname)->next : 0 )


/**
 * @brief: return number of elements in list
 * if we don't have element count in list (SLIST_NO_ELMCOUNT defined), then the whole list structure is traversed.
 */
M_INLINE size_t SLIST_size( SLIST *list )
{
#ifndef SLIST_NO_ELMCOUNT
	return list->elmcount;
#else
	size_t sz;
	SLIST_entry * cur;
	
	SLIST_FOREACH( cur, list ) {
		sz += 1;
	}
	return sz;
#endif
}

/*
 * @brief get the nth element of a list as counted from the beginning of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE SLIST_entry *SLIST_get_nth( SLIST *list, size_t nth)
{

	SLIST_entry *elm;

#ifndef SLIST_NO_ELMCOUNT
	if (nth >= list->elmcount) {
		return 0;
	}

	for(elm = SLIST_get_first(list); nth > 0; nth -= 1, elm = SLIST_get_next(elm)); 
#else
	for(elm = SLIST_get_first(list); elm != 0 && nth > 0; nth -= 1, elm = SLIST_get_next(elm)); 
#endif

	return elm;
}

/**
 * @brief get the nth element of a list as counted from the end of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE SLIST_entry *SLIST_get_nth_reverse( SLIST *list, size_t nth)
{	
	return SLIST_get_nth( list, SLIST_size( list ) - nth );
}


/**
 * @brief iterate over all elements of a list, callback is invoked for either element of the list.
 *
 * @param lst (in) the object.
 * @param eval (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 */
M_INLINE void SLIST_foreach( SLIST *lst, SLIST_VISITOR_V eval, void *context)
{
    SLIST_entry *cur;

	if (!eval) {
	  return;
	}

	SLIST_FOREACH( cur, lst) {
		eval( lst, cur, context);
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
M_INLINE SLIST_entry *SLIST_findif( SLIST *lst, SLIST_VISITOR eval, void *context, int32_t *retval)
{
	int ret;
    SLIST_entry  *cur;

	if (retval) {
		*retval = 0;
	}

	if (!eval) {
	  return 0;
	}

	SLIST_FOREACH(  cur, lst ) {
		ret = eval( lst, cur, context);
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
 * @brief insert new element into sorted list; Maintains ordering relationship between elements of linked list (sort by ascending order)
 * A sorting algorithm based on this function is not very efficient; it is of complexity O(n^2); nevertheless usefull if a list is modified and has to be always maintained in sorted order.
 * @param list (in) the object
 * @param compare (in) comparison function.
 * @param newentry (in) element that is to be inserted into list.
 */
M_INLINE void SLIST_insert_sorted( SLIST *list, SLIST_COMPARE compare, SLIST_entry *newentry) 
{
	SLIST_entry *cur,*prev;
	
	prev = 0;
	SLIST_FOREACH(  cur, list ) {
		if (compare(cur,newentry) > 0) {

			SLIST_insert_after(list, prev,newentry);
			return;
		}
		prev = cur;
	}

	SLIST_push_back( list, newentry );
}

/** 
 * @brief iterate over all entries of the list and delete entries that match predicate from the list, and frees the memory (optionally)
 * @param list (in) the object.
 * @param check_if (in) predicate function; the function returns 1 then inspected argument element will be deleted; if argument pointer is NULL then all entries will be deleted. 
 * @param context (in) data pointer passed to every invocation of check_if
 * @param free_ctx (in) interface used to free data of entry, is argument pointer is NULL then nothing is freed.
 * @param offset_of_link (in) offset of DLIST_entry in embedded structure.
 */
M_INLINE void SLIST_deleteif( SLIST *list, SLIST_VISITOR check_if, void *context,  int offset_of_link)
{
    SLIST_entry *cur,*prev,*del;

	prev = 0;

	for(cur = list->prev; cur;) {
    
		if (!check_if || check_if( list, cur, context))  {

			cur = cur->next;
			del = SLIST_unlink_after( list, prev );
			if (offset_of_link != -1) {
				free( _MEMBEROF(del, offset_of_link) );
			}
			

		} else {

			prev = cur;
			cur = cur->next;
		}
	}
}

/** 
 * @brief iterate over all entries of the list and delete them.
 * @param list (in) the object

 * @param on_delete(in) if not NULL then this callback is called prior to deleting each list element.
 * @param context (in) if on_delete is not NULL then this pointer is passed as argument to on_delete.

 * @param free_ctx (in) allocation interface used to free data of entry.
 * @param offset_of_link (in) offset of SRING in embedded structure.
 */
M_INLINE void SLIST_deleteall( SLIST *list, SLIST_VISITOR_V on_delete, void *context, int offset_of_link)
{
    SLIST_entry *cur,*del;


	for(cur = list->prev; cur;) {
    
		if (on_delete)  {
			on_delete( list, cur, context );
		}

		cur = cur->next;
		del = SLIST_unlink_after( list, 0 );
		if (offset_of_link != -1) {
			free( _MEMBEROF(del, offset_of_link) );
		}
	}
}


/**
 * @brief Reverse a list
 * This function turns the first element into the last element, the second element
 * into the one before the last, and so on and on.
 * @param header (in) the object
 */
M_INLINE void SLIST_reverse( SLIST *lst )
{
	SLIST_entry *cur = lst->prev, *next;
	SLIST_entry *tmp;
	if (cur) {
		next = cur->next;
		cur->next = 0;

		while(next) {
			tmp = next->next;
			
			next->next = cur;
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
 * This function checks for loops in the list, and if count of elements in list is the same as counter of elements in list header.
 * @param header (in) the object
 * @return zero if list contains errors.
 */
M_INLINE int SLIST_check(SLIST *header)
{
	SLIST_entry *cur,*prev = 0,*fast= 0;
#ifndef SLIST_NO_ELMCOUNT
	size_t count = 0;
#endif

	if (!header->prev || !header->next) {
		if (header->prev || header->next) {
			return 0;
		}
		return 1;
	}
	
	fast = header->prev;
	SLIST_FOREACH( cur, header ) {

#ifndef SLIST_NO_ELMCOUNT
		count += 1;
#endif	    
		prev = fast;
		fast = fast->next;
		if (!fast) {
			break;
		}
		if (fast == cur) {
			return 0;
		}

#ifndef SLIST_NO_ELMCOUNT
		count += 1;
#endif
	    prev = fast;
		fast = fast->next;
		if (!fast) {
			break;
		}
		if (fast == cur) {
			return 0;
		}
	}

	if (header->next != prev) {
		return 0;
	}


#ifndef SLIST_NO_ELMCOUNT
	if (header->elmcount != count) {
		return 0;
	}
#endif	
	return 1;
}

/**
 * @}
 */

#ifdef  __cplusplus
}
#endif

#endif
