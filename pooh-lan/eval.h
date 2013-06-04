#ifndef __EVAL_H_Y_Z_
#define __EVAL_H_Y_Z_

#include <pooh-lan/rtlib/rtlib.h>
#include <corothread/stacks.h>
#include "inc.h"

typedef enum {
  EVAL_MODE_SCRIPT,
  EVAL_MODE_GRAMMAR

} EVAL_MODE;

typedef struct {
  EVAL_MODE mode;

  // grammar mode
  char *input_file;

  // script mode.
  char *file_name;
  int   is_trace_on;
  S_PP_RULE_TYPE grammar_trace_flags;
  int   is_verbose;

  char **argv;
  int   argc;

  INC_PATH *inc_path;

}  EVAL_OPTIONS;

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
  int grammar_trace_flags;
  struct tagEVAL_TRACE_ENTRY *last_freed,*top_trace;
  PARSECONTEXT *ctx; // for error messages onlya

  STACKS    stacks; // stacks for co-routines.
} EVAL_CTX;


int EVAL_init( EVAL_CTX *out, PARSECONTEXT *ctx, int is_trace_on, int grammar_trace_flags );
int EVAL_run(  EVAL_CTX *out, AST_BASE *base, char **argv, int argc );
int EVAL_free( EVAL_CTX *out );
BINDING_DATA * EVAL_function(  EVAL_CTX *out, AST_FUNC_CALL *scl );

M_INLINE int EVAL_trace_on( EVAL_CTX *ctx )
{
  return ctx->context.trace_on;
}

#endif

