#ifndef _DLISTUNR_H_
#define _DLISTUNR_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/base.h>
#include <cutils/dring.h>
#include <stdlib.h>
#include <string.h>


typedef struct tagDLISTUNR_entry {
  DRING  ring;	
  size_t elmcount;
  uint8_t buffer[];
} DLISTUNR_entry;



/**
 * @defgroup DLISTUNR
 * @brief unrolled list where each list node contains an array of nodes.
 * Unlike other lists here, each element in a list is of the same size.

 * an unrolled linked list is a variation on the linked list which stores multiple 
 * elements in each node. It can drastically increase cache performance, 
 * while decreasing the memory overhead associated with storing links to other list elements.. 
 *
 * Each node holds up to a certain maximum number of elements, typically just large 
 * enough so that the node fills a single cache line or a small multiple thereof. 
 * A position in the list is indicated by both a reference to the node and a position 
 * in the elements array. It's also possible to include a previous pointer for an unrolled 
 * doubly-linked linked list.
 * Quoted from http://en.wikipedia.org/wiki/Unrolled_linked_list
 *
 * @{
 */
typedef struct {

	size_t elmsize;			/* size of element */
	size_t elmmaxcount;		/* number of elements in one entry */

	size_t elmcount;		/* size of elements in list (global) */
	size_t entrycount;		/* number of entries */

	DLISTUNR_entry root;

}
	DLISTUNR;

typedef void (*DLISTUNR_VISITOR_V)(DLISTUNR *list, void *entry, void *context);
typedef int  (*DLISTUNR_VISITOR)  (DLISTUNR *list, void *entry, void *context);

/**
 * Current position of iteration through an unrolled linked list structure.
 */
typedef struct {
	DLISTUNR_entry *entry;
	size_t				  index;
} DLISTUNR_position;


/**
 * @brief initialises a new unrolled list. List entries are all of a fixed size.
 * @param ctx (in) allocator interface. (if null we are using the default allocator)
 * @param list (in) object that is initialised
 * @param elmsize (in) size of one element in list.
 * @param elmmaxcount (in) number of element stored in one list entry
 */
M_INLINE int	DLISTUNR_init(  DLISTUNR *list, size_t elmsize, size_t elmmaxcount)
{	
	list->elmmaxcount = elmmaxcount;
	list->elmsize = elmsize;

	list->elmcount = list->entrycount = 0;

	DRING_init( &list->root.ring );
	list->root.elmcount = (size_t) -1;

	return 0;
}

/**
 * @brief check validity of unrolled list instance
 * @param list the object
 * @return 0 if list is invalid.
 */
M_INLINE int DLISTUNR_check(DLISTUNR *list)
{
	size_t elmcount, size = 0;
	size_t cursize = 0;
	DRING *cur,*next;

	if (list->root.elmcount != (size_t) -1) {
		return 0;
	}

	DRING_FOREACH( cur, &list->root.ring)	{

		next = cur->next;
		if (!next || next->prev != cur) {		
			return 0;
		}
		elmcount = ((DLISTUNR_entry *) cur)->elmcount;
		if ( elmcount > list->elmmaxcount) {
			return 0;
		}
		if (elmcount == 0) {
			return 0;
		}
		size++;
		cursize += elmcount;
	}

	if (size != list->entrycount) {
		return 0;
	}
	if (cursize != list->elmcount) {
		return 0;
	}

	return 1;
}


/**
 * @brief checks if argument list is empty
 * @param list pointer to list.
 * @returns not zero for non empty list.
 */
M_INLINE int DLISTUNR_isempty( DLISTUNR *list )
{
	return list->elmcount == 0;
}


M_INLINE size_t DLISTUNR_size( DLISTUNR *list) 
{
	return list->elmcount;
}


M_INLINE size_t DLISTUNR_maxsize( DLISTUNR  *list )
{
	return list->elmmaxcount;
}

/**
 * @brief Returns position structure of first element in unrolled linked list.
 * @param list (in) the object
 * @return position structure of first element
 */
M_INLINE DLISTUNR_position DLISTUNR_get_first( DLISTUNR *list )
{
	DLISTUNR_position ret;

	ret.entry = (DLISTUNR_entry *) list->root.ring.next;
	ret.index = 0;

	return ret;
}


/**
 * @brief Returns position structure of last element in unrolled linked list.
 * @param list (in) the object
 * @return position structure of first element
 */
M_INLINE DLISTUNR_position DLISTUNR_get_last( DLISTUNR *list )
{
	DLISTUNR_position ret;

	if (list->elmcount) {
		ret.entry = (DLISTUNR_entry *) list->root.ring.prev;
		ret.index = ret.entry->elmcount - 1;		
	} else  {
		ret = DLISTUNR_get_first( list);
	}
	
	return ret;
}

/**
 * @brief Returns position to the next element in unrolled linked list.
 * @param list (in) the object
 * @param pos (in) iteration position in list.
 * @return position structure of the element that follows the element identified by pos structure.
 */
M_INLINE DLISTUNR_position DLISTUNR_next(DLISTUNR_position pos)
{
	DLISTUNR_position ret;
	DLISTUNR_entry *entry;

	ret = pos;
	entry = pos.entry;

	ret.index++;
	if (ret.index >= entry->elmcount) {
		ret.entry = (DLISTUNR_entry *) entry->ring.next;
		ret.index = 0;
	}
	return ret;
}

/**
 * @brief Returns position to the previous element in unrolled linked list.
 * @param list (in) the object
 * @param pos (in) iteration position in list.
 * @return position structure of the element that precedes the element identified by pos structure.
 */
M_INLINE DLISTUNR_position DLISTUNR_prev(DLISTUNR_position pos)
{
	DLISTUNR_position ret;
	DLISTUNR_entry *entry;

	ret = pos;
	entry = pos.entry;
	if (ret.index) {
		ret.index--;
	} else {
		ret.entry = (DLISTUNR_entry *) entry->ring.prev;
		ret.index = 0;
		if (ret.entry && ret.entry->elmcount != (size_t) -1) {
			ret.index = ret.entry->elmcount - 1;
		} 
	}
	return ret;
}


/**
 * @brief verify a position structure.
 */
M_INLINE int DLISTUNR_check_position( DLISTUNR_position pos)
{
	if (!pos.entry) {
		return -1;
	}
	if (pos.index >= pos.entry->elmcount) {
		return -1;
	}
	return 0;
}

/** 
 * @brief copy list entry identified by position structure (pos) into  user supplied memory area
 * @param list (in) the object
 * @param pos  (in) position pointer
 * @param data (in|out) user supplied buffer
 * @param size (in) size of user supplied buffer (must be equal to the size of one element).
 * @return 0 on success -1 on failure
 */
M_INLINE int DLISTUNR_copy_at(DLISTUNR *list, DLISTUNR_position pos, void *data, size_t size)
{
	void *ptr;
	if (size != list->elmsize) {
		return -1;
	}
	if (DLISTUNR_check_position(pos)) {
		return -1;
	}

	ptr = pos.entry->buffer + pos.index * size;
	memcpy(data, ptr, size);

	return 0;
}

/** 
 * @brief return pointer to list entry identified by position structure (pos).
 * @param list (in) the object
 * @param pos  (in) position pointer
 * @return pointer to data entry in linked list.
 */

M_INLINE uint8_t * DLISTUNR_at(DLISTUNR *list, DLISTUNR_position pos)
{
	if (DLISTUNR_check_position(pos)) {
		return 0;
	}

	return pos.entry->buffer + pos.index * list->elmsize;
}

/**
 * @brief insert new entry after a given entry into this unrolled linked list 
 * @param list (in) pointer to list head
 * @param pos  (in) current position (newentry inserted after this one).
 * @param data (in) pointer to element that is to be inserted into this list.
 * @param size (in) size of area identified by data pointer.
 */
int DLISTUNR_insert_after(DLISTUNR *list, DLISTUNR_position pos, void *data, size_t size);

/**
 * @brief insert new entry before a given entry into this unrolled linked list 
 * @param list (in) pointer to list head
 * @param pos  (in) current position (newentry inserted before this one).
 * @param data (in) pointer to element that is to be inserted into this list.
 * @param size (in) size of area identified by data pointer.
 */
M_INLINE int DLISTUNR_insert_before(DLISTUNR *list, DLISTUNR_position pos, void *data, size_t size)
{
	DLISTUNR_position insert_pos = DLISTUNR_prev(pos);
	
	return DLISTUNR_insert_after( list, insert_pos, data, size);
}


/**
 * @brief delete an element from a unrolled list.
 * @param list (in) the object
 * @param pos (in)  deletes element identified by this position structure
 * return -1 on failure 0 on success.
 */
 int DLISTUNR_unlink(DLISTUNR *list, DLISTUNR_position pos);


/**
 * @brief insert element as last in list (used to maintain queue)
 * @param list (in) the object
 * @param data (in) pointer to element that is to be inserted into this list.
 * @param size (in) size of area identified by data pointer.
 */
M_INLINE int DLISTUNR_push_back(DLISTUNR *list, void *data, size_t size)
{
	DLISTUNR_position last = DLISTUNR_get_last( list );
	return DLISTUNR_insert_after(list, last, data, size);
		
}

/**
 * @brief insert element as last in list (used to maintain queue)
 * @param list (in) the object
 * @param data (in) pointer to element that is to be inserted into this list.
 * @param size (in) size of area identified by data pointer.
 */
M_INLINE int DLISTUNR_push_front(DLISTUNR *list, void *data, size_t size)
{
	DLISTUNR_position last = DLISTUNR_get_first( list );
	return DLISTUNR_insert_before(list, last, data, size);
}


/**
 * @brief copy data of last element into user supplied buffer and remove the last element from list (used to maintain double ended queue)
 * @param list (in) the object
 * @param data (in) pointer to element that is to receive data of list entry
 * @param size (in) size of area identified by data pointer.
 * @return return 0 on success.
 */
M_INLINE int DLISTUNR_pop_back(DLISTUNR *list, void *data, size_t size)
{
	DLISTUNR_position pos;

	
	if (DLISTUNR_isempty(list)) {
		return -1;
	}
	
	pos = DLISTUNR_get_last( list );
	
	if (DLISTUNR_copy_at(list, pos, data, size)) {
		return -1;
	}

	if (DLISTUNR_unlink(list,pos)) {
		return -1;
	}
	return 0;
}

/**
 * @brief copy data of first element into user supplied buffer and remove the first element from list (used to maintain double ended queue)
 * @param list (in) the object
 * @param data (in) pointer to element that is to receive data of list entry
 * @param size (in) size of area identified by data pointer.
 * @return return 0 on success.
 */
M_INLINE int DLISTUNR_pop_front(DLISTUNR *list, void *data, size_t size)
{
	DLISTUNR_position pos;

	
	if (DLISTUNR_isempty(list)) {
		return -1;
	}
	
	pos = DLISTUNR_get_first( list );

	if (DLISTUNR_copy_at(list, pos, data, size)) {
		return -1;
	}
	if (DLISTUNR_unlink(list,pos)) {
		return -1;
	}
	return 0;
}

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DLISTUNR_position) struct that identifies current element
 * @param list (type DLISTUNR *) pointer to the list that is traversed
 */
#define DLISTUNR_FOREACH( loopvarname, list )\
  for((loopvarname) = DLISTUNR_get_first( list );\
      (loopvarname).entry != (DLISTUNR_entry *) &(list)->root;\
      (loopvarname) = DLISTUNR_next( loopvarname ) )

/**
 * @brief Macro for iterate over all elements of a list, the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DLISTUNR_position) pointer to the current element
 * @param list (type DLISTUNR *) pointer to the list that is traversed
 */
#define DLISTUNR_FOREACH_REVERSE( loopvarname, list )\
  for((loopvarname) = DLISTUNR_get_last( list );\
      (loopvarname).entry != (DLISTUNR_entry *) &(list)->root;\
      (loopvarname) = DLISTUNR_prev( loopvarname ) )

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in forward direction from first element to the last element.
 * @param loopvarname (type DLISTUNR_position) pointer to the current element
 * @param loopvarnamenext (type DLISTUNR_position) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DLISTUNR *) pointer to the list that is traversed
 */
#define DLISTUNR_FOREACH_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = DLISTUNR_get_first( list ), (loopvarnext) = DLISTUNR_next( loopvarname );\
      (loopvarname).entry != (DLISTUNR_entry *) &(list)->root;\
      (loopvarname) = (loopvarnext), (loopvarnext) = DLISTUNR_next(loopvarname) )

/**
 * @brief Macro for iterate over all elements of a list, You may delete the current element; the list is traversed in reverse direction from last element to the first element.
 * @param loopvarname (type DLISTUNR_position) pointer to the current element
 * @param loopvarnamenext (type DLISTUNR_position) do not modify! pointer to next element after current element (used for iteration).
 * @param list (type DLISTUNR *) pointer to the list that is traversed
 */
#define DLISTUNR_FOREACH_REVERSE_SAVE( loopvarname, loopvarnext, list )\
  for((loopvarname) = DLISTUNR_get_last(list);\
      (loopvarname).entry != (DLISTUNR_entry *) &(list)->root;\
      (loopvarname) = (loopvarnext), (loopvarnext) = DLISTUNR_prev(loopvarname) )


/**
 * @brief list free all entries of the list. The list will then be an empty list.
 */
M_INLINE void DLISTUNR_free( DLISTUNR *list, DLISTUNR_VISITOR_V free_func, void *context)
{
	DRING *next,*cur;
	DLISTUNR_entry *curr;
	size_t pos,elmsize,i;
	
	elmsize = list->elmsize;

	DRING_FOREACH_SAVE( cur, next, &list->root.ring) {
	
		curr = (DLISTUNR_entry *) cur;
		if (free_func) {
			for(pos = 0,i=0;i<curr->elmcount;i++) {
				free_func(list,  (void *) (curr->buffer +pos), context);
				pos += elmsize;
			}
		}
		/*list->ctx->free( DRING_unlink(cur) );*/
		free( cur );
		
	}	
	DRING_init( &list->root.ring );
	list->entrycount = 0;
	list->elmcount = 0;
}	

#if 0
M_INLINE void DLISTUNR_deleteif( DLISTUNR *list, DLISTUNR_VISITOR free_func, void *context)
{
	DRING *next,*cur;
	DLISTUNR_entry *curr;
	size_t pos,elmsize,i;
	
	elmsize = list->elmsize;

	DRING_FOREACH_SAVE( cur, next, &list->root.ring) {
	
		curr = (DLISTUNR_entry *) cur;
		if (free_func) {
			for(pos = 0,i=0;i<curr->elmcount;i++) {
				free_func(list,  (void *) curr->buffer[ pos ], context);
				pos += elmsize;
			}
		}
		/*list->ctx->free( DRING_unlink(cur) );*/
		list->ctx->free( cur );
		
	}	
	DRING_init( &list->root.ring );
	list->entrycount = 0;
	list->elmcount = 0;
}	
#endif

/*
 * @}
 */
#ifdef  __cplusplus
}
#endif

#endif
