#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "printast.h"
#include <stdio.h>
#include <unistd.h>

void dump_ast( const char *file_path, AST_BASE *base, int idx );
void  delete_temp_results(const char *file_path);

int run(const char *file_path)
{

  PARSECONTEXT *ctx;
  AST_BASE *rval;
 
  ctx = PARSER_init();

  if (LEXER_scan_file( &ctx->lexctx, file_path)) {
    fprintf(stderr,"Can't open file %s\n", file_path);
    goto err;
  }

  if (PARSER_run( ctx, &rval )) {
    fprintf(stderr,"Parsing failed\n");
    goto err;
  }
 
  if (ctx->my_ast_root) { 
    dump_ast( file_path, ctx->my_ast_root, 1 );
  }

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
    unlink( path );
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
