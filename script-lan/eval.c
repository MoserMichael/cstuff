


typedef struct tagEVAL_THREAD {
  AST_BASE_LIST *list; // current instruction runs here.
  AST_BASE *instr; // current instruction
  ARRAY stack; // for evaluation of expressions and passing of function parameters and return values.

} EVAL_THREAD;


typedef struct tagEVAL_CONTEX {
  EVAL_THREAD main;

} EVAL_CONTEXT;

int EVAL_run( PARSECONTEXT *ctx, EVAL_THREAD *thread )
{
  AST_BASE_LIST *body;
  DRING *tmp;
  AST_BASE *base;


  DRING_FOREACH( tmp, &body->statements ) {

  }
}
