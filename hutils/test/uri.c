#include <hutils/uri.h>
#include "vtest/vtest.h"
#include <stdio.h>
#include <string.h>

typedef struct tagURLTEST {
   char *url;
 
  char *scheme;

  char *userinfo_raw;
  char *userinfo;
  
  char *host;
  int   port;
  
  char *path_raw;
  char *path;

  char *query_raw;
  char *query;
  
  char *fragment_raw;
  char *fragment;

  char *opaque_raw;
  char *opaque;
} URLTEST;

URLTEST url_tests[] = {

		
	{ 
	  "/path#fragment",

	  0,  
	  0,
	  0,

	  0,
	  -1, 
	  
	  "/path", 
	  "/path", 
	 
	  0,
	  0,
	 
	  "fragment",
	  "fragment",
	 
	  0,
	  0
	},	

	{ 
	  "http://host/path?query=result#fragment",

	  "http",
	    
	  0,
	  0,

	  "host", 
	  -1,
	  
	  "/path", 
	  "/path", 
	  
	  "query=result",
	  "query=result",
	 
	  "fragment",
	  "fragment",
	 
	  0,
	  0
	},	
	
	{ 
	  "http:///path",

	  "http",
	    
	  0,
	  0,

	  0,
	  -1, 
	 
	  "/path",
	  "/path",
	 
	  0,
	  0,
	 
	  0,
	  0,

	  0,
	  0
	},	


	{ 
	  "http://user%61:pass%61@host:8080/path?%61query=result%61#%61fragment%61",

	  "http",

	  "user%61:pass%61",
	  "usera:passa",

	  "host", 
	  8080, 
	  
	  "/path", 
	  "/path", 
	  
	  "%61query=result%61",
	  "aquery=resulta",
	 
	  "%61fragment%61",
	  "afragmenta",
	 
	  0,
	  0
	},

	{ 
	  "http://user%61:pass%61@138.0.0.1:8080/path?%61query=result%61#%61fragment%61",

	  "http",

	  "user%61:pass%61",
	  "usera:passa",

	  "138.0.0.1", 
	  8080, 
	  
	  "/path", 
	  "/path", 
	  
	  "%61query=result%61",
	  "aquery=resulta",
	 
	  "%61fragment%61",
	  "afragmenta",
	 
	  0,
	  0
	},
	
	{ 
	  "http://user%61:pass%61@[::FFFF:204:152:189:116]:8080/path?%61query=result%61#%61fragment%61",

	  "http",

	  "user%61:pass%61",
	  "usera:passa",

	  "::FFFF:204:152:189:116", 
	  8080, 
	  
	  "/path", 
	  "/path", 
	  
	  "%61query=result%61",
	  "aquery=resulta",
	 
	  "%61fragment%61",
	  "afragmenta",
	 
	  0,
	  0
	},
		{ 
	  "http://user:pass@host:8080/path?query=result#fragment",

	  "http",

  	  "user:pass",
  	  "user:pass", 

	  "host", 
	  8080, 
	  
	  "/path", 
	  "/path", 
	  
	  "query=result",
	  "query=result",
	 
	  "fragment",
	  "fragment",
	 
	  0,
	  0
	},
	
	{ 
	  "http://host:8080/path?query=result#fragment",

	  "http",
	    
	  0,
	  0,

	  "host", 
	  8080, 
	  
	  "/path", 
	  "/path", 
	  
	  "query=result",
	  "query=result",
	 
	  "fragment",
	  "fragment",
	 
	  0,
	  0
	},	
	
	{ 
	  "http://host/path?query=result#fragment",

	  "http",
	    
	  0,
	  0,

	  "host", 
	  -1,
	  
	  "/path", 
	  "/path", 
	  
	  "query=result",
	  "query=result",
	 
	  "fragment",
	  "fragment",
	 
	  0,
	  0
	},	
		
	{ 
	  "http://host:8080/path?query=result",

	  "http",
	    
	  0,
	  0,

	  "host", 
	  8080, 
	  
	  "/path", 
	  "/path", 
	  
	  "query=result",
	  "query=result",
	 
	  0,
	  0,
	 
	  0,
	  0
	},	

	{ 
	  "http://host/path#fragment",

	  "http",
	    
	  0,
	  0,

	  "host", 
	  -1, 
	  
	  "/path", 
	  "/path", 
	 
	  0,
	  0,
	 
	  "fragment",
	  "fragment",
	 
	  0,
	  0
	},	
	
	{ 
	  "http://host/path",

	  "http",
	    
	  0,
	  0,

	  "host", 
	  -1, 
	  
	  "/path", 
	  "/path", 
	 
	  0,
	  0,
	 
	  0,
	  0,

	  0,
	  0
	},	

	{ 
	  "http://host",

	  "http",
	    
	  0,
	  0,

	  "host", 
	  -1, 
	 
	  0,
	  0,
	 
	  0,
	  0,
	 
	  0,
	  0,

	  0,
	  0
	},	

	
	{ 
	   "http://user:pass@host:8080/path?que:ry/res@ult#fr@g:me/n?t",

	  "http",
	    
	  "user:pass",
	  "user:pass",

	  0,
	  -1, 
	 
	  "/path",
	  "/path",
	 
	  "que:ry/res@ult",
	  "que:ry/res@ult",
	 
	  "fr@g:me/n?t",
	  "fr@g:me/n?t",
	 
	  0,
	  0,

	},	

	{ 

	  "http://user:pass@host:8080/path#fr@g:me/n?t",

	  "http",
	    
	  "user:pass",
	  "user:pass",

	  "host",
	  8080, 
	 
	  "/path",
	  "/path",
	 
	  0,
	  0,

	  "fr@g:me/n?t",
	  "fr@g:me/n?t",
	 
	  0,
	  0,

	},	

	{ 

	  "http://user:pass@host:8080?que:ry/res@ult#fr@g:me/n?t",

	  "http",
	    
	  "user:pass",
	  "user:pass",

	  "host",
	  8080, 
	 
	  0,
	  0,

	  "que:ry/res@ult",
	  "que:ry/res@ult",

	  "fr@g:me/n?t",
	  "fr@g:me/n?t",
	 
	  0,
	  0,

	},	



/*
': 
'': 
':
' 

'http://user:pass@host:port#fr@g:me/n?t': 
*/

		
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};



int mystrcmp( const char *a, const char *b)
{
  if (a == 0 || b == 0) {
    if (a || b) {
      return 1;
    }
    return 0;
  }
  return strcmp(a, b);

}
void  URI_test_ok()
{
  int i =0;
  URI uri;

  for( i = 0; url_tests[ i ].url != 0 ; ++i ) {

     fprintf(stdout,"\t\t%s\n", url_tests[ i ].url );
     VASSERT( ! URI_parse( &uri, url_tests[ i ].url ) );
     VASSERT( mystrcmp( url_tests[ i ].scheme, URI_scheme( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].userinfo_raw, URI_userinfo_raw( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].userinfo, URI_userinfo( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].path_raw, URI_path_raw( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].path, URI_path( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].query_raw, URI_query_raw( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].query, URI_query( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].fragment_raw, URI_fragment_raw( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].fragment, URI_fragment( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].opaque_raw, URI_opaque_raw( &uri ) ) == 0 );
     VASSERT( mystrcmp( url_tests[ i ].opaque, URI_opaque( &uri ) ) == 0 );

  }
}


char * fail_urls[] = {
  "http://user:pass@1.2.3.4:8080/path?query=resu%ZZlt#fragment", // illegal escape sequence %zz in query
  "http://user:pass@1.2.3.4.5:8080/path?query=result#fragment", // illegal ip address
  "http://use%ZZr:pass@1.2.3.4.5:8080/path?query=result#fragment", // illegal escape sequence in userinfo
  "http://user:pass@1.2.3.4.5:8080/path?query=result#frag%ZZment", // illegal escape sequence in fragment
};

void  URI_test_fail()
{
  int i =0;
  URI uri;

  for( i = 0; fail_urls[i] != 0 ; ++i ) {

     fprintf(stdout,"\t\t%s\n", fail_urls[i]);
     VASSERT(  URI_parse( &uri, fail_urls[i] ) != 0 );

  }
}

