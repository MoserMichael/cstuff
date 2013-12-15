
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

// = memcpy =

MAKE_FUNC( wmemcpy );
MAKE_FUNC( __wmemcpy_chk );

MAKE_FUNC( memcpy );
MAKE_FUNC( __memcpy_chk );

// = memmove =
MAKE_FUNC( wmemmove );
MAKE_FUNC( __wmemmove_chk );

MAKE_FUNC( memmove );
MAKE_FUNC( __memmove_chk );

// = memset =

MAKE_FUNC( wmemset );
MAKE_FUNC( __wmemset_chk );

MAKE_FUNC( memset );
MAKE_FUNC( __memset_chk );

// = strcpy =

MAKE_FUNC( strcpy );
MAKE_FUNC( __strcpy_chk );

MAKE_FUNC( wcscpy );
MAKE_FUNC( __wcscpy_chk );

// = strncpy =

MAKE_FUNC( wcsncpy );
MAKE_FUNC( __wcsncpy_chk );

MAKE_FUNC( strncpy );
MAKE_FUNC( __strncpy_chk );

// = strcat =

MAKE_FUNC( wcscat );
MAKE_FUNC( __wcscat_chk );

MAKE_FUNC( strcat );
MAKE_FUNC( __strcat_chk );

// - strncat -

MAKE_FUNC( wcsncat );
MAKE_FUNC( __wcsncat_chk );


MAKE_FUNC( strncat );
MAKE_FUNC( __strncat_chk );

// -- strcmp --

MAKE_FUNC( wcscmp );
MAKE_FUNC( strcmp );

// -- strncmp --

MAKE_FUNC( wcsncmp );
MAKE_FUNC( strncmp );


 
static void mem_check_init()
{
  get_memcpy();
  get___memcpy_chk();
  get_wmemcpy();
  get___wmemcpy_chk();
  
  get_memmove();
  get___memmove_chk();
  get_wmemmove();
  get___wmemmove_chk();
 
  get_memset();
  get___memset_chk();
  get_wmemset();
  get___wmemset_chk();

  get_strcpy();
  get___strcpy_chk();
  get_wcscpy();
  get___wcscpy_chk();
  
  get_strncpy();
  get___strncpy_chk();
  get_wcsncpy();
  get___wcsncpy_chk();
  
  get_strcat();
  get___strcat_chk();
  get_wcscat();
  get___wcscat_chk();
  
  get_strncat();
  get___strncat_chk();
  get_wcsncat();
  get___wcsncat_chk();
  
  get_strcmp();
  get_wcscmp();
  
  get_strncmp();
  get_wcsncmp();
  
}

#if 1

static void do_init();

//---------------------------------------------------------------------------
//  implementation of functions checked for memory access.
//---------------------------------------------------------------------------
EXPORT_C void *memcpy (void *__restrict __dest,
              __const void *__restrict __src, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n, MWRITE, __FUNCTION__);
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n, MREAD, __FUNCTION__);
    MT_UNLOCK( &memlock );
     
    CHECK_OVERLAP( __dest, __n, __src, __n, __FUNCTION__); 
  }
  return  get_memcpy() ( __dest, __src, __n);
}

EXPORT_C void *__memcpy_chk (void *__restrict __dest,
              __const void *__restrict __src, size_t __n, size_t nnn)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n, MWRITE, "memcpy");
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n, MREAD, "memcpy");
    MT_UNLOCK( &memlock );
     
    CHECK_OVERLAP( __dest, __n, __src, __n, "memcpy"); 
  }
  return  get___memcpy_chk() ( __dest, __src, __n, nnn);
}



EXPORT_C wchar_t * wmemcpy  (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n * sizeof( wchar_t ) , MWRITE, __FUNCTION__);
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n * sizeof( wchar_t ), MREAD, __FUNCTION__);
    MT_UNLOCK( &memlock );
     
    CHECK_OVERLAP( __dest, sizeof(wchar_t) * __n, __src, sizeof(wchar_t) * __n, __FUNCTION__); 
  }
  return  get_wmemcpy() ( __dest, __src, __n);
}

EXPORT_C wchar_t * __wmemcpy_chk  (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n, size_t nnn)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n * sizeof( wchar_t ) , MWRITE, "wmemcpy");
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n * sizeof( wchar_t ), MREAD, "wmemcpy");
    MT_UNLOCK( &memlock );
     
    CHECK_OVERLAP( __dest, sizeof(wchar_t) * __n, __src, sizeof(wchar_t) * __n, "wmemcpy"); 
  }
  return  get___wmemcpy_chk() ( __dest, __src, __n, nnn);
}


EXPORT_C void *memmove (void *__dest, __const void *__src, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n, MWRITE, __FUNCTION__);
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n, MREAD, __FUNCTION__);
    MT_UNLOCK( &memlock );
  }
  return get_memmove() ( __dest, __src, __n );
}

EXPORT_C void *__memmove_chk (void *__dest, __const void *__src, size_t __n, size_t nnn)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, __n, MWRITE, "memmove");
    DBGMEM_CHECK_MEMORY_RANGE( __src, __n, MREAD, "memmove");
    MT_UNLOCK( &memlock );
  }
  return get___memmove_chk() ( __dest, __src, __n, nnn );
}



EXPORT_C wchar_t *wmemmove (wchar_t *__dest, __const wchar_t *__src, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, sizeof(wchar_t) * __n, MWRITE, __FUNCTION__);
    DBGMEM_CHECK_MEMORY_RANGE( __src, sizeof(wchar_t) * __n, MREAD, __FUNCTION__);
    MT_UNLOCK( &memlock );
  }
  return get_wmemmove() ( __dest, __src, __n );
}

EXPORT_C wchar_t *__wmemmove_chk (wchar_t *__dest, __const wchar_t *__src, size_t __n, size_t nnn)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __dest, sizeof(wchar_t) * __n, MWRITE, "wmemmove");
    DBGMEM_CHECK_MEMORY_RANGE( __src, sizeof(wchar_t) * __n, MREAD, "wmemmove");
    MT_UNLOCK( &memlock );
  }
  return get___wmemmove_chk() ( __dest, __src, __n, nnn );
}


EXPORT_C void *memset( void *__s, int __c, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __s , __n, MWRITE, __FUNCTION__);
    MT_UNLOCK( &memlock );
 
  }
  return get_memset() ( __s, __c, __n );
}

EXPORT_C void *__memset_chk( void *__s, int __c, size_t __n, size_t nnn)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( __s , __n, MWRITE, "memset");
    MT_UNLOCK( &memlock );
 
  }
  return get___memset_chk() ( __s, __c, __n, nnn );
}


EXPORT_C wchar_t * wmemset( wchar_t *__s, wchar_t __c, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( (void *) __s , sizeof(wchar_t) * __n, MWRITE, __FUNCTION__);
    MT_UNLOCK( &memlock );
 
  }
  return get_wmemset() ( __s, __c, __n );
}

EXPORT_C wchar_t * __wmemset_chk( wchar_t *__s, wchar_t __c, size_t __n, size_t nnn)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_MEMORY_RANGE( (void *) __s , sizeof(wchar_t) * __n, MWRITE, "wmemset");
    MT_UNLOCK( &memlock );
 
  }
  return get___wmemset_chk() ( __s, __c, __n, nnn );
}


EXPORT_C char *strcpy (char *__restrict __dest, __const char *__restrict __src)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, (size_t) -1, 0, MSTRREAD, __FUNCTION__);
    if (len != (size_t) -1) {
      DBGMEM_CHECK_STRING_RANGE( __dest, (size_t) -1, len,  MSTRCPY, __FUNCTION__);
      MT_UNLOCK( &memlock );
 
      CHECK_OVERLAP( __dest, len, __src, len, __FUNCTION__);  
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get_strcpy() ( __dest, __src );
}

EXPORT_C char *__strcpy_chk (char *__restrict __dest, __const char *__restrict __src, size_t nnn)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, (size_t) -1, 0, MSTRREAD, "strcpy");
    if (len != (size_t) -1) {
      DBGMEM_CHECK_STRING_RANGE( __dest, (size_t) -1, len,  MSTRCPY, "strcpy");
      MT_UNLOCK( &memlock );
 
      CHECK_OVERLAP( __dest, len, __src, len, __FUNCTION__);  
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get___strcpy_chk() ( __dest, __src, nnn );
}


EXPORT_C wchar_t *wcscpy (wchar_t *__restrict __dest, __const wchar_t *__restrict __src)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, (size_t) -1, 0, MSTRREAD, __FUNCTION__);
    if (len != (size_t) -1) {
      DBGMEM_CHECK_WSTRING_RANGE( __dest, (size_t) -1, len,  MSTRCPY, __FUNCTION__);
      MT_UNLOCK( &memlock );
 
      CHECK_OVERLAP( __dest, sizeof(wchar_t) * len, __src, sizeof(wchar_t) * len, __FUNCTION__);  
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get_wcscpy() ( __dest, __src );
}

EXPORT_C wchar_t *__wcscpy_chk (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t nnn)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, (size_t) -1, 0, MSTRREAD, "wcscpy");
    if (len != (size_t) -1) {
      DBGMEM_CHECK_WSTRING_RANGE( __dest, (size_t) -1, len,  MSTRCPY, "wcscpy");
      MT_UNLOCK( &memlock );
 
      CHECK_OVERLAP( __dest, sizeof(wchar_t) * len, __src, sizeof(wchar_t) * len, __FUNCTION__);  
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get___wcscpy_chk() ( __dest, __src, nnn );
}


EXPORT_C char *strcat (char *__restrict __dest, __const char *__restrict __src)
{
  size_t len;
  
  do_init();
 
  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, (size_t) -1, 0,  MSTRREAD, __FUNCTION__);
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_STRING_RANGE( __dest, (size_t) -1, len, MSTRCAT, __FUNCTION__);
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }  

  return get_strcat() ( __dest, __src );
}

EXPORT_C char *__strcat_chk (char *__restrict __dest, __const char *__restrict __src, size_t nnn)
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

  return get___strcat_chk() ( __dest, __src, nnn );
}

EXPORT_C wchar_t *wcscat (wchar_t *__restrict __dest, __const wchar_t *__restrict __src)
{
  size_t len;
  
  do_init();
 
  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_WSTRING_RANGE( __src, (size_t) -1, 0,  MSTRREAD,  __FUNCTION__);
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_WSTRING_RANGE( __dest, (size_t) -1, len, MSTRCAT,  __FUNCTION__);
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }  

  return get_wcscat() ( __dest, __src );
}

EXPORT_C wchar_t *__wcscat_chk (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t nnn)
{
  size_t len;
  
  do_init();
 
  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_WSTRING_RANGE( __src, (size_t) -1, 0,  MSTRREAD,  "wcscat");
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_WSTRING_RANGE( __dest, (size_t) -1, len, MSTRCAT,  "wcscat");
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }  

  return get___wcscat_chk() ( __dest, __src, nnn );
}



EXPORT_C char *strncpy (char *__restrict __dest,
                           __const char *__restrict __src, size_t __n)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 

    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, __n, 0,  MSTRREAD, __FUNCTION__);
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_STRING_RANGE( __dest, __n, len, MSTRNCPY,  __FUNCTION__);
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }

  return get_strncpy() ( __dest, __src, __n);
}
 
EXPORT_C char *__strncpy_chk (char *__restrict __dest,
                           __const char *__restrict __src, size_t __n, size_t nnn)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 

    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, __n, 0,  MSTRREAD, "strncpy");
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_STRING_RANGE( __dest, __n, len, MSTRNCPY,  "strncpy");
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }

  return get___strncpy_chk() ( __dest, __src, __n, nnn);
}

EXPORT_C wchar_t *wcsncpy (wchar_t *__restrict __dest,
                           __const wchar_t *__restrict __src, size_t __n)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 

    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_WSTRING_RANGE( __src, __n, 0,  MSTRREAD, __FUNCTION__);
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_WSTRING_RANGE( __dest, __n, len, MSTRNCPY, __FUNCTION__);
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }

  return get_wcsncpy() ( __dest, __src, __n);
}
 
EXPORT_C wchar_t *__wcsncpy_chk (wchar_t *__restrict __dest,
                           __const wchar_t *__restrict __src, size_t __n, size_t nnn)
{
  size_t len;
  
  do_init();

  if (!ignore_this_process) { 

    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_WSTRING_RANGE( __src, __n, 0,  MSTRREAD, "wcsncpy");
    if (len != (size_t) -1) {  
      DBGMEM_CHECK_WSTRING_RANGE( __dest, __n, len, MSTRNCPY, "wcsncpy");
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }

  return get___wcsncpy_chk() ( __dest, __src, __n, nnn);
}

EXPORT_C char *strncat (char *__restrict __dest, __const char *__restrict __src, size_t __n)
{
  size_t len;
 
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_STRING_RANGE( __src, __n, 0, MSTRREAD, __FUNCTION__);
    if (len != (size_t) -1) { 
      DBGMEM_CHECK_STRING_RANGE( __dest, __n, len, MSTRCAT, __FUNCTION__);
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get_strncat() ( __dest, __src, __n );
}

EXPORT_C char *__strncat_chk (char *__restrict __dest, __const char *__restrict __src, size_t __n, size_t nnn)
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
  return get___strncat_chk() ( __dest, __src, __n, nnn );
}

EXPORT_C wchar_t *wcsncat (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n)
{
  size_t len;
 
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_WSTRING_RANGE( __src, __n, 0, MSTRREAD, __FUNCTION__);
    if (len != (size_t) -1) { 
      DBGMEM_CHECK_WSTRING_RANGE( __dest, __n, len, MSTRCAT,  __FUNCTION__);
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get_wcsncat() ( __dest, __src, __n );
}

EXPORT_C wchar_t *__wcsncat_chk (wchar_t *__restrict __dest, __const wchar_t *__restrict __src, size_t __n, size_t nnn)
{
  size_t len;
 
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    len = DBGMEM_CHECK_WSTRING_RANGE( __src, __n, 0, MSTRREAD, "wcsncat");
    if (len != (size_t) -1) { 
      DBGMEM_CHECK_WSTRING_RANGE( __dest, __n, len, MSTRCAT,  "wcsncat");
      MT_UNLOCK( &memlock );
    } else {
      MT_UNLOCK( &memlock );
    }
  }
  return get___wcsncat_chk() ( __dest, __src, __n, nnn );
}


#ifdef strcmp
#define save_strcmp
#undef strcmp
#endif

EXPORT_C int strcmp (__const char *__s1, __const char *__s2)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_STRING_RANGE( __s1, (size_t) -1, 0, MSTRREAD,  __FUNCTION__);
    DBGMEM_CHECK_STRING_RANGE( __s2, (size_t) -1, 0, MSTRREAD,  __FUNCTION__);
    MT_UNLOCK( &memlock );
 
  }
  return get_strcmp() ( __s1, __s2 );
}

#ifdef save_strcmp
#define strcmp save_strcmp
#endif

EXPORT_C int wcscmp (__const wchar_t *__s1, __const wchar_t *__s2)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_WSTRING_RANGE( __s1, (size_t) -1, 0, MSTRREAD,  __FUNCTION__);
    DBGMEM_CHECK_WSTRING_RANGE( __s2, (size_t) -1, 0, MSTRREAD,  __FUNCTION__);
    MT_UNLOCK( &memlock );
 
  }
  return get_wcscmp() ( __s1, __s2 );
}

#ifdef strncmp
#define save_strncmp
#undef strncmp
#endif

EXPORT_C int strncmp (__const char *__s1, __const char *__s2, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_STRING_RANGE( __s1, __n, 0, MSTRREAD,  __FUNCTION__);
    DBGMEM_CHECK_STRING_RANGE( __s2, __n, 0, MSTRREAD, __FUNCTION__);
    MT_UNLOCK( &memlock );
  }
  return get_strncmp() ( __s1, __s2, __n );
}

#ifdef save_strncmp
#define strncmp save_strncmp 
#endif

EXPORT_C int wcsncmp (__const wchar_t *__s1, __const wchar_t *__s2, size_t __n)
{
  do_init();

  if (!ignore_this_process) { 
    MT_LOCK( &memlock );
    DBGMEM_CHECK_WSTRING_RANGE( __s1, __n, 0, MSTRREAD,  __FUNCTION__);
    DBGMEM_CHECK_WSTRING_RANGE( __s2, __n, 0, MSTRREAD, __FUNCTION__);
    MT_UNLOCK( &memlock );
  }
  return get_wcsncmp() ( __s1, __s2, __n );
}

#
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
 
