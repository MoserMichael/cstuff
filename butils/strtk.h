#ifndef __STRTK_X_Y_H__
#define __STRTK_X_Y_H__

#include <cutils/base.h>

#define STRTK_PATTERN_SIZE (( 255 / 8 ) + 1)


/**
 * @defgroup STRTK
 *
 * @brief class holds a bitmap of characters, that is later used for tokenisation.
 * Functions like strtok and strpbrk build a bitmap of characters when they are called.
 * They use this bitmap when these function later parse the argument character
 * string, this bitmap is used to check if a character is
 * part of the character set or not.
 *
 * The class STRTK holds the bitmap holding the character set, so that creation
 * of this bitmap can be done in separate step from usage, therefore parsing
 * function will not create character bitmap over and over again.
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

#define STRTK_IS_CHAR( tok, ch ) \
   ( (tok)->pattern[ (ch) >> 3 ] && (1 << ((ch) & 7))  )

/**
 * @brief Returns token where all characters belong to argument character class.
 */

M_INLINE char * STRTK_spn(STRTK *tok, const char *pos)
{
   uint8_t *cpos;

   for( cpos = (uint8_t *) pos; cpos != '\0'; ++cpos ) {
      if (!STRTK_IS_CHAR( tok, *cpos )) {
         break;
      }
   }
   return (char *) cpos;
}

/**
 * @brief Returns token where all characters belong to argument character class, argument string is limited.
 */
M_INLINE char * STRTK_nspn(STRTK *tok, const char *pos, size_t nsize )
{
   uint8_t *cpos;

   for( cpos = (uint8_t *) pos; cpos != '\0' && nsize > 0 ; ++cpos, --nsize ) {
      if (!STRTK_IS_CHAR( tok, *cpos )) {
         break;
      }
   }
   return (char *) cpos;
}

/**
 * @}
 */


#endif

