// ID  =2015A7PS0146P
// Name=Mrigesh Madaan

#include <stdlib.h>
#include <string.h>
#include "lexer.h"

typedef struct _e{
	char * name;
	int type;
	int offset;
	int size;
	int width;
	struct _e * nextEntry;
}Entry;

typedef struct _tn{
	char * str; // this will be function name in case of symbol table
	struct _token * token;
	struct _e * nextEntry;
	struct _tn * next;
	struct _tn * down;
	struct _tn * parent;
}TreeNode;

TreeNode * createNewTreeNode(char * str,Token * tk);

/*

*/