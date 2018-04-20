// ID  =2015A7PS0146P
// Name=Mrigesh Madaan
#include <string.h>
#include <stdlib.h>

typedef struct _s{
	struct _snode * first;
	int length;
}Stack;


typedef struct _snode{
	char * str;
	struct _snode *next;
}SNode;


Stack * createStack();

SNode * newStackNode(char * str);

void push(Stack * s,char * str);

SNode * pop(Stack * s);

char * top(Stack * s);
