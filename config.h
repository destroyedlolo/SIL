/* 
 * Global configuration of SIL
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>	/* types */
#include <stdbool.h>

#define VERSION "0.01"

/*****
 * buffers' size
 ******/
#define MAXLINE		256			/* size of a line to be read */
#define MAXCODE		32*1024		/* size of the tokenized storage */
#define MAXSTRING	16*1024		/* size of the strings storage */

#endif
