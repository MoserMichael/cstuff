#ifndef __CHECKER_H_Y_Z_
#define __CHECKER_H_Y_Z_

#include <cutils/bhash.h>

struct tagAST_BASE;
struct tagAST_FUNC_DECL;


typedef struct tagCHECKERCTX {
  struct tagAST_FUNC_DECL *current_function;
  struct tagAST_VECTOR *func_decls;
 
  int pass;
  int is_root_func;
  int is_function_name;
  int is_left_hand_side;
} CHECKERCTX;

/**
 * Semantics checks
 *
 * for each function all bindings are checked;
 * - checks that all referenced bindings are defined.
 * (have to peel each layer of function off for that).
  
 * for each function call:
 * - checks that only defined functions are called (no call of undefined function)
 *
 */

int CHECKER_init( CHECKERCTX *ctx);

int CHECKER_run( CHECKERCTX *ctx, struct tagAST_BASE *program);

#endif

