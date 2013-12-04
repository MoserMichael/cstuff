
/* 
 * DBGMEM - memory allocation leak tracker and debugging tool.
 *
 *  (c) 2008 by Michael Moser
 * 
 * 3-clause BSD License applies  
 *
 * See LICENSE file for the full copy of the license
 *
 */

#include "memalloc.h"


//---------------------------------------------------------------------------
// RADIX tree library; maps pointer to pointer;
// each entry holds six bits of key range. 		
//---------------------------------------------------------------------------

#if __WORDSIZE == 64

#ifdef x86_64 /* x86_64 */ 

#define VPTR_SIZE 48
#define RADIX_TREE_KEY_BITS_PER_NODE 6

#else  /* x86_64 */ 

#define VPTR_SIZE 64
#define RADIX_TREE_KEY_BITS_PER_NODE 4

#endif /* x86_64 */

#else  /* __WORDSIZE == 64 */  
    
#define VPTR_SIZE 32
#define RADIX_TREE_KEY_BITS_PER_NODE 4

#endif


#define RADIX_TREE_ENTRIES_PER_NODE ((size_t) (1 << RADIX_TREE_KEY_BITS_PER_NODE))

#define RADIX_TREE_ITER_SHIFT (VPTR_SIZE - RADIX_TREE_KEY_BITS_PER_NODE) 

#define RADIX_TREE_ITER_BITMASK ((size_t) (RADIX_TREE_ENTRIES_PER_NODE - 1) << RADIX_TREE_ITER_SHIFT)  

#define RADIX_TREE_PATH_LENGTH (VPTR_SIZE  / RADIX_TREE_KEY_BITS_PER_NODE)


typedef struct {
  int    num_entries;
//size_t parent; 
  size_t entries[ RADIX_TREE_ENTRIES_PER_NODE ];
} 
  RADIX_TREE_ENTRY;

typedef struct {
  size_t entries;
  FIXED_SIZE_ALLOC_HDR * alloc;
}
  RADIX_TREE;

typedef struct {
  RADIX_TREE_ENTRY *entry;
  size_t entry_pos;
  int  next_path_idx;
} 
  RADIX_TREE_PATH_ENTRY;

      
static RADIX_TREE_ENTRY * RADIX_TREE_new_node(RADIX_TREE *tree,size_t *pos)
{
  RADIX_TREE_ENTRY *entry; 
  size_t i;
  size_t entry_pos;

  entry_pos = FIXED_SIZE_ALLOC_malloc( &tree->alloc );
  if (entry_pos == FIXED_SIZE_EOF) {
    return 0;
  }

  if (pos) {
    *pos = entry_pos;
  }

  entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, entry_pos );
  if (!entry) {
    return 0;
  }
  entry->num_entries = 0;

  for( i=0; i < RADIX_TREE_ENTRIES_PER_NODE; i++) {
    entry->entries[ i ] = 0;
  }

  //fprintf(stderr," -> new-node %d %p\n", entry_pos, entry);

  return entry;
}

static int RADIX_TREE_init(RADIX_TREE *tree, size_t init_size )
{
  RADIX_TREE_ENTRY *entry;

  tree->entries = 0;

  tree->alloc = FIXED_SIZE_ALLOC_init( init_size, sizeof(RADIX_TREE_ENTRY) );
  if (!tree->alloc) {
    return -1;
  }

  entry = RADIX_TREE_new_node( tree, 0 );
  if (!entry) {
    return -1;
  }
  //entry->parent = 0;
  entry->num_entries = 1;

  return 0;  
}

static void  RADIX_TREE_release( RADIX_TREE *tree )
{
  FIXED_SIZE_ALLOC_release( tree->alloc );
}
	  
static int RADIX_TREE_add(RADIX_TREE *tree, void *arg_key, size_t value)
{
  size_t i;
  size_t prev_pos = 0, pos = 0;
  size_t bits;
  size_t key = (size_t) arg_key;
  RADIX_TREE_ENTRY *entry = 0;

  //fprintf(stderr,"tree_add key %p value %u\n",arg_key, value);
#if 0
  size_t t1 = RADIX_TREE_ITER_BITMASK;
  size_t t2 = (unsigned int) 3;

  log("t1 %x %x\n", t1, t2);
#endif
  
  entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, pos );
  for( i=0; i < RADIX_TREE_PATH_LENGTH; i++) {
    bits = (key & RADIX_TREE_ITER_BITMASK) >> RADIX_TREE_ITER_SHIFT;
    key <<= RADIX_TREE_KEY_BITS_PER_NODE;

    //fprintf(stderr, " %d %d\n", i, bits );

    prev_pos = pos;
    pos = entry->entries[ bits ];
    if (pos == 0) {

       if ( i == (RADIX_TREE_PATH_LENGTH-1) ) {
	    entry->entries[ bits ] = value;
 	    entry->num_entries ++;
	    break;
       } else {
	    RADIX_TREE_ENTRY * new_node;
	    
	    new_node = RADIX_TREE_new_node( tree, &pos );
	    if (!new_node) {
	      return -1;
	    }
	    
	    entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, prev_pos );

	    entry->entries[ bits ] = pos;
 	    entry->num_entries ++ ;    
	    
	    entry = new_node;
       }
    } else {
      entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, pos );
    }
  }
  return 0;
}

static size_t RADIX_TREE_remove(RADIX_TREE *tree, void *arg_key)
{
  size_t i;
  size_t pos = 0;
  size_t bits;
  size_t key = (size_t) arg_key;
  size_t to_ret = (size_t) -1;
  RADIX_TREE_ENTRY *entry = 0;
  RADIX_TREE_PATH_ENTRY rpath[ RADIX_TREE_PATH_LENGTH ];  
 
  //fprintf(stderr,"tree_remove key %p\n",arg_key);
 
  // find entry to delete.
  for( i=0; i < RADIX_TREE_PATH_LENGTH ; i++) {
    entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, pos );
    
    bits = (key & RADIX_TREE_ITER_BITMASK) >> RADIX_TREE_ITER_SHIFT;
    key <<= RADIX_TREE_KEY_BITS_PER_NODE;
    
    rpath[ i ].entry  = entry;
    rpath[ i ].entry_pos = pos; // index of current entry.
    rpath[ i ].next_path_idx = bits; 
  
    pos = entry->entries[ bits ];
   
 
    //fprintf(stderr, " %d %d (%d : %d)\n", i, bits, pos, entry->num_entries );
    if (pos == 0) {
      break;
    }
  }
  
  if (!pos) {
    //fprintf(stderr, "error nothing to remove\n");
    return (size_t) -1;
  }
  to_ret = pos; 
 
  --i;
  //fprintf(stderr, "removing %d %d \n", pos, i );
  entry->entries[ bits ] = 0;

  // delete all empty nodes along the path to this node, except for root node. 
  while(i != 0 ) {
    entry->num_entries --;

    if (entry->num_entries) {
	  break;
    }  
    //fprintf(stderr,"  del %d %d\n", rpath[ i ].entry_pos, i );
    
    if (i != (RADIX_TREE_PATH_LENGTH - 1) ) { // if not value node - is internal node (i.e. allocated from tree allocator)
      FIXED_SIZE_ALLOC_free( tree->alloc,rpath[ i ].entry_pos );
    }
   
    --i;
    entry = rpath[ i ].entry; 
    entry->entries[ rpath[i].next_path_idx ] = 0;
      
  }
     
  return to_ret;
}

typedef size_t (*MATCH_ENTRY_CALLBACK) (const void *arg_key, size_t value, const void *ctx);

static size_t RADIX_TREE_find_match(RADIX_TREE *tree, const void *arg_key, MATCH_ENTRY_CALLBACK callback, const void *ctx )
{
  size_t i;
  size_t pos = 0;
  size_t bits;
  size_t key = (size_t) arg_key;
  RADIX_TREE_ENTRY *entry = 0;
  RADIX_TREE_PATH_ENTRY rpath[ RADIX_TREE_PATH_LENGTH ];  
  size_t value;

  //fprintf(stderr,"->find %p\n",arg_key);
  

  // find arg_key value in tree.
  for( i=0; i < RADIX_TREE_PATH_LENGTH; i++) {
    entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, pos );

    bits = (key & RADIX_TREE_ITER_BITMASK) >> RADIX_TREE_ITER_SHIFT;
    key <<= RADIX_TREE_KEY_BITS_PER_NODE;
    
    //fprintf(stderr, " %d %d\n", i, bits );

    rpath[ i ].entry  = entry;
  //rpath[ i ].entry_pos = pos;
    rpath[ i ].next_path_idx = bits; 
     
    pos = entry->entries[ bits ];
    if (pos == 0) {
      break;
    }
  }
  
  // exact match
  if (pos != 0) {
    //fprintf( stderr, "exact match %u\n", pos );
    return callback( arg_key, pos, 0 ); 
  }

  //task: find entry with maximum value of key less then arg_key
 
  // search for left brother of current node.
  do {
     
     // search in the current node.
     for( ; bits != (size_t) -1 && entry->entries[ bits ] == 0; bits-- );
    
     if (bits != (size_t) -1) {
       size_t entry_idx;

       i += 1;
       entry_idx = entry->entries[ bits ];


       if ( i == RADIX_TREE_PATH_LENGTH) {
         value = entry_idx;
	 goto try_me;
       }
	       
       
       entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, entry_idx );
       if (!entry) {
         // internal error.
         return (size_t) -1;
       }
       
       break;
     }
  
     // if not found in current node, then try with parent
     --i;
     if (i == (size_t) -1) {
       break;
     }
     entry = rpath[ i ].entry;
     bits = rpath[ i ].next_path_idx - 1;

  } while( 1 );   

  // no left brothers at all
  if ( i == (size_t) -1) {
    return (size_t) -1;
  }
 
  value = (size_t) -1;

  //fprintf(stderr, "found left brother %d %d\n", i, bits );
  
  // search for rightmost brother from here.
  while ( entry && i < RADIX_TREE_PATH_LENGTH ) {
      int j;
      for( j = RADIX_TREE_ENTRIES_PER_NODE - 1; j >= 0; j--) {  
         if (entry->entries[ j ]) {
	    size_t entry_idx;

            entry_idx = entry->entries[ j ]; 

	    if ( i == (RADIX_TREE_PATH_LENGTH - 1) ) {
	      value = entry_idx;
	      goto try_me;
	    }

	    entry = (RADIX_TREE_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( tree->alloc, entry_idx );
	    if (!entry) {
	      //fprintf(stderr,"internal error %i %u \n", i, entry_idx );
              // internal error.
	      return (size_t) -1;
            }

	    i += 1;
	    break;
	 }
      }
      if (j < 0) {
         entry = 0;
      }	 
  }

try_me:
  //fprintf(stderr,"check %u\n",  value);

  if (value != (size_t) -1) {
     return callback( arg_key, value, ctx  ); 

  }

    return 0;
}


