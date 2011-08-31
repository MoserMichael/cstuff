#include "tokparser.h"                                                                      
#include <string.h>                                                                      
#include <malloc.h>                                                                      

int parse_token(const char *token, TOKENDEF *tkdef)
{
  int i;
  for(i=0 ; tkdef[i].token_type != -1 ; i++) {
    if ( strcmp(token, tkdef[i].type) == 0 ) {
      return tkdef[i].token_type;
    }
  }
  return -1;
} 

char  *token_possible_values(TOKENDEF *tkdef)
{
  int i;
  int len = 0;
  char *ret;

  for(i=0 ; tkdef[i].token_type != -1 ; i++) {
    len += strlen( tkdef[i].type ) + 1;
  }

  ret = (char *) malloc( len + 1 );
  if (!ret) {
    return 0;
  }

  strcpy(ret, "" );

  for(i=0 ; tkdef[i].token_type != -1 ; i++) {
    strcat( ret, tkdef[i].type);
    strcat( ret, " " );
  }

  return ret;
}

const char *token_string_for_int(int value, TOKENDEF *tkdef)
{
  int i;

  for(i=0 ; tkdef[i].token_type != -1 ; i++) {
    if ( tkdef[i].token_type == value ) {
      return tkdef[i].type;  
    }  
  }
  return "";
}


