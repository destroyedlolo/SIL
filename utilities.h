/*
 * Various utility functions
 */
#ifndef UTILITIES_H
#define UTILITIES_H

#include "stddef.h"

typedef unsigned short int hash_t;

	/* calculate hash code 
	 *	-> const char *s : string to be calculated
	 *	-> size_t len : string length (-1: up to '\0')
	 *	<- return code : (0 need false)
	 */
extern hash_t hash(const char *, size_t);

#endif
