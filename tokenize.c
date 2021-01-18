/*
 * Tokenise
 */

#include "tokenize.h"
#include "utilities.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

struct reader {
	struct storage *storage;
	size_t linenumber;

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
	r->linenumber = 0;
}


/*****
 * Tokens
 *****/

#define FIRST_TOKEN 128
enum Token {
	TOK_UNKNOWN = 0,
	TOK_INT = FIRST_TOKEN,
	TOK_SIGNED,
	TOK_FLOAT,
	TOK_STRING,
	TOK_VOID,
	TOK_REF,
};

struct XToken {
	enum Token token;
	const char *name;
	hash_t h;
} xtoken[] = {
	{ TOK_INT, "int" },
	{ TOK_SIGNED, "signed" },
	{ TOK_FLOAT, "float" },
	{ TOK_STRING, "string" },
	{ TOK_VOID, "void" },
	{ TOK_REF, "reference" },
};

#define FIRST_TYPE TOK_INT
#define END_TYPE TOK_REF

	/* Calculate hash code for tokens */
void init_token(){
	for(int i=0; i<sizeof(xtoken)/sizeof(xtoken[1]); i++)
		xtoken[i].h = hash( xtoken[i].name, -1 );
}

/*****
 * Identifier
 *****/

struct Ident {
	const char *start;
	size_t len;

	enum Token token;
};

	/* check if a character is suitable for an identifier
	 * valid are letters, '_' and digit but for the 1st one
	 *
	 *	-> c : character to test
	 *	-> first : is the 1st character ?
	 *
	 *	<- true if suitable
	 */
bool isIdentChar( char c, bool first ){
	return( isalpha(c) || c == '_' || (!first && isdigit(c)) );
}

bool Ident_get( struct Ident *ei, const char *s ){
	ei->start = s;
	ei->len = 0;
	ei->token = TOK_UNKNOWN;

	while( isIdentChar(s[ei->len], !ei->len) )
		ei->len++;

	return(!!ei->len);
}

const char *Ident_next( struct Ident *ei, bool strips ){
	const char *s = ei->start + ei->len;
	if(strips)
		while( isspace(*s) ) s++;
	return s;
}

	/* Debuging */
void Ident_print( struct Ident *ei ){
	if(!ei->len){
		printf("(empty)");
		return;
	}

	for(size_t i=0; i<ei->len; i++)
		putchar( ei->start[i] );
}

bool parse( struct reader *reader, size_t *linenumber ){
	char l[MAXLINE];
	enum resReading r;
	int nested_comment = 0;	/* level of nested comments */

	while( !(r=reader->readline(reader, l, MAXLINE)) ){
		const char *s = l;
		(*linenumber)++;

		while( isspace(*s) ) s++;

				/* it's safe to check next character as in worst case,
				 * at the end of the buffer, the last one is provisioned for \0
				 */
		if(!*s)
			continue;
		if(*s == '#'){
			if(s[1] == '*')	/* Multi line comment */
				nested_comment++;
			continue;	/* remaining of the line can be ignored */
		}
		if(nested_comment && *s == '*'){
			if(s[1] == '#'){
				nested_comment--;
				continue;	/*TODO : check if the remaining is empty*/
			}
			/* otherwise, let's continue with the parsing */
		}

		if(nested_comment)	/* inside comments */
			continue;

			/*
			 * Parse a line
			 */
		printf("\n%ld : ", *linenumber);
		struct Ident ident;

				/* Get the 1st ident.
				 * it can be :
				 * 	- type -> variable or function definition
				 * 	- already defined function name -> function call
				 * 	- already defined variable -> calculation
				 * 	- module identifier -> to be done
				 */
		if( !Ident_get(&ident, s) )
			return false;

/*
		while( Ident_get(&ident, s) ){
			s = Ident_next(&ident, true);

			Ident_print(&ident);
			printf(" (%ld -> '%s') - ", ident.len, s);
		}
*/
	}

	if(nested_comment){
		errstr = "Unbalanced comment";
		return false;
	}

	return( r==FINISHED );
}


/*****
 * Reading from a file
 ****/

enum resReading getLineFromFile( struct reader *reader, char *line, size_t max ){
	size_t l;

	if(!fgets(line, max, reader->file))
		return(feof(reader->file) ? FINISHED : ERROR);

		/* remove trailing \n */
	l = strlen(line);
	if(l && line[--l] == '\n')
		line[l] = 0;

	return OK;
}

bool tokenizeFile( struct storage *storage, const char *file, size_t *linenuber ){
	struct reader rdr;
	*linenuber = 0;

	init_reader( &rdr, storage );
	rdr.readline = getLineFromFile;
	if(!(rdr.file = fopen(file, "r"))){
		errstr = strerror(errno);
		return false;
	}

	return parse(&rdr, linenuber);
}

