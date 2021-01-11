/*
 * Tokenise
 */

#include "tokenize.h"

#include <stdio.h>

struct reader {
	struct storage *storage;

		/* Reading from file */
	FILE *src;

	bool (*readline)( struct reader *, char *, size_t max );
};

bool tokenizeFile( struct storage *storage, const char *file ){
}
