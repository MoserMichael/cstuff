
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsectx.h"
%}

%option  noyywrap reentrant bison-bridge bison-locations


%x	 comment

INT_CONST	[0-9]+
ID		[a-zA-Z_][a-zA-Z0-9_]*
PARAM		"~"[a-zA-Z_][a-zA-Z0-9_]*
DOUBLE		[0-9]+"."[0-9]+([Ee][+-][0-9]+)?


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

":"		{ MY_YY_RETURN(TK_HASH_IT); }  

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

