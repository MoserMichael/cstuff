#include "lexer.h"
#include "parser.h"
#include <stdio.h>


int run(const char *file_path)
{

  PARSER_init();

  if (LEXER_scan_file(file_path)) {
    fprintf(stderr,"Can't open file %s\n", file_path);
    goto err;
  }
  if (PARSER_run( 0 )) {
    fprintf(stderr,"Parsing failed\n");
    goto err;
  }

  PARSER_free();
  return 0;
  
err:

  PARSER_free();
  return 1;
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
