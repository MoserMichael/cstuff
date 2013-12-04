#ifndef _MEMDG_DBGUTIL_H_
#define _MEMDG_DBGUTIL_H_


#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
  DBGMEM_NOT_IN_USE,
  DBGMEM_SIMPLE_TOOL,
  DBGMEM_CHECK_TOOL,
}
  DBGMEM_STATUS ;


typedef enum {
  DBGMEM_READ,
  DBGMEM_WRITE,
} 
  DBGMEM_MACCESS_TYPE;

typedef enum {
  DBGMEM_STRREAD,
  DBGMEM_STRCPY,
  DBGMEM_STRNCPY,
  DBGMEM_STRCAT,
} 
  DBGMEM_MACCESS_TYPE_STR;



/**
 * @brief returns if DBGMEM tool is currently running with this process.
 * @returns type of memory checking tool installed.
 */
DBGMEM_STATUS DBGMEM_is_installed();

/**
 * @brief The function dumps all heap memory objects to a file
 *
 * The function iterates over all heap memory objects and does the following steps
 *   1) Open a raw memory report file
 *   2) for each heap object 
 *       - check if it valid (check for memory overwrite/underwrite).
 *         Each error is logged to standard error and to raw report file.       
 *       - Print each memory block information to current raw report file
 */
int DBGMEM_dump_memory();

/**
 * @brief The function checks the heap for consistency
 * 
 * before iteration a lock is acquired, no memory allocation will return until after releasing this lock
 * The function iterates over all heap memory objects and checks for heap overwrites / underwrites. 
 * Each error is logged to standard error and to raw report file.
 */
int DBGMEM_check_heap();

/*
 * Increases current generation tag; generation tag is kept with each allocated heap block; so that the
 * stage at which it was allocated can be distinguished. 
 */
int DBGMEM_increase_generation_tag();

/*** function for checking tool only ***/

/**
 * @brief  check argument pointer range; checks if range is writable/readable (depending on argument)
 * 
 * NOTE: this function works only when the CHECKING TOOL is active.
 *
 * The function first checks if the argument range belongs to any memory block allocated on the heap;
 * If it is then the heap block is checked for overwrites/underwrites.
 * If pointer is not from the heap, then we check if range is 'WRITABLE' and 
 * the we don't smash the stack.  
 * (check triggers when return address of a calling function in the current thread is smashed)
 *
 * @returns 0 if range is ok, 1 if range skips out of a heap block. 
 * 
 * @param arg_ptr   base address of memory range to be check
 * @param length    length of memory range to be checked
 * @param atype     access type / either read acess or write access 
 * @param function_name  calling function name
 */
size_t DBGMEM_CHECK_MEMORY(const void *arg_ptr, size_t length,  DBGMEM_MACCESS_TYPE atype, const char *function_name);


/**
 * @brief  check argument pointer / range is a null terminated string that does not skip out heap memory 
 *
 * checks if argument pointer range is contained in a heap object, and that the heap object is not overwritten.
 *
 * The function first checks if the argument range belongs to any memory block allocated on the heap;
 * If it is then the heap block is checked for overwrites/underwrites.
 * If pointer is not from the heap, then we check if range is 'WRITABLE' (i.e atype != MREAD) and 
 * the we don't smash the stack.  
 * (this check triggers when return address of a calling function in the current thread is smashed)
 *
 *  atype == DBGMEM_STRCPY  
 *    used to check destination arguments in strcpy function
 *	str_bound  - -1 
 *      str_length - the length of the input string
 *    Issues error if we will overwrite the target buffer.
 
 *  atype == DBGMEM_STRNCPY
 *    used to check destination arguments in strncpy function
 *      str_length - the length of the input string
 *	str_bound  - limit on length of destination string 

 *  atype == DBGMEM_STRCAT 
 *    used to check destination arguments in str[n]cat functions
 *	str_bound - (-1) if length of destination argument is not set (strcpy) , n > 0 limit on length of destination string 
 *      str_length - the length of the input string
  
 *  atype == DBGMEM_STRREAD 
 *    used to check source arguments in string functions

 *    str_bound - (-1) if length of source string is not set (strcpy) , n > 0 if destination string is limited 
 *    used to check source argument of str[n]cpy str[n]cat strcmp function
 * 
 *    checks that the character string pointed to by arg_ptr is null terminated within the current heap object
 *    issues a warning if it isn't; as this would cause us to read past of heap block and to read garbage.
 *    if all ok returns length of string pointed to by arg_ptr. 
 *
 *
 * @paran arg_ptr	start of string to be checked
 * @param str_bound
 * @param str_length
 * @param atype		action type
 * @param function_name 
 */
 size_t  DBGMEM_CHECK_STRING(const void *arg_ptr, size_t str_bound, size_t str_length,  DBGMEM_MACCESS_TYPE_STR atype, const char *function_name);

/**
 * @brief  for a pointer to start of heap block, the size of the block is returned (size as requested by the user in malloc) 
 * 
 * NOTE: this function works only when the CHECKING TOOL is active.
 * 
 * @param arg	pointer to check
 * @param ret_size  (return value) the size of the memory block 
 * @return 0 if arg does point to start of heap block; -1 otherwise; 
 * @return the size of the block is return in ret_size if function return value is 0  
 */
int DBGMEM_GET_SIZE( const void *arg, size_t *ret_size);

#ifdef  __cplusplus
}
#endif

#endif
