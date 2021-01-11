/*
 * Tokenise
 */

#ifndef TOKENISE_H
#define TOKENISE_H

#include "config.h"

extern u_int8_t codebuff[];	/* tokenized code storage */
extern size_t codeindex;	/* 1st free place in the code buffer */
extern char stringbuff[];	/* strings storage - format uint8_t size, strings */
extern size_t stringindex;	/* 1st free place in the strings storage */

/* Tokenize a file
 * -> file : filename to tokenise
 *
 * -> buff : buffer to store tokens
 * -> buffsz : size of the buffer
 * -> idx : current position in the buffer
 *
 * -> strbuff : where to store constant strings
 * -> strsz : size of string storage
 * -> stridx : current position in the string storage
 *
 * <- false in case or error
 */

bool tokenizeFile( const char *file, u_int8_t *buff, size_t buffsz, size_t *idx, char *strbuff, size_t strsz, size_t *stridx );

#endif
