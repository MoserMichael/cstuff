
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsectx.h"
%}

%option  noyywrap reentrant bison-bridge bison-locations


%x	 comment
%x	 GRAMMAR
%x	 GRAMMARCHARCL



INT_CONST	[0-9]+
ID		[a-zA-Z_][a-zA-Z0-9_]*
PARAM		"~"[a-zA-Z_][a-zA-Z0-9_]*
DOUBLE		[0-9]+"."[0-9]+([Ee][+-][0-9]+)?
RULENAME	[a-zA-Z_-][a-zA-Z0-9_-]* 
RULEREF		"<"[a-zA-Z_-][a-zA-Z0-9_-]*">"



%%


%{
#ifdef IS_REENTRANT
    LEXCONTEXT *lex_context = (LEXCONTEXT *) yyextra;
 
    if (! yyg->yylloc_r ) {
      yyensure_buffer_stack (yyscanner);
      YYLTYPE_init( yylloc_param );
      if (lex_context->set_init_location) {
	*yylloc_param = lex_context->init_location;
      } 
      yyg->yylloc_r = yylloc_param; 
      //yylineno = 1;
      //yycolumn = 1;	
    }	
#else
    LEXCONTEXT *lex_context = (LEXCONTEXT *) &parse_context->lexctx;
#endif

    if (lex_context->send_init_token) {

      lex_context->send_init_token = 0; 
      MY_YY_RETURN(lex_context->init_token_value);
    }
%}


","	{ MY_YY_RETURN(TK_COMMA); }

"("	{ MY_YY_RETURN(TK_PARENTHESES_OPEN); }

")"	{ MY_YY_RETURN(TK_PARENTHESES_CLOSE); }

"{"	{ MY_YY_RETURN(TK_BRACE_OPEN); }

"}"	{ MY_YY_RETURN(TK_BRACE_CLOSE); }

"["	{ MY_YY_RETURN(TK_BRACKET_OPEN); }

"]"	{ MY_YY_RETURN(TK_BRACKET_CLOSE); }

"for"	{ MY_YY_RETURN(TK_FOR);	  }

"if"	{ MY_YY_RETURN(TK_IF);	  }

"elseif" { MY_YY_RETURN(TK_ELSIF); }  
"elsif" { MY_YY_RETURN(TK_ELSIF); }  

"else"	{ MY_YY_RETURN(TK_ELSE); }


"end"	{ MY_YY_RETURN(TK_END);  }

"sub"	{ MY_YY_RETURN(TK_SUB);  }

"include" { MY_YY_RETURN(TK_INCLUDE); }

"loadextension" { MY_YY_RETURN(TK_LOADEXTENSION); }
 

"=="	{ MY_YY_RETURN(TK_OP_NUM_EQ); }

"="	{ MY_YY_RETURN(TK_ASSIGN);    }

":="	{ MY_YY_RETURN(TK_ASSIGN_REF); }

"!="	{ MY_YY_RETURN(TK_OP_NUM_NE); }

"<="	{ MY_YY_RETURN(TK_OP_NUM_LE); }

">="	{ MY_YY_RETURN(TK_OP_NUM_GE); }

"<"	{ MY_YY_RETURN(TK_OP_NUM_LT); }

">"	{ MY_YY_RETURN(TK_OP_NUM_GT); } 

"eq"	{ MY_YY_RETURN(TK_OP_STR_EQ); }

"ne"	{ MY_YY_RETURN(TK_OP_STR_NE); }

"lt"	{ MY_YY_RETURN(TK_OP_STR_LT); }

"gt"	{ MY_YY_RETURN(TK_OP_STR_GT); }

"le"	{ MY_YY_RETURN(TK_OP_STR_LE); }

"ge"	{ MY_YY_RETURN(TK_OP_STR_GE); }

"..."	{ MY_YY_RETURN(TK_THREE_DOTS); }

".."	{ MY_YY_RETURN(TK_OP_STR_CAT); }

"."     { MY_YY_RETURN(TK_OP_DOT); }

"_"     { MY_YY_RETURN(TK_UNDERSCORE); }


"=~"	{ MY_YY_RETURN(TK_OP_STR_REGEXMATCH); }

"+"	{ MY_YY_RETURN(TK_OP_NUM_ADD); }

"-"	{ MY_YY_RETURN(TK_OP_NUM_SUBST); }

"*"	{ MY_YY_RETURN(TK_OP_NUM_MULT); }

"%"	{ MY_YY_RETURN(TK_OP_NUM_MOD); }  

"/"	{ MY_YY_RETURN(TK_OP_NUM_DIV); }

"^"	{ MY_YY_RETURN(TK_OP_NUM_POW); }
 

"!"	{ MY_YY_RETURN(TK_OP_LOGICAL_NEGATE); }
 
"not"	{ MY_YY_RETURN(TK_OP_LOGICAL_NEGATE); }
 
"&&"	{ MY_YY_RETURN(TK_OP_LOGICAL_AND); } 

"and"	{ MY_YY_RETURN(TK_OP_LOGICAL_AND); } 


"||"	{ MY_YY_RETURN(TK_OP_LOGICAL_OR); }

"or"	{ MY_YY_RETURN(TK_OP_LOGICAL_OR); }

":"	{ MY_YY_RETURN(TK_COLON); }  

"="	{ MY_YY_RETURN(TK_ASSIGN);    }

"return" { MY_YY_RETURN(TK_RETURN); }

"while" { MY_YY_RETURN(TK_WHILE); }

"break" { MY_YY_RETURN(TK_BREAK); }

"next" { MY_YY_RETURN(TK_NEXT); }

"loop"	{ MY_YY_RETURN(TK_LOOP); }

"byref" { MY_YY_RETURN(TK_BYREF); }

"null"  { MY_YY_RETURN(TK_NIL); }
"Null"  { MY_YY_RETURN(TK_NIL); }

"false" { MY_YY_RETURN(TK_FALSE); }
"False" { MY_YY_RETURN(TK_FALSE); }

"true"  { MY_YY_RETURN(TK_TRUE); }
"True"  { MY_YY_RETURN(TK_TRUE); }
 
 
"optional" { MY_YY_RETURN(TK_OPTIONAL); }

{ID}	{ 
	  SET_YY_STYPE( string_value, strdup(yytext) );
	  MY_YY_RETURN(TK_IDENTIFIER); 
	}

{PARAM}	{ 
	  char *label = strdup(yytext);

	  label += 1;

	  SET_YY_STYPE( string_value, label );
	  MY_YY_RETURN(TK_PARAMETER_LABEL);
	}	


{INT_CONST} { 
		SET_YY_STYPE(long_value, atol(yytext) );
		MY_YY_RETURN(TK_INTEGER_CONSTANT); 
	}

{DOUBLE} {
		SET_YY_STYPE(double_value, atof(yytext) );
		MY_YY_RETURN(TK_DOUBLE_CONSTANT);
	}

"\""   { MY_YY_RETURN(TK_EMPTY_STRING); }

"'" {
		int rt;

		rt = parse_string( lex_context, '\'' ) ; 

		if (rt <= 0) {
		  MY_YY_RETURN(TK_ERROR);
		}
	        if (rt == 1) {
	 	  return TK_STRING_CONSTANT;
		}
	        return TK_MULTI_PART_STRING_CONSTANT;  
 }


			
"#" {

	int c;
	while ( (c =  MY_YY_INPUT) != EOF )  {
	
	    if (c ==  '\n' ) {
		break;
	    } 
	}
	//fprintf(stderr,"*** comment ***\n");
    }



[\n]	{
            ++ yylineno;
	    yycolumn = 0;	
	  //fprintf(stderr,"NEWLINE %d\n", yylineno );
	}	 

[ \t]+  { yycolumn += yyleng; }	


<<EOF>> {
			if (LEXER_close_file( lex_context )) {
				yyterminate();
			}
	}


<GRAMMAR>"("	    { MY_YY_RETURN(TK_PARENTHESES_OPEN); }

<GRAMMAR>")"	    { MY_YY_RETURN(TK_PARENTHESES_CLOSE); }

<GRAMMAR>"="	    { MY_YY_RETURN(TK_ASSIGN); }

<GRAMMAR>":"	    { MY_YY_RETURN(TK_COLON); }

<GRAMMAR>"any"	    { MY_YY_RETURN(TK_ANY); }
 
<GRAMMAR>"some"	    { MY_YY_RETURN(TK_SOME); }
 
<GRAMMAR>"none"	    { MY_YY_RETURN(TK_NONE); }
 
<GRAMMAR>"until"    { MY_YY_RETURN(TK_UNTIL); }

<GRAMMAR>"extract"  { MY_YY_RETURN(TK_EXTRACT); }

<GRAMMAR>"optional" { MY_YY_RETURN(TK_OPTIONAL); }



<GRAMMAR>{RULENAME} {
			    SET_YY_STYPE( string_value, strdup(yytext) );
			    MY_YY_RETURN(TK_RULE_NAME); 
		    }

		  
<GRAMMAR>{INT_CONST} { 
			    SET_YY_STYPE(long_value, atol(yytext) );
			    MY_YY_RETURN(TK_INTEGER_CONSTANT); 
		    }

<GRAMMAR>"'"	    {
			    if (!do_parse_string_no_expression( lex_context,  '\'' ))
			    {
				MY_YY_RETURN(TK_STRING_CONSTANT);
			    }
	 		    MY_YY_RETURN(TK_ERROR);
		    }
	
<GRAMMAR>"["	    {

			    STRING_PART *part;
			    DBUF buff;
			 
			    DBUF_init( &buff, 0 );
			    if (parse_expression_only( lex_context,  &buff) )
			    {
				 MY_YY_RETURN(TK_ERROR);
			    }
			    DBUF_add_null( &buff );
			    SET_YY_STYPE( string_value, (const char *) buff.buf );
			    
			    MY_YY_RETURN(TK_CODE_SECTION);
		    }			    
		    
<GRAMMAR>{RULEREF}  {
			    int nlen = strlen( yytext );
			    char *ruleref = malloc( nlen - 1 );
			
			    memcpy( ruleref, yytext + 1, nlen - 2 );
			    ruleref[ nlen - 1 ] = '\0';

			    SET_YY_STYPE( string_value, ruleref );
			    MY_YY_RETURN(TK_RULE_REFERENCE); 
		    }

<GRAMMAR>[\n]	{
            ++ yylineno;
	    yycolumn = 0;	
	  //fprintf(stderr,"NEWLINE %d\n", yylineno );
	}	 

<GRAMMAR>[ \t]+  { yycolumn += yyleng; }	

<GRAMMAR>"/"	{ BEGIN(GRAMMARCHARCL); MY_YY_RETURN(TK_OP_NUM_DIV); }



<GRAMMARCHARCL>[0-9a-zA-Z] {
			    SET_YY_STYPE(long_value, yytext[0] );
			    MY_YY_RETURN(TK_CHARACTER);
		    }
	
<GRAMMARCHARCL>"-"	{ MY_YY_RETURN(TK_OP_NUM_SUBST); }

<GRAMMARCHARCL>"/"	{ BEGIN(GRAMMAR); MY_YY_RETURN(TK_OP_NUM_DIV); }



<GRAMMARCHARCL>"\\"	    {
			    int c;
			    uint32_t val = 0;

			    c =  MY_YY_INPUT;

			    switch( c ) {
				case '0':
				    val = 0;
				    break;
				case 'a':
				    val = '\a';
				    break;
				case 'b':
				    val = '\b';
				    break;
				case 'f':
				    val = '\f';
				    break;
				case 'n':
				    val = '\n';
				    break;
				case 'r':
				    val = '\r';
				    break;
				case 't':
				    val = '\t';
				    break;
				case 'v':
				    val = '\v';
				    break;
				default:
				    if (isxdigit(c)) {
				      int i;

				      val = c;
				      for( i = 1; i < 4; i ++ ) {
					 c = MY_YY_INPUT;
					 if (!isxdigit(c)) {
					    break;
					 }
					 val |= ((uint32_t) c) << (i * 8);
				      }
				      unput(c);
				    } else {
				      val = c;
				    }
				    break;
			    }
			    SET_YY_STYPE(long_value, val );
			    MY_YY_RETURN(TK_ESCAPE_SEQUENCE); 
			    
		    }	    

<GRAMMARCHARCL>[ \t]+  { yycolumn += yyleng; }	
	
<GRAMMARCHARCL>[\n]	{
            ++ yylineno;
	    yycolumn = 0;	
	  //fprintf(stderr,"NEWLINE %d\n", yylineno );
	}	 

	
