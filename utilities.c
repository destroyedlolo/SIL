/*
 * Various utility functions
 */

#include "utilities.h"

hash_t hash( const char *s, size_t l){
	hash_t h=0;
	
	for(size_t i=l; i; i--){
		if(!*s)	/* end of the string */
			break;
		h += *s++;
	}

	return h;
}
