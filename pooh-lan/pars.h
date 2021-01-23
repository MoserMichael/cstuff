/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARS_H_INCLUDED
# define YY_YY_PARS_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    TK_END_OF_FILE = 0,            /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TK_ERROR = 258,                /* TK_ERROR  */
    TK_WHILE = 259,                /* TK_WHILE  */
    TK_FOR = 260,                  /* TK_FOR  */
    TK_IF = 261,                   /* TK_IF  */
    TK_LOOP = 262,                 /* TK_LOOP  */
    TK_ASSIGN = 263,               /* TK_ASSIGN  */
    TK_ASSIGN_REF = 264,           /* TK_ASSIGN_REF  */
    TK_ELSE = 265,                 /* TK_ELSE  */
    TK_ELSIF = 266,                /* TK_ELSIF  */
    TK_END = 267,                  /* TK_END  */
    TK_RETURN = 268,               /* TK_RETURN  */
    TK_BREAK = 269,                /* TK_BREAK  */
    TK_SKIP = 270,                 /* TK_SKIP  */
    TK_IDENTIFIER = 271,           /* TK_IDENTIFIER  */
    TK_INTEGER_CONSTANT = 272,     /* TK_INTEGER_CONSTANT  */
    TK_STRING_CONSTANT = 273,      /* TK_STRING_CONSTANT  */
    TK_MULTI_PART_STRING_CONSTANT = 274, /* TK_MULTI_PART_STRING_CONSTANT  */
    TK_DOUBLE_CONSTANT = 275,      /* TK_DOUBLE_CONSTANT  */
    TK_NIL = 276,                  /* TK_NIL  */
    TK_FALSE = 277,                /* TK_FALSE  */
    TK_TRUE = 278,                 /* TK_TRUE  */
    TK_EMPTY_STRING = 279,         /* TK_EMPTY_STRING  */
    TK_SUB = 280,                  /* TK_SUB  */
    TK_INCLUDE = 281,              /* TK_INCLUDE  */
    TK_LOADEXTENSION = 282,        /* TK_LOADEXTENSION  */
    TK_OP_LOGICAL_AND = 283,       /* TK_OP_LOGICAL_AND  */
    TK_OP_LOGICAL_OR = 284,        /* TK_OP_LOGICAL_OR  */
    TK_OP_NUM_EQ = 285,            /* TK_OP_NUM_EQ  */
    TK_OP_NUM_NE = 286,            /* TK_OP_NUM_NE  */
    TK_OP_NUM_LT = 287,            /* TK_OP_NUM_LT  */
    TK_OP_NUM_GT = 288,            /* TK_OP_NUM_GT  */
    TK_OP_NUM_LE = 289,            /* TK_OP_NUM_LE  */
    TK_OP_NUM_GE = 290,            /* TK_OP_NUM_GE  */
    TK_OP_DOT = 291,               /* TK_OP_DOT  */
    TK_OP_STR_EQ = 292,            /* TK_OP_STR_EQ  */
    TK_OP_STR_NE = 293,            /* TK_OP_STR_NE  */
    TK_OP_STR_LT = 294,            /* TK_OP_STR_LT  */
    TK_OP_STR_GT = 295,            /* TK_OP_STR_GT  */
    TK_OP_STR_LE = 296,            /* TK_OP_STR_LE  */
    TK_OP_STR_GE = 297,            /* TK_OP_STR_GE  */
    TK_OP_STR_CAT = 298,           /* TK_OP_STR_CAT  */
    TK_HASH_IT = 299,              /* TK_HASH_IT  */
    TK_OP_NUM_ADD = 300,           /* TK_OP_NUM_ADD  */
    TK_OP_NUM_SUBST = 301,         /* TK_OP_NUM_SUBST  */
    TK_OP_NUM_DIV = 302,           /* TK_OP_NUM_DIV  */
    TK_OP_NUM_MULT = 303,          /* TK_OP_NUM_MULT  */
    TK_OP_NUM_MOD = 304,           /* TK_OP_NUM_MOD  */
    TK_OP_NUM_POW = 305,           /* TK_OP_NUM_POW  */
    TK_OP_LOGICAL_NEGATE = 306,    /* TK_OP_LOGICAL_NEGATE  */
    TK_VAR_DEF = 307,              /* TK_VAR_DEF  */
    TK_VAR_UNDEF = 308,            /* TK_VAR_UNDEF  */
    TK_ARR_DEF = 309,              /* TK_ARR_DEF  */
    TK_CODEREF_DEF = 310,          /* TK_CODEREF_DEF  */
    TK_OP_STR_REGEXMATCH = 311,    /* TK_OP_STR_REGEXMATCH  */
    TK_COLON = 312,                /* TK_COLON  */
    TK_SEMICOLON = 313,            /* TK_SEMICOLON  */
    TK_COMMA = 314,                /* TK_COMMA  */
    TK_PARENTHESES_OPEN = 315,     /* TK_PARENTHESES_OPEN  */
    TK_PARENTHESES_CLOSE = 316,    /* TK_PARENTHESES_CLOSE  */
    TK_BRACE_OPEN = 317,           /* TK_BRACE_OPEN  */
    TK_BRACE_CLOSE = 318,          /* TK_BRACE_CLOSE  */
    TK_BYREF = 319,                /* TK_BYREF  */
    TK_OPTIONAL = 320,             /* TK_OPTIONAL  */
    TK_THREE_DOTS = 321,           /* TK_THREE_DOTS  */
    TK_BRACKET_OPEN = 322,         /* TK_BRACKET_OPEN  */
    TK_BRACKET_CLOSE = 323,        /* TK_BRACKET_CLOSE  */
    TK_UNDERSCORE = 324,           /* TK_UNDERSCORE  */
    TK_START_STATEMENT = 325,      /* TK_START_STATEMENT  */
    TK_START_EXPRESSION = 326,     /* TK_START_EXPRESSION  */
    TK_PARAMETER_LABEL = 327       /* TK_PARAMETER_LABEL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (PARSECONTEXT *parse_context);

#endif /* !YY_YY_PARS_H_INCLUDED  */
