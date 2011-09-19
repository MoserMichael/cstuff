#include <cutils/dlistunr.h>

M_INLINE DLISTUNR_entry *DLISTUNR_new_list_entry( size_t datasize)
{
	DLISTUNR_entry *entry;

	entry = malloc(  sizeof(DLISTUNR_entry) + datasize );
	if (!entry) {
		return 0;
	}
	entry->elmcount = 0;
	return entry;
}


M_INLINE void *DLISTUNR_insert_entry(DLISTUNR_entry *entry, size_t pos, size_t elmmaxcount, size_t elmsize)
{
	if (!entry) {
		return 0;
	}

	if (entry->elmcount >= elmmaxcount || entry->elmcount == (size_t) -1) {
		return 0;
	}

	pos ++;

	if (pos < entry->elmcount) {	
		memmove(entry->buffer + (pos + 1) * elmsize,
			    entry->buffer +  pos * elmsize, 
				(entry->elmcount - pos) * elmsize);
		
	} 
	
	entry->elmcount ++ ;
	return entry->buffer + (pos * elmsize);
}

M_INLINE void *DLISTUNR_insert_newnode(DLISTUNR *list,
												  DLISTUNR_position insert_pos)
{
	DLISTUNR_entry *newnode;

	newnode = DLISTUNR_new_list_entry( list->elmsize * list->elmmaxcount);
	if (!newnode) {
		return 0;
	}
	
	DRING_insert_after( (DRING *) insert_pos.entry, (DRING *) newnode );

	newnode->elmcount = 1;
	return newnode->buffer;
}



int DLISTUNR_insert_after(DLISTUNR *list, DLISTUNR_position pos, void *data, size_t size)
{
	size_t elmsize = list->elmsize;
	size_t elmmaxcount = list->elmmaxcount;
	DLISTUNR_position insert_pos = pos;
	DLISTUNR_entry *next;
	void *newnodepos;

	if (size != list->elmsize) {
		return -1;
	}

	if (!insert_pos.entry) {
		insert_pos.entry = (DLISTUNR_entry *) list->root.ring.prev;
	}

	if ((newnodepos = DLISTUNR_insert_entry(
								insert_pos.entry, 
 								insert_pos.index, 
								elmmaxcount, 
								elmsize)) == 0) {
		
		next = (DLISTUNR_entry *) insert_pos.entry->ring.next;
		 
		if ((newnodepos = DLISTUNR_insert_entry(next, (size_t) -1, elmmaxcount, elmsize)) == 0){

			if ((newnodepos = DLISTUNR_insert_newnode(list, insert_pos)) == 0) {
				return -1;
			}
			list->entrycount ++;
			next = (DLISTUNR_entry *) insert_pos.entry->ring.next;

		} 
			
		if (insert_pos.entry->elmcount != (size_t) -1 && 
			insert_pos.index < (insert_pos.entry->elmcount - 1) ) {

			/* bad bad */
			insert_pos.index ++;

			/* move elements from prev node to new node */
			memcpy(next->buffer, 
				   insert_pos.entry->buffer + (list->elmmaxcount - 1) * elmsize,
				   elmsize);

			/* move elements in current node */		
			memmove(insert_pos.entry->buffer + (insert_pos.index + 1) * elmsize,
					insert_pos.entry->buffer + insert_pos.index  * elmsize,
					(insert_pos.entry->elmcount - insert_pos.index - 1) * elmsize);
		
			/* copy new node into insert_pos.index */
			newnodepos = insert_pos.entry->buffer + insert_pos.index  * elmsize;
		}
	} 
	
	memcpy(newnodepos, data, elmsize);
	list->elmcount ++;
	return 0;
}


int DLISTUNR_unlink(DLISTUNR *list, DLISTUNR_position pos)
{
	DLISTUNR_entry *entry;
	size_t elmsize = list->elmsize;

	if (DLISTUNR_check_position(pos)) {
		return -1;
	}
	entry = pos.entry;

	if (entry->elmcount == (size_t) -1) {
		return -1;
	}

	if (entry->elmcount != 1) {
		if (pos.index < (entry->elmcount-1)) {
			memmove(entry->buffer + pos.index * elmsize,
					entry->buffer + (pos.index + 1) * elmsize, 
					(entry->elmcount - pos.index - 1) * elmsize );
		}
		entry->elmcount --;
	} else {
		DRING_unlink((DRING *)pos.entry);
		free( pos.entry);
		list->entrycount --;
	}

	list->elmcount --;
	return 0;
}


