#ifndef __EVAL_H_Y_Z_
#define __EVAL_H_Y_Z_

#include <pooh-lan/rtlib/rtlib.h>

typedef struct tagTRACE_OUT {
  int level;
  int max_level;
  FILE *out;
} TRACE_OUT;

typedef enum {
  LOOP_NO_BREAK,
  LOOP_BREAK,
  LOOP_CONTINUE,
  EXIT_FUNCTION,
} EVAL_CTX_BREAK_ACTION;

struct tagEVAL_TRACE_ENTRY;

typedef struct tagEVAL_CTX {
  EVAL_CONTEXT context;
  ARRAY trace_buffer_entries; 
  EVAL_CTX_BREAK_ACTION loop_exit;

  TRACE_OUT trace_out;
  int trace_on;
  struct tagEVAL_TRACE_ENTRY *last_freed,*top_trace;
  PARSECONTEXT *ctx; // for error messages only
} EVAL_CTX;

int EVAL_init( EVAL_CTX *out, PARSECONTEXT *ctx);
int EVAL_run(  EVAL_CTX *out, AST_BASE *base );
int EVAL_free( EVAL_CTX *out );

#endif

