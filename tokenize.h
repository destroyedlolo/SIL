/*
 * Tokenise
 */

#ifndef TOKENISE_H
#define TOKENISE_H

#include "config.h"
#include "storage.h"

extern u_int8_t codebuff[];	/* tokenized code storage */
extern size_t codeindex;	/* 1st free place in the code buffer */
extern char stringbuff[];	/* strings storage - format uint8_t size, strings */
extern size_t stringindex;	/* 1st free place in the strings storage */

/* Tokenize a file
 * -> storage : where to tokenize
 * -> file : filename to tokenise
 *
 * <- false in case or error
 */

bool tokenizeFile( struct storage *storage, const char *file );

#endif
