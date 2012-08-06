#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "printast.h"
#include <eval.h>
#include <stdio.h>
#include <unistd.h>

extern AST_XFUNC_DECL xlib[];


void dump_ast( const char *file_path, AST_BASE *base, int idx );
void  delete_temp_results(const char *file_path);


int loadRTLIB( PARSECONTEXT *ctx )
{ 
  size_t i;

  for(i=0; xlib[i].f_name != 0 ; i++ )
  {
    PARSECONTEXT_add_function_def(  ctx, &xlib[i] );
  }
  return 0;
}

#if 1 
int eval( PARSECONTEXT *ctx )
{
  EVAL_CTX ectx;

  if ( !EVAL_init( &ectx, ctx )) {
    if (EVAL_run( &ectx, ctx->my_ast_root )) {
      return -1;
    } 
  } else {
    fprintf( stderr, "Can't initialize interpreter\n");
    return -1;
  }
  EVAL_free( &ectx );
  return 0;
}
#endif

int run(const char *file_path)
{
  PARSECONTEXT *ctx;
  AST_BASE *rval;
  int is_verbose = getenv("TEST_VERBOSE") != 0;

  ctx = PARSER_init();

  loadRTLIB( ctx );

  if (LEXER_scan_file( &ctx->lexctx, file_path)) {
    fprintf(stderr,"Can't open file %s\n", file_path);
    goto err;
  }

  if (PARSER_run( ctx, &rval )) {
    fprintf(stderr,"Parsing failed\n");
    goto err;
  }
 
  if (!ctx->my_ast_root) { 
    goto ok;
  }

  // dump ast before checker.
  if (is_verbose) {
    dump_ast( file_path, ctx->my_ast_root, 1 );
  }

  if ( CHECKER_run( &ctx->chkctx, ctx->my_ast_root ) ) {
    fprintf(stderr,"Type checking failed\n");
    goto err;
  }
  
  // dump ast after checker
  if (is_verbose) {
    dump_ast( file_path, ctx->my_ast_root, 2 );
  }

#if 1
  if (eval( ctx )) {
    goto err;
  }
#endif

ok:
  PARSER_free(ctx);
  return 0;
  
err:

  delete_temp_results(file_path);
  PARSER_free(ctx);
  return 1;
}

char *make_path( const char *file_path, const char *pattern, int idx)
{
  char *name;

  name = (char *)  malloc( strlen(file_path) + strlen(pattern) + 20 );
  assert(name);
  if (!name) {
    return 0;
  }
  sprintf( name, "%s%s%d", file_path, pattern, idx );
  return name;
}
 

void  delete_temp_results(const char *file_path)
{
  char *path;

  path = make_path( file_path, ".ast.", 1 );
  if (path) {
    //unlink( path );
  }
}

void dump_ast( const char *file_path, AST_BASE *base, int idx )
{
  char *name;
  FILE *fout;


  name = make_path( file_path, ".ast.", idx );
  if (!name) {
    return;
  }
  fout = fopen( name, "w" );
  if (fout) {
    AST_print( fout, base );
    fclose(fout);
  }
  free(name);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
     fprintf(stderr,"Please provide file argument in comand line\n"); 
     exit(1);
  }
  if (run(argv[1])) {
    exit(1);
  }
  return 0;
}
