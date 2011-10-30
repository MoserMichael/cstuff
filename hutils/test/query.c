#include "vtest/vtest.h"
#include <hutils/query.h>
#include <stdio.h>

typedef struct tagPARAMTEST {
  char *name;
  char *value;
} PARAMTEST;

typedef struct tagURLPTEST {
   char *url;
   PARAMTEST *params;
} URLPTEST;

PARAMTEST p_u1[] = {
  { "name1", "value1" },
  { "name2", "value2p_u1" },
  { 0, 0 }
};

PARAMTEST p_u2[] = {
  { "name1", "value1" },
  { "name2", "value2p_u1" },
  { "name3", "" },
  { "value4", 0 },
  { 0, 0 }
};

PARAMTEST p_u3[] = {
  { "namea", "valuea" },
  { "aname1", "avalue1" },
  { "name2", "value2p_u1" },
  { "name3", "" },
  { 0, 0 }
};

URLPTEST ptest[] = {
   {  "http://a/b?name=value&name1=value1&name2=value2p_u1", p_u1 },
   {  "http://a/b?name=value&name1=value1&name2=value2p_u1&name3", p_u2 },
   {  "http://a/b?name%61=value%61&%61name1=%61value1&name2=value2p_u1&name3=", p_u3 },
   { 0, 0 }
};


void QUERY_test()
{
  int i, j;
  PARAMTEST *pvalues; 
  URI uri;
  QUERYSTRING query;
  const char *sval;

  for( i = 0; ptest[i].url != 0; i++) {
    
    fprintf(stdout,"\t\t%s\n", ptest[i].url );
    
    VASSERT( ! URI_parse( &uri, ptest[ i ].url ) );

    VASSERT( ! QUERYSTRING_init( &query, &uri ) );

    pvalues = ptest[i].params;
    for( j = 0; pvalues[j].name != 0; j++) {
      sval = QUERYSTRING_find( &query, pvalues[j].name);

    //fprintf(stdout,"\t\t\t%s %s - %s\n", pvalues[j].name, pvalues[j].value, sval );
      if (pvalues[j].value == 0) {
        VASSERT( sval == pvalues[j].value);
      } else {
        VASSERT( strcmp( sval, pvalues[j].value)  == 0);
      }
    }

    QUERYSTRING_free( &query );
    URI_free( &uri );
  }
}
 
