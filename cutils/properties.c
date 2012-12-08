#include "properties.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <cutils/dbuf.h>

/* I took the liberty to adapt the property file parsing stuff from java.lang.Properties class */

#define IN_BUF_SIZE 8192
#define TRUE 1
#define FALSE 0

static int load_properties(PROPERTIES *props, const char *file_name);
static int readLine(FILE *fp, DBUF *lineBuff, char *inBuf, int *inOff, int *inLimit);
static int loadConvert ( DBUF *ret, const char * in, int off, int len);

static int props_compare_key(HASH_Entry *entry, const void * key, ssize_t key_length)
{
  NAMEVALUE_HASH_ENTRY *pe = (NAMEVALUE_HASH_ENTRY *) entry;

  M_UNUSED( key_length );
  return strcmp( pe->name, key );
}


int PROPERTIES_init( PROPERTIES *props, int num_buckets )
{
  return HASH_init( &props->props, num_buckets, 0, props_compare_key, 0 );
}

void PROPERTIES_free( PROPERTIES *props )
{
   HASH_Entry *cur;
   NAMEVALUE_HASH_ENTRY *entry;

   HASH_DELETEALL(cur, &props->props);
     entry = (NAMEVALUE_HASH_ENTRY *) cur;
     free(entry->name);
     free(entry->value);
     free(entry);
   HASH_DELETEALL_END
 
   HASH_free( &props->props );
}


int PROPERTIES_put( PROPERTIES *props, const char *name , const char *value )
{
  NAMEVALUE_HASH_ENTRY *val;

  val = (NAMEVALUE_HASH_ENTRY *) malloc( sizeof(  NAMEVALUE_HASH_ENTRY ) ); 
  if (!val) {
    return -1;
  }
  val->value = val->name = 0;

  val->name = strdup( name );
  if (!val->name) {
    goto err;
  }
  val->value = strdup( value );
  if (!val->value) {
    goto err;
  }
  if (HASH_insert( &props->props, &val->entry, (void *)  name, -1 )) {
    goto err;
  }
  return 0;
err:
  if (val->value) {
    free( val->value );
  }
  if (val->name) {
    free( val->name );
  }
  free( val );
  return 0;

}

const char * PROPERTIES_get( PROPERTIES *props, const char *name )
{
  NAMEVALUE_HASH_ENTRY *value;

  value = (NAMEVALUE_HASH_ENTRY *) HASH_find( &props->props, (void *) name, -1 );
  if (!value) {
    return 0;
  }
  return value->value;
}




int PROPERTIES_load(PROPERTIES *props, const char *file_name)
{
   return load_properties( props, file_name );
}


static int load_properties(PROPERTIES *props, const char *file_name)
{
  FILE * fp; 
  DBUF line,name,value;
  int limit;
  int keyLen;
  int valueStart;
  char c;
  int hasSep;
  int precedingBackslash;
  char *inBuf;
  int inOff = 0, inLimit = 0;
	
  fp = fopen( file_name , "r");
  if (!fp) {
    return -1;
   }

  inBuf = malloc( IN_BUF_SIZE );
  if (!inBuf) {
    fclose(fp);
    return -1;
  }

  DBUF_init( &line, 0 );

  while( (limit = readLine(fp, &line, inBuf, &inOff, &inLimit )) >=0 ) {
        c = 0;
	DBUF_add( &line, &c, sizeof(c) );
	

        keyLen = 0;
        valueStart = limit;
        hasSep = FALSE;

	//System.out.println("line=<" + new String(lineBuf, 0, limit) + ">");
        precedingBackslash = FALSE;
        while (keyLen < limit) {
            c = line.buf[keyLen];
            //need check if escaped.
            if ((c == '=' ||  c == ':') && !precedingBackslash) {
                valueStart = keyLen + 1;
                hasSep = TRUE;
                break;
            } else if ((c == ' ' || c == '\t' ||  c == '\f') && !precedingBackslash) {
                valueStart = keyLen + 1;
                break;
            } 
            if (c == '\\') {
                precedingBackslash = !precedingBackslash;
            } else {
                precedingBackslash = FALSE;
            }
            keyLen++;
        }
        while (valueStart < limit) {
            c = line.buf[valueStart];
            if (c != ' ' && c != '\t' &&  c != '\f') {
                if (!hasSep && (c == '=' ||  c == ':')) {
                    hasSep = TRUE;
                } else {
                    break;
                }
            }
            valueStart++;
        }

	DBUF_init( &name, 0 );
	DBUF_init( &value, 0 );

	if (loadConvert(&name, (char *) line.buf, 0, keyLen)) {
			return -1;
	}
        if (loadConvert(&value, (char *) line.buf, valueStart, limit - valueStart)) {
			return -1;
	}

        PROPERTIES_put( props, (char *) name.buf, (char *) value.buf );

	DBUF_free( &name );
	DBUF_free( &value );
  }
  fclose(fp);
  free(inBuf);
  DBUF_free(&line);
  return 0;
}


static int readLine(FILE *fp, DBUF *lineBuf, char *inBuf, int *inOff, int *inLimit) 
{
  int len = 0;
  char c = 0;
 
  DBUF_reset( lineBuf );

  int skipWhiteSpace = TRUE;
  int isCommentLine = FALSE;
  int isNewLine = TRUE;
  int appendedLineBegin = FALSE;
  int precedingBackslash = FALSE;
  int skipLF = FALSE;

  while (TRUE) {
		if ( *inOff >= *inLimit ) {
			//inLimit = inStream.read(inBuf);
			*inLimit = fread(inBuf,1,IN_BUF_SIZE,fp);
			*inOff = 0;
			
			if (*inLimit <= 0) {
				if (len == 0 || isCommentLine) { 
					return -1; 
				}
				return len;
			}
		}     
		
		//The line below is equivalent to calling a 
		//ISO8859-1 decoder.
		c = (char) (0xff & inBuf[ (*inOff)++ ]);
		if (skipLF) {
			skipLF = FALSE;
			if (c == '\n') {
				continue;
			}
		}
		if (skipWhiteSpace) {
			if (c == ' ' || c == '\t' || c == '\f') {
				continue;
			}
			if (!appendedLineBegin && (c == '\r' || c == '\n')) {
				continue;
			}
			skipWhiteSpace = FALSE;
			appendedLineBegin = FALSE;
		}
		
		if (isNewLine) {
			isNewLine = FALSE;
			if (c == '#' || c == '!') {
				isCommentLine = TRUE;
				continue;
			}
		}
		
		if (c != '\n' && c != '\r') {
			//lineBuf[len++] = c;
			//lineBuf += c;
			DBUF_add( lineBuf, &c, sizeof(c) );
			len ++;
			
			//flip the preceding backslash flag
			if (c == '\\') {
				precedingBackslash = !precedingBackslash;
			} else {
				precedingBackslash = FALSE;
			}
		}
		else {
			// reached EOL
			if (isCommentLine || len == 0) {
				isCommentLine = FALSE;
				isNewLine = TRUE;
				skipWhiteSpace = TRUE;
				len = 0;
				DBUF_reset( lineBuf );
				continue;
			}
			if ( *inOff >= *inLimit ) {
				//inLimit = inStream.read(inBuf);
				*inLimit = fread(inBuf,1,IN_BUF_SIZE,fp);
				*inOff = 0;
				if ( *inLimit <= 0 ) {
					return len;
				}
			}
			if (precedingBackslash) {
				len -= 1;
				//skip the leading whitespace characters in following line
				skipWhiteSpace = TRUE;
				appendedLineBegin = TRUE;
				precedingBackslash = FALSE;
				if (c == '\r') {
					skipLF = TRUE;
				}
			} else {
				return len;
			}
		}
  }
}

/*
 * Converts encoded &#92;uxxxx to unicode chars
 * and changes special saved chars to their original forms
 */
static int loadConvert ( DBUF *ret, const char * in, int off, int len) {
  char aChar;
  int end = off + len;
  unsigned int value=0; 
  int i;
  
  while (off < end) {
        aChar = in[off++];
        if (aChar == '\\') {
            aChar = in[off++];   
            if(aChar == 'u') {
                // Read the xxxx
               
				for (i=0; i<4; i++) {
					aChar = in[off++];  
					switch (aChar) {
					  case '0': case '1': case '2': case '3': case '4':
					  case '5': case '6': case '7': case '8': case '9':
							 value = (value << 4) + aChar - '0';
						 break;
					  case 'a': case 'b': case 'c':
								  case 'd': case 'e': case 'f':
						 value = (value << 4) + 10 + aChar - 'a';
						 break;
					  case 'A': case 'B': case 'C':
								  case 'D': case 'E': case 'F':
						 value = (value << 4) + 10 + aChar - 'A';
						 break;
					  default:
#if 0
						  if (Log::is_error()) {
							  Log::log( LOG_ERROR, "Malformed \\uxxxx encoding.");
									  return -1;
						  }
#endif
					  return -1;
					}
				}
				DBUF_add( ret, &value, sizeof( value ) );
			} else {
                if (aChar == 't') aChar = '\t'; 
                else if (aChar == 'r') aChar = '\r';
                else if (aChar == 'n') aChar = '\n';
                else if (aChar == 'f') aChar = '\f'; 
		
		DBUF_add( ret, &aChar, sizeof( aChar ) );
            }
        } else {
            DBUF_add( ret, &aChar, sizeof( aChar ) );
        }
  }

  aChar = 0;
  DBUF_add( ret, &aChar, sizeof( aChar ) );
  return 0;
}


