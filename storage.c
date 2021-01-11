/* 
 * storage for tokenized information
 */

#include "storage.h"

#include <stdlib.h>

bool init_storage( struct storage *st, size_t csz, size_t stsz ){
	st->code = malloc(csz);
	st->codesize = csz;
	st->codeindex = 0;

	st->strings = malloc(csz);
	st->stringssize = stsz;
	st->stringsindex = 0;

	if(!st->code || !st->strings)
		return false;
	return true;
}

void free_storage( struct storage *st ){
	if(st->code){
		free(st->code);

		if(st->strings)	/* allocated only if code has been successfully */
			free(st->strings);
	}
}
