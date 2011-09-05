#include <butils/strtk.h>
#include "vtest/vtest.h"
#include <string.h>
#include <stdio.h>

#define TSTRING "ab-cd1.f3g "
#define TPATTERN "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-[]"

#define TKPATTERN " \t\r\n"
#define TKSTRING "The \t quick  \r brown  \r\n fox jumped over the lazy dog"

void STRTK_span_test()
{

  STRTK tok;

  STRTK_init( &tok, TPATTERN );

  VASSERT( (STRTK_spn( &tok, TSTRING ) - TSTRING) == 10 );
  VASSERT( (STRTK_nspn( &tok, TSTRING,  11 ) - TSTRING) == 10 );
}

void STRTK_tok_test()
{
  STRTK tok;
  char *next,*res;

  STRTK_init( &tok, TKPATTERN );

  
  res = STRTK_tok( &tok, strdup(TKSTRING), &next );
  *next = '\0';
  VASSERT( strcmp(  "The", res ) == 0);
 
  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "quick", res ) == 0);
 
  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "brown", res ) == 0);
  
  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "fox", res ) == 0);
 
  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "jumped", res ) == 0);
  
  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "over", res ) == 0);

  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "the", res ) == 0);
  
  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "lazy", res ) == 0);

  res = STRTK_tok( &tok, next+1, &next );
  *next = '\0';
  VASSERT( strcmp(  "dog", res ) == 0);
   
}
  


