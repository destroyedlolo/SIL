/*
 * Various utility functions
 */

#include "utilities.h"

hash_t hash( const char *s, size_t l){
	hash_t h=0;
	
	for(size_t i=l,j=0; i; i--,j++){
		if(!*s)	/* end of the string */
			break;
		h += ((hash_t)*s++)<<(j%8);
	}

	return h;
}
