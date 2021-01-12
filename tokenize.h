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
 *
 * <- false in case or error
 */

bool tokenizeFile( struct storage *storage, const char *file );

#endif
