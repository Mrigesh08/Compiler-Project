// ID  =2015A7PS0146P
// Name=Mrigesh Madaan


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define lex_bufSize 100

char lex_buffer[lex_bufSize];
extern int error_flag;

typedef struct _token{
	char * tokenName;
	int lineNumber;
	int type; // 1= head * , 2= char *, 3 = int, 4= float, 5 =keyword used
	union {
		struct head * h; // for the entry in symbol table
		char * c;
		int i;
		float f;
	};
}Token;

FILE * sourceCode;

Token * getNextToken();

void constructAndPopulateTrie();

void emptyBuffer();