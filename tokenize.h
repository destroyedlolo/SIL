/*
 * Tokenise
 */

#ifndef TOKENISE_H
#define TOKENISE_H

#include "config.h"
#include "storage.h"

/* Tokenize a file
 * -> storage : where to tokenize
 * -> file : filename to tokenise
 * -> linenumber : pointer to linenumber (used to display error)
 *
 * <- false in case or error
 */

extern bool tokenizeFile( struct storage *storage, const char *file, size_t *linenumber );

/*
 * misc functions
 */
extern void init_token();

#endif
