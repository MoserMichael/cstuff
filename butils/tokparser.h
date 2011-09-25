#ifndef _TOKPARSER_H_
#define _TOKPARSER_H_


/**
 * @defgroup TOKENDEF
 * @brief take a string and check if it any one in list of tokens.
 * The entry TOKENDEF defines pair of token string and its integer value that identifies the token.
 * Parsing of string is done by comparing the input token in sequence against each entry in the table.
 * Not very fast, but very convenient.
 *
 * @{
 */
typedef struct tagTOKENDEF {
  int token_type;
  const char *type;
} TOKENDEF;

int parse_token(const char *token, TOKENDEF *tkdef);


const char *token_string_for_int(int value, TOKENDEF *tkdef);

char *token_possible_values(TOKENDEF *tkdef);

/*
 * @}
 */
#endif
