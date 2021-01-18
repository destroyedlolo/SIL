/* SIL - Simple Interpreted Language
 *
 * 10/01/2021 - starting
 */

#include "config.h"
#include "storage.h"
#include "tokenize.h"

#include <stdio.h>
#include <unistd.h>	/*getopts() */

/*****
 * configuration flags
 *****/

bool noexec = false;
bool verbose = false;
bool debug = false;
const char *binoutfile = NULL;

const char *errstr = NULL;	/* last error message */

struct storage mainstorage;

int main( int ac, char **av ){
	int c;
	
	if(ac < 2){
		fprintf(stderr, "%s -h to get some help\n", av[0]);
		exit(EXIT_FAILURE);
	}

	while((c = getopt(ac, av,"ho:bvd")) != EOF) switch(c){
	case 'h':
		fputs("SIL interpretor v" VERSION "\n"
			"SIL [opts] source\n"
			"\t-o=<file> : generate binary in file\n"
			"\t-b : don't execute, generate binary only\n"
			"\t-v : be verbose\n"
			"\t-d : add debug messages\n", stderr
		);
		exit(EXIT_FAILURE);
		break;
	case 'o':
		binoutfile = optarg;
		break;
	case 'b':
		noexec = true;
		break;
	case 'd':
		debug = true;
	case 'v':
		verbose = true;
		break;
	default :
		fprintf(stderr, "'%c' is not known option\n", c);
		exit(EXIT_FAILURE);
	}

	if(verbose){
		fprintf(stderr, "*I* reading sources from '%s'\n", av[optind]);
	}

		/* Initialize internal objects */
	init_token();

		/* let's go */
	if(!init_storage(&mainstorage, MAXCODE, MAXSTRING)){
		fputs("*F* can't allocate storage\n", stderr);
		exit(EXIT_FAILURE);
	}

	size_t linenumber = -1;
	if(!tokenizeFile(&mainstorage, av[optind],&linenumber)){
		if(linenumber != -1)
			fprintf(stderr, "*F* reading '%s:%lu' - %s\n", av[optind], linenumber, errstr ? errstr : "unknow");
		else
			fprintf(stderr, "*F* reading '%s' - %s\n", av[optind], errstr ? errstr : "unknow");
		free_storage(&mainstorage);
		exit(EXIT_FAILURE);
	}
}

