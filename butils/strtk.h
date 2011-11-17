#ifndef __STRTK_X_Y_H__
#define __STRTK_X_Y_H__

#include <cutils/base.h>

#define STRTK_PATTERN_SIZE ( 256 / 8 )


/**
 * @defgroup STRTK
 *
 * @brief defines a character class. holds a bitmap of characters, that is later used for tokenization.
 *
 * Functions like strtok and strpbrk build a bitmap of characters when they are called.
 * They use this bitmap when these function later parse the argument character
 * string, this bitmap is used to check if a character is
 * part of the character set or not.
 *
 * The class STRTK holds the bitmap holding the character set, so that creation
 * of this bitmap can be done in separate step from usage, therefore parsing
 * function will not create character bitmap over and over again.
 *
 * I don't know if this is very usefull, in terms of peformance: a character class bitmap takes 32 bytes, which is two cache lines.
 * So two cache lines have to be moved in order to use this stuff.
 *
 * There are situations when a class like this is much better than strtk; when parsing a string in loop; then again within the loop
 * you want to parse each token with strtk - you can't; you can with this class; Also you do not have to copy the input string; this
 * one does not modify the input string.
 *
 * @{
 */

typedef struct tagSTRTK {
  uint8_t pattern[ STRTK_PATTERN_SIZE ];

} STRTK;


/**
 * @brief initalise a character class
 */
void STRTK_init( STRTK *tok , const char *stop_chars );

M_INLINE void STRTK_add_char( STRTK *tok, uint8_t ch )
{
  tok->pattern[ ch >> 3 ] |= (1 << (ch & 7));
}

/**
 * @brief macro checks if character is part of character class
 */
#define STRTK_IS_CHAR( tok, ch ) \
   ( (tok)->pattern[ (ch) >> 3 ] && (1 << ((ch) & 7))  )

/**
 * @brief Returns token where all characters belong to argument character class.
 * @returns the pointer to the next character that is not part of the character class.
 */
M_INLINE char * STRTK_spn(STRTK *tok, const char *hay)
{
   uint8_t *cpos;

   for( cpos = (uint8_t *) hay; *cpos != '\0'; ++cpos ) {
      if (!STRTK_IS_CHAR( tok, *cpos )) {
         break;
      }
   }
   return (char *) cpos;
}

/**
 * @brief Returns token where all characters belong to argument character class, argument string is limited.
 * @returns the pointer to the next character that is not part of the character class.
 */
M_INLINE char * STRTK_nspn(STRTK *tok, const char *hay, size_t nsize )
{
   uint8_t *cpos;

   for( cpos = (uint8_t *) hay; *cpos != '\0' && nsize > 0 ; ++cpos, --nsize ) {
      if (!STRTK_IS_CHAR( tok, *cpos )) {
         break;
      }
   }
   return (char *) cpos;
}


/** 
 * @brief tokenize a string . the character class is interpreted as set of white spaces.
 * @return pointer to the start of token, end_tok returns position right after the token.
 */
M_INLINE char * STRTK_ntok(STRTK *tok, const char *hay, size_t nsize, char **end_tok )
{
   uint8_t *cpos;
   char *start_tok;

   // skip leading whitespaces.
   for( cpos = (uint8_t *) hay; *cpos != '\0' && nsize > 0 && STRTK_IS_CHAR( tok, *cpos ) ; ++cpos, --nsize );
 
   start_tok = (char *) cpos;

   for( ; *cpos != '\0' && nsize > 0 && !STRTK_IS_CHAR( tok, *cpos ) ; ++cpos, --nsize );

   *end_tok = (char *) cpos;

   if (start_tok == (char *) cpos) {
     return 0;
   }
   return start_tok;
}

/** 
 * @brief tokenize a string . the character class is interpreted as set of white spaces.
 * @return pointer to the start of token, end_tok returns position right after the token.
 */
M_INLINE char * STRTK_tok(STRTK *tok, const char *hay, const char **end_tok )
{
   uint8_t *cpos;
   char *start_tok;

   // skip leading whitespaces.
   for( cpos = (uint8_t *) hay; *cpos != '\0' && STRTK_IS_CHAR( tok, *cpos ) ; ++cpos);
 
   start_tok = (char *) cpos;

   for( ; *cpos != '\0' && !STRTK_IS_CHAR( tok, *cpos )  ; ++cpos);

   *end_tok = (char *) cpos;

   if (start_tok == (char *) cpos) {
     return 0;
   }
   return start_tok;
}

/**
 * @}
 */


#endif

