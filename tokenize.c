/*
 * Tokenise
 */

#include "tokenize.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

struct reader {
	struct storage *storage;

		/* Reading from file */
	FILE *file;

		/* pointer to custom function to read a line from source
		 * -> reader : this structure
		 * -> line : where to store the line
		 * -> max : maximum line length
		 *
		 * <- result
		 *
		 * Notez-bien : to speed-up things, sanity checks HAVE to be made
		 * before calling this function.
		 */
	enum resReading { OK = 0, FINISHED, ERROR } (*readline)( struct reader *reader, char *line, size_t max );
};


/* shared reader initialisation */
void init_reader(struct reader *r, struct storage *s){
	r->storage = s;
}


/*****
 * Reading from a file
 ****/

enum resReading getLineFromFile( struct reader *reader, char *line, size_t max ){
	if(!fgets(line, max, reader->file))
		return ERROR;
	if(ferror(reader->file))
		return FINISHED;
	return OK;
}

bool tokenizeFile( struct storage *storage, const char *file ){
	struct reader rdr;

	init_reader( &rdr, storage );
	rdr.readline = getLineFromFile;
	if(!(rdr.file = fopen(file, "r"))){
		errstr = strerror(errno);
		return false;
	}

	return true;
}
