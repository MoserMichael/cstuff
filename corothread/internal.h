#ifndef __INTERNAL__H__
#define __INTERNAL__H__

//#define HAS_SETJMP_LONGJMP

#ifdef HAS_SETJMP_LONGJMP
#include <setjmp.h>
#endif

#ifdef HAS_WIN32_FIBER
/* use win32 fibers - does not work with Cygwin; here SwitchToFiber gets stuck and nobody cares to fix it ;-) */
#include <windows.h>
#endif

#ifdef HAS_UCONTEXT
#include <ucontext.h>
#endif

#ifdef __CYGWIN__
/* does not have __thread specifier , strange. */
#define NO_TLS
#endif

#if !defined(HAS_UCONTEXT) && !defined(HAS_WIN32_FIBER) && !defined(HAS_SETJMP_LONGJMP)
#error "Please define either one of HAS_UCONTEXT HAS_SETJMP_LONGJMP HAS_WIN32_FIBER"
#endif

#include <stdint.h>



typedef enum {
   STACK_DIR_GROWING_DOWN,
   STACK_DIR_GROWING_UP,
}  STACK_DIR;


uint32_t get_page_size();
STACK_DIR get_stack_direction();

#endif


