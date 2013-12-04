
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


/** --------------------------------------------------------------------------
 * portion of the code has been copied from libsafe
 * - meaning that the whole project becomes LGPL.
 *  --------------------------------------------------------------------------
 */

#ifdef _WIN32
#define check_stack_pointer(x,y) 0
#else

#define PTHREAD_STACK_SIZE	(0x1fffff)

/*
 * Return the highest memory address associated with this addr.  This is just
 * a guess.  We assume that the main thread stack starts at 0xc0000000 and is
 * 8MB.  The other threads start at 0xbf800000 (immediately after the 8MB space
 * for the main thread stack) and are all 2MB.
 */

#if __WORDSIZE == 32

#define find_stack_start(addr)						    \
     /* Past stack area */						    \
    ((addr > (void*)0xc0000000) ? NULL :				    \
									    \
     /* Main thread stack */						    \
     (addr > (void*)0xbf800000) ? (void*)0xc0000000 :			    \
									    \
     /* Other thread stacks */						    \
	((void*)(((size_t)addr & (~PTHREAD_STACK_SIZE)) + PTHREAD_STACK_SIZE))\
									    \
    )
#else 
#define TOP_S 0x7FFFFFFFFFFFFFFF


#define find_stack_start(addr)						    \
     /* Other thread stacks */						    \
	((void*)(((size_t)addr & (~PTHREAD_STACK_SIZE)) + PTHREAD_STACK_SIZE))\

#if 0
#define find_stack_start(addr)						    \
     /* Past stack area */						    \
    ((addr > (void*)TOP_S ) ? NULL :				    \
									    \
     /* Main thread stack */						    \
     (addr > (void*)TOP_S ) ? (void*) TOP_S :		    \
									    \
     /* Other thread stacks */						    \
	((void*)(((size_t)addr & (~PTHREAD_STACK_SIZE)) + PTHREAD_STACK_SIZE))\
									    \
    )
#endif


#endif


/*****************************************************************************
 * 
 * These are functions that do the real work of determining if a libsafe
 * violation has occurred.
 *
 *****************************************************************************/

/* Given an address 'addr' returns 0 iff the address does not point to a stack
 * variable.  Otherwise, it returns a positive number indicating the number of
 * bytes (distance) between the 'addr' and the frame pointer it resides in.
 * Note: stack grows down, and arrays/structures grow up.
 */
static size_t _libsafe_stackVariableP(const void *addr) {
    /*
     * bufsize is the distance between addr and the end of the stack frame.
     * It's what _libsafe_stackVariableP() is trying to calculate.
     */
    uint bufsize = 0;

    /*
     * (Vandoorselaere Yoann)
     * We have now just one cast.
     */
    void *fp, *sp;
    
    /*
     * nextfp is used in the check for -fomit-frame-pointer code.
     */
    void *nextfp;

    /*
     * stack_start is the highest address in the memory space mapped for this
     * stack.
     */
    void *stack_start;

#if 0
    /*
     * If _libsafe_die() has been called, then we don't need to do anymore
     * libsafe checking.
     */
    if (dying)
	return 0;
#endif

    /*
     * (Arash Baratloo / Yoann Vandoorselaere)
     * use the stack address of the first declared variable to get the 'sp'
     * address in a portable way.
     */
    sp = &fp;

    /*
     * Stack grows downwards (toward 0x00).  Thus, if the stack pointer is
     * above (>) 'addr', 'addr' can't be on the stack.
     */
    if (sp > addr)
	return 0;

    /*
     * Note: the program name is always stored at 0xbffffffb (documented in the
     * book Linux Kernel).  Search back through the frames to find the frame
     * containing 'addr'.
     */
    fp = __builtin_frame_address(0);

    /*
     * Note that find_stack_start(fp) should never return NULL, since fp is
     * always guaranteed to be on the stack.
     */
    stack_start = find_stack_start((void*)&fp);

    while ((sp < fp) && (fp <= stack_start)) {
	if (fp > addr) {
	    /*
	     * found the frame -- now check the rest of the stack
	     */
	    bufsize = BPTR_DIFF(fp, addr);
	    //fprintf(stderr," : %d :\n", bufsize );
	    break;
	}

	//fprintf(stderr, "-->fp %p\n",  fp);

	nextfp = *(void **) fp;

	/*
	 * The following checks are meant to detect code that doesn't insert
	 * frame pointers onto the stack.  (i.e., code that is compiled with
	 * -fomit-frame-pointer).
	 */

	/*
	 * Make sure frame pointers are word aligned.
	 */
	if ((size_t)nextfp & 0x03) {
#if 0
	    LOG(2, "fp not word aligned; bypass enabled\n");
	    _libsafe_exclude = 1;
#endif
	    return 0;
	}

	/*
	 * Make sure frame pointers are monotonically increasing.
	 */
	if (nextfp <= fp) {
#if 0
	    LOG(2, "fp not monotonically increasing; bypass enabled\n");
	    _libsafe_exclude = 1;
#endif
	    return 0;
	}

	fp = nextfp;
    }

    /*
     * If we haven't found the correct frame by now, it either means that addr
     * isn't on the stack or that the stack doesn't contain frame pointers.
     * Either way, we will return 0 to bypass checks for addr.
     */
    if (bufsize == 0) {
	return 0;
    }

    /*
     * Now check to make sure that the rest of the stack looks reasonable.
     */
    while ((sp < fp) && (fp <= stack_start)) {
	nextfp = *(void **) fp;

	if (nextfp == NULL) {
	    /*
	     * This is the only correct way to end the stack.
	     */
	    return bufsize;
	}

	/*
	 * Make sure frame pointers are word aligned.
	 */
	if ((size_t)nextfp & 0x03) {
#if 0
	    LOG(2, "fp not word aligned; bypass enabled\n");
	    _libsafe_exclude = 1;
#endif
	    return 0;
	}


	/*
	 * Make sure frame pointers are monotonically * increasing.
	 */
	if (nextfp <= fp) {
#if __WORDSIZE == 32

#if 0
	    LOG(2, "fp not monotonically increasing; bypass enabled\n");
	    _libsafe_exclude = 1;
#endif
	    return 0;
#else
	    return bufsize; // always happens on IA64 don't know why.
#endif
	}

	fp = nextfp;
    }

    /*
     * We weren't able to say for sure that the stack contains valid frame
     * pointers, so we will return 0, which means that no check for addr will
     * be done.
     */
    return 0;
}

static int check_stack_pointer( const void *ptr, size_t size )
{
   size_t sz =  _libsafe_stackVariableP( ptr );

   // didn't find on the stack (or is using fp omission?)
   if (sz == 0) {
     return 0;
   }

   // overwriting the frame pointer here.
   if (sz  < size ) {
     return -1;
   }

   // certified as ok.
   return 0;
}
#endif

