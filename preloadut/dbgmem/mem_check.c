
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




//---------------------------------------------------------------------------

MAKE_FUNC( memcpy );
MAKE_FUNC( memmove );
MAKE_FUNC( memset );
MAKE_FUNC( strncpy );
MAKE_FUNC( strcat );
MAKE_FUNC( strncat );
MAKE_FUNC( strcmp );
MAKE_FUNC( strncmp );
MAKE_FUNC( strcpy );
 
static void mem_check_init()
{
  get_memcpy();
  get_memmove();
  get_memset();
  get_strncpy();
  get_strcat();
  get_strncat();
  get_strcmp();
  get_strncmp();
  get_strcpy();
}

#if 1

static void do_init();

//---------------------------------------------------------------------------
//  implementation of functions checked for memory access.
//---------------------------------------------------------------------------
void *memcpy (void *__restrict __dest,
              __const void *__restrict __src, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n, MWRITE, "memcpy" );
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n, MREAD, "memcpy" );
    MT_UNLOCK( &memlock );
     
    CHECK_OVERLAP( __dest, __n, __src, __n, "memcpy"); 
  }
  return  get_memcpy() ( __dest, __src, __n);
}

void *memmove (void *__dest, __const void *__src, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n, MWRITE, "memmove" );
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n, MREAD, "memmove" );
    MT_UNLOCK( &memlock );
  }
  return get_memmove() ( __dest, __src, __n );
}

void *memset( void *__s, int __c, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __s , __n, MWRITE, "memset" );
    MT_UNLOCK( &memlock );
 
  }
  return get_memset() ( __s, __c, __n );
}

char *strcpy (char *__restrict __dest, __const char *__restrict __src)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, (size_t) -1, 0, MSTRREAD, "strcpy");
    if (len != (size_t) -1) {
      DBGMEM_CHECK_STRING_RANGE( __dest, (size_t) -1, len,  MSTRCPY, "strcpy");
      MT_UNLOCK( &memlock );
 
      CHECK_OVERLAP( __dest, len, __src, len, "strcpy");  
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get_strcpy() ( __dest, __src );
}

char *strcat (char *__restrict __dest, __const char *__restrict __src)
{
  size_t len;
  
  do_init();
 
  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, (size_t) -1, 0,  MSTRREAD, "strcat");
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_STRING_RANGE( __dest, (size_t) -1, len, MSTRCAT, "strcat");
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }  

  return get_strcat() ( __dest, __src );
}

// is broken.
char *strncpy (char *__restrict __dest,
                           __const char *__restrict __src, size_t __n)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 

    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, __n, 0,  MSTRREAD, "strnpy");
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_STRING_RANGE( __dest, __n, len, MSTRNCPY, "strnpy");
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }

  return get_strncpy() ( __dest, __src, __n);
}
 
// broken
char *strncat (char *__restrict __dest, __const char *__restrict __src, size_t __n)
{
  size_t len;
 
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, __n, 0, MSTRREAD, "strncat");
    if (len != (size_t) -1) { 
      DBGMEM_CHECK_STRING_RANGE( __dest, __n, len, MSTRCAT, "strncat");
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get_strncat() ( __dest, __src, __n );
}

int strcmp (__const char *__s1, __const char *__s2)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_STRING_RANGE( __s1, (size_t) -1, 0, MSTRREAD, "strcmp");
    DBGMEM_CHECK_STRING_RANGE( __s2, (size_t) -1, 0, MSTRREAD, "strcmp");
    MT_UNLOCK( &memlock );
 
  }
  return get_strcmp() ( __s1, __s2 );
}
							   
int strncmp (__const char *__s1, __const char *__s2, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_STRING_RANGE( __s1, __n, 0, MSTRREAD, "strncmp");
    DBGMEM_CHECK_STRING_RANGE( __s2, __n, 0, MSTRREAD, "strncmp");
    MT_UNLOCK( &memlock );
  }
  return get_strncmp() ( __s1, __s2, __n );
}
#endif

//---------------------------------------------------------------------------
//  Initialisation 
//---------------------------------------------------------------------------
STATIC_C void init_alloc()
{  
  MENTRY_SIZE = sizeof(MEM_ENTRY) + sizeof(void *) * STACK_FRAMES;
  
  if (MEM_ENTRY_init(STACK_FRAMES)) {
    exit(223);
  } 
  mem_check_init();
}
 
