
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_END_OF_FILE = 0,
     TK_ERROR = 258,
     TK_INCLUDE = 259,
     TK_SUB = 260,
     TK_EMPTY_STRING = 261,
     TK_TRUE = 262,
     TK_FALSE = 263,
     TK_NIL = 264,
     TK_DOUBLE_CONSTANT = 265,
     TK_MULTI_PART_STRING_CONSTANT = 266,
     TK_STRING_CONSTANT = 267,
     TK_INTEGER_CONSTANT = 268,
     TK_IDENTIFIER = 269,
     TK_NEXT = 270,
     TK_BREAK = 271,
     TK_RETURN = 272,
     TK_END = 273,
     TK_ELSIF = 274,
     TK_ELSE = 275,
     TK_ASSIGN_REF = 276,
     TK_ASSIGN = 277,
     TK_LOOP = 278,
     TK_IF = 279,
     TK_FOR = 280,
     TK_WHILE = 281,
     TK_OP_LOGICAL_AND = 282,
     TK_OP_LOGICAL_OR = 283,
     TK_OP_NUM_EQ = 284,
     TK_OP_NUM_NE = 285,
     TK_OP_NUM_LT = 286,
     TK_OP_NUM_GT = 287,
     TK_OP_NUM_LE = 288,
     TK_OP_NUM_GE = 289,
     TK_OP_DOT = 290,
     TK_OP_STR_EQ = 291,
     TK_OP_STR_NE = 292,
     TK_OP_STR_LT = 293,
     TK_OP_STR_GT = 294,
     TK_OP_STR_LE = 295,
     TK_OP_STR_GE = 296,
     TK_OP_STR_CAT = 297,
     TK_HASH_IT = 298,
     TK_OP_NUM_ADD = 299,
     TK_OP_NUM_SUBST = 300,
     TK_OP_NUM_DIV = 301,
     TK_OP_NUM_MULT = 302,
     TK_OP_NUM_MOD = 303,
     TK_OP_LOGICAL_NEGATE = 304,
     TK_VAR_DEF = 305,
     TK_VAR_UNDEF = 306,
     TK_ARR_DEF = 307,
     TK_CODEREF_DEF = 308,
     TK_OP_STR_REGEXMATCH = 309,
     TK_COLON = 310,
     TK_SEMICOLON = 311,
     TK_COMMA = 312,
     TK_PARENTHESES_OPEN = 313,
     TK_PARENTHESES_CLOSE = 314,
     TK_BRACE_OPEN = 315,
     TK_BRACE_CLOSE = 316,
     TK_BYREF = 317,
     TK_OPTIONAL = 318,
     TK_THREE_DOTS = 319,
     TK_UNDERSCORE = 320,
     TK_BRACKET_CLOSE = 321,
     TK_BRACKET_OPEN = 322,
     TK_START_STATEMENT = 323,
     TK_START_EXPRESSION = 324,
     TK_PARAMETER_LABEL = 325
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



