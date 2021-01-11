/*
 * Tokenise
 */

#include "tokenize.h"

#include <stdio.h>

struct reader {
	u_int8_t *code;
	size_t codesz;
	size_t codeidx;

	char *string;
	size_t stringsz;
	size_t stringidx;

		/* Reading from file */
	FILE *src;

	bool (*readline)( struct reader *, char *, size_t max );
};

bool tokenizeFile( const char *file, u_int8_t *buff, size_t buffsz, size_t *idx, char *strbuff, size_t strsz, size_t *stridx ){
}
