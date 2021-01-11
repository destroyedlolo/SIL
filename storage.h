/* 
 * storage for tokenized information
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stdlib.h>

struct storage {
	u_int8_t *code;			/* tokenized code storage */
	size_t codesize;		/* size of the code storage */		
	size_t codeindex;		/* 1st free place in the code buffer */

	char *strings;			/* constant string storage */
	size_t stringssize;		/* size of strings storage */
	size_t stringsindex;	/* 1st free place in the strings storage */
};

/* initialize storages
 * -> storage : structure to be initialised
 * -> codesz : size of the code storage
 * -> stringssz : size of the strings storage
 *
 * <- false in case of error
 */
extern bool init_storage( struct storage *storage, size_t codesz, size_t stringssz );
extern void free_storage( struct storage * );

#endif
