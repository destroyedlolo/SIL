/*
 * Tokenise
 */

#include "tokenize.h"
#include "utilities.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

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
 * Tokens
 *
 * Notez-bien : parsing through function Ident_getToken() depends deeply
 * 	on types' ordering.
 * 	Changing anything on tokens implies to check Ident_getToken()
 *****/

#define FIRST_TOKEN 128
enum Token {
	TOK_UNKNOWN = 0,		/* MUST be 0 */
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
#define END_TYPE_VAR TOK_STRING	/* Last type applicable to variables */
#define END_TYPE_FUNC TOK_VOID	/* Last type applicable to function */
#define END_TYPE_ARG TOK_REF	/* Last type applicable to arguments */

	/* token kind (bits) */
#define TOKK_UNKNOWN	(0b00000000)
#define TOKK_VTYPE		(0b00000001)	/* type applicable to variable */
#define TOKK_FTYPE		(0b00000010)	/* type applicable only to a function */
#define TOKK_ATYPE		(0b00000100)	/* type applicable only to an argument */
#define TOKK_DTYPE		(TOKK_VTYPE | TOKK_FTYPE)				/* type to enter in a definition */
#define TOKK_TYPE		(TOKK_VTYPE | TOKK_FTYPE | TOKK_ATYPE)	/* all kind of types */

typedef uint16_t tokenkind_t;

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

	hash_t h;
	enum Token token;
	tokenkind_t kind;
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

	/* Get the identifier
	 *	-> struct Ident *ei : identifier structure
	 *	-> const char *s : starting of the identifier
	 *	<- can be an identifier (false if doesn't point to a valid identifier name)
	 */
bool Ident_get( struct Ident *ei, const char *s ){
	ei->start = s;
	ei->len = 0;
	ei->token = TOK_UNKNOWN;

	while( isIdentChar(s[ei->len], !ei->len) )
		ei->len++;

	return(!!ei->len);
}


	/* Return the point to the next element
	 *	-> struct Ident *ei : identifier structure
	 *	-> bool strips : remove space as well
	 *	<- const char * : next element of the string
	 */
const char *Ident_next( struct Ident *ei, bool strips ){
	const char *s = ei->start + ei->len;
	if(strips)
		while( isspace(*s) ) s++;
	return s;
}

	/* Find out token value
	 *	-> struct Ident *ei : identifier structure
	 *	<- enum Token : token value for the current id
	 *
	 * NOTEZ-BIEN : depends deeply on type ordering, 
	 * see XToken's comment
	 */
enum Token Ident_getToken( struct Ident *ei ){
	ei->h = hash( ei->start, ei->len );

	for(int i=0; i<sizeof(xtoken)/sizeof(xtoken[1]); i++){
		if( ei->h == xtoken[i].h ){	/* Hash code is matching */
			if( !strncmp(ei->start, xtoken[i].name, ei->len) ){	/* string's matching as well */
				ei->token = xtoken[i].token;
				break;
			}
		}
	}

	ei->kind = 0;
	if( !ei->token )
		;
	else if( ei->token <= END_TYPE_VAR )			/* Variable only */
		ei->kind |= TOKK_VTYPE;
	else if( ei->token <= END_TYPE_FUNC )	/* applicable to a function */
		ei->kind |= TOKK_VTYPE | TOKK_FTYPE;
	else if( ei->token > END_TYPE_FUNC && ei->token <= END_TYPE_ARG )	/* applicable only to arguments */
		ei->kind |= TOKK_VTYPE | TOKK_ATYPE;
	
	return(ei->token);
}

	/* Return flags about ident kind
	 */
tokenkind_t Ident_getKind( struct Ident *ei ){
	return(ei->kind);
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

	/* parse the entire content from a reader
	 * -> struct reader *reader : reader to use to get source
	 * -> size_t * linenumber : pointer to the line number counter
	 */
bool parse( struct reader *reader, size_t *linenumber ){
	char l[MAXLINE];
	enum resReading r;
	int nested_comment = 0;	/* level of nested comments */

	while( !(r=reader->readline(reader, l, MAXLINE)) ){	/* loop on lines */
		const char *s = l;	/* Where are we in this line parsing */
		(*linenumber)++;

		while( isspace(*s) ) s++;	/* skip leading spaces */

				/* it's safe to check next character as in worst case,
				 * at the end of the buffer, the last one is provisioned for \0
				 */
		if(!*s)
			continue;
		if(*s == '#'){	/* Entering in comments */
			if(s[1] == '*')	/* Multi line comment */
				nested_comment++;
			continue;	/* remaining of the line can be ignored */
		}
		if(nested_comment && *s == '*'){	/* End of multi-lines comment ? */
			if(s[1] == '#'){
				nested_comment--;
				continue;	/*TODO : check if the remaining is empty*/
			}
			/* otherwise, let's continue with the parsing */
		}

		if(nested_comment)	/* inside multi-lines comments : skip to next line*/
			continue;

			/*
			 * Parse a line
			 */
printf("\n%ld : ", *linenumber);
		struct Ident head;	/* first item of this line */

				/* Get the 1st item.
				 * it can be :
				 * 	- type -> variable or function definition
				 * 	- already defined function name -> function call
				 * 	- already defined variable -> calculation
				 * 	- module identifier -> to be done
				 */
		if( !Ident_get(&head, s) )
			return false;

		Ident_getToken(&head);	/* find out the token id */

Ident_print(&head);
printf(" (%ld -> '%s' h:%x, tok:%02x [%02x]) - ", head.len, s, head.h, head.token, Ident_getKind(&head) );
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

