#ifndef _TOKPARSER_H_
#define _TOKPARSER_H_


typedef struct tagTOKENDEF {
  int token_type;
  const char *type;
} TOKENDEF;

int parse_token(const char *token, TOKENDEF *tkdef);


const char *token_string_for_int(int value, TOKENDEF *tkdef);

char *token_possible_values(TOKENDEF *tkdef);

#endif
