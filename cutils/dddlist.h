/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VBASE_DDLIST_H_
#define _VBASE_DDLIST_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>




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

/**
 * @defgroup DDDLIST_entry
 * @brief an entry in double linked list, add to structure as member in order to make structure storable in hash table.
 * If the user wants to link his struct(ure) into a DDLIST linked list, then he must embed a DDLIST_entry into his structure.
 * Access to user defined structure is via embedded DDDLIST_entry.
 * @{
 */
typedef struct tagDDDLIST
{
  struct tagDDDLIST *next,*prev;
}  
  DDDLIST;




/**
 * @brief initialises an empty list head
 * @param head (in) list head 
 */
M_INLINE void DDDLIST_init( DDDLIST *head ) 
{ 
  head->next = head->prev = 0; 
}



/**
 * @brief checks if argument list is empty
 * @param list pointer to list.
 * @returns not zero for non empty list.
 */
M_INLINE int DDDLIST_isempty( DDDLIST *head ) 
{
  return head->next == 0;
}

/**
 * @brief insert new entry before a given entry.
 * @param pos	   (in) current position (newentry inserted after this one).
 * @param newentry (in) pointer to new list entry (to be inserted).
 */
M_INLINE int DDLIST_insert_before( DDDLIST *pos, DDDLIST *newentry) 
{

    newentry->prev = pos->prev;
    pos->prev = newentry;
    newentry->next = pos;
    if (pos->prev) {
      pos->prev->next = newentry;
    }
    return 0;
}


/**
 * @brief insert new entry after a given entry.
 * @param pos	   (in) current position (newentry inserted after this one).
 * @param newentry (in) pointer to new list entry (to be inserted).
 */
M_INLINE void DDDLIST_insert_after( DDDLIST *pos, DDDLIST *newentry) 
{
    newentry->next = pos->next;
    pos->next = newentry;
    newentry->prev = pos;
    if (newentry->next) {
      newentry->next->prev = newentry;
    }
}


/**
 * @brief delete an element from a list.
 * @param list (in|out) the object
 * @param link (in) deletes this list element
 */
M_INLINE DDDLIST *DDDLIST_unlink( DDDLIST *link ) 
{
	DDDLIST *next,*prev;

	next = link->next;
	prev = link->prev;

	if (next) {
		next->prev = link->prev;
	}
	if (prev) {
		prev->next = link->next;
	}

        link->prev = link->next = 0;

	return link;
}



M_INLINE DDDLIST *DDDLIST_get_next( DDDLIST *cur) 
{
  return cur->next;
}


M_INLINE DDDLIST *DDLIST_get_prev( DDDLIST *cur ) 
{
  return cur->prev;
}

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DDLIST_entry *) pointer to the current element
 * @param list (type DDLIST *) pointer to the list that is traversed
 */
#define DDDLIST_FOREACH( loopvarname, list )\
  for((loopvarname) = (list)->next;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarname)->next )


/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DDLIST_entry *) pointer to the current element
 * @param loopvarnamenext (type DDLIST_entry *) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DDLIST *) pointer to the list that is traversed
 */
#define DDDLIST_FOREACH_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = (list)->next, (loopvarnext) = (loopvarname) ? (loopvarname)->next : 0;\
      (loopvarname) != 0;\
      (loopvarname) = (loopvarnext), (loopvarnext) = (loopvarname) ? (loopvarname)->next : 0)



/*
 * @brief: return number of elements in list; iterates over the whole list to get this number
 */
M_INLINE size_t DDDLIST_size( DDDLIST *list )
{
	DDDLIST *cur;
	size_t elmcount = 0;
	
	DDDLIST_FOREACH(  cur, list ) {
	  elmcount++;
	}
	return elmcount;
}

/*
 * @brief get the nth element of a list as counted from the beginning of the list.
 * @brief list (IN) the object
 * @brief nth  (IN) index of element to retrieve (null based).
 */
M_INLINE DDDLIST *DDDLIST_get_nth( DDDLIST *list, size_t nth)
{
	DDDLIST *elm;


	for(elm = list->next; elm && nth > 0; nth -= 1, elm = DDDLIST_get_next(elm)); 

	return elm;
}

/**
 * @brief check consistency of list
 * @param lst list header.
 * @return zero if list contains errors.
 */
M_INLINE int DDDLIST_check(DDDLIST *header)
{
	DDDLIST *cur,*next,*prev = header;

	DDDLIST_FOREACH( cur, header ) {

		next = cur->next;
		if (next && next->prev != cur) {		
			return 0;
		}
		if (cur->prev != prev) 
			return 0;
		
		prev = cur;
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
